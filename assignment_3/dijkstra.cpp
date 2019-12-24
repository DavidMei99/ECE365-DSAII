//
// ECE-365 Fall 2019 Assignment #3
// Di Mei
// dijkstra.cpp
//

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include "graph.h"
using namespace std;

// main function
int main()
{
    graph G;
    string infile, outfile, start_node;
    ifstream input;
    
    // take input file
    while (!input.is_open()){
        cout << "Enter a valid graph input file: ";
        cin >> infile;
        input.open(infile);
    }
    
    // import vertices and edges in infile into graph
    G.import(input);
    input.close();
    
    // re-prompt user to enter node id until id is valid
    while (G.contains(start_node) == false){
        cout << "Enter a valid vertex id for the starting vertex: ";
        cin >> start_node;
    }
    
    // execute Dijkstra's Algorithm and record its running time
    clock_t start = clock();
    G.dijkstra(start_node);
    clock_t end = clock();
    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    cout << "Total time (in seconds) to apply Dijkstra's algortihm: " << time << endl;
    
    // outputs paths to a file with name given by user
    cout << "Enter name of output file: ";
    cin >> outfile;
    G.output(outfile);

    return 0;
}
