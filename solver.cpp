#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <list>
#include <cstdlib>
#include <pthread.h>
#include <float.h>
#include "Node.h"
#include "Edge.h"
#include "config.h"

using namespace std;

class Master;
static Master *master;
static unsigned task_number;
static pthread_mutex_t noLock = PTHREAD_MUTEX_INITIALIZER;

class Master {
	public:
	Node **nodeList;
	//list<Node *> pnodeList;
	//list<Edge *> pedgeList;

	unsigned nodeN;
	unsigned edgeN;

	Master() {};
	~Master();

	void createFromFile (const char *fname);
	void initMessage ();
	void oneIteration (bool damp);
	void transformWeight ();
	void postProcess ();
	void printResult ();
	void isMatching ();
};

void Master::createFromFile (const char *fname){
	FILE *fin = fopen (fname, "r");
	unsigned s,d;
	unsigned num_edge=0;
	double weight;
	double noise;

	// Get number of nodes & number of edges
	if(!fscanf (fin, "%u %u\n", &nodeN, &edgeN))
		cout << "ERROR: fail to read input file\n" << endl;

	// Allocate memory for nodes
	nodeList = (Node **) malloc (nodeN * sizeof(Node *));

	srand(time(NULL));

	for(int i=0; i<nodeN; ++i){
	#if VAR_EDGE
		nodeList[i] = new Node ();
	#endif
	#if VAR_NODE
		fscanf (fin, "%lf\n", &weight);
		if (NOISE_NODE > 0.0)
			noise = 2*NOISE_NODE*((double)rand()/(double)RAND_MAX)-NOISE_NODE;
		else
			noise = 0.0;

		nodeList[i] = new Node (weight, noise);
	#endif
		//pnodeList.push_back (nodeList[i]);
	}

	// Create edges
	for(int i=0;i<edgeN;++i){
#if VAR_EDGE
		fscanf (fin, "%u %u %lf\n", &s, &d, &weight);
		if (NOISE_EDGE > 0.0)
			noise = 2*NOISE_EDGE*((double)rand()/(double)RAND_MAX)-NOISE_EDGE;
		else
			noise = 0.0;
		Edge *e = new Edge (nodeList[s-1],nodeList[d-1],weight,noise);
#elif VAR_NODE
		fscanf (fin, "%u %u\n", &s, &d);
		Edge *e = new Edge (nodeList[s-1],nodeList[d-1]);
#endif
		nodeList[s-1]->addOutEdge(e);
		nodeList[d-1]->addInEdge(e);		

		num_edge++;
		//pedgeList.push_back (e);
	}

	if (edgeN != num_edge){
		cout << "WARNING: actual number of edges is different\n" << endl;
		edgeN = num_edge;
	}
	
	fclose (fin);
}

void Master::initMessage () {
	for (int i=0; i<nodeN; ++i)
		nodeList[i]->initMessage();
}

void Master::oneIteration (bool damp){
	for (int i=0; i<nodeN; ++i)
		nodeList[i]->updateMessage(damp);

#if !ASYNC
	for (int i=0; i<nodeN; ++i)
		nodeList[i]->passMessages ();
#endif
}

void Master::transformWeight (){
	for (int i=0; i<nodeN; ++i)
		nodeList[i]->transformWeight();
}

#if VAR_EDGE
bool compare_edges (const Edge *e1, const Edge *e2){
	return (e1->tweight < e2->tweight);
}
#endif

#if VAR_NODE
bool compare_nodes (const Node *n1, const Node *n2){
	return (n1->tweight > n2->tweight);
}
#endif

//** USER DEFINED FUNCTION: post process function  **//
void Master::postProcess (){
	/* Maximum Weighted Matching */

// local greedy algorithm
	for(int i=0;i<nodeN;++i){
		Node *n = nodeList[i];
		Edge *max_e = NULL;
		double max_w = 0.0;
		int limit;

		limit = n->outedgeList.size();
		for(int j=0;j<limit;++j){
			Edge *e = n->outedgeList[j];
			if(e->decision == -1)
				continue;

			if(e->tweight > max_w){
				max_w = e->tweight;
				max_e = e;
			}

			if(e->decision == 1){
				max_w = DBL_MAX;
				max_e = e;
			}
			e->decision = -1;
		}

		limit = n->inedgeList.size();
		for(int j=0;j<limit;++j){
			Edge *e = n->inedgeList[j];
			if(e->decision == -1)
				continue;
			if(e->tweight > max_w){
				max_w = e->tweight;
				max_e = e;
			}
			if(e->decision == 1){
				max_w = DBL_MAX;
				max_e = e;
			}
			e->decision = -1;
		}

		if (max_e && max_e->weight > 0)
			max_e->decision = 1;

	}

/* Minimum Vertex Cover */

// global greedy algorithm
/*
	pnodeList.sort (compare_nodes);
	while(!pnodeList.empty ()){
		int limit;
		bool isCovered = true;
		Node *n = pnodeList.front ();
		pnodeList.pop_front ();

		limit = n->outedgeList.size();
		for(int i=0;i<limit;++i){
			Edge *e = n->outedgeList[i];
			if (e->src->decision == 0 && e->dst->decision == 0){
				isCovered = false;
				n->decision = 1;
				break;
			}	
		}

		if(!isCovered)
			continue;
		
		limit = n->inedgeList.size();
		for(int i=0;i<limit;++i){
			Edge *e = n->inedgeList[i];
			if(e->src->decision == 0 && e->dst->decision == 0){
				isCovered = false;
				n->decision = 1;
				break;
			}
		}

	}
*/

// 2-approx algorithm
/*
	pedgeList.sort (compare_edges);
	while(!pedgeList.empty ()){
		Edge *e = pedgeList.front ();
		pedgeList.pop_front();

		if(e->src->decision == 1 || e->dst->decision == 1)
			continue;
		
		e->src->decision = 1;
		e->dst->decision = 1;		
	}
*/

/* Maximum Independent Set */

// global greedy algorithm
/*
	int limit;
	bool alreadyCovered;

	pnodeList.sort (compare_nodes);
	while(!pnodeList.empty()){
		Node *n = pnodeList.front();
		pnodeList.pop_front ();
		alreadyCovered = false;

		limit = n->outedgeList.size();
		for(int i=0; i<limit; ++i){
			Edge *e = n->outedgeList[i];
			if (e->dst->decision == 1)
				alreadyCovered = true;
		}

		limit = n->inedgeList.size();
		for(int i=0; i<limit; ++i){
			Edge *e = n->inedgeList[i];
			if (e->src->decision == 1)
				alreadyCovered = true;
		}

		if (alreadyCovered)
			n->decision = -1;
		else
			n->decision = 1;		
	}
*/
}

void Master::printResult (){
#if VAR_NODE
	double sum_node = 0.0;
#endif
#if VAR_EDGE
	double sum_edge = 0.0;
#endif
	int limit;
	for(int i=0; i<nodeN; ++i){
		Node *n = nodeList[i];
#if VAR_NODE
		if (n->decision == 1)
			sum_node += n->weight;
#endif
#if VAR_EDGE
		limit = n->outedgeList.size();
		for(int j=0; j<limit; ++j){
			Edge *e = n->outedgeList[j];
			if (e->decision == 1){
				sum_edge += e->weight;
			}
		}
#endif
	}
#if VAR_NODE
	cout << "Sum of Vertex Weight = " << sum_node << endl;
#endif
#if VAR_EDGE
	cout << "Sum of Edge Weight = " << sum_edge << endl;
#endif

}

void Master::isMatching (){
	int count;
	int limit;

	for(int i=0; i<nodeN; ++i){
		Node *n = nodeList[i];
		count = 0;

		limit = n->inedgeList.size();
		for(int j=0; j<limit; ++j){
			Edge *e = n->inedgeList[j];
			if (e->decision == 1)
				count ++;
		}

		limit = n->outedgeList.size();
		for (int j=0; j<limit; ++j){
			Edge *e = n->outedgeList[j];
			if (e->decision == 1)
				count ++;
		}

		if (count > 1)
			cout << "ERROR" << endl;
	}
}

void *runThreadUpdate (void *arg_){
	long arg = (long)arg_;
	unsigned node_start;
	unsigned node_end;
	unsigned local_task_no;
	unsigned set;
	unsigned iter;

	while (true){
		pthread_mutex_lock (&noLock);
		local_task_no = ++task_number;
		pthread_mutex_unlock(&noLock);

		set = local_task_no % N_THREAD;
		iter = local_task_no / N_THREAD;

		if (iter >= N_ITER)
			break;

		node_start = master->nodeN * (set / (float)N_THREAD);
		node_end = master->nodeN * ((set+1) / (float)N_THREAD);
		for(int i=node_start; i<node_end; ++i){
			master->nodeList[i]->updateMessage(iter>=(N_ITER/2));
		}
	}
}

#if !ASYNC
void *runThreadPass (void *arg_){
	long arg = (long)arg_;
	unsigned node_start;
	unsigned node_end;
	unsigned local_task_no;
	unsigned set;
	unsigned iter;

	while (true){
		pthread_mutex_lock (&noLock);
		local_task_no = ++task_number;
		pthread_mutex_unlock(&noLock);

		set = local_task_no % N_THREAD;
		iter = local_task_no / N_THREAD;

		if (iter >= N_ITER)
			break;

		node_start = master->nodeN * (set / (float)N_THREAD);
		node_end = master->nodeN * ((set+1) / (float)N_THREAD);
		for(int i=node_start; i<node_end; ++i){
			master->nodeList[i]->passMessages ();
		}
	}
}
#endif

int main (int argc, char *argv[]){
	char finName[1024];
	struct timespec start;
	struct timespec finish;
	double runtime;

	// Decide input file name
	if (argc > 1)
		sprintf(finName, argv[1]);
	else
		sprintf(finName,"input");

	// Create master
	master = new Master();

	// Load graph from input file
	master->createFromFile (finName);
	clock_gettime(CLOCK_MONOTONIC, &start);

	master->initMessage ();

	// Executing BP iterations
	if (N_THREAD > 1){
		pthread_t thread[N_THREAD];

		for (int j=0;j<N_THREAD;++j)
			pthread_create (&thread[j], NULL, runThreadUpdate, (void *)j);

		for (int j=0;j<N_THREAD;++j)
			pthread_join (thread[j], NULL);
#if !ASYNC
		for (int j=0;j<N_THREAD;++j)
			pthread_create (&thread[j], NULL, runThreadPass, (void *)j);

		for (int j=0; j<N_THREAD;++j)
			pthread_join (thread[j], NULL);
#endif
	}else{
		for (int j=0; j<N_ITER; ++j)
			master->oneIteration(j>=(N_ITER/2));
	}

	// transforming weight
	master->transformWeight ();

	master->postProcess ();
	master->isMatching();
	clock_gettime(CLOCK_MONOTONIC, &finish);
	runtime = (finish.tv_sec - start.tv_sec);
	runtime += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	master->printResult ();
	cout << "Elapsed Time = " << runtime << endl;
}
