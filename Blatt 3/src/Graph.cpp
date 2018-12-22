#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "Graph.h"

Graph::Edge::Edge(unsigned int a, unsigned int b, unsigned int id, int cost) : a(a), b(b), id(id), cost(cost), active(false) { }

unsigned int Graph::Edge::getId() {
  return id;
}

unsigned int Graph::Edge::getA() {
  return a;
}

unsigned int Graph::Edge::getB() {
  return b;
}

bool Graph::Edge::isActive() {
  return active;
}

void Graph::setActive(Edge& e, bool active) {
  unsigned int oldflow = static_cast<unsigned int>(e.isActive());
  e.active = active;

  Node& a = getNode(e.getA());
  Node& b = getNode(e.getB());

  a.outFlow -= oldflow;
  a.outFlow += active;
  b.inFlow -= oldflow;
  b.inFlow += active;
}

Graph::Node::Node(unsigned int id) : id(id) { }

unsigned int Graph::Node::getId() {
  return id;
}

void Graph::addOutEdge(Graph::Node& n, unsigned int e) {
  n.outEdges.push_back(e);
  n.outFlow += static_cast<unsigned int>(getEdge(e).isActive());
}

void Graph::addInEdge(Graph::Node& n, unsigned int e) {
  n.inEdges.push_back(e);
  n.inFlow += static_cast<unsigned int>(getEdge(e).isActive());
}

// Gets the edges going out
std::vector<unsigned int>& Graph::Node::getOutEdges() {
  return outEdges;
}

// Gets the edges going in
std::vector<unsigned int>& Graph::Node::getInEdges() {
  return inEdges;
}

// Gets the flow out of the edge
unsigned int Graph::Node::getOutFlow() {
  return outFlow;
}

// Gets the flow into the edge
unsigned int Graph::Node::getInFlow() {
  return inFlow;
}

// Constructor, initializes the graph
Graph::Graph(unsigned int nc) {
  nodeCount = nc;

  // Add node objects to nodes vector
  nodes.reserve(nc);
  for (unsigned int i = 0; i < nc; i++) {
    nodes.push_back(Node(i));
  }
}

// Parses a graph file and constructs the graph
Graph::Graph(std::string filename) {
  std::fstream file(filename, std::ios_base::in);

  // Check if file is open
  if (!file.is_open()) {
    nodeCount = 0;
    throw(std::runtime_error("File could not be opened."));
    return;
  }

  // Set Node Count of the Graph
  unsigned int nc;
  file >> nc;
  nodeCount = nc + 2; // id of s: getNodeCount-2, id of t: getNodeCount-1

  // Add node objects to nodes vector
  nodes.reserve(nodeCount);
  for (unsigned int i = 0; i < nodeCount; i++) {
    nodes.push_back(Node(i));
  }

  // Parses edges from the file
  unsigned int a, b;
  int cost;
  while (file >> a >> b >> cost) {
    addEdge(a, b, cost);
  }

  // Adds edges from s to all nodes of the left partition and from all nodes of
  // the right partition to t
  for (unsigned int i=0; i<(getNodeCount()-2)/2; i++) {
    addEdge(getNodeCount()-2, i, 0);
  }
  for (unsigned int i = (getNodeCount()-2)/2; i<getNodeCount()-2; i++) {
    addEdge(getNodeCount()-1, i, 0);
  }
}

// Adds an edge to the graph
void Graph::addEdge(unsigned int a, unsigned int b, int cost) {
  Edge e(a, b, edges.size(), cost);
  edges.push_back(e);
  addOutEdge(getNode(a), e.getId());
  addInEdge(getNode(b), e.getId());
}

// Gets the node count of the graph
unsigned int Graph::getNodeCount() {
  return nodeCount;
}

// Gets the edge count of the graph
unsigned int Graph::getEdgeCount() {
  return edges.size();
}

// Gets a node in the graph
Graph::Node& Graph::getNode(unsigned int a) {
  if (a > getNodeCount()) {
    throw(std::runtime_error("Tried to get non-existing node."));
  }
  return nodes[a];
}

// Gets an edge in the graph
Graph::Edge& Graph::getEdge(unsigned int a) {
  if (a > getEdgeCount()) {
    throw(std::runtime_error("Tried to get non-existing edge."));
  }
  return edges[a];
}

void Graph::exportMatching(std::ostream& out) {
}
