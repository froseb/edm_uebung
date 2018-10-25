// Implementation of Kruskal algorithm

#include <iostream>
#include <vector>

class Edge {
public:
  Edge(unsigned int a, unsigned int b, unsigned int cost);
  unsigned int getA();
  unsigned int getB();
  unsigned int getCost();
private:
  unsigned int a;
  unsigned int b;
  unsigned int cost;
};

// Class for a graph
class Graph {
public:
  Graph(unsigned int nodeCount);
  void addEdge(unsigned int a, unsigned int b, unsigned int cost);
  unsigned int getNodeCount();
  friend std::ostream& operator<<(std::ostream& out, const Graph& o);
private:
  unsigned int nodeCount;
  std::vector<Edge> edges;
};

// Constuctor, initializes the edge
Edge::Edge(unsigned int a, unsigned int b, unsigned int cost) {
  // a should be smaller than be
  if (a <= b) {
    this->a = a;
    this->b = b;
  } else {
    this->a = b;
    this->b = a;
  }

  this->cost = cost;
}

unsigned int Edge::getA() {
  return this->a;
}

unsigned int Edge::getB() {
  return this->b;
}

unsigned int Edge::getCost() {
  return this->cost;
}

// Constructor, initializes the graph
Graph::Graph(unsigned int nodeCount) {
  this->nodeCount = nodeCount;
  this->edges = std::vector<Edge>();
}

// Adds an edge to the graph
void Graph::addEdge(unsigned int a, unsigned int b, unsigned int cost) {
  Edge e = Edge(a, b, cost);
  edges.push_back(e);
}

// Returns the node count of the graph
unsigned int Graph::getNodeCount() {
  return this->nodeCount;
}

std::ostream& operator<<(std::ostream& out, const Graph& o) {
  out << "Node count:" << o.nodeCount << "\n";
  for (Edge e : o.edges) {
    out << "Edge from \t" << e.getA() << "\t to \t" << e.getB() << "\t with cost \t" << e.getCost();
  }
  return out;
}

int main() {
  Graph g = Graph(2);
  g.addEdge(1, 0, 100);

  std::cout << g << '\n';
}
