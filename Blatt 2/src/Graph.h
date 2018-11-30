#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <string>
#include <iostream>

// Class for a graph
class Graph {
public:
  class Edge;
  class Node;

  void setFlow(Edge& e, unsigned int fl);
  void addOutEdge(Node& n, unsigned int e);
  void addInEdge(Node& n, unsigned int e);

  // An edge in the graph between two nodes
  class Edge {
  public:
    Edge(unsigned int a, unsigned int b, unsigned int capacity, unsigned int id);
    unsigned int getId();
    unsigned int getA();
    unsigned int getB();
    unsigned int getCapacity();
    unsigned int getFlow();
    friend void Graph::setFlow(Edge& e, unsigned int fl);
  private:
    unsigned int id;
    unsigned int a;
    unsigned int b;
    unsigned int capacity;
    unsigned int flow = 0;
  };

  // A node in the graph
  class Node {
  public:
    Node(unsigned int id);
    unsigned int getId();
    friend void Graph::addOutEdge(Node& n, unsigned int e);
    friend void Graph::addInEdge(Node& n, unsigned int e);
    std::vector<unsigned int>& getOutEdges();
    std::vector<unsigned int>& getInEdges();
    unsigned long long int getOutFlow();
    unsigned long long int getInFlow();
    friend void Graph::setFlow(Edge& e, unsigned int fl);
  private:
    unsigned int id;
    unsigned long long int inFlow = 0;
    unsigned long long int outFlow = 0;
    std::vector<unsigned int> inEdges;
    std::vector<unsigned int> outEdges;
  };

  Graph(unsigned int nodeCount);
  Graph(std::string filename);
  void addEdge(unsigned int a, unsigned int b, unsigned int capacity);
  unsigned int getNodeCount();
  unsigned int getEdgeCount();
  Node& getNode(unsigned int a);
  Edge& getEdge(unsigned int a);
  void pushRelabel();
  void exportFlow(std::ostream& out);
  friend std::ostream& operator<<(std::ostream& out, const Graph& o);
private:
  unsigned int nodeCount;
  std::vector<Node> nodes;
  std::vector<Edge> edges;
};

#endif
