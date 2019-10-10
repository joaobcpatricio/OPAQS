//
// Internal messages used to pass information between different
// layers.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 16-nov-2015
//
// @author major changes by : João Patrício (castanheirapatricio@ua.pt)
// @date :   3-jul-2019
//

// Message to register an application
packet RegisterAppMsg {
    string appName;
    string prefixName;
}

// Message to provide the current neighbour list
packet NeighbourListMsg {
    string neighbourNameList[];
    int neighbourNameCount;
    
    //double neighPos[];
}
// Message to provide the graph from neighLayer when it receives the NeighList
packet GraphUpdtMsg {
    string graph;
    bool noNeighs;
    simtime_t sentTime;	//timeStamp
}

// Message to inform a packet has been sent
packet PcktIsSentMsg {
    string ownAddr;
    double bit_size;
    bool to_Gw;
    bool is_sent;
    double distance;
    simtime_t sentTime;	//timeStamp
}

// Message to provide the table from neighLayer 
packet EnerTableMsg {
    string table;
    //bool noNeighs;
    simtime_t sentTime;	//timeStamp
}


//******************************************************************************************************//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 26-june-2019
//
//               addrs | Prob 		| myProb	 |	MyPosX 	 | MyPosY	| SSI	    |neighGraph |  neighEner | numberVert | total  |
//               s & d | (4 byte)   |  (4 byte)  | (4 byte)  | (4 bytes)| (4 bytes) | (bytes)   |			 |            |        |
// Routing	 ->   6 + 6      4             4           4         4			4		    64            64		  1	          97
//
// Message that sends Beacon received information to routing layer
packet BeaconInfoMsg {
      
    string sourceAddress;    
    string destinationAddress;
    int realPacketSize;	//Hardcoded in NeighboringLayer.cc
	double Prob;
	double myProb;	
	double MyPosX;
	double MyPosY;
	double SSI;
	//int Nic; //0=wifi, 1=Bt,
	string neighGraph;
	string neighEner;
	int numberVert;
	simtime_t sentTime;	//timeStamp
	simtime_t receivedTime;	//timeStamp
	simtime_t injectedTime;	//timeStamp	, generation time on neighboring 
}

//******************************************************************************************************//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 23-july-2019
// Message to provide the current neighbour list by Bluetooth
packet NeighbourListMsgBT {
    string neighbourNameList[];
    int neighbourNameCount;
    
    //double neighPos[];
}
// Message to provide the graph from neighLayer when it receives the NeighList
packet GraphUpdtMsgBT {
    string graph;
    bool noNeighs;
    simtime_t sentTime;	//timeStamp
}

//******************************************************************************************************//
// Message of routing commands: indicate the chosen Nic
packet routingDecisionsMsg {    
    bool sendWifiFirst;
}


