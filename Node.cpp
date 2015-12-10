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
	list<Edge *>::iterator iter;

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		sum += e->d_to_s;
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
		sum += e->s_to_d;
	}

	tweight = getWeight () + sum;
	*/

	// Maximum Independent Set
	/*
	double sum = 0.0;
	list<Edge *>::iterator iter;

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		sum += e->d_to_s;
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
		sum += e->s_to_d;
	}	

	tweight = getWeight () - sum;
	*/
}

// TODO: USER DEFINED FUNCTION: Initializing Message for Node variables
void Node::initNodeMessage (){
	/* Minimum Vertex Cover */
	/*
	list<Edge *>::iterator iter;
	int degree = getDegree ();
	double initMsg = - (getWeight()/(double)degree);
	
	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		e->d_to_s = initMsg;
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
		e->s_to_d = initMsg;
	}
	
	*/

	// Maximum Weight Independent Set	
	/*
	int degree = getDegree ();
	double initMsg = (getWeight()/(double)degree);
	list<Edge *>::iterator iter;

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		e->d_to_s = initMsg;
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
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
	list<Edge *>::iterator iter;
	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
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
	list<Edge *>::iterator iter;

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		tmp = e->getWeight() - e->d_to_s;

		if (tmp >= max){
			secondMax = max;
			max = tmp;
			max_edge = e;
		}else if (tmp > secondMax)
			secondMax = tmp;
	}

	for (iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
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

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		if (DAMPING && damp){
			new_val = 0.5*e->s_to_d + 0.5*((e!=max_edge)?max:secondMax);
			e->updateStoD (new_val);
		}else {
			new_val = ((e!=max_edge)?max:secondMax);
			e->updateStoD (new_val);
		}
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
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
	list<Edge *>::iterator iter;

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		sum += e->d_to_s;
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
		sum += e->s_to_d;
	}

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		double tmp = - getWeight () - sum + e->d_to_s;
		if(tmp > 0.0) tmp = 0.0;
		if (DAMPING && damp)
			e->s_to_d = 0.5*e->s_to_d + 0.5*tmp;
		else
			e->s_to_d = tmp;
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
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
	list<Edge *>::iterator iter;

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		sum += e->d_to_s;
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
		sum += e->s_to_d;
	}

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		double tmp =  getWeight () - sum + e->d_to_s;
		if(tmp < 0.0) tmp = 0.0;
		if (DAMPING && damp)
			e->s_to_d = 0.5*e->s_to_d + 0.5*tmp;
		else
			e->s_to_d = tmp;
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
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
	list<Edge *>::iterator iter;

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		e->d_to_s = e->new_d_to_s;
		e->s_to_d = e->new_s_to_d;
	}
}
#endif

int Node::getDegree (){
#if VAR_EDGE
	int degree = 0;
	list<Edge *>::iterator iter;

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		if(e->decision == 1)
			degree++;
	}

	for(iter = inedgeList.begin(); iter != inedgeList.end(); ++iter){
		Edge *e = *iter;
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
	list<Edge *>::iterator iter;

	for(iter = outedgeList.begin(); iter != outedgeList.end(); ++iter){
		Edge *e = *iter;
		e->transformEdgeWeight();
	}
#endif
}
