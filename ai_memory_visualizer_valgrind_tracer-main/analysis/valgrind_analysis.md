# Valgrind analysis: heap_example.c and aliasing_example.c

Platform: 64-bit Linux, glibc. Addresses change on every run (ASLR), so I refer to blocks symbolically (`H1`, `H2`...) as in `memory_maps.md`.
Uninitialized memory: neither program reads uninitialized data, so Memcheck should report **no** "conditional jump depends on uninitialised value". I state this explicitly because the task asks to look for it.

## 1. How I ran Valgrind

```
gcc -Wall -Wextra -g -O0 -o heap_example heap_example.c
gcc -Wall -Wextra -g -O0 -o aliasing_example aliasing_example.c
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./heap_example
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./aliasing_example
```

`-g -O0` so that Valgrind can give file:line and the compiler does not remove or reorder accesses. `--track-origins=yes` is there for uninitialized values (none expected here).

Valgrind 3.26.0 (Memcheck). Both runs below are my real outputs, and the analyses of sections 2 and 3 are checked against them. The only point left to confirm is the native (no Valgrind) value of `b[2]` in Case B.

### Real output: heap_example

```
==997== Memcheck, a memory error detector
==997== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==997== Using Valgrind-3.26.0 and LibVEX; rerun with -h for copyright info
==997== Command: ./heap_example
==997== 
heap_example: allocations and a deliberate leak
  alice=0x4aae480 name=0x4aae4d0 age=30
  bob=0x4aae520 name=0x4aae570 age=41
==997== 
==997== HEAP SUMMARY:
==997==     in use at exit: 6 bytes in 1 blocks
==997==   total heap usage: 5 allocs, 4 frees, 1,066 bytes allocated
==997== 
==997== 6 bytes in 1 blocks are definitely lost in loss record 1 of 1
==997==    at 0x48A5618: malloc (vg_replace_malloc.c:447)
==997==    by 0x40008CF: person_new (heap_example.c:21)
==997==    by 0x40009CB: main (heap_example.c:51)
==997== 
==997== LEAK SUMMARY:
==997==    definitely lost: 6 bytes in 1 blocks
==997==    indirectly lost: 0 bytes in 0 blocks
==997==      possibly lost: 0 bytes in 0 blocks
==997==    still reachable: 0 bytes in 0 blocks
==997==         suppressed: 0 bytes in 0 blocks
==997== 
==997== For lists of detected and suppressed errors, rerun with: -s
==997== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

### Real output: aliasing_example

Compilation (GCC already warns statically, before any run):

```
aliasing_example.c: In function 'main':
aliasing_example.c:45:34: warning: pointer 'b' used after 'free' [-Wuse-after-free]
aliasing_example.c:44:6: warning: pointer 'b' used after 'free' [-Wuse-after-free]
aliasing_example.c:42:36: warning: pointer 'b' used after 'free' [-Wuse-after-free]
aliasing_example.c:40:5: warning: pointer 'b' used after 'free' [-Wuse-after-free]
   (each one with: aliasing_example.c:38:5: note: call to 'free' here)
```

(Shortened to the warning lines; the `note:` and source excerpts are the same for each.)

Valgrind:

```
==1066== Memcheck, a memory error detector
==1066== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==1066== Using Valgrind-3.26.0 and LibVEX; rerun with -h for copyright info
==1066== Command: ./aliasing_example
==1066== 
aliasing_example: aliasing and use-after-free (Valgrind should report it)
  a=0x4aae480 b=0x4aae480 a[2]=22 b[2]=22
  after free(a): b=0x4aae480 (dangling)
==1066== Invalid read of size 4
==1066==    at 0x40009B0: main (aliasing_example.c:42)
==1066==  Address 0x4aae488 is 8 bytes inside a block of size 20 free'd
==1066==    at 0x48A86B8: free (vg_replace_malloc.c:990)
==1066==    by 0x4000997: main (aliasing_example.c:38)
==1066==  Block was alloc'd at
==1066==    at 0x48A5618: malloc (vg_replace_malloc.c:447)
==1066==    by 0x400089B: make_numbers (aliasing_example.c:12)
==1066==    by 0x400093B: main (aliasing_example.c:30)
==1066== 
  reading b[2]=22
==1066== Invalid write of size 4
==1066==    at 0x40009D0: main (aliasing_example.c:44)
==1066==  Address 0x4aae48c is 12 bytes inside a block of size 20 free'd
==1066==    at 0x48A86B8: free (vg_replace_malloc.c:990)
==1066==    by 0x4000997: main (aliasing_example.c:38)
==1066==  Block was alloc'd at
==1066==    at 0x48A5618: malloc (vg_replace_malloc.c:447)
==1066==    by 0x400089B: make_numbers (aliasing_example.c:12)
==1066==    by 0x400093B: main (aliasing_example.c:30)
==1066== 
==1066== Invalid read of size 4
==1066==    at 0x40009DC: main (aliasing_example.c:45)
==1066==  Address 0x4aae48c is 12 bytes inside a block of size 20 free'd
==1066==    at 0x48A86B8: free (vg_replace_malloc.c:990)
==1066==    by 0x4000997: main (aliasing_example.c:38)
==1066==  Block was alloc'd at
==1066==    at 0x48A5618: malloc (vg_replace_malloc.c:447)
==1066==    by 0x400089B: make_numbers (aliasing_example.c:12)
==1066==    by 0x400093B: main (aliasing_example.c:30)
==1066== 
  wrote b[3]=1234
==1066== 
==1066== HEAP SUMMARY:
==1066==     in use at exit: 0 bytes in 0 blocks
==1066==   total heap usage: 2 allocs, 2 frees, 1,044 bytes allocated
==1066== 
==1066== All heap blocks were freed -- no leaks are possible
==1066== 
==1066== For lists of detected and suppressed errors, rerun with: -s
==1066== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 0 from 0)
```

---

## 2. heap_example.c

### Issue 1: memory leak, "definitely lost", 6 bytes in 1 block

**Confirmed by the real output.** `6 bytes in 1 blocks are definitely lost in loss record 1 of 1`, allocated by `malloc` called from `person_new (heap_example.c:21)`, itself called from `main (heap_example.c:51)`. Line 21 is the second `malloc` (the name buffer) and line 51 is `alice = person_new("Alice", 30)`: the stack points exactly to Alice's name, not to her struct and not to Bob.

| Field | Analysis |
|---|---|
| Valgrind category | `definitely lost` (LEAK SUMMARY) |
| Error type | Memory leak due to lost ownership |
| Object involved | The name buffer `"Alice\0"`, block `H2`, 6 bytes (5 chars + `\0`), allocated by the second `malloc` in `person_new` (line 21) |
| Allocation stack | `malloc` <- `person_new` <- `main` (the call for Alice, line 51) |

**Lifetime violation.** `H2` is only reachable through the field `alice->name`, which lives inside the struct block `H1`. `person_free_partial(alice)` calls `free(p)`, which releases `H1` only: `free` is not recursive and does not follow `p->name`. After that, the last pointer to `H2` is stored in memory that no longer belongs to the program, and `alice` in `main` points to the freed `H1`. At `return 0` nothing can reach `H2` any more. It was never freed, and it can never be freed: ownership of the block was lost.

**Bookkeeping that confirms it.** 4 `malloc`s of my own (H1, H2, H3, H4) and 3 `free`s of my own (`bob->name` = H4, `bob` = H3, `alice` = H1). H2 has no matching `free`. 6 bytes, 1 block: that is exactly the size of `"Alice"` including `\0`, not the 16 bytes of the struct and not 22 bytes.

**Reading `HEAP SUMMARY`.** Valgrind says `5 allocs, 4 frees, 1,066 bytes allocated`, which is one more of each than my own count. The extra pair is the buffer that `stdout` allocates at the first `printf` (1,024 bytes) and that the C library frees at exit. Check: 16 (H1) + 6 (H2) + 16 (H3) + 4 (H4) + 1,024 (stdout) = 1,066. And `in use at exit: 6 bytes in 1 blocks` is exactly the leaked H2, so nothing else is still allocated.

**Addresses of the run, mapped to my memory map.**

| Symbol | Real address | Content |
|---|---|---|
| `alice` = H1 | `0x4aae480` | struct of Alice (16 B) |
| `alice->name` = H2 | `0x4aae4d0` | `"Alice\0"` (6 B), **leaked** |
| `bob` = H3 | `0x4aae520` | struct of Bob (16 B) |
| `bob->name` = H4 | `0x4aae570` | `"Bob\0"` (4 B), freed |

All four addresses are different, which confirms there is no aliasing in this program. The gaps (0x50 = 80 bytes) are larger than the block sizes because Memcheck surrounds each block with guard zones (redzones) and its own bookkeeping; they say nothing about the program.

**Not reported as leaks (and why).**
- Bob: `free(bob->name)` then `free(bob)`, in this order. The inner block is freed first, otherwise `bob->name` would be read from freed memory.
- The 1,024-byte `stdout` buffer: it is freed at exit, so `still reachable` is 0 bytes in this run.

**No invalid access.** The real output contains no `Invalid read` / `Invalid write`: after `free(bob)` and `free(alice)` the pointers `bob` and `alice` are never dereferenced again. They are dangling, but a dangling pointer that is never used is not an error Valgrind reports. `ERROR SUMMARY: 1 errors from 1 contexts` is therefore the leak itself (counted as an error because of `--leak-check=full`), and nothing else. No uninitialised value was reported either, as predicted.

**Fix (not required).** `free(p->name); free(p);` in `person_free_partial`.

---

## 3. aliasing_example.c

Block: `make_numbers(5)` allocates 5 * 4 = 20 bytes (`H1`), allocated at line 12, freed at line 38 (`free(a)`).

**Overview of the real output.** `ERROR SUMMARY: 3 errors from 3 contexts`: one `Invalid read`, one `Invalid write`, one `Invalid read`, all of size 4, all on the same block: `a block of size 20 free'd`, freed at `main (aliasing_example.c:38)` and allocated at `make_numbers (aliasing_example.c:12)` called from `main (aliasing_example.c:30)`. Same block, same free site, same allocation site: the three errors have a single root cause (section "Common cause" below).

| Symbol | Real address | Relation to the block |
|---|---|---|
| `a` = `b` = H1 | `0x4aae480` | start of the 20-byte block (first line of output: `a=... b=...` identical) |
| `&b[2]` | `0x4aae488` | `0x4aae480 + 8`: "8 bytes inside" (2 * 4) |
| `&b[3]` | `0x4aae48c` | `0x4aae480 + 12`: "12 bytes inside" (3 * 4) |

The two offsets reported by Valgrind match my prediction `index * sizeof(int)`, and the block is 20 bytes (5 ints), so index 2 and 3 are inside the block's old bounds: the problem is the **lifetime** (freed), not the bounds (no overflow).

### Issue 2: Invalid read of size 4 (use-after-free), `b[2]`

**Confirmed:** `Invalid read of size 4 at main (aliasing_example.c:42)`, `Address 0x4aae488 is 8 bytes inside a block of size 20 free'd`.

| Field | Analysis |
|---|---|
| Valgrind message | `Invalid read of size 4` ... `Address ... is 8 bytes inside a block of size 20 free'd` |
| Error type | Use-after-free (read) |
| Object involved | `H1`, element 2 (offset 8) |
| Lines | access at line 42, free at line 38, allocation at line 12 (`make_numbers`) |

### Issue 3: Invalid write of size 4 (use-after-free), `b[3] = 1234`

**Confirmed:** `Invalid write of size 4 at main (aliasing_example.c:44)`, `Address 0x4aae48c is 12 bytes inside a block of size 20 free'd`.

| Field | Analysis |
|---|---|
| Valgrind message | `Invalid write of size 4` ... `Address ... is 12 bytes inside a block of size 20 free'd` |
| Error type | Use-after-free (write) |
| Object involved | `H1`, element 3 (offset 12) |
| Line | 44 |

A write is worse than a read: it modifies memory that the allocator now owns. Under Valgrind the effect is invisible (the program just prints `wrote b[3]=1234`), but in a native run offsets 8 to 15 of a freed small block are where glibc's tcache bookkeeping can live, so this write can corrupt allocator metadata, not just "a number". The program "works" and the memory is corrupted: this is why use-after-free is dangerous, and why an invisible effect proves nothing.

### Issue 4: Invalid read of size 4 (use-after-free), `b[3]` after the write

**Confirmed:** `Invalid read of size 4 at main (aliasing_example.c:45)`, `Address 0x4aae48c is 12 bytes inside a block of size 20 free'd`: same address as the write of Issue 3.

Same block, same offset (12), line 45 (the `printf` that reads `b[3]` back). Reading back the value I just wrote does not make the access valid: the write and the read target memory whose lifetime ended.

### Common cause for issues 2, 3 and 4: aliasing + dead block, living variables

`b = a` copies the **address**, not the 20 bytes (one block, two names). `free(a)` ends the lifetime of the block. `a` is not modified by `free` (it still holds `H1`), and `b` is invalidated too even though nothing was written to `b`: aliases do not "know" about each other. The variables `a` and `b` live until the end of `main`, but the memory they point to is dead since line 38. A pointer variable being alive does not make the pointed memory valid.

**Not reported by Valgrind: line 40.** `printf("... b=%p")` only passes the pointer value, it does not read the memory behind it, so Memcheck has nothing to flag. GCC, however, **does** warn about it (`-Wuse-after-free` at line 40), because the C standard says the value of a pointer becomes indeterminate once the object it points to is freed. So the two tools do not report the same thing: GCC flags 4 lines (40, 42, 44, 45) by static analysis of the source, Valgrind flags 3 accesses (42, 44, 45) that really read or write the freed memory at runtime. Valgrind only reports what executes; the compiler also sees the suspicious use of the pointer value.

**No leak.** `in use at exit: 0 bytes in 0 blocks`, `All heap blocks were freed -- no leaks are possible`. `2 allocs, 2 frees, 1,044 bytes allocated`: my block (20 bytes) + the `stdout` buffer (1,024 bytes) = 1,044, both freed. This is consistent with Section 2 (the buffer is freed at exit).

**Result vs prediction.** I predicted 3 invalid accesses (2 reads, 1 write), 0 leaks, and the same free/allocation sites: all confirmed by the real output. An invalid access and a leak are two different lifetime errors: in `heap_example` memory is never released (leak), here memory is released and then still used (use-after-free). The same `free` that is missing in one program is "too early" in the other.

---

## 4. Where the AI explanation was wrong or misleading

### Case A (heap_example.c): "no leak for Alice because `person_free_partial` frees her"

- **AI claim:** `person_free_partial(alice)` releases Alice, so there is no leak, or the leak is the 16-byte `Person`.
- **Why it is wrong:** `free(p)` releases only the struct (`H1`). The name buffer `H2` is a separate allocation. The leak is 6 bytes in 1 block, and it is the **name**, not the struct.
- **How I verified it:** counting `malloc`/`free` pairs per block (H1, H3, H4 freed, H2 not) in my memory map, then checking it against the real Valgrind output: `definitely lost: 6 bytes in 1 blocks`, allocated at `person_new (heap_example.c:21)` called from `main (heap_example.c:51)`. If the AI were right, Valgrind would report 16 bytes (or 22) and the allocation site would be line 14 (the struct), not line 21 (the name).

### Case B (aliasing_example.c): "after `free(a)`, `b[2]` still prints 22, so it is harmless"

- **AI claim:** the old value is still in memory, so the read is safe, or only `a` is invalid.
- **What my real run shows:** under Valgrind the program does print `reading b[2]=22`. So the AI's *observation* is correct in this run. Its *conclusion* is wrong: on the very next lines Valgrind reports `Invalid read of size 4 ... 8 bytes inside a block of size 20 free'd` for that same access, and a second one for `b[3]` after the write.
- **Why it is wrong:** reading freed memory is undefined behavior whatever it prints. And `b` is invalid because it aliases `a`: nothing was written to `b`, yet it is dangling since line 38. The value 22 is only the old content that has not been overwritten yet; it is not a guarantee. Valgrind keeps freed blocks aside and does not reuse them right away, which is part of why the old value survives under Valgrind.
- **Check on a native run without Valgrind [TO CONFIRM on my machine]:** in a test run of the same code compiled with `gcc -g -O0` and run directly, `b[2]` did **not** print 22 but a garbage value (`-1722845552`). glibc reuses the start of a freed block for its own bookkeeping (tcache), and offset 8 (`b[2]`) is part of that. So the same line prints 22 under Valgrind and something else natively: the printed value depends on the allocator, and a program that "printed the right thing" is not proof that the access was valid. The exact garbage value changes from run to run. To keep this claim I must run `./aliasing_example` without Valgrind on my machine and note the value I get.

### Case C (about Valgrind itself): "`still reachable` is also a leak I must fix"

- **AI claim:** every block in the LEAK SUMMARY is a leak of my code.
- **Why it is misleading:** `definitely lost` means no pointer to the block exists any more (lost ownership, my bug: H2). `still reachable` means a pointer still exists at exit, which is usually not a bug of mine. The two categories must not be treated the same way. In my real `heap_example` run only `definitely lost` is non-zero.

### Case D (heap_example.c): "a ~1 KB `still reachable` block from `stdout` will appear"

- **AI claim (in my first draft of this analysis, written before running Valgrind):** the buffer allocated by `printf` would show up as `still reachable`.
- **Why it is wrong:** the real output shows `still reachable: 0 bytes in 0 blocks` and `5 allocs, 4 frees`: the C library frees the `stdout` buffer at exit, so it is counted as allocated and freed, and `in use at exit` is only the 6 leaked bytes.
- **How I verified it:** the arithmetic of `HEAP SUMMARY`: 16 + 6 + 16 + 4 + 1,024 = 1,066 bytes allocated, 4 of the 5 blocks freed, the only block left being H2. Lesson: an explanation that sounds plausible about library internals must be checked against the real run, not kept because it is reasonable. (Whether the buffer is freed or left reachable can depend on the libc version, so this result is specific to my machine.)
