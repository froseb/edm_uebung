/*
 * Implementation of the Successive Shortest Path algorithm for finding a
 * perfect matching in a bipartite graph with minimal cost.
 * Requires c++11 standard for compiling
 */

#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include "Graph.h"

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

  g.perfectMatching();

  if (!outputfileSpecified) {
    g.exportMatching(std::cout);
  } else {
    std::fstream file(outputfile, std::ios_base::out);
    g.exportMatching(file);
  }
  return 0;
}
