#include <iostream>
#include "Node.h"
#include "Edge.h"
#include "config.h"

#define MAX(A,B) (A>B?A:B)

#if VAR_NODE
Node::Node (double w, double n){
	decision = 0;
	weight = w;
	noise = n;
}
#endif

#if VAR_NODE
// TODO: USER DEFINED FUNCTION: Node Weight Transform Function
void Node::transformNodeWeight (){
//** Here goes the user defined function for transforming node weights **//
	/* Minimum Vertex Cover */
	/*
	double sum = 0.0;
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		sum += e->d_to_s;
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		sum += e->s_to_d;
	}

	tweight = getWeight () + sum;
	*/

	// Maximum Independent Set
	/*
	double sum = 0.0;
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		sum += e->d_to_s;
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		sum += e->s_to_d;
	}	

	tweight = getWeight () - sum;
	*/
}

// TODO: USER DEFINED FUNCTION: Initializing Message for Node variables
void Node::initNodeMessage (){
	/* Minimum Vertex Cover */
	/*
	int limit;
	int degree = getDegree ();
	double initMsg = - (getWeight()/(double)degree);
	
	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		e->d_to_s = initMsg;
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		e->s_to_d = initMsg;
	}
	
	*/

	// Maximum Weight Independent Set	
	/*
	int degree = getDegree ();
	double initMsg = (getWeight()/(double)degree);
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		e->d_to_s = initMsg;
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		e->s_to_d = initMsg;
	}	
	*/
}
#endif

void Node::addInEdge (Edge *e){
	inedgeList.push_back (e);
}

void Node::addOutEdge (Edge *e){
	outedgeList.push_back (e);
}


void Node::initMessage (){
#if VAR_NODE
	initNodeMessage ();
#endif
#if VAR_EDGE
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		e->initEdgeMessage ();
	}	
#endif
}


// TODO: USER DEFINED Function: Message Update Function
void Node::updateMessage (bool damp){

/* Maximum Weight Matching */
	Edge *max_edge = NULL;
	double max = -1;
	double secondMax = 0;
	double tmp;
	double new_val;
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		tmp = e->getWeight() - e->d_to_s;

		if (tmp >= max){
			secondMax = max;
			max = tmp;
			max_edge = e;
		}else if (tmp > secondMax)
			secondMax = tmp;
	}

	limit = inedgeList.size();
	for (int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		tmp = e->getWeight() - e->s_to_d;

		if (tmp >= max){
			secondMax = max;
			max = tmp;
			max_edge = e;
		}else if (tmp > secondMax)
			secondMax = tmp;
	}

	if (max < 0)
		max = 0;
	if (secondMax < 0)
		secondMax = 0;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		if (DAMPING && damp){
			new_val = 0.5*e->s_to_d + 0.5*((e!=max_edge)?max:secondMax);
			e->updateStoD (new_val);
		}else {
			new_val = ((e!=max_edge)?max:secondMax);
			e->updateStoD (new_val);
		}
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		if (DAMPING && damp){
			new_val = 0.5*e->d_to_s + 0.5*((e!=max_edge)?max:secondMax);
			e->updateDtoS (new_val);
		}else {
			new_val = ((e!=max_edge)?max:secondMax);
			e->updateDtoS (new_val);
		}
	}

/* Minimum Weight Vertex Cover */
/*
	double sum=0.0;
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		sum += e->d_to_s;
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		sum += e->s_to_d;
	}

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		double tmp = - getWeight () - sum + e->d_to_s;
		if(tmp > 0.0) tmp = 0.0;
		if (DAMPING && damp)
			e->s_to_d = 0.5*e->s_to_d + 0.5*tmp;
		else
			e->s_to_d = tmp;
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		double tmp = - getWeight () - sum + e->s_to_d;
		if (tmp > 0.0) tmp = 0.0;
		if (DAMPING && damp)
			e->d_to_s = 0.5*e->d_to_s + 0.5*tmp;
		else
			e->d_to_s = tmp;
	}
*/

/* Maximum Weight Independent Set */
/*
	double sum=0.0;
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		sum += e->d_to_s;
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		sum += e->s_to_d;
	}

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		double tmp =  getWeight () - sum + e->d_to_s;
		if(tmp < 0.0) tmp = 0.0;
		if (DAMPING && damp)
			e->s_to_d = 0.5*e->s_to_d + 0.5*tmp;
		else
			e->s_to_d = tmp;
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		double tmp =  getWeight () - sum + e->s_to_d;
		if (tmp < 0.0) tmp = 0.0;
		if (DAMPING && damp)
			e->d_to_s = 0.5*e->d_to_s + 0.5*tmp;
		else
			e->d_to_s = tmp;
	}
*/
}

#if !ASYNC
void Node::passMessages (){
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		e->d_to_s = e->new_d_to_s;
		e->s_to_d = e->new_s_to_d;
	}
}
#endif

int Node::getDegree (){
#if VAR_EDGE
	int degree = 0;
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		if(e->decision == 1)
			degree++;
	}

	limit = inedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = inedgeList[i];
		if(e->decision == 1)
			degree++;
	}
	return degree;
#endif
#if VAR_NODE
	return (outedgeList.size() + inedgeList.size());
#endif
}

void Node::transformWeight (){

#if VAR_NODE
	// Node Weight Transform
	transformNodeWeight ();
#endif

#if VAR_EDGE
	// Edge Weight Transform
	int limit;

	limit = outedgeList.size();
	for(int i=0; i<limit; ++i){
		Edge *e = outedgeList[i];
		e->transformEdgeWeight();
	}
#endif
}
