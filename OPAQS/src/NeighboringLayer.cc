//
// The model implementation for the Neighbouring management layer
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 19-march-2019
//

#include "NeighboringLayer.h"

#include "BaseNodeInfo.h"

Define_Module(NeighboringLayer);

void NeighboringLayer::initialize(int stage)
{
    if (stage == 0) {


        graphe=GraphT();
        log=Logger();

        // get parameters
        ownMACAddress = par("ownMACAddress").stringValue();
        ownBTMACAddress = par("ownBTMACAddress").stringValue();
        antiEntropyInterval = par("antiEntropyInterval");
        maximumRandomBackoffDuration = par("maximumRandomBackoffDuration");
        usedRNG = par("usedRNG");
        numEventsHandled = 0;
        syncedNeighbourListIHasChanged = TRUE;
        myProb=par("ProbInit");

        GWAddr=par("GWAddr").stringValue();
        distProb=0.1;
        isNeighNeigh=0;

        //Added 23/07/19
        sendWifiFirst = par("sendWifiFirst");
        msgIsBT=false;

        delayPerDataMsg = par("delayPerDataMsg");

        maximumNoVert = par("maximumNoVert");
        //graphe.maximumNoVert(maximumNoVert);
        maxLengthGraph=par("maxLengthGraph");

        max_age = par("max_age");
        max_absent_time = par("max_absent_time");

        resetGPeriod = par("resetGPeriod");


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

        for (int i = 0; i <N_nodes ; ++i) {
            Ener[i]=0;
        }


    } else if (stage == 2) {
        //calcEnerg(0,false);
        myEner=EnergyStart;
        ener_spent=0;
        MyBat=9999;

        //set period to clean graph and ener table
        cMessage *resetG = new cMessage("Reset Graph Event");
        //EV<<"Checking GW status \n";
        resetG->setKind(RESETGRAPH_EVENT_CODE);
        scheduleAt(simTime() + resetGPeriod, resetG);

    } else {
        EV_FATAL << NEIGHBORINGLAYER_SIMMODULEINFO << "Something is radically wrong in initialisation \n";
    }
}

int NeighboringLayer::numInitStages() const
{
    return 3;
}


void NeighboringLayer::handleMessage(cMessage *msg)
{

    cGate *gate;
    char gateName[64];

    numEventsHandled++;

    // self messages
    if (msg->isSelfMessage() && msg->getKind() == RESETGRAPH_EVENT_CODE){
        graphe.cleanGraph();
        cleanEnerTable();
        EV<<"Af clen:"<<graphe.returnGraphT()<<"\n";
        //set period to clean graph and ener table
                cMessage *resetG = new cMessage("Reset Graph Event");
                //EV<<"Checking GW status \n";
                resetG->setKind(RESETGRAPH_EVENT_CODE);
                scheduleAt(simTime() + resetGPeriod, resetG);

    }else if (msg->isSelfMessage()) {
        EV_INFO << NEIGHBORINGLAYER_SIMMODULEINFO << "Received unexpected self message" << "\n";
        delete msg;

    // messages from other layers
    } else {

       // get message arrival gate name
        gate = msg->getArrivalGate();
        strcpy(gateName, gate->getName());

        //Wifi Neighbour list -> DIRECT NEIGHBOURS
        if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<NeighbourListMsg*>(msg) != NULL) {
            EV<<"Neighboring: handleNeighBourListMsg vizinhos\n";
            handleNeighbourListMsgFromLowerLayer(msg);

        // Beacon message arrived from the lower layer (link layer)
        } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<BeaconMsg*>(msg) != NULL) {
            EV<<"Neighboring: handling Beacon\n";
            handleBeaconMsgFromLowerLayer(msg);

//ADDED 23/07/19 16h15
            //BT Neighbour list -> DIRECT NEIGHBOURS

        }else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<NeighbourListMsgBT*>(msg) != NULL) {
                EV<<"Neighboring: handleNeighBourListMsgBT vizinhos\n";
                updateNeighbourListBT(msg);

        // DataReq message arrived from the lower layer (link layer)
        } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<DataReqMsg*>(msg) != NULL) {
            EV<<"Neighboring: handling DataReq\n";
            handleDataReqMsgFromLowerLayer(msg);


        //Wifi GraphUpdtMsg -> DIRECT NEIGHBOURS
        }else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<GraphUpdtMsg*>(msg) != NULL) {
                EV<<"Neighboring: handleGraphUpdtMsg\n";
                handleGraphUpdtMsgFromLowerLayer(msg);
        //BT GraphUpdtMsg -> DIRECT NEIGHBOURS
        }else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<GraphUpdtMsgBT*>(msg) != NULL) {
            EV<<"Neighboring: handleGraphUpdtMsg\n";
            handleGraphUpdtMsgFromLowerLayer(msg);

        }else if (strstr(gateName, "upperLayerIn") != NULL && dynamic_cast<PcktIsSentMsg*>(msg) != NULL) {
            EV<<"Neighboring upp: handlepcktSentMsg\n";
            handlePcktSentMsg(msg);

        }else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<PcktIsSentMsg*>(msg) != NULL) {
            EV<<"Neighboring low: handlepcktSentMsg\n";
            handlePcktSentMsg(msg);

        // received some unexpected packet
        } else {

            EV_INFO << NEIGHBORINGLAYER_SIMMODULEINFO << "Received unexpected packet" << "\n";
            delete msg;
        }
    }
}


//Handle Received BeaconMsg
void NeighboringLayer::handleBeaconMsgFromLowerLayer(cMessage *msg)//neigh
{
    BeaconMsg *BeaconReceived = dynamic_cast<BeaconMsg*>(msg);

    int v = 5;//9; //there are 6 vertices in the graph
    string sourAdd = BeaconReceived->getSourceAddress();


    saveLastBeContact(sourAdd);

    cleanOldContacts();


    string myAdd;
    if((sourAdd.substr(0,2))=="BT"){
            myAdd = ownBTMACAddress;
        }else{
            myAdd=ownMACAddress;
        }
    int srcID=graphe.add_element(sourAdd);
    int myID=graphe.add_element(myAdd);

    //EV<<"Graph Before Update: \n";
    //graphe.displayMatrix(v);
   // string answb=graphe.returnGraphT();
    //EV<< "O meu v de display é:"<<v<<"\n";


//UPdate of graph
    bool updG =updateGraph(msg);
    //bool updMyG=updateMyNGraph();
    //EV<<"RSSI:"<<calculateSSI(msg)<<"\n";

    updateNeighEner(msg);

    //graphe.displayMatrix(v);
    string answ=graphe.returnGraphT();
    //EV<<"Graph After Update: "<<answ<<"\n";
    //graphe.dijkstra(myID,srcID);
    //EV<<"End test: \n";


    //Save weight on file
    string noS=ownMACAddress.substr(15,17);
    string noN=BeaconReceived->getSourceAddress();
    int weigH=graphe.returnWGrapfT(myID,srcID);
    log.saveWeight(noS, noN, weigH);
    log.saveResultsWeight(ownMACAddress, noN, weigH);
    log.saveResultsWTime(ownMACAddress, noN);

    //log.saveEnerTable(ownMACAddress, returnEnerTable());
    log.saveMyEner(ownMACAddress, myEner);


//-------------------

    double ssi_ext=calculateSSI(msg);

    myProb=updateProbability(distProb,ssi_ext);

    isNeighNeigh=BeaconReceived->getProb();

    //make Beacon copy with extra info for upper layer
    BeaconInfoMsg *infoMsg = new BeaconInfoMsg();
    infoMsg->setSourceAddress(BeaconReceived->getSourceAddress());
    infoMsg->setProb(BeaconReceived->getProb());    //Gives probability
    infoMsg->setMyProb(myProb);
    int realPacketSize = 6 + 6 + 4 + 4 + 4 + 4 + 4 + 64 + 64 + 1;
    infoMsg->setRealPacketSize(realPacketSize);
    infoMsg->setByteLength(realPacketSize);
    infoMsg->setMyPosX(BeaconReceived->getMyPosX());
    infoMsg->setMyPosY(BeaconReceived->getMyPosY());
    infoMsg->setDestinationAddress(BeaconReceived->getDestinationAddress());
    infoMsg->setNeighGraph(graphe.returnGraphT().c_str());
    infoMsg->setNumberVert(graphe.returnVertIDSize());
    infoMsg->setSentTime(BeaconReceived->getSentTime().dbl());
    infoMsg->setReceivedTime(BeaconReceived->getReceivedTime().dbl());
    infoMsg->setInjectedTime(BeaconReceived->getInjectedTime().dbl());
    infoMsg->setNeighEner(returnEnerTable().c_str());
    //infoMsg->setNic(BeaconReceived->getNic());

    //EV<<"Beac: "<<BeaconReceived->getRealPacketSize()<<" Cop of beac: "<<infoMsg->getRealPacketSize()<<" \n";

    //EV<<"Neighboring: Sending msg to upperLayer\n";
    send(infoMsg, "upperLayerOut");
    sendEnerTable();

//Added 26/07/2019
    string SouceBAdd = BeaconReceived->getSourceAddress();
    if((SouceBAdd.substr(0,2))=="BT"){
        cancelBackOffTBT(msg);
    }else{
        cancelBackOffT(msg);
    }
    //cancelBackOffT(msg);
    delete(msg);
}

//Handle Received DataReqMsg from neighbor
void NeighboringLayer::handleDataReqMsgFromLowerLayer(cMessage *msg){
    EV<<"Sending dataReq from Neigh \n";
    DataReqMsg *dataRequestMsg = dynamic_cast<DataReqMsg*>(msg);

    double timeSt=dataRequestMsg->getBeaconSentT().dbl();
    double timeRc=simTime().dbl();
    double difTims=timeRc-timeSt;
    //EV<<"Delay Beacon to Req:"<<difTims<<"\n";

    send(dataRequestMsg, "upperLayerOut");
}

/*******************************************************************************************
 *Handles Msg from lower layer (Wireless)
 */
void NeighboringLayer::handleNeighbourListMsgFromLowerLayer(cMessage *msg)//neigh (uses cache)
{   // if(0.5<= simTime().dbl()){
    updateNeighbourList(msg);//}
}


//-----
/********************************************************************************************************
 *Verifies neighborhood and updates the neighbors list and checks if GW is my neighbor
 */
void NeighboringLayer::updateNeighbourList(cMessage *msg){ //por fazer
    //Remove no longer direct-neighbors from me on graph
    //EV<<"updateNeighList \n";
    bool updtMyNeig=updateMyNGraph(msg);
    string graf=graphe.returnGraphT();
    //EV<<"Graph by each neigh is:"<<graf<<"\n";
    NeighbourListMsg *neighListMsg = dynamic_cast<NeighbourListMsg*>(msg);
     // if no neighbours or cache is empty, just return
     if (neighListMsg->getNeighbourNameListArraySize() == 0){       //AQUI VIA SE A CACHE ESTAVA VAZIA
         // setup sync neighbour list for the next time - only if there were some changes
         if (syncedNeighbourListIHasChanged) {
             setSyncingNeighbourInfoForNoNeighbours();
             syncedNeighbourListIHasChanged = FALSE;
         }
         delete msg;
         return;
     }
     // send summary vector messages (if appropriate) to all nodes to sync in a loop
     int i = 0;
     while (i < neighListMsg->getNeighbourNameListArraySize()) {
         string nodeMACAddress = neighListMsg->getNeighbourNameList(i);
         EV<<"The neigh is:"<<nodeMACAddress<<"\n";
         // get syncing info of neighbor
         SyncedNeighbour *syncedNeighbour = getSyncingNeighbourInfo(nodeMACAddress);

         // indicate that this node was considered this time
         syncedNeighbour->nodeConsidered = TRUE;

         bool syncWithNeighbour = FALSE;

         if (syncedNeighbour->syncCoolOffEndTime >= simTime().dbl()) {
              EV<<"if the sync was done recently, don't sync again until the anti-entropy interval has elapsed \n";
             syncWithNeighbour = FALSE;

         } else if (syncedNeighbour->randomBackoffStarted && syncedNeighbour->randomBackoffEndTime >= simTime().dbl()) {
              EV<<"if random backoff to sync is still active, then wait until time expires \n";
             syncWithNeighbour = FALSE;

         } else if (syncedNeighbour->neighbourSyncing && syncedNeighbour->neighbourSyncEndTime >= simTime().dbl()) {
              EV<<"if this neighbour has started syncing with me, then wait until this neighbour finishes \n";
             syncWithNeighbour = FALSE;


         } else if (syncedNeighbour->randomBackoffStarted && syncedNeighbour->randomBackoffEndTime < simTime().dbl()) {
              EV<<"has the random backoff just finished - if so, then my turn to start the syncing process \n";
             syncWithNeighbour = TRUE;


         } else if (syncedNeighbour->neighbourSyncing && syncedNeighbour->neighbourSyncEndTime < simTime().dbl()) {
              EV<<"has the neighbours syncing period elapsed - if so, my turn to sync \n";
             syncWithNeighbour = TRUE;

         } else {
              EV<<"neighbour seen for the first time (could also be after the cool off period) then start the random backoff \n";
             syncedNeighbour->randomBackoffStarted = TRUE;
             double randomBackoffDuration = uniform(1.0, maximumRandomBackoffDuration, usedRNG);
             syncedNeighbour->randomBackoffEndTime = simTime().dbl() + randomBackoffDuration;
             syncWithNeighbour = FALSE;
         }
         // from previous questions - if syncing required
         if (syncWithNeighbour) {
             // set the cooloff period
             syncedNeighbour->syncCoolOffEndTime = simTime().dbl() + antiEntropyInterval;
             // initialize all other checks
             syncedNeighbour->randomBackoffStarted = FALSE;
             syncedNeighbour->randomBackoffEndTime = 0.0;
             syncedNeighbour->neighbourSyncing = FALSE;
             syncedNeighbour->neighbourSyncEndTime = 0.0;

             //Added 23/07/19 15h25
             if(sendWifiFirst){
                 //bool updtMyNeig=updateMyNGraph(msg);

                 EV<<"Send Beacon through Wifi\n";
                 msgIsBT=false;
                 // send beacon (to start syncing)
                 BeaconMsg *beaconMsg = makeBeaconVectorMessage(msg);
                 beaconMsg->setDestinationAddress(nodeMACAddress.c_str());
                 //Added 27/06 1h26
                 //get my prob based on distance
                 distProb=GWisMyNeigh(msg);
                 //EV<<"Set on beacon g: "<<beaconMsg->getNeighGraph()<<"\n";
                 //EV<<"Destin: "<<beaconMsg->getDestinationAddress()<<"\n";
                 send(beaconMsg, "lowerLayerOut");
             }
         }
         i++;
     }
     // setup sync neighbour list for the next time
     setSyncingNeighbourInfoForNextRound();
     // synched neighbour list must be updated in next round
     // as there were changes
     syncedNeighbourListIHasChanged = TRUE;


    // EV<<"Updated remove from graph\n";
     //string answ=graphe.returnGraphT();

     GraphUpdtMsg *graphUpdt = makeGraphUpdtMessage();
     graphUpdt->setNoNeighs(false);
     //EV<<"Neighboring: Sending GraphUpdtmsg to RoutingLayer\n";
     send(graphUpdt,"upperLayerOut");
     sendEnerTable();


     // delete the received neighbor list msg
     delete msg;
}
/********************************************************************************************************
 *BLUETOOTH Verifies neighborhood and updates the neighbors list and checks if GW is my neighbor
 */
void NeighboringLayer::updateNeighbourListBT(cMessage *msg){ //por fazer
    EV<<"N: updateNeighbourListBT \n";
     NeighbourListMsgBT *neighListMsg = dynamic_cast<NeighbourListMsgBT*>(msg);
     // if no neighbours or cache is empty, just return
     if (neighListMsg->getNeighbourNameListArraySize() == 0){       //AQUI VIA SE A CACHE ESTAVA VAZIA
         // setup sync neighbour list for the next time - only if there were some changes
         if (syncedNeighbourListBTHasChanged) {
             setSyncingNeighbourInfoForNoNeighboursBT();
             syncedNeighbourListBTHasChanged = FALSE;
         }
         delete msg;
         return;
     }
     // send Beacon messages (if appropriate) to all nodes to sync in a loop
     int i = 0;
     while (i < neighListMsg->getNeighbourNameListArraySize()) {
         string nodeMACAddress = neighListMsg->getNeighbourNameList(i);

         // get syncing info of neighbor
         SyncedNeighbour *syncedNeighbour = getSyncingNeighbourInfoBT(nodeMACAddress);

         // indicate that this node was considered this time
         syncedNeighbour->nodeConsidered = TRUE;

         bool syncWithNeighbour = FALSE;

         if (syncedNeighbour->syncCoolOffEndTime >= simTime().dbl()) {
              EV<<"if the sync was done recently, don't sync again until the anti-entropy interval has elapsed \n";
              //EV<<"timee: "<<simTime().dbl()<<"time Neigh: "<<syncedNeighbour->syncCoolOffEndTime<<" \n";
             syncWithNeighbour = FALSE;

         } else if (syncedNeighbour->randomBackoffStarted && syncedNeighbour->randomBackoffEndTime >= simTime().dbl()) {
              EV<<"BT: if random backoff to sync is still active, then wait until time expires \n";
             syncWithNeighbour = FALSE;

         } else if (syncedNeighbour->neighbourSyncing && syncedNeighbour->neighbourSyncEndTime >= simTime().dbl()) {
              EV<<"if this neighbour has started syncing with me, then wait until this neighbour finishes \n";
             syncWithNeighbour = FALSE;


         } else if (syncedNeighbour->randomBackoffStarted && syncedNeighbour->randomBackoffEndTime < simTime().dbl()) {
              EV<<"has the random backoff just finished - if so, then my turn to start the syncing process \n";
             syncWithNeighbour = TRUE;

         } else if (syncedNeighbour->neighbourSyncing && syncedNeighbour->neighbourSyncEndTime < simTime().dbl()) {
              EV<<"has the neighbours syncing period elapsed - if so, my turn to sync \n";
             syncWithNeighbour = TRUE;

         } else {
              EV<<"neighbour seen for the first time (could also be after the cool off period) then start the random backoff \n";
             syncedNeighbour->randomBackoffStarted = TRUE;
             double randomBackoffDuration = uniform(1.0, maximumRandomBackoffDuration, usedRNG);
             syncedNeighbour->randomBackoffEndTime = simTime().dbl() + randomBackoffDuration;
             syncWithNeighbour = FALSE;
         }
         // from previous questions - if syncing required
         if (syncWithNeighbour) {
             // set the cooloff period
             syncedNeighbour->syncCoolOffEndTime = simTime().dbl() + antiEntropyInterval;
             // initialize all other checks
             syncedNeighbour->randomBackoffStarted = FALSE;
             syncedNeighbour->randomBackoffEndTime = 0.0;
             syncedNeighbour->neighbourSyncing = FALSE;
             syncedNeighbour->neighbourSyncEndTime = 0.0;

             //Added 23/07/19 15h25
             if(!sendWifiFirst){


                 //Remove no longer direct-neighbors from me on graph
                      bool updtMyNeig=updateMyNGraph(msg);
                      // EV<<"Updated remove from graph\n";
                      //string answ=graphe.returnGraphT();

                 EV<<"N: Send Beacon through BT\n";
                 msgIsBT=true;
                 // send beacon (to start syncing)
                 BeaconMsg *beaconMsg = makeBeaconVectorMessage(msg);
                 beaconMsg->setDestinationAddress(nodeMACAddress.c_str());
                 //Added 27/06 1h26
                 //get my prob based on distance
                 distProb=GWisMyNeighBT(msg);
                 //EV<<"Sent lowerLayerOutBT";
                 //calcEnerg(beaconMsg->getRealPacketSize());
                 send(beaconMsg, "lowerLayerOut");
             }
         }
         i++;
     }
     // setup sync neighbour list for the next time
     setSyncingNeighbourInfoForNextRoundBT();
     // synched neighbour list must be updated in next round
     // as there were changes
     syncedNeighbourListBTHasChanged = TRUE;


     //Remove no longer direct-neighbors from me on graph
    // bool updtMyNeig=updateMyNGraph(msg);
     // EV<<"Updated remove from graph\n";
     //string answ=graphe.returnGraphT();


     // delete the received neighbor list msg
     delete msg;
}

/*************************************************************************************************
 *Makes Msg that is sent under
 */
BeaconMsg* NeighboringLayer::makeBeaconVectorMessage(cMessage *msg)//cache
{
    EV << "Neighboring: makeBeaconVectorMessage\n";
    //myProb=updateProbability(msg,ssi_ext);


//My position:
    inet::Coord ownCoord = ownNodeInfo->nodeMobilityModule->getCurrentPosition();
    //EV<<"My x= "<<ownCoord.x<<" My y= "<<ownCoord.y<<" \n";

    // make a summary vector message
    BeaconMsg *beaconMsg = new BeaconMsg();
    if(msgIsBT){
        beaconMsg->setSourceAddress(ownBTMACAddress.c_str());
        //beaconMsg->setNic(1);
    }else{
        //beaconMsg->setNic(0);
        beaconMsg->setSourceAddress(ownMACAddress.c_str());
    }
    beaconMsg->setProb(myProb);
    //EV<<" My current Prob is: "<<myProb<<" \n";
    int realPacketSize = 6 + 6 + 4 + 4 + 4 + 64 + 64 + 1;//(1 * NEIGHBORINGLAYER_MSG_ID_HASH_SIZE); //REVER TAMANHO AQUI CORRETO
    beaconMsg->setRealPacketSize(realPacketSize);
    beaconMsg->setByteLength(realPacketSize);
    beaconMsg->setMyPosX(ownCoord.x);
    beaconMsg->setMyPosY(ownCoord.y);
    //Added 5/09/19
    beaconMsg->setNeighGraph(graphe.returnGraphT().c_str());
    //EV<<"Set on beacon g: "<<graphe.returnGraphT().c_str()<<"\n";
    beaconMsg->setNeighEner(returnEnerTable().c_str());
    beaconMsg->setNumberVert(graphe.returnVertIDSize());
    beaconMsg->setInjectedTime(simTime().dbl()); //timeStamp , generation time

    return beaconMsg;
    //Aqui cria o beacon de broadcast que faz reconhecer a vizinhança
}

/**********************************************************************************************************
 * ⇒ Receives a node's MACAdd, checks if it's on the list, if not it adds it at the list end; Returns the memory add where the syncedNeighbour(node) is saved;
 */
NeighboringLayer::SyncedNeighbour* NeighboringLayer::getSyncingNeighbourInfo(string nodeMACAddress)//neigh
{
    // check if sync entry is there
    SyncedNeighbour *syncedNeighbour = NULL;
    list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour;
    bool found = FALSE;
    iteratorSyncedNeighbour = syncedNeighbourList.begin();
    while (iteratorSyncedNeighbour != syncedNeighbourList.end()) {
        syncedNeighbour = *iteratorSyncedNeighbour;
        if (syncedNeighbour->nodeMACAddress == nodeMACAddress) {
            found = TRUE;
            break;
        }
        iteratorSyncedNeighbour++;
    }
    if (!found) {
        // if sync entry not there, create an entry with initial values
        syncedNeighbour = new SyncedNeighbour;
        syncedNeighbour->nodeMACAddress = nodeMACAddress.c_str();
        syncedNeighbour->syncCoolOffEndTime = 0.0;
        syncedNeighbour->randomBackoffStarted = FALSE;
        syncedNeighbour->randomBackoffEndTime = 0.0;
        syncedNeighbour->neighbourSyncing = FALSE;
        syncedNeighbour->neighbourSyncEndTime = 0.0;
        syncedNeighbour->nodeConsidered = FALSE;
        syncedNeighbourList.push_back(syncedNeighbour);
    }
    return syncedNeighbour;
}
NeighboringLayer::SyncedNeighbour* NeighboringLayer::getSyncingNeighbourInfoBT(string nodeMACAddress)//neigh
{
    // check if sync entry is there
    SyncedNeighbour *syncedNeighbour = NULL;
    list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour;
    bool found = FALSE;
    iteratorSyncedNeighbour = syncedNeighbourListBT.begin();
    while (iteratorSyncedNeighbour != syncedNeighbourListBT.end()) {
        syncedNeighbour = *iteratorSyncedNeighbour;
        if (syncedNeighbour->nodeMACAddress == nodeMACAddress) {
            found = TRUE;
            break;
        }
        iteratorSyncedNeighbour++;
    }
    if (!found) {
        // if sync entry not there, create an entry with initial values
        syncedNeighbour = new SyncedNeighbour;
        syncedNeighbour->nodeMACAddress = nodeMACAddress.c_str();
        syncedNeighbour->syncCoolOffEndTime = 0.0;
        syncedNeighbour->randomBackoffStarted = FALSE;
        syncedNeighbour->randomBackoffEndTime = 0.0;
        syncedNeighbour->neighbourSyncing = FALSE;
        syncedNeighbour->neighbourSyncEndTime = 0.0;
        syncedNeighbour->nodeConsidered = FALSE;
        syncedNeighbourListBT.push_back(syncedNeighbour);
    }
    return syncedNeighbour;
}

/***********************************************************************************************************
 * Cleans syncNeighbourList
 */
void NeighboringLayer::setSyncingNeighbourInfoForNoNeighbours()//neigh
{
    // loop thru syncing neighbor list and set for next round
    list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour;
    iteratorSyncedNeighbour = syncedNeighbourList.begin();
    while (iteratorSyncedNeighbour != syncedNeighbourList.end()) {
        SyncedNeighbour *syncedNeighbour = *iteratorSyncedNeighbour;
        syncedNeighbour->randomBackoffStarted = FALSE;
        syncedNeighbour->randomBackoffEndTime = 0.0;
        syncedNeighbour->neighbourSyncing = FALSE;
        syncedNeighbour->neighbourSyncEndTime = 0.0;
        syncedNeighbour->nodeConsidered = FALSE;
        iteratorSyncedNeighbour++;
    }
}
void NeighboringLayer::setSyncingNeighbourInfoForNoNeighboursBT()//neigh
{
    // loop thru syncing neighbor list and set for next round
    list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour;
    iteratorSyncedNeighbour = syncedNeighbourListBT.begin();
    while (iteratorSyncedNeighbour != syncedNeighbourListBT.end()) {
        SyncedNeighbour *syncedNeighbour = *iteratorSyncedNeighbour;
        syncedNeighbour->randomBackoffStarted = FALSE;
        syncedNeighbour->randomBackoffEndTime = 0.0;
        syncedNeighbour->neighbourSyncing = FALSE;
        syncedNeighbour->neighbourSyncEndTime = 0.0;
        syncedNeighbour->nodeConsidered = FALSE;
        iteratorSyncedNeighbour++;
    }
}

/********************************************************************************************************
 * ⇒ Verifies if neighbor node was considered, case not it activates flags as it wasn't in the neighborhood
*********************************************************************************************************/
void NeighboringLayer::setSyncingNeighbourInfoForNextRound()//neigh
{
    //⇒ Verifies if neighbor node was considered, case not it activates flags as it wasn't in the neighborhood
    // loop thru syncing neighbor list and set for next round
    list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour;
    iteratorSyncedNeighbour = syncedNeighbourList.begin();
    while (iteratorSyncedNeighbour != syncedNeighbourList.end()) {
        SyncedNeighbour *syncedNeighbour = *iteratorSyncedNeighbour;
        if (!syncedNeighbour->nodeConsidered) {
            // if neighbour not considered this time, then it means the neighbour was not in my neighbourhood - so init all flags and timers
            syncedNeighbour->randomBackoffStarted = FALSE;
            syncedNeighbour->randomBackoffEndTime = 0.0;
            syncedNeighbour->neighbourSyncing = FALSE;
            syncedNeighbour->neighbourSyncEndTime = 0.0;
        }
        // setup for next time
        syncedNeighbour->nodeConsidered = FALSE;
        iteratorSyncedNeighbour++;
    }
}
void NeighboringLayer::setSyncingNeighbourInfoForNextRoundBT()//neigh
{
    //⇒ Verifies if neighbor node was considered, case not it activates flags as it wasn't in the neighborhood
    // loop thru syncing neighbor list and set for next round
    list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour;
    iteratorSyncedNeighbour = syncedNeighbourListBT.begin();
    while (iteratorSyncedNeighbour != syncedNeighbourListBT.end()) {
        SyncedNeighbour *syncedNeighbour = *iteratorSyncedNeighbour;
        if (!syncedNeighbour->nodeConsidered) {
            // if neighbour not considered this time, then it means the neighbour was not in my neighbourhood - so init all flags and timers
            syncedNeighbour->randomBackoffStarted = FALSE;
            syncedNeighbour->randomBackoffEndTime = 0.0;
            syncedNeighbour->neighbourSyncing = FALSE;
            syncedNeighbour->neighbourSyncEndTime = 0.0;
        }
        // setup for next time
        syncedNeighbour->nodeConsidered = FALSE;
        iteratorSyncedNeighbour++;
    }
}

void NeighboringLayer::saveLastBeContact(string Naddress){//neigh

    list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour;
    iteratorSyncedNeighbour = syncedNeighbourList.begin();
    while (iteratorSyncedNeighbour != syncedNeighbourList.end()) {
        SyncedNeighbour *syncedNeighbour = *iteratorSyncedNeighbour;
        if (syncedNeighbour->nodeMACAddress.c_str()==Naddress) {
            syncedNeighbour->lastBrecT = simTime().dbl();
            //EV<<"Last contact time saved \n";
            break;
        }

        iteratorSyncedNeighbour++;
    }
}

void NeighboringLayer::cancelBackOffT(cMessage *msg){ //vector<string> & selectedMessageIDList, cMessage *msg){ //REVER PARA FUTURO
    EV<<"Canceling BackOffT\n";
    //vector<string> selectedMessageIDList;
    BeaconMsg *beaconMsg = dynamic_cast<BeaconMsg*>(msg);

    // cancel the random backoff timer (because neighbour started syncing)
    string nodeMACAddress = beaconMsg->getSourceAddress();
    SyncedNeighbour *syncedNeighbour = getSyncingNeighbourInfo(nodeMACAddress);
    syncedNeighbour->randomBackoffStarted = FALSE;
    syncedNeighbour->randomBackoffEndTime = 0.0;
    // second - start wait timer until neighbour has finished syncing
    syncedNeighbour->neighbourSyncing = TRUE;
    //double delayPerDataMessage = 0.1; // assume 100ms//500 milli seconds per data message
    syncedNeighbour->neighbourSyncEndTime = simTime().dbl() + (1 * delayPerDataMsg);//(selectedMessageIDList.size() * delayPerDataMessage); //REVER PARA FUTURO
    // synched neighbour list must be updated in next round
    // as there were changes
    syncedNeighbourListIHasChanged = TRUE;
    //EV<<"BackOffT canceled\n";
}
void NeighboringLayer::cancelBackOffTBT(cMessage *msg){ //vector<string> & selectedMessageIDList, cMessage *msg){ //REVER PARA FUTURO
    EV<<"Canceling BackOffT\n";
    //vector<string> selectedMessageIDList;
    BeaconMsg *beaconMsg = dynamic_cast<BeaconMsg*>(msg);

    // cancel the random backoff timer (because neighbour started syncing)
    string nodeMACAddress = beaconMsg->getSourceAddress();
    SyncedNeighbour *syncedNeighbour = getSyncingNeighbourInfoBT(nodeMACAddress);
    syncedNeighbour->randomBackoffStarted = FALSE;
    syncedNeighbour->randomBackoffEndTime = 0.0;
    // second - start wait timer until neighbour has finished syncing
    syncedNeighbour->neighbourSyncing = TRUE;
    //double delayPerDataMessage = 0.1; // assume 100ms//500 milli seconds per data message
    syncedNeighbour->neighbourSyncEndTime = simTime().dbl() + (1 * delayPerDataMsg);//(selectedMessageIDList.size() * delayPerDataMessage); //REVER PARA FUTURO
    // synched neighbour list must be updated in next round
    // as there were changes
    syncedNeighbourListBTHasChanged = TRUE;
    EV<<"BackOffT canceled\n";
}
//--------------------------------------------------------------------------------------------------------------------------------------------

//--GRAPH Methods----------------------------------------------------------
/*************************************************************************
 * ⇒ Checks my Neighbor list and removes no longer direct-neighbors from Graph.
 */
bool NeighboringLayer::updateMyNGraph(cMessage *msg){
 NeighbourListMsg *neighListMsg = dynamic_cast<NeighbourListMsg*>(msg);
 EV<<"My g bef updtN"<<graphe.returnGraphT()<<"\n";

    bool found = FALSE;
    string addrN;
    int o=0;
    int myID=graphe.add_element(ownMACAddress);
    for(int i=0;i<maxLengthGraph;i++){
        while (o < neighListMsg->getNeighbourNameListArraySize()){//checks if id is direct neigh
            addrN = neighListMsg->getNeighbourNameList(o);
            //EV<<"AddrN: "<<addrN<<"\n";
            int idN=std::stoi( addrN.substr(15,17));
            //EV<<"AddrNum: "<<idN<<"\n";
            if (i==idN) {

                /*double answf =findInNeigLayerList(addrN);
                if(answf!=0){
                   double difTime= simTime().dbl()-answf;
                   if(difTime<=max_absent_time){
                       //EV<<"idN:"<<idN<<"\n";
                       found = TRUE;
                       break;
                   }else{
                       EV<<"Absent too long \n";
                   }
                }else{
                    found =TRUE;
                    break;
                }
            }
            if(found){
                break;*/


               found=TRUE;
               break;
            }
            o++;
        }
        if(!found){ //if it's not, clean m
            if(i!=myID){
                EV<<"updateMyNGraph->rem id:"<<i<<"\n";
                graphe.rem_edge(myID,i);
                //removEdge(i); not here because it doesn't know if its neigh at more tah 1 hop
            }
        }
        found=FALSE;
        o=0;
    }


    //check graph and clean if there is no path
    //EV<<"Sauce:"<<ownMACAddress<<"\n";
    myID=graphe.add_element(ownMACAddress);
    //EV<<"Now:"<<myID<<"\n";
    for(int p=0;p<graphe.returnVvalue();p++){
        string spath=graphe.returnShortestPath(myID, p);
        //EV<<"Stfu:"<<spath<<"\n";
        if(spath==""){
            for(int p2=0;p2<graphe.returnVvalue();p2++){
                graphe.rem_edge(p,p2);
                //EV<<"rem path:"<<p<<" by "<<myID<<"\n";
                removEdge(p);   //here if no path, then delete this node's info from table
                //EV<<"Cleaning graph \n";
            }
            EV<<"rem path:"<<p<<" by "<<myID<<"\n";

        }
    }

    //graphe.rem_dge(10)
    string graf=graphe.returnGraphT();
        EV<<"Graph by MAA:"<<graf<<"\n";

    return true;
}

/*************************************************************************
 * ⇒ Saves the received graph from neighboring here for later use in decision;
 */
bool NeighboringLayer::updateGraph(cMessage *msg){ //String:" 1->2:4;\n2->1:4;\n "
    BeaconMsg *BeaconReceived = dynamic_cast<BeaconMsg*>(msg);
    string srcAdd=BeaconReceived->getSourceAddress();
    string graphS=BeaconReceived->getNeighGraph();
    string myAdd=ownMACAddress;
    //EV<<"My g bef updt"<<graphe.returnGraphT()<<"\n";
    //EV<<"Graph Recebido do: "<<srcAdd<<" é "<<graphS<<" de comprimento:"<<graphS.length()<<"\n";
    int srcID=graphe.add_element(srcAdd);
    int myID=graphe.add_element(myAdd);
    int weight =2;

    int array[maxLengthGraph][maxLengthGraph];
    for(int p1=0;p1<maxLengthGraph;p1++){
        for(int p2=0;p2<maxLengthGraph;p2++){
            array[p1][p2]=0;//-1;
        }
    }

    std::string delimiter = ";";
    int i=0;//, q1=0;
    for(i=0;i<graphS.length();i++){
        int j=graphS.find(delimiter,i);
        if(j==std::string::npos){
            return false;
        }else{
            std::string token = graphS.substr(i, j-i);
            int q1 = graphS.find("-",i);
            int q2 = graphS.find(":",i);
            string v1=graphS.substr(i,q1-i);
            string v2=graphS.substr(q1+2,q2-(q1+2));
            string w1=graphS.substr(q2+1,j-(q2+1));
            int vert1 = std::stoi (v1);
            int vert2 = std::stoi (v2);
            int weight1 = std::stod (w1);
            //EV<<"WEI: "<<w1<<"\n";
            array[vert1][vert2]=weight1;
            array[vert2][vert1]=weight1;
            i =j+1;
            //EV<<"i="<<i<<"\n";
        }
    }
   /* EV<<"Array recebido: \n";
    int f=0, g=0;
    //int count=0;
    for(f = 0; f < 5; f++) {
        for(g = 0; g < 5; g++) {
            EV << array[f][g] << " ";
        }
        EV<<"\n";
    }*/

    //Clean direct-link line of source
    for(int h=0;h<maxLengthGraph;h++){
        graphe.rem_edge(srcID, h);
    }

    bool im_in_path=false;
    int o=0;
    //Adding the received graph to mine
    for(int s=0;s<maxLengthGraph;s++){//graphe.returnMaxNoVert();s++){
        o=s;
        for(o;o<maxLengthGraph;o++){//graphe.returnMaxNoVert();o++){
            //see if i'm not in path
            string spath=graphe.returnShortestPath(srcID, o);
            //EV<<"ShrtP s o:"<<spath<< "with length "<<spath.length()<<"\n";
            if(spath!=""){
                int posi=0;
                for(int st=0; st<spath.length();st++){
                    //EV<<"st:"<<spath[1]<<"\n";
                    posi= spath.find(">",st);
                    //EV<<"p1:"<<posi<<"\n";
                    int vID = std::stoi (spath.substr(st,posi-1-st));
                    //EV<<"Me:"<<myID<<"him"<<vID<<"\n";
                    if(vID==myID){
                       // EV<<"in path \n";
                        im_in_path=true;
                    }else{im_in_path=false;}
                    //int p2 = graphS.find("-",p1);*/
                    st=posi;
                }
            }



            //if it's not my position, if the array has value, if the info is not about a direct neigh  of mine, or it is but also of src
            if((s!=myID && o!=myID)){//}&& array[s][o]>=0){

   // EV<<"ups"<<s<<";"<<o<<"mys"<<array[s][myID]<<"by me"<<graphe.returnWGrapfT(s,myID)<<"myo"<<array[myID][o]<<"by me"<<graphe.returnWGrapfT(o,myID)<< "w:"<<array[s][o]<<"from"<<srcID<<"\n";
                //if((graphe.returnWGrapfT(myID, s)<=0 && o==srcID)||(graphe.returnWGrapfT(myID,o)<=0 && s==srcID)){// && !im_in_path){////||graphe.returnWGrapfT(srcID, s)>0)){
                if((!im_in_path) && (o==srcID || s==srcID)){ //|| array[s][o]>0
                    //nao estou no shortpth e é sobre alguém vizinho direto do src
                    graphe.add_edge(s,o,array[s][o]);
                   // EV<<"Added: "<<array[s][o]<<"at"<<s<<o<<" from"<<srcID<<"\n";
            /*  }else if(graphe.returnWGrapfT(myID, s)<=0 && graphe.returnWGrapfT(myID, o)<=0 && s!=srcID && o!=srcID ){
                    graphe.add_edge(s,o,array[s][o]);
                    EV<<"Added at hop w info "<<s<<o<<"\n";*/
                }else if((!im_in_path) && s!=srcID && o!=srcID && array[s][o]>0 && graphe.returnWGrapfT(myID, s)<=0 && graphe.returnWGrapfT(myID, o)<=0){
                    //if im not in shrtpth and they're not src
                    graphe.add_edge(s,o,array[s][o]);
                    //if(s==0 && o==3){
                   // EV<<"Added at hop w info "<<s<<o<<" w "<<array[s][o]<<"\n";
                    //}
                }else if((!im_in_path || array[s][o]>0) && s!=srcID && o!=srcID && graphe.returnWGrapfT(myID, s)<=0 && graphe.returnWGrapfT(myID, o)<=0){//array[myID][o]<=0 && array[myID][s]<=0){
                    //if i'm not in path or they're direct neighs, if they're not src or my direct neigh, add any value
                    graphe.add_edge(s,o,array[s][o]);
                    //if(s==3 && o==0){
                    //EV<<"Added at hop w info2 "<<s<<o<<" w "<<array[s][o]<<"\n";
                    //}
                }
            }
            if(((s==myID && o==srcID)||(s==srcID && o==myID))&& array[s][o]>=0){
                graphe.add_edge(s,o,array[s][o]);
               // EV<<"Added2: "<<array[s][o]<<"at"<<s<<o<<" from"<<srcID<<"\n";
            }
        }
    }
    //UpdateMyGraph (OLD)
/*    double ssi_ext=fabs(calculateSSI(msg));
    int ssi_extInt = static_cast<int>(ssi_ext < 0 ? ssi_ext - 0.5 : ssi_ext + 0.5);
    graphe.add_edge(myID, srcID, ssi_extInt);
    //graphe.displayMatrix(maxLengthGraph);
*/



    //My Weight:
    double myCalcWeight=(1-calcMyLQE(msg))*100;
    int myCalcWeight_Int = static_cast<int>(myCalcWeight < 0 ? myCalcWeight - 0.5 : myCalcWeight + 0.5);
    //EV<<"MyCalcWeight="<<myCalcWeight<<" MyCalcWeight_Int"<<myCalcWeight_Int<<"\n";
    graphe.add_edge(myID, srcID, myCalcWeight_Int);

    //EV<<"My"<<myID<<" g aft updt"<<graphe.returnGraphT()<<"\n";
    return true;
}


GraphUpdtMsg* NeighboringLayer::makeGraphUpdtMessage(){
    // make a graph message
    GraphUpdtMsg *graphMsg = new GraphUpdtMsg();
    graphMsg->setGraph(graphe.returnGraphT().c_str());
    return graphMsg;
}

void  NeighboringLayer::handleGraphUpdtMsgFromLowerLayer(cMessage *msg){    //only gets this msgs when it has no neighs
    GraphUpdtMsg *neighGraphMsg = dynamic_cast<GraphUpdtMsg*>(msg);
    bool noNeigh = neighGraphMsg->getNoNeighs();
    if(noNeigh){
        graphe.cleanGraph();
        cleanEnerTable();
    }
    send(neighGraphMsg,"upperLayerOut");
    sendEnerTable();
}

//---

//--Calculations Methods-----------------------------------------------------------
/*************************
 * ⇒ Calculates the distance between the two nodes from the position data received in the beacon
 */
double NeighboringLayer::calculateDistance(cMessage *msg){
    BeaconMsg *beaconR = dynamic_cast<BeaconMsg*>(msg);
    int senderX=beaconR->getMyPosX();
    int senderY=beaconR->getMyPosY();
    inet::Coord myPos = ownNodeInfo->nodeMobilityModule->getCurrentPosition();
    double d = sqrt(pow(fabs(myPos.x-senderX),2)+pow(fabs(myPos.y-senderY),2));
    EV<<"Distance btw me and host "<<beaconR->getSourceAddress()<<" is: "<<d<<"m \n";
    return d;

}

/*************************
 * ⇒ Calculates the estimated SSI between the two nodes from the position data received in the beacon
 */
double NeighboringLayer::calculateSSI(cMessage *msg){
    double x=calculateDistance(msg);

    double ssi_ext=-0.0000207519*pow(x,4)+0.0005124292*pow(x,3)+0.0589678*pow(x,2)-2.72277*x-57.5612;
    EV<<"SSI_ext: "<<ssi_ext<<" dBm \n";
    //double ssi_ext2=-0.0000207519*pow(40,4)+0.0005124292*pow(40,3)+0.0589678*pow(40,2)-2.72277*40-57.5612;
    //EV<<"SSI de teste: "<<ssi_ext2<<"\n"; //deve dar -92dBm
    return ssi_ext;

}

double NeighboringLayer::calculateLinkStability(cMessage *msg){
    double x=calculateDistance(msg);
    double link_stability=0;

    if(x<5){
        link_stability=100;
    }else if(x>=40){
        link_stability=0;
    }else{
        link_stability=-0.002*pow(x,3)+0.0104762*pow(x,2)+0.454762*x+97.2143;
    }
        return link_stability;

}

//CALCULATE LQE
double NeighboringLayer::calcMyLQE(cMessage *msg){
    BeaconMsg *BeaconReceived = dynamic_cast<BeaconMsg*>(msg);
    double SSI_ext = calculateSSI(msg);
    double rssi_norm = 1.0 + ((float)((SSI_ext - (-56)) * (0 - 1)) / ((-100) - (-56)));

    double link_stability =calculateLinkStability(msg)/100;

    double rDelay=BeaconReceived->getReceivedTime().dbl()-BeaconReceived->getSentTime().dbl(); //not being used as it makes no difference for being so low

    // 24/10/2019
    /*double BitRate=0, betaB=0;
    double Age_factor=0;
    if(rDelay>0){
        BitRate=BeaconReceived->getRealPacketSize()/rDelay;
        Age_factor=1-(rDelay/max_age);
        betaB=Age_factor/1;
    }else Age_factor=1;*/
    //
    double Link_Quality=(1-omega)*rssi_norm+omega*link_stability;

    EV<<"My Link Quality="<<Link_Quality<<" My rDelay:"<<rDelay<<" My ssi_norm"<<rssi_norm<<" my ssi:"<<SSI_ext<<" my link_stability"<<link_stability<<"at a distance:"<<calculateDistance(msg)<<"\n";

    return Link_Quality;
}

/*************************
 * ⇒ Update the Quality Value between the two nodes.
 */
double NeighboringLayer::updateProbability(double distProb, double ssi){
    //EV<<"Gateway Addr: "<<GWAddr<<" \n";

    double newProb;
    //set ssi range
    double gama;
    if(ssi>-40){
        gama=1;
    } else if(ssi<=-40 && ssi>-70){
        gama=0.75;
    }else if(ssi<=-70 && ssi >-80){
        gama =0.55;
    }else if(ssi<=-90 && ssi >-90){
        gama=0.4;
    } else{
        gama = 0.1;
    }

    newProb=distProb*gama;
    //EV<<"Updated new Probability \n";
    return newProb;
}


//--Gw List methods------------------------------------
/*************************
 * ⇒ Verifies if GW is my direct neighbor
 */
double NeighboringLayer::GWisMyNeigh(cMessage *msg){
    NeighbourListMsg *neighListMsg = dynamic_cast<NeighbourListMsg*>(msg);
    double isNeigh;
    bool check;
    int i=0;

    while (i < neighListMsg->getNeighbourNameListArraySize()){
             string nodeMACAddress = neighListMsg->getNeighbourNameList(i);
             if(nodeMACAddress==GWAddr){
                 EV<<"GW is my neighbor \n";
                 check=true;
                 break;
             }
             i++;
    }
    if(check){
        isNeigh=1;
    }else if(isNeighNeigh>=0.5){
        isNeigh=0.75;
    }
    else{isNeigh=0.25; }

    return isNeigh;
}

double NeighboringLayer::GWisMyNeighBT(cMessage *msg){
    NeighbourListMsgBT *neighListMsg = dynamic_cast<NeighbourListMsgBT*>(msg);
    double isNeigh;
    bool check;
    int i=0;

    while (i < neighListMsg->getNeighbourNameListArraySize()){
             string nodeMACAddress = neighListMsg->getNeighbourNameList(i);
             if(nodeMACAddress==GWAddr){
                 EV<<"GW is my neighbor \n";
                 check=true;
                 break;
             }
             i++;
    }
    if(check){
        isNeigh=1;
    }else if(isNeighNeigh>=0.5){
        isNeigh=0.75;
    }
    else{isNeigh=0.25; }

    return isNeigh;
}

void NeighboringLayer::cleanOldContacts(){  //I think i can delete this
    list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour;
        iteratorSyncedNeighbour = syncedNeighbourList.begin();
        while (iteratorSyncedNeighbour != syncedNeighbourList.end()) {
            SyncedNeighbour *syncedNeighbour = *iteratorSyncedNeighbour;
            if ((simTime().dbl()-syncedNeighbour->lastBrecT) >=max_absent_time) {

                //string graf=graphe.returnGraphT();
                //EV<<"Graph bf old clean:"<<graf<<"\n";
                //int myID=graphe.add_element(ownMACAddress);
                //string addrN=syncedNeighbour->nodeMACAddress;
                //int idN=std::stoi( addrN.substr(15,17));
                //graphe.rem_edge(myID,idN); Removing here is deleting neighs that finished sync but might be there!
                //EV<<"Removed absent neigh from graph: "<<idN<<"\n";
            }

            iteratorSyncedNeighbour++;
        }

}


//--Energy Methods--------------------------------------------------------------------

void NeighboringLayer::handlePcktSentMsg(cMessage *msg){
    PcktIsSentMsg *pcktSent = dynamic_cast<PcktIsSentMsg*>(msg);
    double pckt_size=pcktSent->getBit_size();
    bool from_Gw=pcktSent->getTo_Gw();
    double dist=pcktSent->getDistance();
    bool is_sent=pcktSent->getIs_sent();
    calcEnerg(pckt_size, from_Gw, dist, is_sent);

    delete msg;
}

//calculate future energy value through expenditure on sending Msgs.
void NeighboringLayer::calcEnerg(double size_bits, bool from_gw, double distance, bool is_sent){
    double ET, ER, EneG, Ech;
    double d=distance;
    double d0=sqrt(Efs/Emp);

    double senderX=BS_X;
    double senderY=BS_Y;
    inet::Coord myPos = ownNodeInfo->nodeMobilityModule->getCurrentPosition();
    double d_gw = sqrt(pow(fabs(myPos.x+senderX),2)+pow(fabs(myPos.y-senderY),2)); //here sum because BS is previous to initial pos of USVs

    if(from_gw){
        d=d_gw;
        if(is_sent){
            //
            //Ech=size_bits*(Eelec*pow(10,-9)+Efs*pow(10,-12)*distance+Eda*pow(10,-9)) - energy ch per round
            //
            EV<<"GW sent->"<<size_bits<<"\n";
            ET=Eelec*pow(10,-9)*size_bits+Eamp*pow(10,-12)*size_bits*pow(d,lambBS);
            EneG=ET;
        }else{
            EV<<"GW rec \n";
            ER=Eelec*pow(10,-9)*size_bits;
            EneG=ER;
        }
    }else{
        EV<<"Other sent \n";
        if(is_sent){
            ET=Eelec*pow(10,-9)*size_bits+Eamp*pow(10,-12)*size_bits*pow(d,lambIC);
            EneG=ET;
        }else{
            EV<<"other rec \n";
            ER=Eelec*pow(10,-9)*size_bits;
            EneG=ER;
        }
    }

    myEner=myEner-EneG;
    EV<<"MY ene: "<<myEner<<"Spent:"<<EneG<<"\n";
    MyBat=((float)((myEner/EnergyStart)*100)); //with normalization i get my batery from 0(min) to 100% (max)
    EV<<"My batery:"<<MyBat<<"\n";
    //double Enorm = 1.0 + ((float)((Energ - (-56)) * (0 - 1)) / ((-100) - (-56)));
    int idMy=std::stoi( ownMACAddress.substr(15,17));
    //old method
    /*int my_enerS=round(ener_spent+size_bits*Beta);
    ener_spent=my_enerS;//update my value
    Ener[idMy]=my_enerS;//update table my value;
    */

    int MyBatRound=floor(MyBat);//round(MyBat);
    Ener[idMy]=MyBatRound;//update table my value;

}

bool NeighboringLayer::updateNeighEner(cMessage *msg){
    EV<<"Update Neigh Ener \n";
    BeaconMsg *BeaconReceived = dynamic_cast<BeaconMsg*>(msg);
    string srcAdd=BeaconReceived->getSourceAddress();
    string tabS=BeaconReceived->getNeighEner();
    string myAdd=ownMACAddress;
    //EV<<"Tabela Ener Recebida: "<<tabS<<" de comprimento:"<<tabS.length()<<"\n";
    int srcID=graphe.add_element(srcAdd);  //returns id
    int myID=graphe.add_element(myAdd);

    int array[N_nodes];
    for(int p1=0;p1<N_nodes;p1++){ //clean coppy arr
        array[p1]=-1;
    }

    std::string delimiter = ";";
    int i=0;//, q1=0;
    for(i=0;i<tabS.length();i++){
        int j=tabS.find(delimiter,i);
        if(j==std::string::npos){
            EV<<"false \n";
            return false;
        }else{
            EV<<"Here I am \n";
            std::string token = tabS.substr(i, j-i);
            int q1 = tabS.find("-",i);
            int q2 = tabS.find(";",i);
            string v=tabS.substr(i,q1-i);
            string w=tabS.substr(q1+2,q2-(q1+2));
            int vert = std::stoi (v);
            int weight = std::stoi (w);
            //EV<<"WEI: "<<w<<"\n";
            array[vert]=weight;
            i =j+1;
                //EV<<"i="<<i<<"j="<<j<<"q1:"<<q1<<"q2:"<<q2<<"v:"<<v<<"w:"<<w<<"\n";
        }
    }
    EV<<"I'm here lel \n";
        EV<<"Array recebido: \n";
        int f=0;
        //int count=0;
        for(f = 0; f < N_nodes; f++) {
                if(array[f]>=0){EV << array[f] << " ";}
            EV<<"\n";
        }
        //Clean direct-link line - I doon't thin kwe need this here
        /*for(int h=0;h<maxLengthGraph;h++){
            Ener[srcID]=0;
            //graphe.rem_edge(srcID, h);
        }*/

    for(int vi=0;vi<maxLengthGraph;vi++){
        int ret=graphe.returnWGrapfT(myID,vi);
        EV<<"Ret is:"<<ret<<"\n";
        if(vi!=myID && ret<=0 && array[vi]>0){   //if it is not my direct neigh, save
            Ener[vi]=array[vi];EV<<"Save1\n";
        }
        if(vi==srcID){  //if it's senders position, save
            Ener[vi]=array[vi];EV<<"Save2\n";
        }
    }


     /*
    //Adding the received graph to mine
    for(int s=0;s<maxLengthGraph;s++){//graphe.returnMaxNoVert();s++){

        for(int o=0;o<maxLengthGraph;o++){//graphe.returnMaxNoVert();o++){
            //int ret=graphe.returnWGrapfT(s,o);
            if((s!=myID && o!=myID)&& array[o]>=0){ //if not my direct neigh and ener>0
                Ener[o]=array[o];
                EV<<"Added: "<<array[o]<<"at"<<o<<"\n";
            }
            if(((s==myID && o==srcID)||(s==srcID && o==myID))&& array[o]>=0){ //if my pos and yours
                Ener[o]=array[o];
                EV<<"Added2: "<<array[o]<<"at"<<o<<"\n";
            }
        }
    }*/

        //Update my Weight:
        Ener[myID]=MyBat;//ener_spent;
        return true;
}

string NeighboringLayer::returnEnerTable(){
    //Returns the table on a string
    std::string tableS;
    int k,l;
    for(k = 0; k < N_nodes; k++) {
        if(Ener[k]!=0 && Ener[k]!=(-1)){
            tableS=tableS+std::to_string(k)+"->"+std::to_string(Ener[k])+";\n";
        }
    }
    EV<<"Table Ener: \n"<<tableS<<"\n";
    return tableS;
}

void NeighboringLayer::removEdge(int id){
    Ener[id]=-1;
    //EV<<"Removed:"<<id<<"\n";
}

void NeighboringLayer::cleanEnerTable(){    //cleans table exept my value
      int myID=graphe.add_element(ownMACAddress.c_str());
      int my_Ener=Ener[myID];
      //int count=0;
      for(int i = 0; i < N_nodes; i++) {
          removEdge(i);
      }
      Ener[myID]=my_Ener;
}

void NeighboringLayer::sendEnerTable(){
    string tableS=returnEnerTable();
    EnerTableMsg *EnerMsg = new EnerTableMsg();
    EnerMsg->setTable(tableS.c_str());
    EnerMsg->setSentTime(simTime().dbl());

    send(EnerMsg,"upperLayerOut");
}
//---------------------------------------------------------------------------


//FINISH
void NeighboringLayer::finish(){

    recordScalar("numEventsHandled", numEventsHandled);

    // clear synced neighbour info list
    list<SyncedNeighbour*> syncedNeighbourList;
    while (syncedNeighbourList.size() > 0) {
        list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour = syncedNeighbourList.begin();
        SyncedNeighbour *syncedNeighbour = *iteratorSyncedNeighbour;
        syncedNeighbourList.remove(syncedNeighbour);
        delete syncedNeighbour;
    }
    // clear synced neighbour info list BT
        list<SyncedNeighbour*> syncedNeighbourListBT;
        while (syncedNeighbourListBT.size() > 0) {
            list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour = syncedNeighbourListBT.begin();
            SyncedNeighbour *syncedNeighbour = *iteratorSyncedNeighbour;
            syncedNeighbourListBT.remove(syncedNeighbour);
            delete syncedNeighbour;
        }
}



//From Daniela, not being used right now

/*double NeighboringLayer::calcWeight(cMessage *msg){
    double Weight=(1-calcLinkQuality(msg))*100;
    return Weight;
}*/

/*double NeighboringLayer::calcNeighWeight(cMessage *msg){
    double weight = (1-calcLinkQuality(msg))*100;
    return weight;
}*/

//Calculate the quality of the link
/*double NeighboringLayer::calcLinkQuality(cMessage *msg){
    BeaconMsg *BecMsg = dynamic_cast<BeaconMsg*>(msg);
    double bps_norm=0;
    double B = calcFactorB(msg);
    double SSI_ext = calculateSSI(msg);
    double rssi_norm = 1.0 + ((float)((SSI_ext - (-56)) * (0 - 1)) / ((-100) - (-56)));
    double BitRate=calcBitRate(msg);
    if(BitRate<=180000){
            bps_norm = 1 + (((BitRate - (180000)) * (0 - 1)) / (0 - 180000));
        }else{
            bps_norm=1;
        }
    double Age_factor=calcAgeFact(msg);
    double Link_Quality=(1-B)*rssi_norm+B*(bps_norm*Age_factor)/Age_factor;

    EV<<"Link Quality="<<Link_Quality<<"\n";

    return Link_Quality;
}*/

//Calculate the BitRate
/*double NeighboringLayer::calcBitRate(cMessage *msg){
    BeaconMsg *BecMsg = dynamic_cast<BeaconMsg*>(msg);
    double pk_delay= BecMsg->getReceivedTime().dbl() - BecMsg->getSendingTime().dbl();
    double pk_size=(BecMsg->getByteLength() * 8);
    double bit_rate=pk_size/pk_delay;

    if(bit_rate<=180000){
        double bps_norm = 1 + (((bit_rate - (180000)) * (0 - 1)) / (0 - 180000));
    }else{
        double bps_norm=1;
    }

    EV<<"BitRate="<<bit_rate<<"\n";

    return bit_rate;
}*/

//Calculate factor B
/*double NeighboringLayer::calcFactorB(cMessage *msg){
    BeaconMsg *BecMsg = dynamic_cast<BeaconMsg*>(msg);
    double B=calcAgeFact(msg)/1;

    EV<<"B factor="<<B<<"\n";
    return B;
}*/

//Returns Age Factor
/*double NeighboringLayer::calcAgeFact(cMessage *msg){
    BeaconMsg *BecMsg = dynamic_cast<BeaconMsg*>(msg);
    double time_rec=BecMsg->getReceivedTime().dbl();
    double time_now=simTime().dbl();
    EV<<"time now:"<<time_now<<"time rec:"<<time_rec<<"\n";
    double dif=time_now-time_rec;
    double AF=1-(std::min(dif,max_age))/max_age;
    EV<<"Dif="<<dif<<"Age factor="<<AF<<"\n";
    return AF;
}*/

//Other methods:
/*double NeighboringLayer::findInNeigLayerList(string addrN){
    list<SyncedNeighbour*>::iterator iteratorSyncedNeighbour;
    iteratorSyncedNeighbour = syncedNeighbourList.begin();
    while (iteratorSyncedNeighbour != syncedNeighbourList.end()) {
        SyncedNeighbour *syncedNeighbour = *iteratorSyncedNeighbour;
        if (syncedNeighbour->nodeMACAddress.c_str()==addrN) {
            double timstp= syncedNeighbour->lastBrecT.dbl();
            return timstp;
        }
        iteratorSyncedNeighbour++;

    }
    return 0;
}*/


