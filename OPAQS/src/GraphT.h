/*
 * GraphT.h
 *
 *  Created on: 30/08/2019
 *      Author: mob
 */


// SOURCE: https://www.sanfoundry.com/cpp-program-find-shortest-path-from-source-vertex-all-other-vertices-linear-time/
//         https://www.tutorialspoint.com/cplusplus-program-to-represent-graph-using-adjacency-matrix

#ifndef GRAPHT_H_
#define GRAPHT_H_

#include <stdio.h>
#include <limits.h>
#include <iostream>
#include <omnetpp.h>
#include <cstdlib>
#include <sstream>
#include <string>

using namespace omnetpp;
using namespace std;
#define V 20        //HARDECODED VARIABLE of number of vertices


class GraphT{
public:
    GraphT();
    ~GraphT();

    int Vv=20;
    int graph[V][V]; //HARDECODED VARIABLE  //the adjacency matrix initially 0

    void displayMatrix(int v);
    void add_edge(int u, int v, int weight);
    void rem_edge(int u, int v);


    int minDistance(int dist[], bool sptSet[]);
    int printSolution(int dist[], int n); //-> parar para parte vazia do array
    void dijkstra( int src);



};



#endif /* GRAPHT_H_ */
