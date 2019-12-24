//
// ECE-365 Fall 2019 Assignment #3
// Di Mei
// graph.h
//

#ifndef _graph_h
#define _graph_h

#include <fstream>
#include <string>
#include <list>
#include "heap.h"

class graph
{
	public:
		// constructor of a graph
        graph();
        
        // check whether the graph contains the node with given id
        bool contains(const std::string &id);
        
        // insert a new node into the graph from a row in input file
        void insert(const std::string &src, const std::string &dest, int cost);
    
        // import vertices and edges from input file to the graph
        void import(std::ifstream &infile);

        // Dijkstra's Algorithm: it starts from the node with given id
        void dijkstra(const std::string &id);

        // output shortest paths into a file with given name
        void output(const std::string &file);

	private:
		int size; // the size of a graph
		class edge;
		
		// node: an inner class within the graph
        class node{
			public:
				std::string id; // a node's id
                bool check; // check if the node is visited or not (known or not)
                int distance; // distance from starting node to current node
                std::list<edge> adjacency; // adjacency list
				std::list<std::string> path; // the path to current node
		};
    
        // edge: an inner class within the graph
        class edge{
            public:
                int weight; // weight of an edge
                node *next; // points to the destination node of an edge
        };
        
		std::list<node *> nodeList; // a linked list of pointers to nodes
        hashTable *mapping; // map ids and nodes in graph
};

#endif
