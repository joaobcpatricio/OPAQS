/*
 * StorageM.cc
 *
 *  Created on: 30/08/2019
 *  Author: João Patrício
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


void GraphT::cleanGraph(){
      int i, j;
      int v = V;
      //int count=0;
      for(i = 0; i < v; i++) {
         for(j = 0; j < v; j++) {
             rem_edge(i,j);
         }
      }
}


/*****************************************************************************
 * Defines the maximum size of graph - NOT WORKING YET
 */
void GraphT::maximumNoVert(int maximumNoVert){
    maxVert=maximumNoVert;
}
int GraphT::returnVvalue(){
    return V;
}

//Return no. Max of Vertices
int GraphT::returnMaxNoVert(){
    return maxVert;
}

//Displays the Matrix
void GraphT::displayMatrix(int v) {
    EV<<"Matrix: \n";
   int i, j;
   //int count=0;
   for(i = 0; i < v; i++) {
      for(j = 0; j < v; j++) {
         EV << graph[i][j] << " ";
      }
      EV<<"\n";
   }
}

/************************
 * ⇒ Returns the ID of a node given its  Address - what counts is the last 2 digit of the address (works till 99)
 * ⇒ Creates list of IDs of each Address, adds element if it's not already present
 */
int GraphT::add_element(string source){
    //EV<<"Sauce:"<<source<<"\n";
    auto itC=vertID.begin();
    bool exists = FALSE;
    int i=0;
    while(i<V){
        if(vertID[i]==source){
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

//Add edge into the matrix
void GraphT::add_edge(int u, int v, int weight) { //if weight is 0, it considers it a non-passing connection
    //EV<<"Edges & Weight Added to Graph\n";
    graph[u][v] = weight;
    graph[v][u] = weight;
    count++;
}

//Remove edge from the Matrix = 0
void GraphT::rem_edge(int u, int v){
    graph[u][v] = 0;
    graph[v][u] = 0;
}

//Returns the graph on a string
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
    //EV<<"Graph: \n"<<graphS<<"\n";
    return graphS;
}

//Count num of nodes in the graph
int GraphT::numGElem(){
    int nElem=0;
    int k=0,l=0;
    bool found=false;

    for(k = 0; k < V; k++) {
        found=false;
        for(l = 0; l < V; l++){
            if(graph[k][l]!=0 && !found){
                nElem++;
                found=true;
            }
        }
    }
    //EV<<"Graph: \n"<<graphS<<"\n";
    return nElem;
}



//Returns the Weight on a position of the graph
int GraphT::returnWGrapfT(int u, int v){
    return graph[u][v];
}

//-----------------------------------------------------------------------------------------------------------------------------------//
/************************************************************************************************************
 * Dijkstra Treatment - Calculate distance between Nodes
 */

/*******************************************************************
 * A utility function to find the vertex with minimum distance value,
 * from the set of vertices not yet included in shortest path tree
 */
int GraphT::minDistance(int dist[], bool sptSet[]){
    //EV<<"Calculating minDistance \n";
    // Initialize min value
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++){
        if (sptSet[v] == false && dist[v] <= min){
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}

//clean string smallPath;
void GraphT::cleanSmallPath(){
    smallPath.clear();
}

/**********************************************
 * Function to print shortest path from source to j using parent array
 * Base Case : If j is source -> it goes from the endpoint till it reaches the source through minimum paths
 */
void GraphT::printPath(int parent[], int j){
    std::string sP = "->";
    if (parent[j] == - 1)
        return;
    printPath(parent, parent[j]);
    //EV<<"->"<<j; //coment?

    //sP.append(std::to_string(j));
    //EV<<"sP:"<<sP<<"\n";
    smallPath.append(std::to_string(j));//sP);
    smallPath.append("->");
}

/********************************************
 * A utility function to print the constructed distance array
 */
int GraphT::printSolution(int dist[], int n,int parent[], int src){
    //EV<<"Vertex\t Weight\t Path\n";
    for (int i = 0; i < V-1; i++){
        if(dist[i]!=INT_MAX){
           // EV<<src<<"->"<<i<<"    \t\t"<<dist[i]<<"     \t\t"<<src;
            smallPath=std::to_string(src);
            smallPath.append("->");
            printPath(parent, i);
            //EV<<"\n";
           // EV<<"Small Path: "<<smallPath<<"\n";
            cleanSmallPath();
        }
    }
}

/**********************************************
 * A utility function to print and return the smallest weight path between two vertices.
 */
string GraphT::returnSmallPath(int dist[],int parent[], int src, int dst){
    cleanSmallPath();
    string returnSmallPath;
    //EV<<"Small path between "<<src<<" and "<<dst<<"\n";
    //EV<<"Vertex\t Weight\t Path\n";
    if(dist[dst]!=INT_MAX){
        //EV<<src<<"->"<<dst<<"    \t\t"<<dist[dst]<<"     \t\t"<<src;
        smallPath=std::to_string(src);
        smallPath.append("->");
        printPath(parent, dst);
        //EV<<"\n";
        //EV<<"Small Path: "<<smallPath<<"\n";
    }
    returnSmallPath=smallPath;
    return returnSmallPath;
}

//Returns the shortest path between two vertices;
string GraphT::returnShortestPath(int src, int dst){
    dijkstra(src,dst);
    return smallPath;
}

//Verifies if destiny is in the sort path.
bool GraphT::isInShortPath(int src,int gw, int dest){
    //EV<<"Is in short path btw "<<src<<" and "<<gw<<"\n";
    dijkstra(src,gw);
    string sPath = smallPath;
    std::string delimiter = "-";

    int i=0;//, q1=0;
    for(i=0;i<sPath.length();i++){
        int j=sPath.find(delimiter,i);
        if(j==std::string::npos){
            return false;
        }else{
            string v2;
            int vert2;
            int q1 = sPath.find("-",i);
            int q2 = sPath.find("-",q1+1);
            string v1=sPath.substr(i,q1-i);
            int vert1 = std::stoi (v1);
            if(q2!=std::string::npos){
                v2=sPath.substr(q1+2,q2-(q1+2));
                vert2 = std::stoi(v2);
            }else{
                vert2=INT_MAX;
            }
            if(vert1==dest || vert2==dest){
                return true;
                //EV<<"Seria true \n";
            }
            if(q2!=std::string::npos){
            i =q2+1;
            }else{break;}
        }
    }
    return false;
}


/*****************************************************************************
 * Funtion that implements Dijkstra's single source shortest path algorithm
 * for a graph represented using adjacency matrix representation
 */
void GraphT::dijkstra(int src, int dst){
    int dist[V]; // The output array.  dist[i] will hold the shortest distance from src to i
    bool sptSet[V]; // sptSet[i] will true if vertex i is included in shortest path tree or shortest distance from src to i is finalized
    // Parent array to store shortest path tree
     int parent[V];
    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++){
        parent[src] = -1;
        dist[i] = INT_MAX, sptSet[i] = false;
    }
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
                    + graph[u][v] < dist[v]){
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }
    cleanSmallPath();
    // print the constructed distance array
    printSolution(dist, V, parent, src);
    returnSmallPath(dist,parent, src, dst);
}


