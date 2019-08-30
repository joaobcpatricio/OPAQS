/*
 * GraphT.h
 *
 *  Created on: 30/08/2019
 *      Author: mob
 */


// SOURCE: https://www.sanfoundry.com/cpp-program-find-shortest-path-from-source-vertex-all-other-vertices-linear-time/

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
#define V 10

class GraphT{
public:
    int minDistance(int dist[], bool sptSet[]);
    int printSolution(int dist[], int n);
    void dijkstra(int graph[V][V], int src);

};



#endif /* GRAPHT_H_ */
