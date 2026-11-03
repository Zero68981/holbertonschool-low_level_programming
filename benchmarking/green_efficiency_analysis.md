# Green Efficiency Analysis

## Measurement Methodology
I ran two programs three times each and recorded the execution time that each program printed. The first program runs two algorithms, a naive one and a single-pass one, and reports the result and elapsed time of each. The second program executes a loop of 100,000,000 iterations and reports its execution time. The inputs were identical across runs, so differences between runs reflect measurement variation only. I summarise each program by the mean of its three runs. No energy-measuring tool was used: the only data recorded are the wall-clock times printed by the programs.

## Observed Performance Differences
The naive algorithm took 1.155705 s, 1.159000 s and 1.164751 s, a mean of 1.159819 s. The single-pass algorithm took 0.000052 s, 0.000055 s and 0.000062 s, a mean of about 0.0000563 s (56 microseconds). On average, the single-pass algorithm was therefore roughly 20,600 times faster. Per run, the ratio ranged from about 18,800 (run 3) to about 22,200 (run 1).

The 100,000,000-iteration loop took 0.107190 s, 0.106457 s and 0.106457 s, a mean of 0.106701 s, or about 1.07 nanoseconds per iteration. Run-to-run variation was small: the slowest naive run was 0.8% slower than the fastest, and the slowest loop run was 0.7% slower than the fastest. The single-pass times varied more in relative terms (0.000052 s to 0.000062 s, about 19%), although the absolute difference is only 10 microseconds.

## Relation Between Runtime and Energy Consumption
I did not measure energy, so I cannot report any value in joules. The general relation energy = average power x time implies that, if two programs draw similar power, the one with the shorter runtime consumes less energy. My data supports only the runtime half of this statement: the single-pass algorithm ran about 20,600 times shorter than the naive one. Because power draw was not recorded, any energy saving is an inference from runtime, not a measured result. The same applies to the loop program, whose 0.1067 s mean runtime says nothing about its energy use on its own.

## Limitations of the Experiment
First, I made only three runs per program, which is too few to estimate variation reliably. Second, energy was never measured directly, so the link between runtime and energy is inferred. Third, the two algorithms printed different results (625025000 for the naive one and 25000 for the single-pass one), so my records do not confirm that they solve exactly the same task; the speed comparison may not be like-for-like. Fourth, I did not record the machine, compiler options or background load, and I tested only one input size, so I cannot say how the gap changes with larger or smaller inputs. Finally, the single-pass times are on the order of tens of microseconds, where small absolute differences are large relative differences.

## Practical Engineering Takeaway
In these measurements, the single-pass algorithm reduced runtime from about 1.16 s to about 56 microseconds. Since energy is power multiplied by time, cutting runtime by this factor is a strong first step toward lower energy use. However, before claiming real savings, an engineer should first confirm that both algorithms return equivalent results and then measure energy directly.