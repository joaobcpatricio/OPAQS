//
// Created by daniela on 12-06-2018.
//

#ifndef TEST_ADJACENCY_GRAPH_WIFINEIGHBORGRAPH_H
#define TEST_ADJACENCY_GRAPH_WIFINEIGHBORGRAPH_H


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/pending/property.hpp>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <string.h>


//#include "simpleLogger.h"


typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
//typedef boost::adjacency_list<boost::listS, boost::vecS,boost::undirectedS,boost::no_property,EdgeWeightProperty> UndirectedGraph;



typedef boost::property<boost::vertex_index2_t, int>
        VertexIndexProperty;


typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS,VertexIndexProperty, EdgeWeightProperty > Graph;
        //boost::no_property



/*
typedef boost::adjacency_list < boost::listS, boost::listS, boost::undirectedS,
        //boost::no_property
        VertexIndexProperty, EdgeWeightProperty > Graph;
*/

typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;

typedef boost::graph_traits < Graph >::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits < Graph >::edge_descriptor edge_descriptor;
typedef std::pair<int, int> Edge;




class WifiNeighborGraph{

public:
    WifiNeighborGraph();
    ~WifiNeighborGraph();

    bool add_node(int eid);
    bool remove_node(int eid); //not invokes remove_vertex: invalidates all of the iterators and vertex descriptors

    bool remove_link(int s1, int s2);
    bool update_link(int s1, int s2, int weight);
    bool add_link(int s1, int s2, int weight);
    bool get_routing_path(int src, int dst, std::queue<int>* path);
    bool clear_node(int eid);
    std::string json_dumps();
    void get_adjacent_nodes(int eid, std::set<int>* set);
    bool exists(int eid);
    void copy_graph(WifiNeighborGraph* copy, uint32_t *list, int listSize);
    bool isReachable(int s, int d);
private:
    //Graph::vertex_descriptor* get_vertex_by_eid(int eid);

    Graph g;
    Graph::vertex_descriptor EID[255];//get vertex by eid

    // Property accessors
    boost::property_map<Graph, boost::vertex_index2_t>::type index2 = get(boost::vertex_index2, g);
};



#endif //TEST_ADJACENCY_GRAPH_WIFINEIGHBORGRAPH_H
