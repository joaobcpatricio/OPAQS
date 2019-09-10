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

//******************************************************************************************************//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 26-june-2019
//
//               addrs | Prob 		| myProb	 |	MyPosX 	 | MyPosY	| SSI	    |neighGraph | numberVert | total  |
//               s & d | (4 byte)   |  (4 byte)  | (4 byte)  | (4 bytes)| (4 bytes) | (bytes)   |            |        |
// Routing	 ->   6 + 6      4             4           4         4			4		    64            1	          97
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
	int numberVert;
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

//******************************************************************************************************//
// Message of routing commands: indicate the chosen Nic
packet routingDecisionsMsg {    
    bool sendWifiFirst;
}

//******************************************************************************************************//
// --THIS WAS INCLUDED IN BEACONINFOMSG too--Message to provide the current metwork graph
packet NetworkGraphMsg {
    //string graphN[];
    int numberVert;
    //int countVert;
    string graphN;
    
    //double neighPos[];
}

