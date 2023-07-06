#ifndef __Edge_h__
#define __Edge_h__

#include "cellbase.h"

class Edge
{
public:
	Edge(int fromNodeId, int toNodeId, int weight);

	void SetFromId(int id);
	void SetToId(int id);
	void SetWeight(int weight);

	int GetFromId() const;
	int GetToId() const;
	int GetWeight() const;

private:
	int _fromNodeId;
	int _toNodeId;
	int _weight;
};

#endif __Edge_h__
