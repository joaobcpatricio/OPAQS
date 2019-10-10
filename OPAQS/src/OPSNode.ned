//
// @date: 08-11-2015
// @author: Asanga Udugama (adu@comnets.uni-bremen.de)
//
// @author major changes by : João Patrício (castanheirapatricio@ua.pt)
// @date :   3-jul-2019
//

package opaqs;

import inet.mobility.contract.IMobility;

module OPSNode
{
    parameters:
        @networkNode();

        // unique node identifier
        int nodeID;
        
        // type of routinglayer used
        string routingLayer @enum(RoutingLayer,EpidemicRoutingLayer) = default("RoutingLayer");

        // build own address from nodeID (in the form of a MAC address)
        int digit1 = int((nodeID - (nodeID % 10000)) / 10000);
        int digit2 = int(((nodeID - (this.digit1 * 10000)) - ((nodeID - (this.digit1 * 10000)) % 1000)) / 1000);
        int digit3 = int(((nodeID - (this.digit1 * 10000) - (this.digit2 * 1000)) - ((nodeID - (this.digit1 * 10000) - (this.digit2 * 1000)) % 100)) / 100);
        int digit4 = int(((nodeID - (this.digit1 * 10000) - (this.digit2 * 1000) - (this.digit3 * 100)) - ((nodeID - (this.digit1 * 10000) - (this.digit2 * 1000) - (this.digit3 * 100)) % 10)) / 10);
        int digit5 = int(nodeID - (this.digit1 * 10000) - (this.digit2 * 1000) - (this.digit3 * 100) - (this.digit4 * 10));
        //string ownAddress = "02:0" + string(this.digit1) + ":0" + string(this.digit2) + ":0" + string(this.digit3) + ":0" + string(this.digit4) + ":0" + string(this.digit5);
        //string ownAddress = "Wf:0" + string(this.digit1) + ":0" + string(this.digit2) + ":0" + string(this.digit3) + ":0" + string(this.digit4) + ":0" + string(this.digit5);
        string ownAddress = "Wf:0" + string(this.digit1) + ":0" + string(this.digit2) + ":0" + string(this.digit3) + ":0" + string(this.digit3) + ":"+ string(this.digit4) + string(this.digit5);
        //string ownBTAddress = "BT:0" + string(this.digit1) + ":0" + string(this.digit2) + ":0" + string(this.digit3) + ":0" + string(this.digit4) + ":0" + string(this.digit5);
		string ownBTAddress = "BT:0" + string(this.digit1) + ":0" + string(this.digit2) + ":0" + string(this.digit3) + ":0" + string(this.digit4) + ":"+ string(this.digit4) + string(this.digit5);
        
	
        *.ownMACAddress = ownAddress;
        *.ownBTMACAddress = ownBTAddress;
        //adicionar aqui o id do nó
        *.nodeIndex = nodeID;

        // mobility model used
        string mobilityType = default("StationaryMobility");

        // icon
        //@display("i=device/palm2;bgb=250,340");
        @display("i=device/dagama1;bgb=250,340");

    gates:
        input radioIn @directIn;
        //added 9/07/19
        input bluetoothrIn @directIn;

    submodules:

        // application layer
        application: AppLayer {
            //nodeIndex = nodeID;
            @display("p=122,28");
        }

        // routing layer
        routing: <routingLayer> like IRoutingLayer {
            @display("p=122,105");
        }

        //Adicionado 25/06 00h20
        //Neighboring layer
        neighboring: NeighboringLayer {
            @display("p=55.033333,165.09999");
        }
       

        // link adaptation layer
        linkAdapt: LinkAdaptLayer {
            @display("p=122,179");
        }

        // wifi adhoc interface
        wirelessIfc: WirelessInterface {
            @display("p=88.899994,252.30666");
        }
        // Bluetooth adhoc interface
        bluetoothIfc: BluetoothInterface {
            @display("p=161.71333,252.30666");
        }

        // mobility
        mobility: <mobilityType> like IMobility {

            @display("p=31,253");
        }
    connections allowunconnected:

        // gate connections between application and routing layer
        application.lowerLayerIn <-- routing.upperLayerOut;
        application.lowerLayerOut --> routing.upperLayerIn;

        // gate connections routing layer and link adaptation layer
        routing.lowerLayerIn <-- linkAdapt.upperLayerOut;
        routing.lowerLayerOut --> linkAdapt.upperLayerIn;

        // gate connections routing layer and neighboring layer
        routing.neighLayerIn <-- neighboring.upperLayerOut;
        routing.neighLayerOut --> neighboring.upperLayerIn;

        // gate connections neighboring layer and link adaptation layer
        neighboring.lowerLayerIn <-- linkAdapt.neighLayerOut;
        neighboring.lowerLayerOut --> linkAdapt.neighLayerIn;


        // gate connections between link adaptation layer and the wireless interface
        linkAdapt.lowerLayerIn <-- wirelessIfc.upperLayerOut;
        linkAdapt.lowerLayerOut --> wirelessIfc.upperLayerIn;

        // gate connections between wireless interface and physical layer
        // only considered receipts as @directIn is specified and nodes send
        // directly to other nodes
        wirelessIfc.lowerLayerIn <-- radioIn;

 //ADDED 7/07/19 23h39
         // gate connections between link adaptation layer and the wireless interface
        linkAdapt.BTlowerLayerIn <-- bluetoothIfc.upperLayerOut;
        linkAdapt.BTlowerLayerOut --> bluetoothIfc.upperLayerIn;
        // gate connections between wireless interface and physical layer
        // only considered receipts as @directIn is specified and nodes send
        // directly to other nodes
        bluetoothIfc.lowerLayerIn <-- bluetoothrIn;
        

        
       

}
