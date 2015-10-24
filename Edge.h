#ifndef __EDGE_H__
#define __EDGE_H__
#include "config.h"

using namespace std;
class Node;

class Edge{
public:
	Node *src;
	Node *dst;
#if VAR_EDGE
	double weight;
	double tweight;	// transformed weight
	double noise;
	char decision;
#endif
	double s_to_d;
	double d_to_s;

#if VAR_EDGE
	Edge (Node *s, Node *d, double w, double n);
#endif
#if VAR_NODE
	Edge (Node *s, Node *d);
#endif
#if VAR_EDGE
	void initEdgeMessage ();
	double getWeight()
	{return (weight + noise);}
#endif
	void transformEdgeWeight ();
};
#endif
