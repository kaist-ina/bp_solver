#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include "config.h"

using namespace std;
class Edge;

class Node {
	public:
	vector<Edge *> inedgeList;
	vector<Edge *> outedgeList;

#if VAR_NODE
	double weight;
	double tweight; //transformed weight
	double noise;
	char decision;

	Node (double w, double n);
	void transformNodeWeight ();
	void initNodeMessage ();
	double getWeight ()
	{return (weight+noise);}
#endif

	void addInEdge (Edge *e);
	void addOutEdge (Edge *e);
	void initMessage ();
	void updateMessage (bool damp);
	void transformWeight ();
	int getDegree ();
};


#endif
