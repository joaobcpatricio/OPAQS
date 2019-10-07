//
// @date: 08-11-2015
// @author: Asanga Udugama (adu@comnets.uni-bremen.de)
//
// @major changes by : João Patrício (castanheirapatricio@ua.pt)
// @date :   3-jul-2019
//
// Achievable Throughput: https://www.wolframalpha.com/input/?i=fit+%7B5%2C25.510175%7D%2C%7B10%2C24.481619%7D%2C%7B15%2C16.450636%7D+%2C%7B20+%2C8.27464%7D%2C%7B25%2C+6.756953%7D%2C%7B30%2C++5.875773%7D%2C%7B35%2C3.537482%7D%7B40%2C+0.075%7D
//
//

#include "WirelessInterface.h"

#include "BaseNodeInfo.h"


Define_Module(WirelessInterface);

void WirelessInterface::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        ownMACAddress = par("ownMACAddress").stringValue();
        wirelessRange = par("wirelessRange");
        expectedNodeTypes = par("expectedNodeTypes").stringValue();
        neighbourScanInterval = par("neighbourScanInterval");
        bandwidthBitRate = par("bandwidthBitRate");
        wirelessHeaderSize = par("wirelessHeaderSize");

        // set other parameters
        broadcastMACAddress = "FF:FF:FF:FF:FF:FF";
        minSSI=par("minSSI");
        limitQueueTime = par("limitQueueTime");
        my_limit_rate = par("my_limit_rate");


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
        ownNodeInfo->nodeWirelessIfcModule = this;


        //File stores data if I'm Src
        string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsSrc";
        string noS=ownMACAddress.substr(15,17);
        nameF.append(noS);
        nameF.append(".txt");
        //EV<<"nameF: "<<nameF<<"\n";
        ofstream outfile(nameF,ios::out);
        outfile<<"RESULTS FILE \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
        outfile.close();
        std::ofstream out(nameF, std::ios_base::app);
        auto start = std::chrono::system_clock::now();
        // Some computation here
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        out<< "Started simulation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
        out.close();

        //File stores sent data
        string nameS="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsSent";
        nameS.append(noS);
        nameS.append(".txt");
        ofstream outfileS(nameS,ios::out);
        outfileS<<"RESULTS Sent DataMsg \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
        outfileS.close();
        std::ofstream outS(nameS, std::ios_base::app);
        auto startS = std::chrono::system_clock::now();
        // Some computation here
        auto endS = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_secondsS = endS-startS;
        std::time_t end_timeS = std::chrono::system_clock::to_time_t(endS);
        outS<< "Started simulation at " << std::ctime(&end_timeS) << "elapsed time: " << elapsed_secondsS.count() << "s\n";
        outS.close();

        //File stores Received data
        string nameR="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsReceived";
                nameR.append(noS);
                nameR.append(".txt");
                ofstream outfileR(nameR,ios::out);
                outfileR<<"RESULTS Received DataMsg \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
                outfileR.close();
                std::ofstream outR(nameR, std::ios_base::app);
                auto startR = std::chrono::system_clock::now();
                // Some computation here
                auto endR = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_secondsR = endR-startR;
                std::time_t end_timeR = std::chrono::system_clock::to_time_t(endR);
                outR<< "Started simulation at " << std::ctime(&end_timeR) << "elapsed time: " << elapsed_secondsR.count() << "s\n";
                outR.close();



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
                if (dynamic_cast<WirelessInterface*>(*it) != NULL) {
                    nodeInfo->nodeWirelessIfcModule = dynamic_cast<WirelessInterface*>(*it);
                }
            }

            // wireless ifc module & mobility module must be present, else something wrong
            if (nodeInfo->nodeMobilityModule == NULL || nodeInfo->nodeWirelessIfcModule == NULL) {
                delete nodeInfo;
                continue;
            }

            allNodeInfoList.push_back(nodeInfo);
        }


        // setup first event to build neighbourhood node list and send to forwarding layer
        cMessage *sendNeighEvent = new cMessage("Send Neighbourhood Event");
        sendNeighEvent->setKind(WIRELESSINTERFACE_NEIGH_EVENT_CODE);
        scheduleAt(simTime() + neighbourScanInterval, sendNeighEvent);

        // setup pkt send event message
        sendPacketTimeoutEvent = new cMessage("Send Packet Timeout Event");
        sendPacketTimeoutEvent->setKind(WIRELESSINTERFACE_PKTSEND_EVENT_CODE);

        // setup statistics signals
        neighbourhoodSizeSignal = registerSignal("neighbourhoodSize");

    } else {
        EV_FATAL <<  WIRELESSINTERFACE_SIMMODULEINFO << "Something is radically wrong\n";
    }

}

int WirelessInterface::numInitStages() const
{
    return 3;
}

void WirelessInterface::handleMessage(cMessage *msg)
{

    // find and send neighbour list to upper layer
    if (msg->isSelfMessage() && msg->getKind() == WIRELESSINTERFACE_NEIGH_EVENT_CODE) {

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
                /*double dst=sqrt(l);
                double link_stability=-0.002*pow(dst,3)+0.0104762*pow(dst,2)+0.454762*dst+ 97.2143; //xE[5,40]
                bool loosePkt=true;
                if(dst<5){
                    loosePkt=false;
                }else{
                    loosePkt = (rand() % 100) < link_stability;
                    loosePkt=!loosePkt;
                    //EV<<"Loose:"<<loosePkt<<" val:"<<link_stability<<"\n";
                    //loosePkt=false;
                }*/

                //if(!loosePkt){
                    EV<<"SSI wireless nic: "<<valSSI<<" dBm at a dist:"<<sqrt(l)<<"m \n";
                    currentNeighbourNodeInfoList.push_back(nodeInfo);
                //}else{ EV<<"Lost neigh wifi \n"; }

            }
            iteratorAllNodeInfo++;
        }

        // emit stat signal
        emit(neighbourhoodSizeSignal, (long) currentNeighbourNodeInfoList.size());

        // if there are neighbours, send message
        if (currentNeighbourNodeInfoList.size() > 0) {

            // build message
            int neighCount = 0;

            NeighbourListMsg *neighListMsg = new NeighbourListMsg("Neighbour List Msg");
            neighListMsg->setNeighbourNameListArraySize(currentNeighbourNodeInfoList.size());
            /*//Added 26/06 17h28
            neighListMsg->setNeighPosArraySize(currentNeighbourNodeInfoList.size());*/

            neighListMsg->setNeighbourNameCount(currentNeighbourNodeInfoList.size());

            list<BaseNodeInfo*>::iterator iteratorCurrentNeighbourNodeInfo = currentNeighbourNodeInfoList.begin();
            while (iteratorCurrentNeighbourNodeInfo != currentNeighbourNodeInfoList.end()) {
                BaseNodeInfo *nodeInfo = *iteratorCurrentNeighbourNodeInfo;

                string nodeAddress = nodeInfo->nodeModule->par("ownAddress").stringValue();
                neighListMsg->setNeighbourNameList(neighCount, nodeAddress.c_str());
                /*//ADDED 26/06 17h25
                inet::Coord coorD =nodeInfo->nodeMobilityModule->getCurrentPosition();
                neighListMsg->setNeighPos(coorD.x, coorD.y);*/

                neighCount++;
                iteratorCurrentNeighbourNodeInfo++;
            }

            // send msg to upper layer
            send(neighListMsg, "upperLayerOut");

        }else {
            GraphUpdtMsg *graphMsg = new GraphUpdtMsg;
            graphMsg->setNoNeighs(true);
            send(graphMsg,"upperLayerOut");
        }

        // setup next event to build neighbourhood node list and send to forwarding layer
        cMessage *sendNeighEvent = new cMessage("Send Neighbourhood Event");
        sendNeighEvent->setKind(WIRELESSINTERFACE_NEIGH_EVENT_CODE);
        scheduleAt(simTime() + neighbourScanInterval, sendNeighEvent);

        delete msg;

    // trigger to send pending packet and setup new send
    } else if (msg->isSelfMessage() && msg->getKind() == WIRELESSINTERFACE_PKTSEND_EVENT_CODE) {

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

            /*DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);
            if (dataMsg) {
                EV<<"Wir: DataMsg:"<<dataMsg->getNMsgOrder()<<" at time:"<<simTime().dbl()<<"\n";
            }*/



            //setSentTimeSrc(msg);
            // if currently there is a pending msg, then queue this msg
            if (sendPacketTimeoutEvent->isScheduled()) {
                EV<<"Packet in queue \n";
                packetQueue.push(msg);

            // no queued msgs
            } else {

                // so setup for next message sending and start timer
                setupSendingMsg(msg);

              }

        // from lowerLayerIn
        } else {
            EV<<"Wifi sending to upper layer \n";
            // send msg to upper layer
            setReceivedTime(msg);
            setRecTimeGW(msg);
            outputResultsReceived();
            send(msg, "upperLayerOut");

        }
    }
}

void WirelessInterface::setupSendingMsg(cMessage *msg)
{
    string destinationAddress = getDestinationAddress(msg);
    bool isBroadcastMsg = FALSE;
    if (destinationAddress == broadcastMACAddress) {
        isBroadcastMsg = TRUE;
    }

    // make the neighbour list at begining of msg tx (to check later if those neighbours are still there)
    list<BaseNodeInfo*>::iterator iteratorCurrentNeighbourNodeInfo = currentNeighbourNodeInfoList.begin();
    while (iteratorCurrentNeighbourNodeInfo != currentNeighbourNodeInfoList.end()) {
        BaseNodeInfo *nodeInfo = *iteratorCurrentNeighbourNodeInfo;
        string nodeAddress = nodeInfo->nodeModule->par("ownAddress").stringValue();

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
    EV<<"Bytes to send="<<bitsToSend/8<<"\n";
    double txDuration = bitsToSend / bandwidthBitRate;

    //Added 19/09/2019 -> The txDuration is for the wifi bitrate, my_limit_rate is the limit on bitrate that I define, under the wifi bitrate.
    if(my_limit_rate>0){
        double txSumDuration=bitsToSend/my_limit_rate;
        txDuration+=txSumDuration;
    }
    EV<<"Schedule to send at "<<simTime()+txDuration<<"\n";
    // setup timer to trigger at tx duration
    scheduleAt(simTime() + txDuration, sendPacketTimeoutEvent);

}

void WirelessInterface::sendPendingMsg()
{
    // check if nodes to deliver are still in neighbourhood, if so send the packet
    list<BaseNodeInfo*>::iterator iteratorAtTxNeighbourNodeInfo = atTxNeighbourNodeInfoList.begin();
    while (iteratorAtTxNeighbourNodeInfo != atTxNeighbourNodeInfoList.end()) {
        BaseNodeInfo *atTxNeighbourNodeInfo = *iteratorAtTxNeighbourNodeInfo;
        string atTxNeighbourNodeAddress = atTxNeighbourNodeInfo->nodeModule->par("ownAddress").stringValue();

        list<BaseNodeInfo*>::iterator iteratorCurrentNeighbourNodeInfo = currentNeighbourNodeInfoList.begin();
        while (iteratorCurrentNeighbourNodeInfo != currentNeighbourNodeInfoList.end()) {
            BaseNodeInfo *currentNeighbourNodeInfo = *iteratorCurrentNeighbourNodeInfo;
            string currentNeighbourNodeAddress = currentNeighbourNodeInfo->nodeModule->par("ownAddress").stringValue();

            // check if node is still in neighbourhood
            if (atTxNeighbourNodeAddress == currentNeighbourNodeAddress) {


                //Verify if msg time has reached its limit
                EV<<"Current pkt time gen:"<<getPacketInjectedTime(currentPendingMsg)<<" and current time:"<<simTime().dbl()<<"\n";
                if((simTime().dbl()-getPacketInjectedTime(currentPendingMsg))>=limitQueueTime){
                    EV<<"Packet reached Queue time limit \n";
                    break;
                }



                //Set sending Time

                //cMessage *msgB =currentPendingMsg;
                EV<<"Sent currentPendingMsg \n";
                setSentTime(currentPendingMsg);
                setSentTimeSrc(currentPendingMsg);
                //By this time the wifi sent the packet, it's just simulating if it gets lost or not, so i can count here energy spent on sending
                setPckSentMsg(currentPendingMsg);

                // make duplicate of packet
                cPacket *outPktCopy =  dynamic_cast<cPacket*>(currentPendingMsg->dup());

                //print to trace Msg
                DataMsg *dataMsg = dynamic_cast<DataMsg*>(currentPendingMsg->dup());
                if (dataMsg) {
                    EV<<"Wir: DataMsg:"<<dataMsg->getNMsgOrder()<<" at time:"<<simTime().dbl()<<"\n";
                }





                //Added 9/09/19
                //We indtroduce delay measured on real simulations
                inet::Coord ownCoord = ownNodeInfo->nodeMobilityModule->getCurrentPosition();
                inet::Coord neighCoord = currentNeighbourNodeInfo->nodeMobilityModule->getCurrentPosition();
                double l = ((neighCoord.x - ownCoord.x) * (neighCoord.x - ownCoord.x)) + ((neighCoord.y - ownCoord.y) * (neighCoord.y - ownCoord.y));
                double bitsToSend = (outPktCopy->getByteLength() * 8) + (wirelessHeaderSize * 8);
                EV<<"R="<<sqrt(l)<<"Bits to send:"<<bitsToSend<<" AT="<<realAquaticAchievableThroughput(sqrt(l))<<"\n";

                simtime_t  delay;
                if(realAquaticAchievableThroughput(sqrt(l))==0){
                    delay = bitsToSend;
                    EV<<"SHOULD NOT END UP HERE \n";
                    EV_INFO << WIRELESSINTERFACE_SIMMODULEINFO << "Range to big for real scenario data" << "\n";
                }else{
                    delay = bitsToSend / realAquaticAchievableThroughput(sqrt(l));
                }
                EV<<"Delay is:"<<delay<<"\n";

                //Simulates measured lin stability on aquatic environments;
                double dst=sqrt(l);
                double link_stability=-0.002*pow(dst,3)+0.0104762*pow(dst,2)+0.454762*dst+ 97.2143; //xE[5,40]
                bool loosePkt=true;
                if(dst<5){
                    loosePkt=false;
                }else{
                    loosePkt = (rand() % 100) < link_stability;
                    loosePkt=!loosePkt;
                    EV<<"Loose:"<<loosePkt<<" val:"<<link_stability<<"\n";
                    //loosePkt=false;
                }


                if(!loosePkt){
                // send to node
                    outputResultsSent();
                    EV<<"Sending\n";
                    sendDirect(outPktCopy,delay,0, currentNeighbourNodeInfo->nodeModule, "radioIn");
                //sendDirect(outPktCopy,currentNeighbourNodeInfo->nodeModule, "radioIn");
                }else{
                    EV<<"Packet lost here "<<ownMACAddress<<"\n";
                }
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


//--SET/GET TIMES--------------------------------------
//Set the time that the Msg was received here
void WirelessInterface::setSentTime(cMessage *msg){
    EV<<"setSentTime\n";
    BeaconMsg *beaconMsg = dynamic_cast<BeaconMsg*>(msg);
    if (beaconMsg) {
        EV<<"Set sent time of Beacon:"<<simTime().dbl()<<"\n";
        beaconMsg->setSentTime(simTime().dbl());
        //return beaconMsg->getDestinationAddress();
    }
    DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);
    if (dataMsg) {
        EV<<"Set sent time of DataMsg:"<<simTime().dbl()<<"\n";
        dataMsg->setSentTime(simTime().dbl());

        //save info into file
                    string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsSent";
                    string noS=ownMACAddress.substr(15,17);
                    nameF.append(noS);
                    nameF.append(".txt");
                    std::ofstream out(nameF, std::ios_base::app);
                    //Generation Mac
                    string srcMAC=dataMsg->getOriginatorNodeMAC();
                    string srcer="Source: ";
                    srcer.append(srcMAC);
                    out<<srcer;
                    //Destination next
                    string dstMAC=dataMsg->getDestinationAddress();
                    string dster=" | NextDest: ";
                    dster.append(dstMAC);
                    out<<dster;
                    //Message Name
                    std::string msNam=dataMsg->getDataName();//getMsgUniqueID();
                    string msNamIs=" | Message ID: ";
                    msNamIs.append(msNam);
                    out<<msNamIs;
                    //MessageID
                    std::string msID=std::to_string(dataMsg->getNMsgOrder());//getMsgUniqueID();
                    string msIDis=" | Message ID: ";
                    msIDis.append(msID);
                    out<<msIDis;
                    //Time sent
                    std::string timeMsg = std::to_string(dataMsg->getSentTime().dbl());//getInjectedTime().dbl());
                    string timeGen=" | Time SentFromNIC: ";
                    timeGen.append(timeMsg);
                    out<<timeGen;
                    out<<" | End \n";
                    out.close();

                    resultsSentDataName(msg);



    }

}

void WirelessInterface::setSentTimeSrc(cMessage *msg){
    DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);
    if (dataMsg) {
        if(ownMACAddress==dataMsg->getOriginatorNodeMAC()){
            //dataMsg->setSentTimeRout(simTime().dbl());
            EV<<"Set sent time from src \n";

            //save info into file
            string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsSrc";
            string noS=ownMACAddress.substr(15,17);
            nameF.append(noS);
            nameF.append(".txt");
            std::ofstream out(nameF, std::ios_base::app);
            //Generation Mac
            string srcMAC=dataMsg->getOriginatorNodeMAC();
            string srcer="Source: ";
            srcer.append(srcMAC);
            out<<srcer;
            //MessageID
            std::string msID=std::to_string(dataMsg->getNMsgOrder());//getMsgUniqueID();
            string msIDis=" | Message ID: ";
            msIDis.append(msID);
            out<<msIDis;
            //Time generated
            std::string timeMsg = std::to_string(dataMsg->getSentTimeRout().dbl());//getInjectedTime().dbl());
            string timeGen=" | Time SentFromSrc: ";
            timeGen.append(timeMsg);
            out<<timeGen;
            out<<" |End \n";
            out.close();
        }
    }
}

void WirelessInterface::setRecTimeGW(cMessage *msg){
    DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);
    if (dataMsg) {
        if(ownMACAddress==dataMsg->getFinalDestinationNodeName()){
            dataMsg->setReceivedTimeRout(simTime().dbl());
            EV<<"Set received time from on GW \n";
        }
    }
}

//set the time the Msg was sent from here
void WirelessInterface::setReceivedTime(cMessage *msg){
    BeaconMsg *beaconMsg = dynamic_cast<BeaconMsg*>(msg);
    if (beaconMsg) {
        EV<<"Set received time:"<<simTime().dbl()<<"\n";
        beaconMsg->setReceivedTime(simTime().dbl());
    }
    DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);
    if (dataMsg) {
        EV<<"Set received time:"<<simTime().dbl()<<"\n";
        dataMsg->setReceivedTime(simTime().dbl());

        //save info into file
                    string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsReceived";
                    string noS=ownMACAddress.substr(15,17);
                    nameF.append(noS);
                    nameF.append(".txt");
                    std::ofstream out(nameF, std::ios_base::app);
                    //Generation Mac
                    string srcMAC=dataMsg->getOriginatorNodeMAC();
                    string srcer="Source: ";
                    srcer.append(srcMAC);
                    out<<srcer;
                    //From neighbor
                    string dstMAC=dataMsg->getSourceAddress();
                    string dster=" | From: ";
                    dster.append(dstMAC);
                    out<<dster;
                    //Message Name
                    std::string msNam=dataMsg->getDataName();//getMsgUniqueID();
                    string msNamIs=" | Message ID: ";
                    msNamIs.append(msNam);
                    out<<msNamIs;
                    //MessageID
                    std::string msID=std::to_string(dataMsg->getNMsgOrder());//getMsgUniqueID();
                    string msIDis=" | Message ID: ";
                    msIDis.append(msID);
                    out<<msIDis;
                    //Time generated
                    std::string timeMsg = std::to_string(dataMsg->getReceivedTime().dbl());//getInjectedTime().dbl());
                    string timeGen=" | Time Received Here: ";
                    timeGen.append(timeMsg);
                    out<<timeGen;
                    out<<" | End \n";
                    out.close();
    }
}

double WirelessInterface::getPacketInjectedTime(cMessage *msg){
    DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);

    if (dataMsg) {
        //return dataMsg->getSentTimeRout().dbl();
        return simTime().dbl();
    }

    AckMsg *ackMsg = dynamic_cast<AckMsg*>(msg);
    if (ackMsg) {
        return ackMsg->getInjectedTime().dbl();
    }

    DataReqMsg *dataReqMsg = dynamic_cast<DataReqMsg*>(msg);
    if (dataReqMsg) {
        return dataReqMsg->getInjectedTime().dbl();
    }

    BeaconMsg *beaconMsg = dynamic_cast<BeaconMsg*>(msg);
    if (beaconMsg) {
        return beaconMsg->getInjectedTime().dbl();
    }



    EV_FATAL <<  WIRELESSINTERFACE_SIMMODULEINFO << ">!<Unknown message type. Check \"string WirelessInterface::getPAcketInjectedTime(cMessage *msg)\"\n";
    throw cRuntimeError("Unknown message type in KWirelessnterface");
    return double();
}

//--------------------------------------------------------------------
string WirelessInterface::getDestinationAddress(cMessage *msg)
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


    EV_FATAL <<  WIRELESSINTERFACE_SIMMODULEINFO << ">!<Unknown message type. Check \"string WirelessInterface::getDestinationAddress(cMessage *msg)\"\n";

    throw cRuntimeError("Unknown message type in KWirelessnterface");

    return string();
}

//--Calculations --------------------------------------------------------------
double WirelessInterface::calculateSSI(double x){

    double ssi_ext=-0.0000207519*pow(x,4)+0.0005124292*pow(x,3)+0.0589678*pow(x,2)-2.72277*x-57.5612;
    //EV<<"SSI wireless nic: "<<ssi_ext<<" dBm \n";
    //double ssi_ext2=-0.0000207519*pow(40,4)+0.0005124292*pow(40,3)+0.0589678*pow(40,2)-2.72277*40-57.5612;
    //EV<<"SSI de teste: "<<ssi_ext2<<"\n"; //deve dar -92dBm
    return ssi_ext;

}

double WirelessInterface::realAquaticAchievableThroughput(double x){ //xE[5,40]m;
    if(x>=40){
        return 0;
    }else if(x<5){
        return abs(25*pow(10,6));
    }else{
        double AT=-0.000177*pow(x,4)+0.01603*pow(x,3)-0.472862*pow(x,2)+4.20788*x+14.6655; //Mbs/s
        //EV<<"AT:"<<abs(AT*pow(10,6))<<"AT5="<<-0.000177*pow(5,4)+0.01603*pow(5,3)-0.472862*pow(5,2)+4.20788*5+14.6655<<"AT40="<<-0.000177*pow(40,4)+0.01603*pow(40,3)-0.472862*pow(40,2)+4.20788*40+14.6655<<"\n";
        //EV<<"BIG"<<-0.000177*pow(200,4)+0.01603*pow(200,3)-0.472862*pow(200,2)+4.20788*200+14.6655<<"\n";
        return abs(AT*pow(10,6));
    }

}

//--Ener table anouncement -------------------------------------------------------
void WirelessInterface::pcktSentMsg(double size_p, bool from_GW){
    PcktSentMsg *sentMsg = new PcktSentMsg();
    sentMsg->setOwnAddr(ownMACAddress.c_str());
    sentMsg->setBit_size(size_p);
    sentMsg->setTo_Gw(from_GW);
    sentMsg->setSentTime(simTime().dbl());

    send(sentMsg,"upperLayerOut");
}
void WirelessInterface::setPckSentMsg(cMessage *msg){
    BeaconMsg *beaconMsg = dynamic_cast<BeaconMsg*>(msg);
    if (beaconMsg) {
        pcktSentMsg(beaconMsg->getRealPacketSize(), false);
    }
    DataReqMsg *dataRequestMsg = dynamic_cast<DataReqMsg*>(msg);
    if(dataRequestMsg){
        pcktSentMsg(dataRequestMsg->getRealPacketSize(), false);
    }
    DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);
    if (dataMsg) {
        pcktSentMsg(dataMsg->getRealPacketSize(), false);
    }
    AckMsg *ackMsg = dynamic_cast<AckMsg*>(msg);
    if(ackMsg){
        pcktSentMsg(ackMsg->getRealPacketSize(), false);
    }
}


//--RESULTS------------------------------------------------------------
void WirelessInterface::outputResultsSent(){
    //save info into file
                        string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/AllPktSent";
                        string noS=ownMACAddress.substr(15,17);
                        nameF.append(noS);
                        nameF.append(".txt");
                        std::ofstream out(nameF, std::ios_base::app);
                        //Generation Mac
                        string srcer="Sent ";
                        out<<srcer;
                        //Time sent
                        std::string timeMsg = std::to_string(simTime().dbl());
                        string timeGen=" | Time SentFromHere: ";
                        timeGen.append(timeMsg);
                        out<<timeGen;
                        out<<" | End \n";
                        out.close();
}
void WirelessInterface::outputResultsReceived(){
    //save info into file
                        string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/AllPktRec";
                        string noS=ownMACAddress.substr(15,17);
                        nameF.append(noS);
                        nameF.append(".txt");
                        std::ofstream out(nameF, std::ios_base::app);
                        //Generation Mac
                        string srcer="Received ";
                        out<<srcer;
                        //Time sent
                        std::string timeMsg = std::to_string(simTime().dbl());
                        string timeGen=" | Time SentFromHere: ";
                        timeGen.append(timeMsg);
                        out<<timeGen;
                        out<<" | End \n";
                        out.close();
}
void WirelessInterface::resultsSentDataName(cMessage *msg){
    DataMsg *dataMsg = dynamic_cast<DataMsg*>(msg);
    string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/dataNameSent";
                        string noS=ownMACAddress.substr(15,17);
                        nameF.append(noS);
                        nameF.append(".txt");
                        std::ofstream out(nameF, std::ios_base::app);
                        //Message Name
                        std::string msNam=dataMsg->getDataName();//getMsgUniqueID();
                        string msNamIs=msNam;
                        msNamIs.append("\n");
                        out<<msNamIs;
                        out.close();
}


void WirelessInterface::finish()
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

