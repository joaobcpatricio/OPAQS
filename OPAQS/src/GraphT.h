/*
 * GraphT.h
 *
 *  Created on: 30/08/2019
 *      Author: mob
 */


// SOURCE: https://www.sanfoundry.com/cpp-program-find-shortest-path-from-source-vertex-all-other-vertices-linear-time/
//         https://www.tutorialspoint.com/cplusplus-program-to-represent-graph-using-adjacency-matrix

/*
 * Notes:
The value of V is set here;
As the ID of the nodes it is used the last 2 numbers of the MAC Address (considered in NeighboringLayer)
 */

#ifndef GRAPHT_H_
#define GRAPHT_H_

#define TRUE                            1
#define FALSE                           0

#include <stdio.h>
#include <limits.h>
#include <iostream>
#include <omnetpp.h>
#include <cstdlib>
#include <sstream>
#include <string>

using namespace omnetpp;
using namespace std;
#define V 15//20//50//20        //HARDECODED VARIABLE of number of vertices - must be the same of "**.neighboring.maxLengthGraph"


class GraphT{
public:
    GraphT();
    ~GraphT();

    int Vv=V;//20;
    int maxVert=V;
    int graph[V][V]; //HARDECODED VARIABLE  -> change to vector for better initialization
    int count=1;

    void maximumNoVert(int maximumNoVert);      //not really good and used
    void displayMatrix(int v);                  //prints matrix
    void add_edge(int u, int v, int weight);    //Adds edge to matrix
    void rem_edge(int u, int v);                //Removes edge from matrix
    //int return_graph();
    vector<string> vertID;
    int add_element(string source);                 //Returns the ID of a node given its  Address - what counts is the last 2 digit of the address (works till 99)
    int minDistance(int dist[], bool sptSet[]);     //A utility function to find the vertex with minimum distance value, from the set of vertices not yet included in shortest path tree
    int printSolution(int dist[],int n, int parent[], int srcs);    //A utility function to print the constructed distance array
    void printPath(int parent[], int j);            //Function to print shortest path from source to j using parent array
    void dijkstra( int src, int dst);               //Funtion that implements Dijkstra's single source shortest path algorithm
    int returnVertIDSize();             //returns size of verts
    string returnGraphT();              //Returns the graph on a string
    int returnMaxNoVert();              //not really good and used
    int returnWGrapfT(int u, int v);    //returns weight on a position of the graph
    string smallPath;
    void cleanSmallPath();              //clean string smallPath;
    string returnSmallPath(int dist[],int parent[], int src, int dst);      //A utility function to print and return the smallest weight path between two vertices.

    string returnShortestPath(int src, int dst);            //Returns the shortest path between two vertices;
    bool isInShortPath(int src, int gw, int dest);          //Verifies if destiny is in the sort path.
    void cleanGraph();                                      //Cleans Graph
    int returnVvalue();

    int numGElem(); //returns number of elements in graph

};


#endif /* GRAPHT_H_ */


