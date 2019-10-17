//
// The model for the neighboring layer
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 20-june-2019
//
//
package opaqs;

simple NeighboringLayer
{
    parameters:

        // unique address of the node
        string ownMACAddress;
        string ownBTMACAddress;

        // duration after which the next anti-entropy session must be started for a given host
        double antiEntropyInterval @unit(s) = default(300s);


        // maximum duration to use when obtaining a random backoff duration
        double maximumRandomBackoffDuration @unit(s) = default(5s);
        

	    // OMNeT RNG which is used for generation of random numbers
	    int usedRNG = default(0);
	    
	    //Initial Probability
	    double ProbInit = default(0.5);
	    
	    //Gateway
	    string GWAddr =default("02:00:00:00:00:01");

        // icon used
        @display("i=block/transport");
        
        //Added 23/07/19 15h25
        bool sendWifiFirst = default(true);
        
        //Added 26/08/19 18h31
        double delayPerDataMsg = default(0.1); //s

		int maximumNoVert = default(50);
		
		int maxLengthGraph=default(20);
		
		double max_age @unit(s) = default(999999999999s);
		
		double max_absent_time @unit(s) = default(9999s);
		double resetGPeriod @unit(s) = default(99999s);
		

        
    
    gates:

        // interface to the upper layer (which may be the Routing layer)
        input upperLayerIn;
        output upperLayerOut;

        // interface to the lower layer (which may be the link adaptation layer)
        input lowerLayerIn;
        output lowerLayerOut;




}
