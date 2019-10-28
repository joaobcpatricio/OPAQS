//
// Format of a Data packet.
//ver tese rodrigo pg55
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 10-sep-2019
//
// example sizes in bytes
//
//               addrs | data name | group type     | payload size | payload | valid until | final dest | Addr Generator | hops	     | nMsgOrder  | destination | prev Hops | total  |
//               s & d | or msg ID |  (1 byte int)  | (2 byte int) | (bytes) | (4 byte int)| (dest orie)|   Node		 |  (1 byte) |            |  oriented   |  list     | (bytes)|
// Epidemic ->   6 + 6 .    64 (32?)     .       1        .      2       . 64      .      4      .      6     .      6		 .		1    .    2       .     1       .    32     .  231+64=295
//
//Real packet size hardcoded on StorageM.cc
//
packet DataMsg {

    // common fields
    string sourceAddress;		//												(6bytes)
    string destinationAddress;	//												(6bytes)
    string dataName;//sequence number?? - is the messageID em herald			(*-64 bytes)		Eg.:"/app/item-22086"
    string messageID; //ID na storage->cacheList  ?  							(*-64 bytes)		Eg.:"/app/item-22086"
    int msgUniqueID;	//ID da msg em int										(2)					Eg.:"86"
    int groupType = 50;//(previouys goodness value) Groupt type 				(1byte)   
    simtime_t injectedTime;	//timeStamp	, generation time						(?)
    int realPacketSize; //tells total size of the message 						(2 byte)
    int realPayloadSize; //size of the data -> dataSizeInBytes 					(10 bytes? .ini)       
	double validUntilTime = 0; //ttl - defines when data time expires			(4b ytes)
    string finalDestinationNodeName; //final destination MAC - GW				(6 bytes)
    string originatorNodeMAC; // I can chose to store here the MACAdd 			(6 bytes)
    						//of the node that generated the Msg				-------
    //int hopsTravelled;// = 0;													------
    int nHops;		//															(1byte)
    int nMsgOrder;  	//Identifies the order of its generation on its node	(2bytes)			Eg.:"87"
    					//could be used instead msgUniqueID or messageID
    bool destinationOriented = false;	//										(1 byte)
    string prevHopsList[];	//	list of previous nodes where the data 			(32 bytes)
    						//passes through - loop avoidance mechanism
    	
    simtime_t sentTimeRout;//Time messaged sent from routing from generated node
    simtime_t receivedTimeRout;//Time msg received in routing of any receiving node						
    simtime_t sentTime;	//timeStamp
	simtime_t receivedTime;	//timeStamp
	
	bool reached_gw=false;
    
    //OTHERS old:    
    //string dummyPayloadContent;	
    //int msgType; //?? not sure 4 what
    // destination oriented delivery specific fields

}



//************************************************************************************//
// Acknowledge message.
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 18-june-2019
//
// example sizes in bytes
//
//                addrs | msg ID hashes  | if is final | total  |
//                s & d | 2 bytes x count| destination | (bytes)|
//           ->   6 + 6 .   2 x 1 (32?)       .     1       .  15
//Real packet size hardcoded on RoutingLayer
//
packet AckMsg {

    // common fields
    string sourceAddress;
    string destinationAddress;
	int realPacketSize; // (6+6) from addr + 1 from messageID + hash size
    // routing specific fields
    string messageID;  // will inform the msg id
    bool isFinalDest;		//Indicates if it reached the final destination
    //bool reachedGW - to insert later if to use (1 byte)
    simtime_t injectedTime;	//timeStamp	, generation time	
}

//***********************************************************************************//
// Beacon message.
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 26-june-2019
//
// example sizes in bytes
//
//               addrs | Prob 		| MyPosX 	 | MyPosY	| neighGraph | neighEner	|numberVert | total  |
//               s & d | (4 byte)   |  (4 byte)  | (4 byte) |            |            	|			| (bytes)|
// Routing	 ->   6 + 6      4             4           4        64            64        |	1		|	152/3|   
//
//Real packet size hardcoded on NeighboringLayer
//
packet BeaconMsg {

    // common fields
    string sourceAddress;
    string destinationAddress;
    int realPacketSize;	//Hardcoded in NeighboringLayer.cc
	double Prob;	
	double MyPosX;
	double MyPosY;
	string neighGraph;	//Graph of neighbors LQE
	int numberVert;
	simtime_t sentTime;	//timeStamp
	simtime_t receivedTime;	//timeStamp
	simtime_t injectedTime;	//timeStamp	, generation time on neighboring 
	string neighEner;	//Table of energy of neighbors
}

//***********************************************************************************//
// Data Request message.
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 26-june-2019
//
//               addrs | bool send me	|Prob 		|    SSI	|total   |
//               s & d | 	  6 byte    |(4 byte)   | (4 bytes) | (bytes)|
// Routing	 ->   6 + 6       6             4           4  			26		add positions
//
packet DataReqMsg {

    // common fields
    string sourceAddress;
    string destinationAddress;
    int realPacketSize;
    bool SendMeData;
    
	double Prob;	
	//double MyPosX;
	//double MyPosY;
	double SSI;
	simtime_t beaconSentT; //Time when the received beacon was sent
	//int Nic; //0=wifi, 1=Bt,
	simtime_t injectedTime;	//timeStamp	, generation time	
	simtime_t sentTime;	//timeStamp
	simtime_t receivedTime;	//timeStamp
}
//***********************************************************************************//

