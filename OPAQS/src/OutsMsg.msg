//
// Format of a Data packet.
//ver tese rodrigo pg55
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de)
// @date   : 06-oct-2015
//
// example sizes in bytes
//
//               addrs | data name | goodness value | payload size | payload | valid until | final dest | hop count | total  |
//               s & d | or msg ID |  (1 byte int)  | (2 byte int) | (bytes) | (4 byte int)| (dest orie)|  (1 byte) | (bytes)|
// Keetchi  ->   6 + 6     64             1               2           512          4             6           -         601
// RRS      ->   6 + 6     64             -               2           512          4             6           -         600
// Epidemic ->   6 + 6     64             -               2           512          4             6           1         601
//

packet DataMsg {

    // common fields
    string sourceAddress;
    string destinationAddress;
    string dataName;//sequence number?? - is the messageID em herald
    int msgUniqueID;	//ID da msg em int
    int groupType = 50;//(previouys goodness value) Groupt type    
    simtime_t injectedTime;	//timeStamp
    int realPayloadSize; //size of the data - dataSizeInBytes        
    int realPacketSize; //total size of the message
    
    string dummyPayloadContent;
    int msgType; //?? not sure 4 what
    double validUntilTime = 0; //ttl - defines when data time expires
    
    
    int hopsTravelled;// = 0;

    // destination oriented delivery specific fields
    string originatorNodeMAC; // I can chose to store here the MACAdd of the node that generated the Msg
    string finalDestinationNodeName; //final destination MAC - GW
    bool destinationOriented = false;



    // epidemic routing specific fields
    string messageID; //ID na storage->cacheList
    //int hopCount;// = 255;
    
    int nHops;
    
    //ADDED 1/07 15h28
    int nMsgOrder;
    
    //Added 27/08/19
    string prevHopsList[];	//	list of previous nodes where the data passes through - loop avoidance mechanism
}



//************************************************************************************//
// Acknowledge message.
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 18-june-2019
//
// example sizes in bytes
//
//                addrs | msg ID hashes  | total  |
//                s & d | 2 bytes x count| (bytes)|
// Epidemic  ->   6 + 6     2 x 1           14

packet AckMsg {

    // common fields
    string sourceAddress;
    string destinationAddress;
	int realPacketSize; // (6+6) from addr + 1 from messageID + hash size
    // routing specific fields
    string messageID;  // will inform the msg id
    bool isFinalDest;		//Indicates if it reached the final destination
}

//***********************************************************************************//
// Beacon message.
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 26-june-2019
//
// example sizes in bytes
//
//               addrs | Prob 		| MyPosX 	 | MyPosY	| total  |
//               s & d | (4 byte)   |  (4 byte)  | (4 byte) | (bytes)|
// Routing	 ->   6 + 6      4             4           4         24
//
packet BeaconMsg {

    // common fields
    string sourceAddress;
    string destinationAddress;
    int realPacketSize;
	double Prob;	
	double MyPosX;
	double MyPosY;
	string neighGraph;
	int numberVert;
	//int Nic; //por remover, passa a usar-se pelo MacAdd
}

//***********************************************************************************//
// Data Request message.
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 26-june-2019
//
//               addrs | bool send me	|Prob 		|    SSI	|total   |
//               s & d | 	  6 byte    |(4 byte)   | (4 bytes) | (bytes)|
// Routing	 ->   6 + 6       6             4           4  			26
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
	//int Nic; //0=wifi, 1=Bt,
}
//***********************************************************************************//

