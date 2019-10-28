//
// The model implementation for link adaptations between
// the used link technology (below) and the upper layer.
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 3-jul-2019
//
//Careful when comparing using strstr as two similar words can produce to unwanted actions

#include "LinkAdaptLayer.h"

Define_Module(LinkAdaptLayer);

void LinkAdaptLayer::initialize(int stage)
{
    if (stage == 0) {
        // get parameters
        ownMACAddress = par("ownMACAddress").stringValue();
        ownBTMACAddress = par("ownBTMACAddress").stringValue();
        // set other parameters
        broadcastMACAddress = "FF:FF:FF:FF:FF:FF";


    } else if (stage == 1) {
    } else if (stage == 2) {


    } else {
        EV_FATAL << LINKADAPTLAYER_SIMMODULEINFO << "Something is radically wrong\n";
    }

}

int LinkAdaptLayer::numInitStages() const
{
    return 3;
}

void LinkAdaptLayer::handleMessage(cMessage *msg)
{
    EV<<"LinkLayer handle Message \n";
    //EV<<"Test1: "<<strstr("low", "BTlow")<<"\n";

    cGate *gate;
    char gateName[64];




    // self messages
        if (msg->isSelfMessage()) {
            EV_FATAL << LINKADAPTLAYER_SIMMODULEINFO << " Something is radically wrong !!! \n";
            delete msg;

        // messages from other layers
        } else {

           // get message arrival gate name
            gate = msg->getArrivalGate();
            strcpy(gateName, gate->getName());


//NeighbourListMsg--------------------------------------------------------------------------------------------------
            // neighbour list message arrived from the lower layer (link layer)

            //neighbourList from BT
            if (strstr(gateName, "BTlowerLayerIn") != NULL && dynamic_cast<NeighbourListMsgBT*>(msg) != NULL) {
                //EV<<"NeighListBT neighLayerOut\n";
                send(msg, "neighLayerOut");
            } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<NeighbourListMsg*>(msg) != NULL) {
                //EV<<"NeighList neighLayerOut\n";
                send(msg, "neighLayerOut");


//GraphUpdtMsg--------------------------------------------------------------------------------------------------
                // graphUpdt message arrived from the lower layer (link layer)
                //neighbourList from BT
            }else if (strstr(gateName, "BTlowerLayerIn") != NULL && dynamic_cast<GraphUpdtMsgBT*>(msg) != NULL) {
                    EV<<"GraphUpdtMsg neighLayerOut\n";
                    send(msg, "neighLayerOut");
                } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<GraphUpdtMsg*>(msg) != NULL) {
                    EV<<"GraphUpdtMsg neighLayerOut\n";
                    send(msg, "neighLayerOut");



//DataMsg---------------------------------------------------------------------------------------------------------------
            // data message arrived from the lower layer (link layer)
            } else if (strstr(gateName, "BTlowerLayerIn") != NULL && dynamic_cast<DataMsg*>(msg) != NULL) {
                //EV<<"BT- DataMsg received\n";
                send(msg, "upperLayerOut");
            } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<DataMsg*>(msg) != NULL) {
                            //EV<<"Wifi- DataMsg received\n";
                            send(msg, "upperLayerOut");
            } else if (strstr(gateName, "upperLayerIn") != NULL && dynamic_cast<DataMsg*>(msg) != NULL) {
                DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);
                string SouceDAdd = dataMsg->getSourceAddress();
                if((SouceDAdd.substr(0,2))=="BT"){
                    //EV<<"BT DataMsg sent \n";
                    send(msg, "BTlowerLayerOut");
                }else{
                    //EV<<"WIFI DataMsg sent\n";
                    send(msg, "lowerLayerOut");
                }


//AckMsg----------------------------------------------------------------------------------------------------
            } else if (strstr(gateName, "BTlowerLayerIn") != NULL && dynamic_cast<AckMsg*>(msg) != NULL) {
                //EV<<"BT: AckMsg received\n";
                send(msg, "upperLayerOut");
            } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<AckMsg*>(msg) != NULL) {
                //EV<<"Wifi: AckMsg received\n";
                send(msg, "upperLayerOut");
            } else if (strstr(gateName, "upperLayerIn") != NULL && dynamic_cast<AckMsg*>(msg) != NULL) {
                AckMsg *ackMsg = dynamic_cast<AckMsg*>(msg);
                string SouceAAdd = ackMsg->getSourceAddress();

                EV<<"Teste: "<<SouceAAdd.substr(0,2)<<"\n";
                if((SouceAAdd.substr(0,2))=="BT"){
                    //EV<<"BT AckMsg sent \n";
                    send(msg, "BTlowerLayerOut");
                }else{
                    //EV<<"WIFI AckMsg sent\n";
                    send(msg, "lowerLayerOut");
                }

//BeaconMsg----------------------------------------------------------------------------------------------------

            } else if (strstr(gateName, "neighLayerIn") != NULL && dynamic_cast<BeaconMsg*>(msg) != NULL) {
                BeaconMsg *beaconMsg = dynamic_cast<BeaconMsg*>(msg);
                /*int i=0;
                i=beaconMsg->getNic();
                if(i==1){*/
                string SouceBAdd = beaconMsg->getSourceAddress();
                //EV<<"Teste: "<<SouceBAdd.substr(0,2)<<"\n";
                if((SouceBAdd.substr(0,2))=="BT"){
                   //EV<<"BT Beacon sent \n";
                   send(msg, "BTlowerLayerOut");
                }else{
                    //EV<<"WIFI Beacon sent\n";
                    send(msg, "lowerLayerOut");
                }

            } else if (strstr(gateName, "BTlowerLayerIn") != NULL && dynamic_cast<BeaconMsg*>(msg) != NULL) {
                BeaconMsg *beaconMsg = dynamic_cast<BeaconMsg*>(msg);
                string SouceBAdd = beaconMsg->getSourceAddress();
                //EV<<"Source: "<<SouceBAdd.substr(0,2)<<" \n";
                //EV<<"BT Beacon received\n";
                send(msg, "neighLayerOut");
            } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<BeaconMsg*>(msg) != NULL) {
                //EV<<"Wifi Beacon received\n";
                send(msg, "neighLayerOut");

//DataReqMsg------------------------------------------------------------------------------------------------------
            } else if (strstr(gateName, "BTlowerLayerIn") != NULL && dynamic_cast<DataReqMsg*>(msg) != NULL) {
                //EV<<"BT-DataReqMsg received\n";
                send(msg, "neighLayerOut");//"upperLayerOut");
            } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<DataReqMsg*>(msg) != NULL) {
                EV<<"wifi-DataReqMsg received\n";
                send(msg, "neighLayerOut");//"upperLayerOut");
            //ADDED 23/07/19 18h41
            } else if (strstr(gateName, "upperLayerIn") != NULL && dynamic_cast<DataReqMsg*>(msg) != NULL) {
                DataReqMsg *dataRequestMsg = dynamic_cast<DataReqMsg*>(msg);
                string SouceDRAdd = dataRequestMsg->getSourceAddress();
                //int a=0;
                //a=dataRequestMsg->getNic();
                //if(a==1){
                if((SouceDRAdd.substr(0,2))=="BT"){
                    //EV<<"BT DataReqMsg sent \n";
                    send(msg, "BTlowerLayerOut");
                }else{
                    EV<<"Wifi DataReqMsg sent\n";
                    send(msg, "lowerLayerOut");
                }


//PckSentMsg----------------------------------------------------------------------------------------------------
            } else if (strstr(gateName, "BTlowerLayerIn") != NULL && dynamic_cast<PcktIsSentMsg*>(msg) != NULL) {
                //PcktIsSentMsg *sentMsg = dynamic_cast<PcktIsSentMsg*>(msg);
                //string SouceBAdd = sentMsg->getOwnAddr();
                //EV<<"Source: "<<SouceBAdd.substr(0,2)<<" \n";
                //EV<<"BT PcktIsSentMsg received\n";
                send(msg, "neighLayerOut");
            } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<PcktIsSentMsg*>(msg) != NULL) {
                EV<<"Wifi PcktIsSentMsg received\n";
                send(msg, "neighLayerOut");


 //-------------------------------------------------------------------------------
            } else {

                EV_FATAL << LINKADAPTLAYER_SIMMODULEINFO << " Something is radically wrong !!! \n";
                delete msg;
            }
        }
}
