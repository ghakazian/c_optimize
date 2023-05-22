# c_optimize
This is my solution to reduce the computational time required by the algorithm as supplied to me by ERP and included here as the file `test_j.c`.

The main issue that I identified with this algorithm from an efficiency perspective is that there are a lot of compute-intensive calculations being executed within the innermost loop of the algorith with an O(n3) complexity. Without a deeper understanding of the use case for the algorithm, it is difficult for me to know whether the algorithm's behavior can be modified to execute in something less than O(n3) complexity. With that in mind, I will focus my optimization on moving as many of the expensive calculations out of the innermost loop, which should reduce the amount of time used executing within that loop.

After examining the code, I identified four approaches to focus on for optimizations:
1.  Identify variables used to store constant values and move them out to global constants.
2.  Identify static calculations (not dependent on loop parameters) and initialize them outside of the loop.
3.  Identify calculations that are calculated multiple times with the same parameters and employ memoization to reduce redundant calculations.
4.  Reorder the loops and move calculations out of the inner loop as desired to reduce redundant calculations.

For the first area, there were a number of values declared within the innermost loop function as variables, but their values were constant. While these values were not being calculated, they were defined as variables in the innermost loop which means they were being copied to the stack every time the loop was executed. By moving these into global constants, we may see modest improvements to the program efficiency. Examples of these values are a, b, g, gC, etc.

For the second area, there were a number of calculated values that were static (not dependent on the f, fp, or fptilde parameters of the loop) but were being calculated in each execution of the loop. Because these values were calculated using functions, they cannot be made constants, but I was able to move them out as global variables and calculate them once at the start of the program execution using an init() function. This should ensure they are not being calculated multiple times as the algorithm executes and should yield significant improvements to the code efficiency.

For the third area, I identified some calculations that were being executed multiple times within the algorithm with the same parameters. These calculations are not static so they could not be moved out into the init() function, but they were only dependent on a subset of the loop parameters and so were being executed with the same parameters multiple times. As "low-hanging fruit" for this optimization, I identified a number of values (alpha, gamma, sigma_a, sigma_b) which are calculated based on the value of fptilde only. This meant they were being executed redundantly ~1000000 times; for each value of f and fp we were calculating the same set of 1000 values corresponding to unique values for fptilde. To address this, I implemented a simple cache of these values, utilizing the source value of fptilde as the key, so that I could memoize them as they are needed throughout the algorithm execution. Converting the double value of fptilde (or more correctly, fpt within the algorithm loop) into an index to look up existing values turned out to be trivial since I knew there was a finite range and step used for these values. If this were not the case, I would have needed to implement a hash set or similar algorithm to efficiently look up values based on a double key. There are probably other values that can be memoized within the algorithm, such as exp1arg which is calculated from f and fp only. However, this is not executed as redundantly as the fptilde-based values and I decided to not try memoize these additional calculations due to expected diminishing returns in regard to performance improvements vs the added complexity of two-index cache.

For the fourth area, I tried an alternate approach to the third area to accomplish a similar improvement. For this change, I changed the order of the loops. Since there were a number of calculations based solely on the value of fptilde, it makes sense to calculate these in the outer loop as pass them as parameters into the inner loops rather than calculate them there or even look them up in the inner loop as done in #3. To accomplish this, I changed the loop order to fptilde -> f -> fp, calculated the values in the outer loop, and changed the function_j signature to take the values it needs from the higher loop as inputs. This effectively does the same thing as #3 by eliminating the redundant calculations of these values in the inner loop, but it is somewhat more performant in that it does not have the overhead of cache lookup or storage. Changing the order of the loops does not change the set of calculations performed but it does alter the order they are executed in, so this approach may not work if we depend on the results being returned in a particular order.

Evaluation:
Below are the performance evaluations of these steps of improvement compared to the original base case. To evaluate the performance, I ran the unix time command with the original and optimized version at each step and recorded the time required for the algorithm to complete.
Base case / Original: 148.34s
Step A: 142.97s
Step B: 96.36s
Step C: 52.12s
Step D: 46.98s

Conclusion:
As expected, the first step of optimization yielded a minimal improvement in performance. My guess is that this same optimization could have been handled by a compiler optimization flag. The second and third steps, however, yielded significant improvements in performance, of over 30% reduction in time to complete for each. Combined, the optimized program executes in about 65% less time than the original. The improvements from the fourth step were marginally better still. This was done with no changes to the logical behavior of the algorithm; with more time and understanding of the underlying math I believe it might be feasible to rewrite the algorithm to something with less than O(n3) complexity. Such an improvement would either further reduce the time required to execute against this set of inputs, or allow it to run with more fine-grained stepping of the values used as inputs.
