// Implementation of Kruskal algorithm
// Requires c++11 standard for compiling

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
  Edge(unsigned int a, unsigned int b, int cost);
  unsigned int getA();
  unsigned int getB();
  int getCost();
private:
  unsigned int a;
  unsigned int b;
  int cost;
};

// Class for a graph
class Graph {
public:
  Graph();
  Graph(unsigned int nodeCount);
  Graph(std::string filename);
  void addEdge(unsigned int a, unsigned int b, int cost);
  void addEdge(Edge e);
  unsigned int getNodeCount();
  void initGraph(unsigned int nc);
  unsigned int getEdgeCount();
  long long int getWeight();
  void sortEdges();
  void kruskal(Graph& res);
  friend std::ostream& operator<<(std::ostream& out, const Graph& o);
private:
  unsigned int nodeCount;
  std::vector<Edge> edges;
  long long int weight;
};

UnionFindData::UnionFindData(unsigned int pr) {
  prev = pr;
  rank = 0;
}

// returns the previous element of the tree element
unsigned int UnionFindData::getPrev() {
  return prev;
}

// returns the rank of the tree element
unsigned int UnionFindData::getRank() {
  return rank;
}

// sets the previous element of the union find tree
void UnionFindData::setPrev(unsigned int a) {
  prev = a;
}

// increases the rank of the data object
void UnionFindData::incrRank() {
  rank++;
}

UnionFind::UnionFind(unsigned int size) {
  elements.reserve(size);
  // for every number from 0 to size-1, store a data object
  for (unsigned int i = 0; i<size; i++) {
    UnionFindData data(i);
    elements.push_back(data);
  }
}

// find function
unsigned int UnionFind::find(unsigned int a) {
  if (elements[a].getPrev() != a) {
    elements[a].setPrev(find(elements[a].getPrev()));
  }
  return elements[a].getPrev();
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
Edge::Edge(unsigned int pa, unsigned int pb, int c) {
  // a should be smaller than be
  if (pa <= pb) {
    a = pa;
    b = pb;
  } else {
    a = pb;
    b = pa;
  }

  cost = c;
}

// Returns the first connected node
unsigned int Edge::getA() {
  return a;
}

// Returns the second connected node
unsigned int Edge::getB() {
  return b;
}

// Returns the cost of the edge
int Edge::getCost() {
  return cost;
}

Graph::Graph() {
  nodeCount = 0;
  weight = 0;
}

// Constructor, initializes the graph
Graph::Graph(unsigned int nc) {
  nodeCount = nc;
  weight = 0;
}

// Parses a graph file and constructs the graph
Graph::Graph(std::string filename) {
  std::fstream file(filename, std::ios_base::in);

  // Check if file is open
  if (!file.is_open()) {
    nodeCount = 0;
    weight = 0;
    std::cout << "Input file does not exist." << '\n';
    return;
  }

  // Set Node Count of the Graph
  unsigned int nc;
  file >> nc;
  nodeCount = nc;

  weight = 0;

  // Parses edges from the file
  unsigned int a, b, cost;
  while (file >> a >> b >> cost) {
    addEdge(a, b, cost);
  }
}

// Adds an edge to the graph
void Graph::addEdge(unsigned int a, unsigned int b, int cost) {
  if (a < getNodeCount() && b < getNodeCount()) {
    Edge e(a, b, cost);
    edges.push_back(e);
    weight += cost;
  } else {
    std::cout << "Wrong use of addEdge." << '\n';
  }
}

// Adds an edge to the graph
void Graph::addEdge(Edge e) {
  if (e.getA() < getNodeCount() && e.getB() < getNodeCount()) {
    edges.push_back(e);
    weight += e.getCost();
  } else {
    std::cout << "Wrong use of addEdge." << '\n';
  }
}

// Returns the node count of the graph
unsigned int Graph::getNodeCount() {
  return nodeCount;
}

// Sets the node Count if the graph is empty
void Graph::initGraph(unsigned int nc) {
  if (getNodeCount() == 0) {
    nodeCount = nc;
  } else {
    std::cout << "Wrong use of setNodeCount." << '\n';
  }
}

// Returns the edge count of the graph
unsigned int Graph::getEdgeCount() {
  return edges.size();
}

// Returns the sum of all edge cost values
long long int Graph::getWeight() {
  return weight;
}

// Compares the cost values of two edges
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
  out << "Weight: " << o.weight << "\n";
  for (Edge e : o.edges) {
    out << "{" << e.getA() << ", " << e.getB() << "}\t with cost \t" << e.getCost() << "\n";
  }
  return out;
}

// Makes res an MST of the graph using kruskal algorithm with union find data structure
// res has to be empty
void Graph::kruskal(Graph& res) {
  res.initGraph(getNodeCount());
  UnionFind uf(getNodeCount());

  sortEdges();

  for (Edge e : edges) {
    if (uf.find(e.getA()) != uf.find(e.getB())) {
      res.addEdge(e);
      uf.un(e.getA(), e.getB());
    }

    // if res is a tree, we are done
    if(res.getEdgeCount() == getNodeCount()-1) {
      return;
    }
  }
}

int main(int argc, char** argv) {
  std::string filename;
  std::string outputfile;

  if (argc <= 1) {
    std::cout << "Please enter a source filename:" << '\n';
    std::cin >> filename;
  } else {
    filename.append(argv[1]);
  }

  std::cout << "Please enter an output filename or \"c\" for console output:" << '\n';
  std::cin >> outputfile;

  Graph g(filename);

  if (g.getNodeCount() == 0) {
    return 0;
  }

  Graph res;
  g.kruskal(res);

  if (outputfile == "c") {
    if (res.getEdgeCount() != res.getNodeCount()-1) {
      std::cout << "The graph is not connected." << "\n";
    } else {
      std::cout << res << '\n';
    }
  } else {
    if (res.getEdgeCount() != res.getNodeCount()-1) {
      std::cout << "The graph is not connected." << "\n";
    } else {
      std::fstream file(outputfile, std::ios_base::out);
      file << res;
    }
  }

  return 0;
}
