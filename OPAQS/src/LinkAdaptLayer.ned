//
// The model for link adaptations between the used link
// technology (below) and the upper layer.
//
//
// @author :  João Patrício (castanheirapatricio@ua.pt)
// @date   :  3-jul-2019
//
package opaqs;

simple LinkAdaptLayer
{
    parameters:

        // own address
        string ownMACAddress;
        string ownBTMACAddress;

        @display("i=block/layer");

    gates:
        // interface to the upper layer (forwarding layer)
        input upperLayerIn;
        output upperLayerOut;
        
        // interface to the upper layer (neighboring layer)
        input neighLayerIn;
        output neighLayerOut;

        // interface to the lower layer (which may be the link technology used, e.g., emulated, WiFi adhoc, WiFi direct, etc.)
        input lowerLayerIn;
        output lowerLayerOut;
        
        // interface to the lower layer (which may be the link technology used, e.g., emulated, WiFi adhoc, WiFi direct, etc.)
        input BTlowerLayerIn;
        output BTlowerLayerOut;
        

        
        

}
