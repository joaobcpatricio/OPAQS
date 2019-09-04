#include "WifiNeighborGraph.h"


WifiNeighborGraph::WifiNeighborGraph() {

    for (int i = 0; i <255 ; ++i) {
        EID[i] = Graph::null_vertex();
    }

}

WifiNeighborGraph::~WifiNeighborGraph() {

}

bool WifiNeighborGraph::add_node(int eid) {
    if(exists(eid) ) return true;
    try{
        Graph::vertex_descriptor v0 = boost::add_vertex(g);
        std::cout <<  v0 << '\n' << std::endl;
        EID[eid] = v0; //Nota: nao pode ser por ponteiros pois o endereço é sempre o mesmo
        index2[v0] = eid;
        std::cout <<  '\n' << std::endl;
    } catch (...) {
        return false;
    }
    return true;

}


bool WifiNeighborGraph::add_link(int s1, int s2, int weight) {
    std::cout << 'Add NOde:\n' << std::endl;
    EdgeWeightProperty weight0(weight);
    if(!exists(s1) || !exists(s2) ) return false;
    try{// Add weighted edges
        boost::add_edge(EID[s1], EID[s2], weight0, g);
    } catch (...) {
        return false;
    }
    return true;
}

bool WifiNeighborGraph::get_routing_path(int src, int dst, std::queue<int>* route) {

    if(!exists(src) || !exists(dst) ) return false;

    // Create things for Dijkstra
    // Create vectors to store the predecessors (p) and the distances from the root (d)
    std::vector<vertex_descriptor> p(boost::num_vertices(g));
    std::vector<int> d(boost::num_vertices(g));


    // Create descriptor for the source node
    vertex_descriptor s = vertex(EID[src], g);
    vertex_descriptor goal = vertex(EID[dst], g);

    // Compute shortest paths from v0 to all vertices, and store the output in p and d
    //Evaluate Dijkstra on graph g with source v0, predecessor_map p and distance_map d
    boost::dijkstra_shortest_paths(g, s, boost::predecessor_map(&p[0]).distance_map(&d[0]));
    //p[] is the predecessor map obtained through dijkstra
    //name[] is a vector with the names of the vertices
    //s and goal are vertex descriptors
    std::vector<boost::graph_traits<Graph>::vertex_descriptor > path;
    boost::graph_traits<Graph>::vertex_descriptor current = goal;

    while(current!=s)
    {
        path.push_back(current);
        current = p[current];
    }
    path.push_back(s);

    // Prints the path obtained in reverse
    //std::cout << "Path from " << name[s] << " to " << name[goal] << std::endl;
    std::vector<boost::graph_traits<Graph>::vertex_descriptor>::reverse_iterator it;

    for (it = path.rbegin(); it != path.rend(); ++it) {
        //std::cout << index2[*it] << " ";
        route->push(index2[*it]);
    }
    //std::cout << std::endl;
}

bool WifiNeighborGraph::remove_link(int s1, int s2) {
    if(!exists(s1) || !exists(s2) ) return false;
    try{
        boost::remove_edge(EID[s1], EID[s2],g);
    } catch (...) {
        return false;
    }
    return true;
}

bool WifiNeighborGraph::update_link(int s1, int s2, int weight) {
    if(!exists(s1) || !exists(s2) ) return false;
    if(!boost::edge(EID[s1], EID[s2],g).second ){
        return WifiNeighborGraph::add_link(s1, s2, weight);
    }else{
        try{
            std::pair<edge_descriptor, bool> ed = boost::edge( EID[s1], EID[s2],g);
            //int weight = get(boost::edge_weight_t(), g, ed.first);
            //int weightToAdd = weight;
            boost::put(boost::edge_weight_t(), g, ed.first, weight);

        } catch (...) {
            return false;
        }
    }

    return true;
}

bool WifiNeighborGraph::exists(int eid) {
    if(EID[eid] == Graph::null_vertex() ) {
        //std::cout << "not exists "<< std::endl;
        return false;
    }
    return true;
}

bool WifiNeighborGraph::remove_node(int eid) {

    try{
        std::cout << EID[eid] << std::endl;
        boost::clear_vertex(EID[eid], g);
        //boost::remove_vertex(EID[eid], g);
        EID[eid] = Graph::null_vertex(); //remove from list
        std::cout << EID[eid] << std::endl;

    } catch (...) {
        return false;
    }
    return true;
}

bool WifiNeighborGraph::clear_node(int eid) {
    try{
        boost::clear_vertex(EID[eid], g);
    } catch (...) {
        return false;
    }
    return true;
}

std::string WifiNeighborGraph::json_dumps(){

    /*boost::property_map<Graph, boost::edge_weight_t>::type EdgeWeightMap = get(boost::edge_weight_t(), g);

    typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
    std::pair<edge_iter, edge_iter> edgePair;
    for(edgePair = edges(g); edgePair.first != edgePair.second; ++edgePair.first)
    {
        std::cout << EdgeWeightMap[*edgePair.first] << " ";
    }
    std::cout << std::endl;*/


    /*std::cout << "edges(g) = ";
    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
        std::cout << "(" << index2[source(*ei, g)]
        << "," << index2[target(*ei, g)] << "," << boost::get( boost::edge_weight, g, *ei )
                << ") ";
    std::cout << std::endl;*/

    //access an edge
    /*std::pair<Graph::edge_descriptor, bool> edgePair = boost::edge(v0, v1, g);
    Graph::edge_descriptor edge = edgePair.first;
    std::cout << "Edge (" << v0 << ", " << v1 << ") has weight " << boost::get( boost::edge_weight, g, edge ) << std::endl;

*/


    std::string res = ""; //"{\"graph\": \"";
    //eid 1 liga ao eid 2 com 3 e ao eid 3 com 5 --> 1 2 3 1 3 5
    //std::string s = "1-2-45;";

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){
        res += std::to_string(index2[source(*ei, g)]) + "-" +
               std::to_string(index2[target(*ei, g)]) + "-" +
               std::to_string(boost::get( boost::edge_weight, g, *ei )) + ";";
    }

    return res ; //+ "\"}";
}


void WifiNeighborGraph::get_adjacent_nodes(int eid, std::set<int>* set) {
    if(!exists(eid) ) return;
    typedef boost::graph_traits < Graph >::adjacency_iterator adjacency_iterator;

    std::pair<adjacency_iterator, adjacency_iterator> neighbors =
            boost::adjacent_vertices(EID[eid], g);

    for(; neighbors.first != neighbors.second; ++neighbors.first)
    {
        set->insert( index2[*neighbors.first] );
    }


}


void WifiNeighborGraph::copy_graph(WifiNeighborGraph* copy, uint32_t *list, int listSize){
    std::cout << "vertices(g) = ";
    typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
    std::pair<vertex_iter, vertex_iter> vp;
    bool found= false;
    //copy vertex in order to have the same index
    for (vp = vertices(g); vp.first != vp.second; ++vp.first){
        for (uint32_t i = 0; i < listSize; i++) {
            if (index2[*vp.first] == list[i]) {    // the it->dst_eid already as the packet!!
                found = true;
                break;
            }
        }
        if(!found){
            copy->add_node(index2[*vp.first]);
            std::cout << index2[*vp.first] <<  " ";
        }

    }
    std::cout << std::endl;

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){

        copy->add_link(index2[source(*ei, g)], index2[target(*ei, g)] , boost::get( boost::edge_weight, g, *ei ));
    }

}


// A BFS based function to check whether d is reachable from s.
bool WifiNeighborGraph::isReachable(int s, int d)
{
    // Base case
    if (s == d)
        return true;

    // Mark all the vertices as not visited
    bool visited[255];
    for (int i = 0; i < 255; i++)
        visited[i] = false;

    // Create a queue for BFS
    std::list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    // it will be used to get all adjacent vertices of a vertex
    std::set<int>::iterator i;
    std::set<int> adj;


    while (!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        queue.pop_front();

        // Get all adjacent vertices of the dequeued vertex s
        // If a adjacent has not been visited, then mark it visited
        // and enqueue it
        get_adjacent_nodes(s, &adj);
        for (i = adj.begin(); i != adj.end(); ++i)
        {
            // If this adjacent node is the destination node, then
            // return true
            if (*i == d)
                return true;

            // Else, continue to do BFS
            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }

    // If BFS is complete without visiting d
    return false;
}
