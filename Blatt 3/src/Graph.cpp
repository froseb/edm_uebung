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

int Graph::Edge::getCost() {
  return cost;
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
    addEdge(i, getNodeCount()-1, 0);
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
  long long int value = 0;
  for (unsigned int e=0; e<getEdgeCount(); e++) {
    if (getEdge(e).isActive() && getEdge(e).getA() != getNodeCount()-2 && getEdge(e).getB() != getNodeCount()-1) {
      value += getEdge(e).getCost();
    }
  }
  out << value << '\n';

  for (unsigned int e=0; e<getEdgeCount(); e++) {
    if (getEdge(e).isActive() && getEdge(e).getA() != getNodeCount()-2 && getEdge(e).getB() != getNodeCount()-1) {
      out << getEdge(e).getA() << ' ' << getEdge(e).getB() << '\n';
    }
  }

}

// Returns the reduced cost
long long int redCost(Graph::Edge& e, std::vector<long long int>& potential) {
  return e.getCost() + potential[e.getA()] - potential[e.getB()];
}

// Gets the next active node for dijkstra algorithm
unsigned int getNextActive(std::list<unsigned int>& open, std::vector<long long int>& dist) {
  unsigned int res;

  // the next element from the open list
  auto minOpen = open.begin();
  for (auto it = open.begin(); it != open.end(); ++it) {
    if (dist[*it] < dist[*minOpen]) {
      minOpen = it;
    }
  }

  res = *minOpen;
  open.erase(minOpen);

  return res;
}

// pushes the shortest path edges from s to t into resEdges and updates the potential function
void Graph::dijkstra(std::list<unsigned int>& resEdges, std::vector<long long int>& potential){
  // Stores all the distances from s to any node
  std::vector<long long int> dist(getNodeCount(), -1);
  dist[getNodeCount()-2] = 0;

  // Stores the previous edges
  std::vector<long long int> prev(getNodeCount(), -1);

  // Stores open nodes in a list
  std::list<unsigned int> open;
  open.push_back(getNodeCount()-2);

  while (open.size() > 0) {
    Node& active = getNode(getNextActive(open, dist));
    for (unsigned int e : active.getOutEdges()) {
      if (!getEdge(e).isActive() && (dist[active.getId()] + redCost(getEdge(e), potential) < dist[getEdge(e).getB()] || dist[getEdge(e).getB()] == -1)) {
        if (dist[getEdge(e).getB()] == -1) {
          open.push_back(getEdge(e).getB());
        }
        prev[getEdge(e).getB()] = e;
        dist[getEdge(e).getB()] = dist[active.getId()] + redCost(getEdge(e), potential);
      }
    }
    for (unsigned int e : active.getInEdges()) {
      if (getEdge(e).isActive() && (dist[active.getId()] - redCost(getEdge(e), potential) < dist[getEdge(e).getA()] || dist[getEdge(e).getA()] == -1)) {
        if (dist[getEdge(e).getA()] == -1) {
          open.push_back(getEdge(e).getA());
        }
        prev[getEdge(e).getA()] = e;
        dist[getEdge(e).getA()] = dist[active.getId()] - redCost(getEdge(e), potential);
      }
    }
  }

  // Finished pathfinding here

  // Check if t is reachable
  if (dist[getNodeCount()-1] == -1) {
    throw(std::runtime_error("dijkstra: Failed to find s-t-path. Therefore, there is no perfect matching."));
    return;
  }

  // Write result into resEdges
  unsigned int tmp = getNodeCount()-1;
  while (tmp != getNodeCount()-2) {
    resEdges.push_front(prev[tmp]);
    if (tmp == getEdge(prev[tmp]).getA()) {
      tmp = getEdge(prev[tmp]).getB();
    } else {
      tmp = getEdge(prev[tmp]).getA();
    }
  }

  // Update potential
  for (unsigned int i=0; i<getNodeCount(); i++) {
    if (dist[i] != -1) {
      potential[i] += dist[i];
    } else {
      potential[i] += dist[getNodeCount()-1];
    }
  }
}

void Graph::perfectMatching() {
  std::vector<long long int> potential(getNodeCount(), 0);
  // Set up initial potential
  for (unsigned int e=0; e<getEdgeCount(); e++) {
    if (getEdge(e).getCost() < potential[getEdge(e).getB()] && getEdge(e).getB() != getNodeCount()-1) {
      potential[getEdge(e).getB()] = getEdge(e).getCost();
    }
  }
  for (unsigned int e=0; e<getEdgeCount(); e++) {
    if (getEdge(e).getB() == getNodeCount()-1 && potential[getEdge(e).getA()] < potential[getNodeCount()-1]) {
      potential[getNodeCount()-1] = potential[getEdge(e).getA()];
    }
  }

  std::list<unsigned int> resEdges;

  while (getNode(getNodeCount()-2).getOutFlow() != (getNodeCount()-2)/2) {
    dijkstra(resEdges, potential);
    for (unsigned int e : resEdges) {
      setActive(getEdge(e), !getEdge(e).isActive());
    }
    resEdges.clear();
  }
}
