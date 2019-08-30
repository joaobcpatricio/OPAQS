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

    } else if (stage == 2) {


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
    if (msg->isSelfMessage()) {
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



        // received some unexpected packet
        } else {

            EV_INFO << NEIGHBORINGLAYER_SIMMODULEINFO << "Received unexpected packet" << "\n";
            delete msg;
        }
    }
}


/*******************************************************************************************
 *Handles Msg from lower layer (Wireless)
 */
void NeighboringLayer::handleNeighbourListMsgFromLowerLayer(cMessage *msg)//neigh (uses cache)
{   // if(0.5<= simTime().dbl()){
    updateNeighbourList(msg);//}
}

/********************************************************************************************************
 *Verifies neighborhood and updates the neighbors list and checks if GW is my neighbor
 */
void NeighboringLayer::updateNeighbourList(cMessage *msg){ //por fazer

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
                 EV<<"Send Beacon through Wifi\n";
                 msgIsBT=false;
                 // send beacon (to start syncing)
                 BeaconMsg *beaconMsg = makeBeaconVectorMessage(msg);
                 beaconMsg->setDestinationAddress(nodeMACAddress.c_str());
                 //Added 27/06 1h26
                 //get my prob based on distance
                 distProb=GWisMyNeigh(msg);
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
                 EV<<"N: Send Beacon through BT\n";
                 msgIsBT=true;
                 // send beacon (to start syncing)
                 BeaconMsg *beaconMsg = makeBeaconVectorMessage(msg);
                 beaconMsg->setDestinationAddress(nodeMACAddress.c_str());
                 //Added 27/06 1h26
                 //get my prob based on distance
                 distProb=GWisMyNeighBT(msg);
                 //EV<<"Sent lowerLayerOutBT";
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
    EV<<"My x= "<<ownCoord.x<<" My y= "<<ownCoord.y<<" \n";

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
    EV<<" My current Prob is: "<<myProb<<" \n";
    int realPacketSize = 6 + 6 + 4 + 4 + 4;//(1 * NEIGHBORINGLAYER_MSG_ID_HASH_SIZE); //REVER TAMANHO AQUI CORRETO
    beaconMsg->setRealPacketSize(realPacketSize);
    beaconMsg->setByteLength(realPacketSize);
    beaconMsg->setMyPosX(ownCoord.x);
    beaconMsg->setMyPosY(ownCoord.y);
    return beaconMsg;
    //Aqui cria o beacon de broadcast que faz reconhecer a vizinhança
}


/**********************************************************************************************************
 ⇒Receives a node's MACAdd, checks if it's on the list, if not it adds it at the list end; Returns the memory add where the syncedNeighbour(node) is saved;
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
//ADDED 23/07/19 15h15
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

void NeighboringLayer::handleBeaconMsgFromLowerLayer(cMessage *msg)//neigh
{


//-----------------------
    //Test of Dijkstra
    int graph[10][10] =

            { { 0, 4, 0, 0, 0, 0, 0, 8, 0 }, { 4, 0, 8, 0, 0, 0, 0, 11, 0 }, {

                    0, 8, 0, 7, 0, 4, 0, 0, 2 },

                    { 0, 0, 7, 0, 9, 14, 0, 0, 0 }, { 0, 0, 0, 9, 0, 10, 0, 0,

                            0 }, { 0, 0, 4, 0, 10, 0, 2, 0, 0 }, { 0, 0, 0, 14,

                            0, 2, 0, 1, 6 }, { 8, 11, 0, 0, 0, 0, 1, 0, 7 }, {

                            0, 0, 2, 0, 0, 0, 6, 7, 0 } };


    graphe.dijkstra(graph, 0);

//-------------------

    EV<<"My first Add: "<<ownMACAddress.at(0)<<"\n";
    EV<<"My first Add: "<<ownMACAddress.substr(0,2)<<"\n";

    BeaconMsg *BeaconReceived = dynamic_cast<BeaconMsg*>(msg);

    double ssi_ext=calculateSSI(msg);

    myProb=updateProbability(distProb,ssi_ext);

    isNeighNeigh=BeaconReceived->getProb();

    //make Beacon copy with extra info for upper layer
    BeaconInfoMsg *infoMsg = new BeaconInfoMsg();
    infoMsg->setSourceAddress(BeaconReceived->getSourceAddress());
    infoMsg->setProb(BeaconReceived->getProb());    //Gives probability
    infoMsg->setMyProb(myProb);
    int realPacketSize = 6 + 6 + 4 + 4 + 4 + 4 + 4;
    infoMsg->setRealPacketSize(realPacketSize);
    infoMsg->setByteLength(realPacketSize);
    infoMsg->setMyPosX(BeaconReceived->getMyPosX());
    infoMsg->setMyPosY(BeaconReceived->getMyPosY());
    //infoMsg->setNic(BeaconReceived->getNic());

    //EV<<"Beac: "<<BeaconReceived->getRealPacketSize()<<" Cop of beac: "<<infoMsg->getRealPacketSize()<<" \n";

    EV<<"Neighboring: Sending msg to upperLayer\n";
    send(infoMsg, "upperLayerOut");

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
    EV<<"BackOffT canceled\n";
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


double NeighboringLayer::calculateDistance(cMessage *msg){
    BeaconMsg *beaconR = dynamic_cast<BeaconMsg*>(msg);
    int senderX=beaconR->getMyPosX();
    int senderY=beaconR->getMyPosY();
    inet::Coord myPos = ownNodeInfo->nodeMobilityModule->getCurrentPosition();
    double d = sqrt(pow(fabs(myPos.x-senderX),2)+pow(fabs(myPos.y-senderY),2));
    EV<<"Distance btw me and host "<<beaconR->getSourceAddress()<<" is: "<<d<<"m \n";
    return d;

}

double NeighboringLayer::calculateSSI(cMessage *msg){
    double x=calculateDistance(msg);

    double ssi_ext=-0.0000207519*pow(x,4)+0.0005124292*pow(x,3)+0.0589678*pow(x,2)-2.72277*x-57.5612;
    EV<<"SSI_ext: "<<ssi_ext<<" dBm \n";
    //double ssi_ext2=-0.0000207519*pow(40,4)+0.0005124292*pow(40,3)+0.0589678*pow(40,2)-2.72277*40-57.5612;
    //EV<<"SSI de teste: "<<ssi_ext2<<"\n"; //deve dar -92dBm
    return ssi_ext;

}

double NeighboringLayer::updateProbability(double distProb, double ssi){
    EV<<"Gateway Addr: "<<GWAddr<<" \n";


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
    EV<<"Updated new Probability \n";
    return newProb;
}

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
