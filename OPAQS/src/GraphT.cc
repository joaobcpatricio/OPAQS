/*
 * GraphT.cc
 *
 *  Created on: 30/08/2019
 *      Author: mob
 */

#include "GraphT.h"

/***********************************************************************************************************
 * Graph - Create the Graph of the network
 */

GraphT::GraphT(){

    for (int i = 0; i <Vv ; ++i) {
        for (int o = 0; o <Vv ; ++o) {
            graph[i][o]=0;
            graph[o][i]=0;
        }
    }
    for(int u=0;u<Vv;u++){
        vertID.push_back("");
    }

}

GraphT::~GraphT(){
}

/*****************************************************************************
 * Defines the maximum size of the cache
 */
void GraphT::maximumNoVert(int maximumNoVert){
    maxVert=maximumNoVert;
}


void GraphT::displayMatrix(int v) {
   int i, j;
   //int count=0;
   for(i = 0; i < v; i++) {
      for(j = 0; j < v; j++) {
         EV << graph[i][j] << " ";
      }
      EV<<"\n";
   }
}

//creates list of IDs of each Address, adds element if it's not already present and returns its ID;
int GraphT::add_element(string source){

    auto itC=vertID.begin();
    bool exists = FALSE;
    int i=0;
    while(i<V){
        if(vertID[i]==source){//if(bertV]==source){
            exists=TRUE;
            break;
        }
        i++;
    }
    if(!exists){
        int IDadd=std::stoi( source.substr(15,17));
        vertID.insert(itC+IDadd,source);

        return IDadd;
    }else{
        int IDe=std::stoi( source.substr(15,17));
        return IDe;
    }
}

int GraphT::returnVertIDSize(){
    return vertID.size();
}

/*int GraphT::returnCount(){
    count =0;
    for(int i=0;i<vertID.size()-1;i++){
       if(vertID[i]!=""){
           count++;
       }

    }
    return count;
}*/

void GraphT::add_edge(int u, int v, int weight) { //function to add edge into the matrix
    EV<<"Edges & Weight Added to Graph\n";
    graph[u][v] = weight;
    graph[v][u] = weight;
    count++;
}

void GraphT::rem_edge(int u, int v){
    graph[u][v] = 0;
    graph[v][u] = 0;
}
/*int GraphT::return_graph(){
    int graph[V][V];
    for(int a = 0; a < V + 1; ++a) {
            for(int b = 0; b < V + 1; ++b) {
                graf[a][b] = graph[a][b];
            }
        }
    return graf;
}*/

string GraphT::returnGraphT(){
    std::string graphS;
    int k,l;
    for(k = 0; k < V; k++) {
        for(l = 0; l < V; l++) {
            if(graph[k][l]!=0){
                graphS=graphS+std::to_string(k)+"->"+std::to_string(l)+":"+std::to_string(graph[k][l])+";\n";
            }
        }
    }
    EV<<"Gráfico: \n"<<graphS<<"\n";

    return graphS;
}


/************************************************************************************************************
 * Dijkstra Treatment - Calculate distance between Nodes
 */


// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int GraphT::minDistance(int dist[], bool sptSet[]){

    // Initialize min value
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++){
        if (sptSet[v] == false && dist[v] <= min){
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}


// A utility function to print the constructed distance array
int GraphT::printSolution(int dist[], int n){

    EV<<"Vertex   Distance from Source\n";
    for (int i = 0; i < V; i++){
        if(dist[i]>=2000){
            break;
        }
        EV<<" "<<i<<"\t\t "<<dist[i]<<"\n";
    }
}



// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void GraphT::dijkstra(int src){//(int graph[V][V], int src){
    //int graph[20][20]=vertArr;

    int dist[V]; // The output array.  dist[i] will hold the shortest distance from src to i
    //initialize dist array;
    /*for(int iz=0; iz<V;iz++){
        dist[iz]=0;
    }*/

    bool sptSet[V]; // sptSet[i] will true if vertex i is included in shortest path tree or shortest distance from src to i is finalized
    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++){
        // Pick the minimum distance vertex from the set of vertices not yet processed. u is always equal to src in first iteration.
        int u = minDistance(dist, sptSet);
        // Mark the picked vertex as processed
        sptSet[u] = true;
        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++)
            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u]
                    + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    // print the constructed distance array
    printSolution(dist, V);

}



