// Implementation of Kruskal algorithm

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

// Data for a union find tree element
class UnionFindData {
public:
  UnionFindData(unsigned int prev);
  unsigned int getPrev();
  unsigned int getRank();
  void setPrev(unsigned int a);
  void incrRank();
private:
  unsigned int prev;
  unsigned int rank;
};

// Union find data structure
class UnionFind {
public:
  UnionFind(unsigned int size);
  unsigned int find(unsigned int a);
  void un(unsigned int a, unsigned int b);
private:
  std::vector<UnionFindData> elements;
};

// Class for an edge
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
  Graph(std::string filename);
  void addEdge(unsigned int a, unsigned int b, unsigned int cost);
  void addEdge(Edge e);
  unsigned int getNodeCount();
  void sortEdges();
  Graph kruskal();
  friend std::ostream& operator<<(std::ostream& out, const Graph& o);
private:
  unsigned int nodeCount;
  std::vector<Edge> edges;
};

UnionFindData::UnionFindData(unsigned int prev) {
  this->prev = prev;
  this->rank = 0;
}

unsigned int UnionFindData::getPrev() {
  return prev;
}

unsigned int UnionFindData::getRank() {
  return rank;
}

void UnionFindData::setPrev(unsigned int a) {
  prev = a;
}

void UnionFindData::incrRank() {
  rank++;
}

UnionFind::UnionFind(unsigned int size) {
  elements = std::vector<UnionFindData>();
  for (unsigned int i = 0; i<size; i++) {
    UnionFindData data(i);
    elements.push_back(data);
  }
}

// find function
unsigned int UnionFind::find(unsigned int a) {
  unsigned int v = a;
  if (elements[v].getPrev() != v) {
    elements[v].setPrev(find(elements[v].getPrev()));
  }
  return elements[v].getPrev();
}

// union function
void UnionFind::un(unsigned int a, unsigned int b) {
  // root elements
  unsigned int rA = find(a);
  unsigned int rB = find(b);

  if (elements[rA].getRank() > elements[rB].getRank()) {
    elements[rB].setPrev(rA);
  } else {
    elements[rA].setPrev(rB);
    if (elements[rA].getRank() == elements[rB].getRank()) {
      elements[rB].incrRank();
    }
  }
}

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
  return a;
}

unsigned int Edge::getB() {
  return b;
}

unsigned int Edge::getCost() {
  return cost;
}

// Constructor, initializes the graph
Graph::Graph(unsigned int nodeCount) {
  this->nodeCount = nodeCount;
  this->edges = std::vector<Edge>();
}

// Parses a graph file and constructs the graph
Graph::Graph(std::string filename) {
  std::fstream file(filename, std::ios_base::in);

  // Set Node Count of the Graph
  unsigned int nodeCount;
  file >> nodeCount;
  this->nodeCount = nodeCount;

  unsigned int a, b, cost;
  while (file >> a >> b >> cost) {
    this->addEdge(a, b, cost);
  }
}

// Adds an edge to the graph
void Graph::addEdge(unsigned int a, unsigned int b, unsigned int cost) {
  Edge e = Edge(a, b, cost);
  edges.push_back(e);
}

void Graph::addEdge(Edge e) {
  edges.push_back(e);
}

// Returns the node count of the graph
unsigned int Graph::getNodeCount() {
  return nodeCount;
}

bool cmp(Edge a, Edge b) {
  return a.getCost() < b.getCost();
}

// sorts the edges of the graph by cost
void Graph::sortEdges() {
  std::sort(edges.begin(), edges.end(), cmp);
}

// custom output for graphs (for printing)
std::ostream& operator<<(std::ostream& out, const Graph& o) {
  out << "Node count: " << o.nodeCount << "\n";
  for (Edge e : o.edges) {
    out << "{" << e.getA() << ", " << e.getB() << "}\t with cost \t" << e.getCost() << "\n";
  }
  return out;
}

// Kruskal algorithm to find a MST
Graph Graph::kruskal() {

  // resulting graph
  Graph res(getNodeCount());

  // union find object
  UnionFind uf(getNodeCount());

  // sort edges by cost
  sortEdges();
  for (Edge e : edges) {
    if (uf.find(e.getA()) != uf.find(e.getB())) {
      res.addEdge(e);
      uf.un(e.getA(), e.getB());
    }
  }

  return res;
}

int main() {
  std::string filename;
  std::string outputfile;

  std::cout << "Please enter a source filename:" << '\n';
  std::cin >> filename;
  std::cout << "Please enter an output filename or \"c\" for console output:" << '\n';
  std::cin >> outputfile;

  Graph g(filename);

  Graph res = g.kruskal();

  if (outputfile == "c") {
    std::cout << res << '\n';
  } else {
    std::fstream file(outputfile, std::ios_base::out);
    file << res;
  }
}
