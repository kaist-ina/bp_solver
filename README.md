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
Input file should be given **edgelist** format. First line should give total number of nodes and total number of edges. 
If node weights exists, they should be specified first where each line should contain node weight.
If edge weights exists, they should be specified after node weight where each line should cantain source node, destination node, and weight.

```
NUMBER_OF_NODES NUMBER_OF_EDGES
WEIGHT_OF_NODE_1
WEIGHT_OF_NODE_2
...
WEIGHT_OF_NODE_N
SOURCE_NODE DESTINATION_NODE WEIGHT_OF_EDGE_1
SOURCE_NODE DESTINATION_NODE WEIGHT_OF_EDGE_2
...
SOURCE_NODE DESTINATION_NODE WEIGHT_OF_EDGE_M
```

Node that source node and destination node should be between 1 and NUMBER_OF_NODES.

If there is no node weight (VAR_NODE = 0 in configuration), you can skip the weights of nodes.

```
NUMBER_OF_NODES NUMBER_OF_EDGES
SOURCE_NODE DESTINATION_NODE WEIGHT_OF_EDGE_1
SOURCE_NODE DESTINATION_NODE WEIGHT_OF_EDGE_2
...
SOURCE_NODE DESTINATION_NODE WEIGHT_OF_EDGE_M
``` 

Similarly, if there is no edge weight (VAR_EDGE = 0 in configuration), you should skip the weights of edes.

```
NUMBER_OF_NODES NUMBER_OF_EDGES
WEIGHT_OF_NODE_1
WEIGHT_OF_NODE_2
...
WEIGHT_OF_NODE_N
SOURCE_NODE DESTINATION_NODE 
SOURCE_NODE DESTINATION_NODE 
...
SOURCE_NODE DESTINATION_NODE 
```


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

Post processing algorithm should be implemented this user-defined function. 
Here you should utilize the transformed weight (tweight) instead of original weight (weight) of node or edge. 
As a result of post-processing, 'decision' should be set appropriately. 
Initially all decision is set to 0.
You can specify the decision -1 for unselected, 0 for undecided, 1 for selected (i.e. x_e in IP formulation is decision).

## Implementation TIPs
### 1. Directed and Undirected Graph
This implementation treat all input graph as directed graph. 
If you need to process undirected graph, your implementation should treat it as undirected graph.
i.e. you can treat in-edges (which is specified in inedgeList in class Node) and out-edges (which is specified in outedgeList in class Node) as connected edges.

### 2. Traversing Nodes and Edges
Class Master holds just list of nodes in 'nodeList'.
Here how to traverse nodes and edges is explained. 

```
for(int i=0; i<nodeN; ++i){
	int limit;
	Node *n = nodeList[i];
	// traversing nodes

	limit = n->outedgeList.size();
	for(int j=0; j<limit; ++j){
		Edge *e = n->outedgeList[j];
		// traversing edges
	}
}
```

Nodes can be traversed simply by looking at nodeList of class Master.
To traverse edges, while traversing nodes by traversing its outedgeList all of the edges can be traversed from the source side.
One can traverse all of the edges from the destination side using 'inedgeList', too.
