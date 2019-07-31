//
// @date: 08-11-2015
// @author: Asanga Udugama (adu@comnets.uni-bremen.de)
//
// @major changes by : João Patrício (castanheirapatricio@ua.pt)
// @date :   3-jul-2019
//

#include "BluetoothInterface.h"

#include "BaseNodeInfo.h"


Define_Module(BluetoothInterface);

void BluetoothInterface::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        ownBTMACAddress = par("ownBTMACAddress").stringValue();
        wirelessRange = par("wirelessRange");
        expectedNodeTypes = par("expectedNodeTypes").stringValue();
        neighbourScanInterval = par("neighbourScanInterval");
        bandwidthBitRate = par("bandwidthBitRate");
        wirelessHeaderSize = par("wirelessHeaderSize");

        // set other parameters
        broadcastMACAddress = "FF:FF:FF:FF:FF:FF";

        //added 7/07/19 18h51
        minSSI=par("minSSI");


    } else if (stage == 1) {


        // get own module info
        ownNodeInfo = new BaseNodeInfo();
        ownNodeInfo->nodeModule = getParentModule();
        for (cModule::SubmoduleIterator it(getParentModule()); !it.end(); ++it) {
            ownNodeInfo->nodeMobilityModule = dynamic_cast<inet::IMobility*>(*it);
            if (ownNodeInfo->nodeMobilityModule != NULL) {
                break;
            }
        }
        ownNodeInfo->nodeBluetoothIfcModule = this;


    } else if (stage == 2) {

        // get module info of all other nodes in network
        for (int id = 0; id <= getSimulation()->getLastComponentId(); id++) {
            cModule *unknownModule = getSimulation()->getModule(id);
            if (unknownModule == NULL) {
                continue;
            }

            // has to be a node type module given in expectedNodeTypes parameter
            if(strstr(expectedNodeTypes.c_str(), unknownModule->getModuleType()->getName()) == NULL) {
                continue;
            }

            // if module is a KNode or KHeraldNode but is yourself
            if (unknownModule == ownNodeInfo->nodeModule) {
                continue;
            }

            BaseNodeInfo *nodeInfo = new BaseNodeInfo();
            nodeInfo->nodeModule = unknownModule;

            // find the wireless ifc module & mobility module
            for (cModule::SubmoduleIterator it(unknownModule); !it.end(); ++it) {

                if (dynamic_cast<inet::IMobility*>(*it) != NULL) {
                    nodeInfo->nodeMobilityModule = dynamic_cast<inet::IMobility*>(*it);
                }
                if (dynamic_cast<BluetoothInterface*>(*it) != NULL) {
                    nodeInfo->nodeBluetoothIfcModule = dynamic_cast<BluetoothInterface*>(*it);
                }
            }

            // wireless ifc module & mobility module must be present, else something wrong
            if (nodeInfo->nodeMobilityModule == NULL || nodeInfo->nodeBluetoothIfcModule == NULL) {
                delete nodeInfo;
                continue;
            }

            allNodeInfoList.push_back(nodeInfo);
        }

//EV<<"Check Point!\n";
        // setup first event to build neighbourhood node list and send to forwarding layer
        cMessage *sendNeighEvent = new cMessage("Send Neighbourhood Event");
       // EV<<"Check Point: \n"<<;
        sendNeighEvent->setKind(BLUETOOTHINTERFACE_NEIGH_EVENT_CODE);
        scheduleAt(simTime() + neighbourScanInterval, sendNeighEvent);

        // setup pkt send event message
        sendPacketTimeoutEvent = new cMessage("Send Packet Timeout Event");
        sendPacketTimeoutEvent->setKind(BLUETOOTHINTERFACE_PKTSEND_EVENT_CODE);

        // setup statistics signals
        neighbourhoodSizeSignal = registerSignal("neighbourhoodSize");

    } else {
        EV_FATAL <<  BLUETOOTHINTERFACE_SIMMODULEINFO << "Something is radically wrong\n";
    }

}

int BluetoothInterface::numInitStages() const
{
    return 3;
}

void BluetoothInterface::handleMessage(cMessage *msg)
{
    EV<<"Handle Message BT_Nic \n";
    // find and send neighbour list to upper layer
    if (msg->isSelfMessage() && msg->getKind() == BLUETOOTHINTERFACE_NEIGH_EVENT_CODE) {

        // init current neighbor list
        while (currentNeighbourNodeInfoList.size() > 0) {
            list<BaseNodeInfo*>::iterator iteratorCurrentNeighbourNodeInfo = currentNeighbourNodeInfoList.begin();
            BaseNodeInfo *nodeInfo = *iteratorCurrentNeighbourNodeInfo;
            currentNeighbourNodeInfoList.remove(nodeInfo);
        }

        // get current position of self
        inet::Coord ownCoord = ownNodeInfo->nodeMobilityModule->getCurrentPosition();

        // check which nodes are neighbours and if so, add to list
        list<BaseNodeInfo*>::iterator iteratorAllNodeInfo = allNodeInfoList.begin();
        while (iteratorAllNodeInfo != allNodeInfoList.end()) {
            BaseNodeInfo *nodeInfo = *iteratorAllNodeInfo;
            inet::Coord neighCoord = nodeInfo->nodeMobilityModule->getCurrentPosition();

            double l = ((neighCoord.x - ownCoord.x) * (neighCoord.x - ownCoord.x))
                + ((neighCoord.y - ownCoord.y) * (neighCoord.y - ownCoord.y));
            double r = wirelessRange * wirelessRange;
//ADDED 7/07/2019 18h47
            valSSI=(calculateSSI(sqrt(l)));
            if (l <= r && (valSSI)>=minSSI) {
                EV<<"SSI wireless nic: "<<valSSI<<" dBm \n";
                currentNeighbourNodeInfoList.push_back(nodeInfo);
            }
            iteratorAllNodeInfo++;
        }

        // emit stat signal
        emit(neighbourhoodSizeSignal, (long) currentNeighbourNodeInfoList.size());

        // if there are neighbours, send message
        if (currentNeighbourNodeInfoList.size() > 0) {

            // build message
            int neighCount = 0;

            NeighbourListMsgBT *neighListMsg = new NeighbourListMsgBT("Neighbour List Msg");
            neighListMsg->setNeighbourNameListArraySize(currentNeighbourNodeInfoList.size());
            /*//Added 26/06 17h28
            neighListMsg->setNeighPosArraySize(currentNeighbourNodeInfoList.size());*/

            neighListMsg->setNeighbourNameCount(currentNeighbourNodeInfoList.size());

            list<BaseNodeInfo*>::iterator iteratorCurrentNeighbourNodeInfo = currentNeighbourNodeInfoList.begin();
            while (iteratorCurrentNeighbourNodeInfo != currentNeighbourNodeInfoList.end()) {
                BaseNodeInfo *nodeInfo = *iteratorCurrentNeighbourNodeInfo;

                string nodeAddress = nodeInfo->nodeModule->par("ownBTAddress").stringValue();
                neighListMsg->setNeighbourNameList(neighCount, nodeAddress.c_str());
                /*//ADDED 26/06 17h25
                inet::Coord coorD =nodeInfo->nodeMobilityModule->getCurrentPosition();
                neighListMsg->setNeighPos(coorD.x, coorD.y);*/

                neighCount++;
                iteratorCurrentNeighbourNodeInfo++;
            }

            // send msg to upper layer
            send(neighListMsg, "upperLayerOut");

        }

        // setup next event to build neighbourhood node list and send to forwarding layer
        cMessage *sendNeighEvent = new cMessage("Send Neighbourhood Event");
        sendNeighEvent->setKind(BLUETOOTHINTERFACE_NEIGH_EVENT_CODE);
        scheduleAt(simTime() + neighbourScanInterval, sendNeighEvent);

        delete msg;

    // trigger to send pending packet and setup new send
    } else if (msg->isSelfMessage() && msg->getKind() == BLUETOOTHINTERFACE_PKTSEND_EVENT_CODE) {

        EV<<"Send pending Msg \n";
        // send the pending packet out
        sendPendingMsg();

        // if there are queued packets, setup for sending the next one at top of queue
        if (!packetQueue.empty()) {

            // get next at the top of queue
            cMessage *nextMsg = packetQueue.front();
            packetQueue.pop();

            // setup for next message sending and start timer
            setupSendingMsg(nextMsg);
        }

    // process a packet (arriving from upper or lower layers)
    } else {

        cGate *gate;
        char gateName[32];

       // get message arrival gate name
        gate = msg->getArrivalGate();
        strcpy(gateName, gate->getName());

        // msg from upper layer
        if (strstr(gateName, "upperLayerIn") != NULL) {


            EV<<"Upper layer message received BT_NIC \n";

            // if currently there is a pending msg, then queue this msg
            if (sendPacketTimeoutEvent->isScheduled()) {
                EV<<"Packet Queued \n";
                packetQueue.push(msg);

            // no queued msgs
            } else {
                EV<<"Set up sending Msg \n";
                // so setup for next message sending and start timer
                setupSendingMsg(msg);

              }

        // from lowerLayerIn
        } else {

            EV<<"BT sending to upper layer \n";
            // send msg to upper layer
            send(msg, "upperLayerOut");

        }
    }
}

void BluetoothInterface::setupSendingMsg(cMessage *msg)
{
    EV<<"Set up sending Msg method \n";
    string destinationAddress = getDestinationAddress(msg);
    bool isBroadcastMsg = FALSE;
    if (destinationAddress == broadcastMACAddress) {
        isBroadcastMsg = TRUE;
    }

    // make the neighbour list at begining of msg tx (to check later if those neighbours are still there)
    list<BaseNodeInfo*>::iterator iteratorCurrentNeighbourNodeInfo = currentNeighbourNodeInfoList.begin();
    while (iteratorCurrentNeighbourNodeInfo != currentNeighbourNodeInfoList.end()) {
        BaseNodeInfo *nodeInfo = *iteratorCurrentNeighbourNodeInfo;
        string nodeAddress = nodeInfo->nodeModule->par("ownBTAddress").stringValue();

        // if broadcast, add all addresses to tx time neighbour list
        // if unicast, add only the specific address
        if (isBroadcastMsg || destinationAddress == nodeAddress) {
            atTxNeighbourNodeInfoList.push_back(nodeInfo);
        }

        iteratorCurrentNeighbourNodeInfo++;
    }

    // save the msg to send
    currentPendingMsg = msg;

    // compute transmission duration
    cPacket *currentPendingPkt = dynamic_cast<cPacket*>(currentPendingMsg);
    double bitsToSend = (currentPendingPkt->getByteLength() * 8) + (wirelessHeaderSize * 8);
    double txDuration = bitsToSend / bandwidthBitRate;


    EV<<"Time for pacjettimeout event: "<<(simTime() + txDuration)<<" \n";
    // setup timer to trigger at tx duration
    scheduleAt(simTime() + txDuration, sendPacketTimeoutEvent);

}

void BluetoothInterface::sendPendingMsg()
{
    EV<<"Send pending Msg method \n";

    // check if nodes to deliver are still in neighbourhood, if so send the packet
    list<BaseNodeInfo*>::iterator iteratorAtTxNeighbourNodeInfo = atTxNeighbourNodeInfoList.begin();
    while (iteratorAtTxNeighbourNodeInfo != atTxNeighbourNodeInfoList.end()) {
        BaseNodeInfo *atTxNeighbourNodeInfo = *iteratorAtTxNeighbourNodeInfo;
        string atTxNeighbourNodeAddress = atTxNeighbourNodeInfo->nodeModule->par("ownBTAddress").stringValue();

        list<BaseNodeInfo*>::iterator iteratorCurrentNeighbourNodeInfo = currentNeighbourNodeInfoList.begin();
        while (iteratorCurrentNeighbourNodeInfo != currentNeighbourNodeInfoList.end()) {
            BaseNodeInfo *currentNeighbourNodeInfo = *iteratorCurrentNeighbourNodeInfo;
            string currentNeighbourNodeAddress = currentNeighbourNodeInfo->nodeModule->par("ownBTAddress").stringValue();

            // check if node is still in neighbourhood
            if (atTxNeighbourNodeAddress == currentNeighbourNodeAddress) {

                // make duplicate of packet
                cPacket *outPktCopy =  dynamic_cast<cPacket*>(currentPendingMsg->dup());

                EV<<"Sending direct packet \n";

                // send to node
                sendDirect(outPktCopy, currentNeighbourNodeInfo->nodeModule, "bluetoothrIn");

                break;
            }

            iteratorCurrentNeighbourNodeInfo++;
        }

        iteratorAtTxNeighbourNodeInfo++;
    }

    // remove original message
    delete currentPendingMsg;
    currentPendingMsg = NULL;

    // remove entries in list used to check neighbour list at begining of msg tx
    while (atTxNeighbourNodeInfoList.size() > 0) {
        list<BaseNodeInfo*>::iterator iteratorAtTxNeighbourNodeInfo = atTxNeighbourNodeInfoList.begin();
        BaseNodeInfo *nodeInfo = *iteratorAtTxNeighbourNodeInfo;
        atTxNeighbourNodeInfoList.remove(nodeInfo);
    }

}

string BluetoothInterface::getDestinationAddress(cMessage *msg)
{
    DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);
    if (dataMsg) {
        return dataMsg->getDestinationAddress();
    }






    //Added 17/06 22:34
    AckMsg *ackMsg = dynamic_cast<AckMsg*>(msg);
        if (ackMsg) {
            return ackMsg->getDestinationAddress();
        }

    //Added 25/07 16h11
        DataReqMsg *dataReqMsg = dynamic_cast<DataReqMsg*>(msg);
            if (dataReqMsg) {
                return dataReqMsg->getDestinationAddress();
            }
            BeaconMsg *beaconMsg = dynamic_cast<BeaconMsg*>(msg);
                if (beaconMsg) {
                    return beaconMsg->getDestinationAddress();
                }


    EV_FATAL <<  BLUETOOTHINTERFACE_SIMMODULEINFO << ">!<Unknown message type. Check \"string BluetoothInterface::getDestinationAddress(cMessage *msg)\"\n";

    throw cRuntimeError("Unknown message type in KWirelessnterface");

    return string();
}

//ADDED 7/07/19 18h53
double BluetoothInterface::calculateSSI(double x){

    double ssi_ext=-0.0000207519*pow(x,4)+0.0005124292*pow(x,3)+0.0589678*pow(x,2)-2.72277*x-57.5612;
    //EV<<"SSI wireless nic: "<<ssi_ext<<" dBm \n";
    //double ssi_ext2=-0.0000207519*pow(40,4)+0.0005124292*pow(40,3)+0.0589678*pow(40,2)-2.72277*40-57.5612;
    //EV<<"SSI de teste: "<<ssi_ext2<<"\n"; //deve dar -92dBm
    return ssi_ext;

}

void BluetoothInterface::finish()
{
    // remove send msg timeout
    if (sendPacketTimeoutEvent != NULL) {
        if (sendPacketTimeoutEvent->isScheduled()) {
            cancelEvent(sendPacketTimeoutEvent);
        }
        delete sendPacketTimeoutEvent;
    }
    // remove all messages
    while(!packetQueue.empty()) {
        cMessage *nextMsg = packetQueue.front();
        packetQueue.pop();
        delete nextMsg;
    }
    if (currentPendingMsg != NULL) {
        delete currentPendingMsg;
        currentPendingMsg = NULL;
    }
}

