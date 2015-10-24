# BP-solver
The BP-based framework to solve large-scale combinatorial optimization.

*Inho Cho, Soya Park, Sejun Park, Dongsu Han, and Jinwoo Shin, "Practical Message-passing Framework for Large-scale Combinatorial Optimization", 2015 IEEE International Conference on Big Data*

## Complilation and Execution

```
> make
> ./solver [INPUT_FILENAME="input"(default)]
```
## Configurations
All important parameters can be configured in config.h.
- **N_ITER**: Total number of BP iterations. For maximum weight matching and minimum vertex cover 100 are sufficient.
- **N_THREAD**: Number of thread to solve the problem
- **VAR_NODE**: This should be 1 if the problem contains node variables such as minimum weight vertex cover, maximum independent set. Otherwise 0.
- **VAR_EDGE**: This should be 1 if the problem contains edge variables such as maximum weight matching, shortest path, travelling salesman problem.
- **NOISE_NODE**: Range of noise on node weight. Noise will be randomly assigned in range [-NOISE_NODE, NOISE_NODE]. We recommend to configure NOISE_NODE to 10% of minimum distance among node weights.
- **NOISE_EDGE**: Range of noise on edge weight. Noise will be randomly assigned in range [-NOISE_EDGE, NOISE_EDGE]. We recommend to configure NOISE_EDGE to 10% of minimum distance among edge weights.
- **DAMPING**: 1 for hybrid damping, 0 for without damping.

## Input file format


## User-defined functions
To utilize BP-solver for combinatorial optimization problem, you need to modify 4 functions. Defaults are set for maximum weight matching

### 1. Message Initialization
- Edge::initEdgeMessage () in Edge.cpp (for edge variables)
- Node::initNodeMessage () in Node.cpp (for node variables)

You should specify message initialization rule in one of these functions. 
If you try to use node variables (VAR_NODE = 1), specify the message initialization rule in initNodeMessage() function and empty initEdgeMessage() function.
If you try to use edge variables (VAR_EDGE = 1), specify the message initialization rule in initEdgeMessage() function and empty initNodeMessage() function.

For each edge there are two corresponding messages: one from source to destination (s_to_d) and the other from destination to source (d_to_s).
If you are not sure how to initialize the messages we recommend to initialize 0.5*EDGE_WEIGHT for edge variables, NODE_WEIGHT/DEGREE_OF_NODE for node variables.

### 2. BP Message Update
- Node::updateMessage (bool damp) in Node.cpp (for both edge and node variables)

Message Update Rule in Belief Propagation Formulation should be implemented in this user-defined function.
Use a given code for maximum weight matching as a skeleton.

### 3. Weight Transformation
- Edge::transformEdgeWeight () in Edge.cpp (for edge variables)
- Node::transformNodeWeight () in Node.cpp (for node variables)

Weight Transformation Rule derived from Decision Rule in Belief Propagation Formulation should be implemented in these user-defined functions.
Note that original weight is stored in 'weight' variable, transformed weight is sotred in 'tweight' variable for each node and edge.
Use a given code for maximum weight matching as a skeleton.

### 4. Post-Processing
- Master::postProcess () in solver.cpp (for both edge and node varialbes)

Post processing algorithm should be implemented this user-defined function. Here you should utilize the transformed weight (tweight) instead of original weight (weight) of node or edge. 

## Implementation TIPs
### 1.
