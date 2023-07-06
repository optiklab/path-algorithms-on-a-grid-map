#include "edge.h"

Edge::Edge(int fromNodeId, int toNodeId, int weight) : _fromNodeId(fromNodeId), _toNodeId(toNodeId), _weight(weight)
{
}

int Edge::GetToId() const
{
	return _toNodeId;
}

void Edge::SetToId(int id)
{
	_toNodeId = id;
}

int Edge::GetFromId() const
{
	return _fromNodeId;
}

void Edge::SetFromId(int id)
{
	_fromNodeId = id;
}

void Edge::SetWeight(int weight)
{
	_weight = weight;
}

int Edge::GetWeight() const
{
	return _weight;
}