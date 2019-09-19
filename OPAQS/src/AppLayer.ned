//
// The model for the Event Notifier (Herald) application.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de), Anna Förster (afoerster@uni-bremen.de)
// @date   : 15-aug-2016, updated 6-febr-2018
//
//
// @author major changes by : João Patrício (castanheirapatricio@ua.pt)
// @date :   3-jul-2019
//
package opaqs;

simple AppLayer
{
     parameters:

        // assigned ID for the node
        int nodeIndex;


        // percentage of nodes from notificationCount to assign popularity values
        double popularityAssignmentPercentage = default(25.0); // 25.0 = 25%

        // RNG to use
        int usedRNG = default(0);

        // how often data is generated
        double dataGenerationInterval @unit(s) = default(15min);


        // data size
        int dataSizeInBytes = default(10000);

        // TTL of messages - message valid until time - defines when data time expires
        double ttl @unit(s) = default(999999999999s);

        

        // statistics and signals
        @signal[likedDataBytesReceived](type=long);
        @signal[nonLikedDataBytesReceived](type=long);
        @signal[duplicateDataBytesReceived](type=long);
        @signal[totalDataBytesReceived](type=long);

        @signal[likedDataBytesReceivableByAllNodes](type=long);
        @signal[nonLikedDataBytesReceivableByAllNodes](type=long);
        @signal[totalDataBytesReceivableByAllNodes](type=long);

        @signal[dataDelay](type=simtime_t);
        
        
        //Adicionei:
        string destinationAddr;//=default("Wf:00:00:00:00:01");
        string destinationBTAddr;//=default("Bt:00:00:00:00:01");
        //1/07 15h38
		int nodeGenMsg = default(0);
		
		bool nodesThatGen = default(true); //indicate if the following nodes are the ones that generate data (true) or that don't (false)
		string nodesGenMsg = default("0");
		
		//26/08/2019
		bool startMultipleMsg = default(false);	// indicates if it will be generating several Data Msgs at once
		double numMultipleMsg = default(100);	// number of messages to be generated at the begining

		int hopsListSize = default(50);
		
		double endGeneratingMsg @unit(s) = default(999999999999s);
		


		// icon used
        @display("i=block/app");

    gates:

        // interfaces to the lower layer
        input lowerLayerIn;
        output lowerLayerOut;

}
