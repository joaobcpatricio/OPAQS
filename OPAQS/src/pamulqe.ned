//
// The model for the Forwarding Routing layer
//
// @author :  João Patrício (castanheirapatricio@ua.pt)
// @date  :   3-jul-2019
//
package opaqs;

simple pamulqe like IRoutingLayer
{
    parameters:

		
		// assigned ID for the node
        int nodeIndex;

        // unique address of the node
        string ownMACAddress;
        string ownBTMACAddress;

        // size of cache in bytes - zero means unlimited caching
        int maximumCacheSize @unit(byte) = default(5000000byte);

        // duration after which the next anti-entropy session must be started for a given host
        double antiEntropyInterval @unit(s) = default(300s);

        // maximum amount of hops to forward for a data
        int maximumHopCount = default(25);

        // maximum duration to use when obtaining a random backoff duration
        double maximumRandomBackoffDuration @unit(s) = default(5s);
        
        // switch on and off the TTL handling (expensive procedure)
        bool useTTL = default(false);

	    // OMNeT RNG which is used for generation of random numbers
	    int usedRNG = default(0);


		//27=06
		int inCache = default(0);
		
		double waitBFsend = default(0);
		double max_age @unit(s) = default(999999999999s);
		string actual_gateway = default("");
		string gateway_list = default("");//("Wf:00:00:00:00:02;0|.");
		
		double gwCheckPeriod @unit(s) = default(999999999999s);
		
		double kill_pcktP @unit(s) = default(999999999999s);
		
		
        // icon used
        @display("i=block/transport");

        // statistics and signals
        @signal[dataBytesReceived](type=long);
        @signal[sumVecBytesReceived](type=long);
        @signal[dataReqBytesReceived](type=long);
        @signal[totalBytesReceived](type=long);

        
    
    gates:

        // interface to the upper layer (which may be the application)
        input upperLayerIn;
        output upperLayerOut;

        // interface to the lower layer (which may be the link adaptation layer)
        input lowerLayerIn;
        output lowerLayerOut;
        
        input neighLayerIn;
        output neighLayerOut; 
        //Added 9/07/19
        input BTneighLayerIn;
        output BTneighLayerOut; 

}
