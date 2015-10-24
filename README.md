# BP-solver
The BP-based framework to solve large-scale combinatorial optimization.

*Inho Cho, Soya Park, Sejun Park, Dongsu Han, and Jinwoo Shin, "Practical Message-passing Framework for Large-scale Combinatorial Optimization", 2015 IEEE International Conference on Big Data*

## Complilation and Execution

```
> make
> ./solver [INPUT_FILENAME=input(default)]
```
## Configurations
All important parameters can be configured in config.h.
- N_ITER: Total number of BP iterations. For maximum weight matching and minimum vertex cover 100 are sufficient.
- N_THREAD: Number of thread to solve the problem
- VAR_NODE: This should be 1 if the problem contains node variables such as minimum weight vertex cover, maximum independent set. Otherwise 0.
- VAR_EDGE: This should be 1 if the problem contains edge variables such as maximum weight matching, shortest path, travelling salesman problem.
- NOISE_NODE: Range of noise on node weight. Noise will be randomly assigned in range [-NOISE_NODE, NOISE_NODE]. We recommend to configure NOISE_NODE to 10% of minimum distance among node weights.
- NOISE_EDGE: Range of noise on edge weight. Noise will be randomly assigned in range [-NOISE_EDGE, NOISE_EDGE]. We recommend to configure NOISE_EDGE to 10% of minimum distance among edge weights.
- DAMPING: 1 for hybrid damping, 0 for without damping.


