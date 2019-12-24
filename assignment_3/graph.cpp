//
// ECE-365 Fall 2019 Assignment #3
// Di Mei
// graph.cpp
//

#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <climits>
#include "graph.h"
#define SIZE 500000 // initial size of a hashtable

// constructor of a graph
graph::graph()
{
	mapping = new hashTable(SIZE);
    size = 0;
}

// check whether the graph contains the node with given id
bool graph::contains(const std::string &id)
{
    return mapping->contains(id);
}

// insert a new node into the graph from a row in input file
void graph::insert(const std::string &src, const std::string &dest, int cost)
{
    node *src_node, *dest_node;
	// source vertex of an edge
    if (mapping->contains(src)){ // if node already in graph
		src_node = (node *)mapping->getPointer(src);
    }
	else{
		src_node = new node;
		src_node->id = src;
		src_node->check = false;
		src_node->distance = INT_MAX; // initialize distance to be very large
		nodeList.push_back(src_node);
		mapping->insert(src, src_node);
		size++;
	}
	// destination vertex of an edge
    if (mapping->contains(dest)){ // if node already in graph
		dest_node = (node *)mapping->getPointer(dest);
    }
	else{
		dest_node = new node;
		dest_node->id = dest;
		dest_node->check = false;
		dest_node->distance = INT_MAX; // initialize distance to be very large
		nodeList.push_back(dest_node);
		mapping->insert(dest, dest_node);
		size++;
	}
	// insert a new edge into adjacent list of source vertex
    edge edg;
	edg.weight = cost;
	edg.next = dest_node;
	src_node->adjacency.push_back(edg);
}

// import vertices and edges from input file to the graph
void graph::import(std::ifstream &infile)
{
    std::string src, dest;
    int cost;
    // read infile row by row
    while (infile >> src >> dest >> cost){
        insert(src, dest, cost);
    }
}

// Dijkstra's Algorithm: it starts from the node with given id
void graph::dijkstra(const std::string &id)
{
	heap dijkstraQueue(size);
	// declare and initialize the start node
	node *startNode = (node *)mapping->getPointer(id);
	startNode->distance = 0;
	startNode->path.push_back(id);
	dijkstraQueue.insert(id, startNode->distance, startNode);
	// insert nodes except for starting node into dijkstraQueue
	for (std::list<node *>::const_iterator it = nodeList.begin(); it != nodeList.end(); it++){
        if ((*it)->id != id) dijkstraQueue.insert((*it)->id, (*it)->distance, *it);
	}
	// main part of Dijkstra's Algorithm
	for (int i = 0; i < size; i++){
		node *current;
		dijkstraQueue.deleteMin(nullptr, nullptr, &current);
        // relaxation process in Dijkstra's Algorithm
        // implemented based on pseudocode in fig 9.31 in textbook
		for (std::list<edge>::const_iterator it = current->adjacency.begin(); it != current->adjacency.end() && current->distance != INT_MAX; it++){
			// if an adjacent vertex is unknown and its new distance is shorter than its previous distance
			if (it->next->check == false && (it->weight+current->distance) < it->next->distance){
				// update the path to that vertex
                it->next->path.clear();
                it->next->path.insert(it->next->path.begin(), current->path.begin(), current->path.end());
                it->next->path.push_back(it->next->id);
                // update that vertex's distance and dijkstraQueue
				it->next->distance = it->weight + current->distance;
				dijkstraQueue.setKey(it->next->id, it->next->distance);
			}
		}
		current->check = true;
	}
}

// output shortest paths into a file with given name
void graph::output(const std::string &file)
{
	std::ofstream output;
	output.open(file);
	for(std::list<node *>::const_iterator it = nodeList.begin(); it != nodeList.end(); it++){
        if ((*it)->distance == INT_MAX){
			output << (*it)->id << ": NO PATH" << std::endl;
        }
		else{
			output << (*it)->id << ": " << (*it)->distance << " [";
            // write paths in square brackets
            for (std::list<std::string>::const_iterator iter = (*it)->path.begin(); iter != --(*it)->path.end(); iter++){
				output << *iter << ", ";
            }
            // write the last vertex
			output << (*it)->path.back() << "]" << std::endl;
		} 
	}
	output.close();
}
