/*
 * Implementation of the Goldberg-Tarjan Push-Relabel algorithm
 */

#include <iostream>
#include <vector>
#include <fstream>

// Class for a graph
class Graph {
public:
  class Edge;
  class Node;

  // An edge in the graph between two nodes
  class Edge {
  public:
    Edge(Node *a, Node *b, unsigned int capacity);
    Node *getA();
    Node *getB();
    unsigned int getCapacity();
    unsigned int getFlow();
    void setFlow(unsigned int fl);
  private:
    Node *a;
    Node *b;
    unsigned int capacity;
    unsigned int flow = 0;
  };

  // A node in the graph
  class Node {
  public:
    Node();
    void addOutEdge(Edge *e);
    void addInEdge(Edge *e);
    std::vector<Edge *> getOutEdges();
    std::vector<Edge *> getInEdges();
    void relabel();
    void setLabel(unsigned int label);
    unsigned int getLabel();
    friend void Edge::setFlow(unsigned int fl);
  private:
    unsigned int label = 0;
    unsigned long long int inFlow = 0;
    unsigned long long int outFlow = 0;
    std::vector<Edge *> inEdges;
    std::vector<Edge *> outEdges;
  };

  Graph(unsigned int nodeCount);
  Graph(std::string filename);
  void addEdge(Node *a, Node *b, unsigned int capacity);
  unsigned int getNodeCount();
  unsigned int getEdgeCount();
  Node& getNode(unsigned int a);
  Edge& getEdge(unsigned int a);
  void pushRelabel();
  friend std::ostream& operator<<(std::ostream& out, const Graph& o);
private:
  unsigned int nodeCount;
  std::vector<Node> nodes;
  std::vector<Edge> edges;
};

Graph::Edge::Edge(Graph::Node *a, Graph::Node *b, unsigned int capacity) {
  this->a = a;
  this->b = b;
  this->capacity = capacity;
}

Graph::Node *Graph::Edge::getA() {
  return a;
}

Graph::Node *Graph::Edge::getB() {
  return b;
}

unsigned int Graph::Edge::getCapacity() {
  return capacity;
}

unsigned int Graph::Edge::getFlow() {
  return flow;
}

void Graph::Edge::setFlow(unsigned int fl) {
  unsigned int oldflow = fl;
  flow = fl;
  getA()->outFlow -= oldflow;
  getA()->outFlow += fl;
  getB()->inFlow -= oldflow;
  getB()->outFlow += fl;
}

Graph::Node::Node() { }

void Graph::Node::addOutEdge(Edge *e) {
  outEdges.push_back(e);
  outFlow += e->getFlow();
}

void Graph::Node::addInEdge(Edge *e) {
  inEdges.push_back(e);
  inFlow += e->getFlow();
}

// Gets the edges going out
std::vector<Graph::Edge *> Graph::Node::getOutEdges() {
  return outEdges;
}

// Gets the edges going in
std::vector<Graph::Edge *> Graph::Node::getInEdges() {
  return inEdges;
}

unsigned int Graph::Node::getLabel() {
  return label;
}

void Graph::Node::setLabel(unsigned int label) {
  this->label = label;
}

// Constructor, initializes the graph
Graph::Graph(unsigned int nc) {
  nodeCount = nc;

  // Add node objects to nodes vector
  nodes.reserve(nc);
  for (unsigned int i = 0; i < nc; i++) {
    nodes.push_back(Node());
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
  nodeCount = nc;

  // Add node objects to nodes vector
  nodes.reserve(nc);
  for (unsigned int i = 0; i < nc; i++) {
    nodes.push_back(Node());
  }

  // Parses edges from the file
  unsigned int a, b, capacity;
  while (file >> a >> b >> capacity) {
    addEdge(&getNode(a), &getNode(b), capacity);
  }
}

// Adds an edge to the graph
void Graph::addEdge(Graph::Node *a, Graph::Node *b, unsigned int capacity) {
  Edge e(a, b, capacity);
  edges.push_back(e);
  a->addOutEdge(&edges.back());
  b->addInEdge(&edges.back());
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

// Push relabel algorithm
void Graph::pushRelabel() {
  Node& s = getNode(0);
  Node& t = getNode(1);

  s.setLabel(getNodeCount());

  // Maximize flow of edges adjacent to s
  for (Edge *e : s.getOutEdges()) {
    e->setFlow(e->getCapacity());
  }
}

// Main function
int main() {
  std::string filename;

  std::cout << "Please enter an input filename" << '\n';
  std::cin >> filename;

  Graph g(filename);

  return 0;
}
