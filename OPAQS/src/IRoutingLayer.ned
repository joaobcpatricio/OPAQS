//
// A module interface that represents the forwarding layer in the nodes.
// 

package opaqs;

moduleinterface IRoutingLayer
{
        // icon used
        @display("i=block/transport");
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
