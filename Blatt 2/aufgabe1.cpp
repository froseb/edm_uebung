/*
 * Implementation of the Goldberg-Tarjan Push-Relabel algorithm
 * Requires c++11 standard for compiling
 */

#include <iostream>
#include <vector>
#include <list>
#include <fstream>

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

Graph::Edge::Edge(unsigned int a, unsigned int b, unsigned int capacity, unsigned int id) {
  this->id = id;
  this->a = a;
  this->b = b;
  this->capacity = capacity;
}

unsigned int Graph::Edge::getId() {
  return id;
}

unsigned int Graph::Edge::getA() {
  return a;
}

unsigned int Graph::Edge::getB() {
  return b;
}

unsigned int Graph::Edge::getCapacity() {
  return capacity;
}

unsigned int Graph::Edge::getFlow() {
  return flow;
}

void Graph::setFlow(Edge& e, unsigned int fl) {
  unsigned int oldflow = e.getFlow();
  e.flow = fl;

  Node& a = getNode(e.getA());
  Node& b = getNode(e.getB());

  a.outFlow -= oldflow;
  a.outFlow += fl;
  b.inFlow -= oldflow;
  b.inFlow += fl;
}

Graph::Node::Node(unsigned int id) {
  this->id = id;
}

unsigned int Graph::Node::getId() {
  return id;
}

void Graph::addOutEdge(Graph::Node& n, unsigned int e) {
  n.outEdges.push_back(e);
  n.outFlow += getEdge(e).getFlow();
}

void Graph::addInEdge(Graph::Node& n, unsigned int e) {
  n.inEdges.push_back(e);
  n.inFlow += getEdge(e).getFlow();
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
unsigned long long int Graph::Node::getOutFlow() {
  return outFlow;
}

// Gets the flow into the edge
unsigned long long int Graph::Node::getInFlow() {
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
  nodeCount = nc;

  // Add node objects to nodes vector
  nodes.reserve(nc);
  for (unsigned int i = 0; i < nc; i++) {
    nodes.push_back(Node(i));
  }

  // Parses edges from the file
  unsigned int a, b, capacity;
  while (file >> a >> b >> capacity) {
    addEdge(a, b, capacity);
  }
}

// Adds an edge to the graph
void Graph::addEdge(unsigned int a, unsigned int b, unsigned int capacity) {
  Edge e(a, b, capacity, edges.size());
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

// Push Relabel help functions:

// finds the minimum label of a neighboured node
unsigned int findMinimumLabel(std::vector<unsigned int> labels, Graph& g, Graph::Node& active) {
  unsigned int minimumLabel = 2*g.getNodeCount(); // will be made smaller

  // search minimum label adjacent to the node in residual graph
  for (unsigned int e : active.getOutEdges()) {
    // Check if the edge is in the residual graph
    if (g.getEdge(e).getFlow() < g.getEdge(e).getCapacity()) {
      // Is the label smaller than the ones found?
      if (labels[g.getEdge(e).getB()] < minimumLabel) {
        minimumLabel = labels[g.getEdge(e).getB()];
      }
    }
  }
  for (unsigned int e : active.getInEdges()) {
    // Check if the edge is in the residual graph
    if (g.getEdge(e).getFlow() > 0) {
      // Is the label smaller than the ones found?
      if (labels[g.getEdge(e).getA()] < minimumLabel) {
        minimumLabel = labels[g.getEdge(e).getA()];
      }
    }
  }

  return minimumLabel;
}

// updates the list of allowed edges for a node
void updateAllowedEdges(std::vector<std::list<unsigned int>>& allowedEdges, std::vector<unsigned int>& labels, Graph& g, Graph::Node& active) {
  // clear the list
  allowedEdges[active.getId()].clear();

  for (unsigned int e : active.getOutEdges()) {
    // Check if the edge is in the residual graph
    if (g.getEdge(e).getFlow() < g.getEdge(e).getCapacity()) {
      // check if the edge is allowed
      if (labels[active.getId()] == labels[g.getEdge(e).getB()]+1) {
        allowedEdges[active.getId()].push_back(e);
      }
    }
  }
  for (unsigned int e : active.getInEdges()) {
    // Check if the edge is in the residual graph
    if (g.getEdge(e).getFlow() > 0) {
      // Check if the edge is allowed
      if (labels[active.getId()] == labels[g.getEdge(e).getA()]+1) {
        allowedEdges[active.getId()].push_back(e);
      }
    }
  }
}

// pushes the flow along an allowed edge
void pushAllowedEdge(std::vector<std::list<unsigned int>>& allowedEdges, std::vector<unsigned int>& labels, std::vector<std::list<unsigned int>>& labelBucket, unsigned int maxLabel, Graph& g, Graph::Node& active, Graph::Edge& allowed) {
  // The excess of the active node
  unsigned int excess = active.getInFlow() - active.getOutFlow();

  // Which kind of residual edge?
  if (active.getId() == allowed.getA()) {
    // Is the push saturating? Has the active state to be updated?
    if (excess > allowed.getCapacity()-allowed.getFlow()) {
      g.setFlow(allowed, allowed.getCapacity());
      // The edge does not exist any more in the residual graph
      allowedEdges[active.getId()].pop_front();
    } else {
      g.setFlow(allowed, allowed.getFlow() + excess);
      // The node is no longer active
      labelBucket[maxLabel].pop_front();
    }
    // We created a new active node if allowed is not connected to t
    if (allowed.getB() > 1) {
      labelBucket[labels[allowed.getB()]].push_back(allowed.getB());
    }
  } else {
    // Is the push saturating? Has the active state to be updated?
    if (excess > allowed.getFlow()) {
      g.setFlow(allowed, 0);
      // The edge does not exist any more in the residual graph
      allowedEdges[active.getId()].pop_front();
    } else {
      g.setFlow(allowed, allowed.getFlow() - excess);
      // The node is no longer active
      labelBucket[maxLabel].pop_front();
    }
    // We created a new active node if allowed is not connected to t
    if (allowed.getA() > 1) {
      labelBucket[labels[allowed.getA()]].push_back(allowed.getA());
    }
  }
}

// Push relabel algorithm
void Graph::pushRelabel() {
  // Maps a node id to a label
  std::vector<unsigned int> labels(getNodeCount());
  labels[0] = getNodeCount();

  // Stores allowed edges in the residual graph for each node
  std::vector<std::list<unsigned int>> allowedEdges(getNodeCount());

  // Bucket for active nodes with the same label value
  unsigned int maxLabel = 0;
  std::vector<std::list<unsigned int>> labelBucket(2*getNodeCount()-1);

  Node &s = getNode(0);

  // Maximize flow of edges adjacent to s and store their heads as active nodes
  for (unsigned int e : s.getOutEdges()) {
    setFlow(getEdge(e), getEdge(e).getCapacity());
    if (getEdge(e).getFlow() > 0) {
      labelBucket[0].push_back(getEdge(e).getB());
    }
  }

  // While we have active nodes
  while(maxLabel != 0 || !(labelBucket[0].empty())) {
    if (labelBucket[maxLabel].empty()) {
      maxLabel--;
      continue;
    }

    // Get the active node
    Node& active = getNode(labelBucket[maxLabel].front());
    if (allowedEdges[active.getId()].empty()) {
      // relabel has to be done
      unsigned int minimumLabel = findMinimumLabel(labels, *this, active);

      // Update label of the active node
      labels[active.getId()] = minimumLabel + 1;

      // Update list of allowed edges
      // Incoming edges in the residual graph may have become not allowed because
      // of the increased label of the active note. Therefore, it has to be checked,
      // whether an edge in the allowedEdges ist really is allowed!
      updateAllowedEdges(allowedEdges, labels, *this, active);

      // Label bucket has to be updated

      // Delete first item of the old bucket
      labelBucket[maxLabel].pop_front();

      // Add the active node to the new bucket
      labelBucket[labels[active.getId()]].push_back(active.getId());


      maxLabel = labels[active.getId()];
    } else {
      // Push can be done
      Edge& allowed = getEdge(allowedEdges[active.getId()].front());

      // Check if the edge is really allowed, see comment in relabel
      if ((active.getId() == allowed.getA() && labels[active.getId()] != labels[allowed.getB()]+1) ||
        (active.getId() == allowed.getB() && labels[active.getId()] != labels[allowed.getA()]+1)) {
        // the edge is not allowed and has to be removed
        allowedEdges[active.getId()].pop_front();
        continue;
      }

      pushAllowedEdge(allowedEdges, labels, labelBucket, maxLabel, *this, active, allowed);
    }
  }
}

void Graph::exportFlow(std::ostream& out) {
  unsigned long long int flowValue = getNode(0).getOutFlow() - getNode(0).getInFlow();
  out << flowValue << '\n';
  for (Edge& e : edges) {
    if (e.getFlow() > 0) {
      out << e.getId() << " " << e.getFlow() << '\n';
    }
  }
}

// Main function
int main(int argc, char** argv) {
  std::string outputfile = "";
  std::string filename = "";
  bool filenameSpecified = false;
  bool outputfileSpecified = false;
  for (int i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      // Output file can be specified
      if (argv[i][1] == 'o') {
        if (i+1 < argc) {
          outputfile = std::string(argv[i+1]);
          outputfileSpecified = true;
          i++;
        }
      }
    } else {
      filename = argv[i];
      filenameSpecified = true;
    }
  }

  if (!filenameSpecified) {
    std::cout << "Please specify your input filename." << '\n';
    return 0;
  }


  Graph g(filename);

  g.pushRelabel();

  if (!outputfileSpecified) {
    g.exportFlow(std::cout);
  } else {
    std::fstream file(outputfile, std::ios_base::out);
    g.exportFlow(file);
  }

  return 0;
}
