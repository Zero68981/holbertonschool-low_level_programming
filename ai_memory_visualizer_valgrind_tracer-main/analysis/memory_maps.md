# Memory maps: heap, stack, aliasing

Assumptions: 64-bit Linux (LP64), glibc, stack grows toward lower addresses.
So `sizeof(char *) = 8`, `sizeof(int) = 4`, `sizeof(Person) = 16` (8 + 4 + 4 bytes of padding).
Real addresses change on every run (ASLR). I write them symbolically (`H1`, `S3`...) and paste real values from my own run where I checked them.

---

## 1. heap_example.c: allocations and a deliberate leak

### Objects and lifetimes

| Object | Region | Created | Destroyed |
|---|---|---|---|
| `alice`, `bob` (the pointer variables, 8 bytes each) | stack, frame of `main` | start of `main` | `return 0` of `main` |
| `Person` struct of Alice (16 B) | heap | `malloc` in `person_new` | `free(p)` in `person_free_partial` |
| name buffer "Alice" (6 B: 5 chars + `\0`) | heap | second `malloc` in `person_new` | **never freed** |
| `Person` struct of Bob (16 B) | heap | `malloc` in `person_new` | `free(bob)` |
| name buffer "Bob" (4 B) | heap | second `malloc` in `person_new` | `free(bob->name)` |
| `p`, `i`, `len`, `name` (locals/params of `person_new`) | stack, frame of `person_new` | call | return of `person_new` |

### Step by step

**After `alice = person_new("Alice", 30)`** (the `person_new` frame is already gone, only the heap objects survive):

```
STACK (main)                     HEAP
alice  = H1  ------------------> H1: [ name = H2 | age = 30 | pad ]   16 B
bob    = NULL                    H2: [ 'A' 'l' 'i' 'c' 'e' '\0' ]      6 B
```

**After `bob = person_new("Bob", 41)`:**

```
STACK (main)                     HEAP
alice  = H1  ------------------> H1: [ name = H2 | age = 30 ]   16 B
                                 H2: "Alice\0"                    6 B
bob    = H3  ------------------> H3: [ name = H4 | age = 41 ]   16 B
                                 H4: "Bob\0"                      4 B
```

No aliasing here: `alice`, `bob` and the two `name` fields all hold different addresses.

**After `free(bob->name); free(bob);`** (order matters: the inner block first, because once `H3` is freed, `bob->name` can no longer be read legally):

```
alice  = H1 --> H1 [ name = H2 | 30 ]   still alive
                H2 "Alice\0"            still alive
bob    = H3 --> (freed)                 dangling, never used again
                H4 (freed)              nobody holds H4 any more
```

**During `person_free_partial(alice)`:** the parameter `p` is a **copy** of `alice`. Both hold `H1` while the function runs. `free(p)` releases `H1`. Freeing `p` does not change `alice` in `main`: `alice` still contains the address `H1`, which is now invalid (dangling).

**At `return 0`:**

```
STACK: the frame of main is destroyed (alice, bob disappear)
HEAP : H2 "Alice\0" is still allocated, but the only pointer to it
       (the `name` field) lived inside H1, which was freed.
       => no way to reach H2 any more: leaked.
```

Expected Valgrind verdict (to confirm in the Valgrind task): 1 block of 6 bytes definitely lost, allocated in `person_new`. 4 `malloc`s of our own, 3 `free`s.

### Why `person_free_partial` is the bug

`free(p)` releases only the 16 bytes of the struct. It does not follow `p->name`. The fix would be `free(p->name); free(p);`, which is exactly what `main` does for `bob`.

---

## 2. stack_example.c: recursion and stack frames

`walk_stack(0, 3)` creates 4 nested frames. `dump_frame` is called and **returns** before each recursive call, so its frame is not on the stack while the deeper `walk_stack` runs.

### Variables and lifetimes

| Variable | Frame | Lifetime |
|---|---|---|
| `depth`, `max_depth`, `marker` | `walk_stack(d)` | from entry of that call until it returns |
| `local_int`, `local_buf[16]`, `p_local` | `dump_frame` | only during one call of `dump_frame` |

Values per depth: `marker = 10*d`, `local_int = 100+d`, `local_buf[0] = 'A'+d` (A, B, C, D), `local_buf[1] = '\0'`. `local_buf[2..15]` is **never written** (uninitialized garbage, never read).

### Deepest point (inside `dump_frame("enter", 3)`), high addresses on top

```
high addresses
+--------------------------------------+
| main                                 |
+--------------------------------------+
| walk_stack(0,3)  marker=0            |  &marker = S0
+--------------------------------------+
| walk_stack(1,3)  marker=10           |  &marker = S1  (S1 < S0)
+--------------------------------------+
| walk_stack(2,3)  marker=20           |  &marker = S2  (S2 < S1)
+--------------------------------------+
| walk_stack(3,3)  marker=30           |  &marker = S3  (S3 < S2)
+--------------------------------------+
| dump_frame("enter",3)                |
|   local_int=103  (&local_int = L3)   |
|   p_local = L3   (points to itself's |
|                   own frame: same    |
|                   address as &local_int)
|   local_buf[0]='D'                   |
+--------------------------------------+
low addresses
```

### Call sequence (what is on the stack when)

```
walk_stack(0): dump_frame(enter,0)  -> returns   (marker=0 printed)
  walk_stack(1): dump_frame(enter,1) -> returns
    walk_stack(2): dump_frame(enter,2) -> returns
      walk_stack(3): dump_frame(enter,3) -> returns
                     no recursion (3 is not < 3)
                     dump_frame(exit,3) -> returns
    dump_frame(exit,2) -> returns
  dump_frame(exit,1) -> returns
dump_frame(exit,0) -> returns
```

### What I can predict, and what I must check in the real output

- `&marker` decreases as depth increases (S0 > S1 > S2 > S3), because each recursive call adds a frame below the previous one.
- `&marker` of a given depth is **the same** at "enter" and "exit": it is the same variable, in the same frame, and it keeps its value (`10*d`).
- `&local_int` printed at "enter d" and at "exit d" is **the same** address, since `dump_frame` is called from the same depth both times (same frame position, reused).
- `p_local == &local_int` on every line pair: it points to a variable in its own frame, valid because the frame is alive while it is printed.
- Exact gaps between frames depend on the compiler (frame size, alignment, optimization), so I do not claim a number of bytes.

---

## 3. aliasing_example.c: aliasing and use-after-free

### Step by step (n = 5, so the block is 5 * 4 = 20 bytes)

**After `a = make_numbers(5)`** (locals `i`, `arr`, `n` of `make_numbers` are gone, only the heap block survives):

```
STACK (main)                HEAP
a = H1 -------------------> H1: [0 | 11 | 22 | 33 | 44]   20 B
b = NULL
```

**After `b = a;`:** this copies the **address**, not the 20 bytes. There is still only one block.

```
a = H1 ----+
           +----------------> H1: [0 | 11 | 22 | 33 | 44]
b = H1 ----+
```

`a[2]` and `b[2]` are the same `int` (value 22).

**After `free(a);`:**

```
a = H1 ----+
           +----------------> H1: FREED (no longer owned by the program)
b = H1 ----+
```

- `free(a)` does not change `a`. It still holds `H1`.
- `b` is invalidated too. Aliases do not "know" about each other: nothing was written to `b`, yet it is dangling.
- The block's lifetime ended at `free(a)`, but the variables `a` and `b` live until the end of `main`. A variable being alive does not mean the memory it points to is valid.

**Three invalid accesses afterwards:**

| Line | Access | Offset in freed block |
|---|---|---|
| `printf(... b[2])` | read, 4 bytes | 8 bytes inside |
| `b[3] = 1234` | write, 4 bytes | 12 bytes inside |
| `printf(... b[3])` | read, 4 bytes | 12 bytes inside |

Expected Valgrind report (to confirm): "Invalid read of size 4", "Invalid write of size 4", "Invalid read of size 4", each saying "inside a block of size 20 free'd", with the block allocated in `make_numbers` and freed in `main`. There is no leak: the block was freed.

---

## 4. Where the AI explanation was wrong or misleading

### Case A (heap_example.c): "person_free_partial frees Alice, so no leak for Alice"

- **Typical AI claim:** `alice` and `bob` are both released, so the program has no leak, or the leak is the whole `Person` of Alice (16 bytes).
- **Why it is wrong:** `free(p)` releases only the struct. The separate `name` block (6 bytes) is never freed. The leak is 6 bytes in 1 block, not 16 or 22.
- **Correction:** I tracked the two `malloc`s per person separately (H1/H2, H3/H4) and counted which ones have a matching `free`: H1, H3, H4 yes, H2 no.

### Case B (aliasing_example.c): "after free(a), b[2] still prints 22, so it is safe"

- **Typical AI claim:** the memory still contains the old value, so the read is harmless, or only `a` becomes invalid.
- **Why it is wrong:** reading `b[2]` is undefined behavior whatever it prints. Under Valgrind it is reported as an invalid read. Without Valgrind, glibc may reuse the first bytes of a freed block for its own bookkeeping (tcache), so `b[2]` can print something other than 22. I must check this in my own run before keeping this sentence.
- **Correction:** the lifetime of the block ended at `free(a)`, and `b` is an alias of the same block, so it is dangling as well.

### Case C (stack_example.c): "each dump_frame call has a different address because frames are separate"

- **Typical AI claim:** `dump_frame` frames at the same depth are different addresses, or all `dump_frame` frames are stacked on top of each other.
- **Why it is wrong:** `dump_frame` returns before each recursive call. Its frame is reused at the same position when called again from the same `walk_stack` depth, so enter and exit of the same depth print the same `&local_int`.
- **Correction:** I drew the stack at each call/return instead of one picture for the whole run.
