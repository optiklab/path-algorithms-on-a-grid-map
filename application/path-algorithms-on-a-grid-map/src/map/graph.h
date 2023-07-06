#ifndef GRAPH_H
#define GRAPH_H

#include "cell.h"
#include "edge.h"

// We work with a grid. Which is UNORIENTED graph or Cells.

// Adjacency list is the best way to represent such a graph.
typedef std::vector<std::vector<std::shared_ptr<Cell>>> Graph; // Vector[NodeId]->Vector[Neighbour Nodes]

// For Bellman-Ford, however, we need edges list.

// Edges list
typedef std::vector<std::shared_ptr<Edge>> GraphEdgesList; // Vector[NodeId]->Vector[Edges]

#endif
