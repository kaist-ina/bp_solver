#include "Edge.h"
#include <iostream>
#include "config.h"

#if VAR_EDGE
Edge::Edge (Node *s, Node *d, double w, double n){
	this->src = s;
	this->dst = d;
	
	this->weight = w;
	this->tweight = 0;
	this->noise = n;

	this->s_to_d = 0.5 * w;
	this->d_to_s = 0.5 * w;

	this->decision = 0;
}
#endif

#if VAR_NODE
Edge::Edge (Node *s, Node *d){
	this->src = s;
	this->dst = d;

	this->s_to_d = 0;
	this->d_to_s = 0;
}
#endif

#if VAR_EDGE
// TODO: USER DEFINED FUNCTION: Initialize Message
void Edge::initEdgeMessage (){
	/* Maximum Weight Matching */
	s_to_d = 0.5 * weight;
	d_to_s = 0.5 * weight;

}

// TODO: USER DEFINED FUNCTION: Edge Weight Transform Function
void Edge::transformEdgeWeight (){
	//** Here goes the implementation for transforming edge weight function **//	

	// Maximum Weight Matching
	tweight = weight - (s_to_d + d_to_s);
}
#endif
