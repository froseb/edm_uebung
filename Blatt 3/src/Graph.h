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

  void setActive(Edge& e, bool active);
  void addOutEdge(Node& n, unsigned int e);
  void addInEdge(Node& n, unsigned int e);

  // An edge in the graph between two nodes
  class Edge {
  public:
    Edge(unsigned int a, unsigned int b, unsigned int id, int cost);
    unsigned int getId();
    unsigned int getA();
    unsigned int getB();
    bool isActive();
    int getCost();
    friend void Graph::setActive(Edge& e, bool active);
  private:
    unsigned int a;
    unsigned int b;
    unsigned int id;
    int cost;
    bool active;
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
    unsigned int getOutFlow();
    unsigned int getInFlow();
    friend void Graph::setActive(Edge& e, bool active);
  private:
    unsigned int id;
    unsigned int inFlow = 0;
    unsigned int outFlow = 0;
    std::vector<unsigned int> inEdges;
    std::vector<unsigned int> outEdges;
  };

  Graph(unsigned int nodeCount);
  Graph(std::string filename);
  void addEdge(unsigned int a, unsigned int b, int cost);
  unsigned int getNodeCount();
  unsigned int getEdgeCount();
  Node& getNode(unsigned int a);
  Edge& getEdge(unsigned int a);
  void exportMatching(std::ostream& out);
  friend std::ostream& operator<<(std::ostream& out, const Graph& o);
private:
  unsigned int nodeCount;
  std::vector<Node> nodes;
  std::vector<Edge> edges;
};

#endif
