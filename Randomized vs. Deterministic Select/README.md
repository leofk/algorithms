# 2010

A Clash of Titans: Randomized vs. Deterministic

This program compares the runtimes of two implementations of the quick select algorithm.
The goal of the algorithm is, provided an unsorted array and a rank (k-th smallest position) in said array, return the position (or value) of the element at said rank.
The algorithms are implemented by extending the classic quick-sort algorithm with prune-and-search strategies.
The key difference between each is how we chose a pivot for partitioning the sub-arrays. 
Randomized Select -- as implied -- choses a pivot at random. Whereas Deterministic Select choses a pivot by approximating the median of the array.
The program anylizes runtime by searching for the median element of a randomized unsorted array. 

To run in command line:
- go to file directory
- g++ deterministic-select.cpp -o run
- ./run
- Input _Size_ of randomly generated array
- Default number of iterations (_attempts_) is initialized to 100, but can be easily changed in the program. 
