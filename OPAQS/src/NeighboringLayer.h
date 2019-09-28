//
// The model implementation for the Neighbouring management layer
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 3-jul-2019
//

#ifndef NEIGHBORINGLAYER_H_
#define NEIGHBORINGLAYER_H_

#define TRUE                            1
#define FALSE                           0

#include <omnetpp.h>
#include <cstdlib>
#include <sstream>
#include <string>

#include "OutsMsg_m.h"
#include "InternalMsg_m.h"

#include "StorageM.h"
#include "inet/mobility/contract/IMobility.h"
#include <queue>

#include "GraphT.h"


using namespace omnetpp;

using namespace std;

class BaseNodeInfo;

class NeighboringLayer : public cSimpleModule
{
protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const;
    virtual void finish();



private:
    string ownMACAddress;
    string ownBTMACAddress;
    double antiEntropyInterval;
    double maximumRandomBackoffDuration;
    int usedRNG;
    int numEventsHandled;
    double myProb;
    string GWAddr;
    //BaseNodeInfo *ownNodeInfo;
    list<BaseNodeInfo*> allNodeInfoList;
    BaseNodeInfo *ownNodeInfo;
    double distProb;
    double isNeighNeigh;
    double delayPerDataMsg;
    double max_absent_time;

    struct SyncedNeighbour {
        string nodeMACAddress;
        double syncCoolOffEndTime;
        bool randomBackoffStarted;
        double randomBackoffEndTime;
        bool neighbourSyncing;
        double neighbourSyncEndTime;
        bool nodeConsidered;
        simtime_t lastBrecT=0;
    };




    list<SyncedNeighbour*> syncedNeighbourList;
    bool syncedNeighbourListIHasChanged;

    void handleRoutingMsg(cMessage *msg);
    void handleNeighbourListMsgFromLowerLayer(cMessage *msg);
    BeaconMsg* makeBeaconVectorMessage(cMessage *msg);
    SyncedNeighbour* getSyncingNeighbourInfo(string nodeMACAddress);
    void setSyncingNeighbourInfoForNextRound();
    void updateNeighbourList(cMessage *msg);
    void setSyncingNeighbourInfoForNoNeighbours();
    void cancelBackOffT(cMessage *msg);
    void handleBeaconMsgFromLowerLayer(cMessage *msg);
    double calculateDistance(cMessage *msg);
    double calculateSSI(cMessage *msg);
    double updateProbability(double distProb, double ssi);
    double GWisMyNeigh(cMessage *msg);
    void updateNeighbourListBT(cMessage *msg);
    list<SyncedNeighbour*> syncedNeighbourListBT;
    bool syncedNeighbourListBTHasChanged;
    void setSyncingNeighbourInfoForNoNeighboursBT();
    bool sendWifiFirst;
    void setSyncingNeighbourInfoForNextRoundBT();
    SyncedNeighbour* getSyncingNeighbourInfoBT(string nodeMACAddress);
    void cancelBackOffTBT(cMessage *msg);
    bool msgIsBT;
    double GWisMyNeighBT(cMessage *msg);



    //29/08
     GraphT graphe;
     int maximumNoVert;


     bool updateGraph(cMessage *msg);
     bool updateMyNGraph(cMessage *msg);

     int maxLengthGraph;
     double max_age;
     double calcAgeFact(cMessage *msg);
     double calcFactorB(cMessage *msg);
     double calcBitRate(cMessage *msg);
     double calcLinkQuality(cMessage *msg);
     double calcWeight(cMessage *msg);
     double calcNeighWeight(cMessage *msg);
     double calcMyLQE(cMessage *msg);
     double calculateLinkStability(cMessage *msg);
     void saveLastBeContact(string Naddress);    //updates the last time this direct-neigh sent a beacon
     void cleanOldContacts(); //removes direct neighbors that are absent from graph
     //double findInNeigLayerList(string addrN);  //returns the time of the last contact on the previous saved list of neighbors
     void handleDataReqMsgFromLowerLayer(cMessage *msg);
     void saveResultsWeight(cMessage *msg, string weightH);
     void saveResultsWTime(cMessage *msg, string timeRMsg);
     GraphUpdtMsg* makeGraphUpdtMessage();
     void handleGraphUpdtMsgFromLowerLayer(cMessage *msg);


};

#define NEIGHBORINGLAYER_SIMMODULEINFO         " NeighboringLayer>!<" << simTime() << ">!<" << getParentModule()->getFullName()

#define NEIGHBORINGLAYER_MSG_ID_HASH_SIZE      4 // in bytes

#endif /* NEIGHBORINGLAYER_H_ */
