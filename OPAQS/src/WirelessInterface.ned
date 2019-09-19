//
// Wireless interface used in Keetchi with functionality
// to handle sending of packets considering wireless range
// and broadcast of packets. It should have access to the
// mobility management components.
//
// @date: 08-11-2015
// @author: Asanga Udugama (adu@comnets.uni-bremen.de)
//
// @author major changes by : João Patrício (castanheirapatricio@ua.pt)
// @date :   3-jul-2019
//
package opaqs;

simple WirelessInterface
{
    parameters:
        string ownMACAddress;
        string interfaceTypeName = default("wireless");
        double wirelessRange @unit(m) = default(30m);
        string expectedNodeTypes = default("KNode, KOPSNode");
        double neighbourScanInterval @unit(s) = default(1s);
        double bandwidthBitRate @unit(bps) = default(100000bps); // 100 Kbps 
        int wirelessHeaderSize @unit(byte) = default(32byte); // 32byte = 802.11 a/b/g, 16byte = BT
        
        int minSSI = default(-90);	//Limit of SSI lvl, default is -90dbM
        
        double limitQueueTime @unit(s) = default (60s); //after this time, it doesn't send the msg, it deletes it
	
		double my_limit_rate @unit(bps) = default(100000bps);


        // icon
        @display("i=device/card");
 
        // statistics and signals
        @signal[neighbourhoodSize](type=long);
 
    gates:
        // interface to the upper layer (which may be the link adaptation layer)
        input upperLayerIn;
        output upperLayerOut;

        // interface to the lower layer (which may be the physical layer)
        // there is no lowerLayerOut as packets are sent directly to
        // emulate wireless communications (uses @directIn)
        input lowerLayerIn;
}
