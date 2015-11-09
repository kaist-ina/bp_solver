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
#if !ASYNC
	double new_s_to_d;
	double new_d_to_s;
#endif

#if VAR_EDGE
	Edge (Node *s, Node *d, double w, double n);
#endif
#if VAR_NODE
	Edge (Node *s, Node *d);
#endif
#if VAR_EDGE
	void initEdgeMessage ();
	void updateDtoS (double new_msg)
	{
#if ASYNC
	d_to_s = new_msg;
#else
	new_d_to_s = new_msg;
#endif
	}
	void updateStoD (double new_msg)
	{
#if ASYNC
	s_to_d = new_msg;
#else
	new_s_to_d = new_msg;
#endif
	}
	double getWeight()
	{return (weight + noise);}
#endif
	void transformEdgeWeight ();
};
#endif
