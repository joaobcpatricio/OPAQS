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
#include "Logger.h"



using namespace omnetpp;

using namespace std;

class BaseNodeInfo;

#define N_nodes 15//20//50        //HARDECODED VARIABLE of number of vertices - must be the same set on RoutingLqeGw
#define Beta 0.01  //constant to control relation from bits to energ

#define Eelec 50 //nJ/bit       -> Energy exhausted in electronic circuitry
#define Efs 10//pJ/bit/m2       -> Energy dissipated by amplifier for free space
#define Emp 0.0013//pJ/bit/m4   -> Energy dissipated by amplifier for multipath
#define Eda 5//nJ/bit/report    -> Energy exhausted in data fusion

#define Eamp 100 //pJ/bit/m2    -> energy dissipation of the transmission amplifier
#define lambIC 2 //path loss intra-cluster
#define lambBS 2.5 //path loss base station transmission


#define di 40
#define EnergyStart 100//770//300//100//138600 //J (em 10000mAh das powerbank usadas) ->Energy level during deployment

#define omega 0.5       //The weight of SSI on LQE
#define delta 0.5    //The weight of link stability on LQE

#define BS_X  500 //500 //base station coordinate x
#define BS_Y  25  //25 //base station coordinate y



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

    Logger log;

    //29/08
     GraphT graphe;
     int maximumNoVert;


     bool updateGraph(cMessage *msg);
     bool updateMyNGraph(cMessage *msg);

     int maxLengthGraph;
     double max_age;

     double calcMyLQE(cMessage *msg);
     double calculateLinkStability(cMessage *msg);
     void saveLastBeContact(string Naddress);    //updates the last time this direct-neigh sent a beacon
     void cleanOldContacts(); //removes direct neighbors that are absent from graph
     void handleDataReqMsgFromLowerLayer(cMessage *msg);
     GraphUpdtMsg* makeGraphUpdtMessage();
     void handleGraphUpdtMsgFromLowerLayer(cMessage *msg);

     //Ener
     int Ener[N_nodes];
     int ener_spent=0;
     double myEner=1; //
     double MyBat=99999;
     void calcEnerg(double size_bits,bool from_gw, double distance, bool is_sent);
     void handlePcktSentMsg(cMessage *msg);
     bool updateNeighEner(cMessage *msg);
     string returnEnerTable();
     void removEdge(int id);
     void cleanEnerTable();
     void sendEnerTable();

     double resetGPeriod;



     //double calcAgeFact(cMessage *msg);
     //double calcFactorB(cMessage *msg);
     //double calcBitRate(cMessage *msg);
     //double calcLinkQuality(cMessage *msg);
     //double calcWeight(cMessage *msg);
     //double calcNeighWeight(cMessage *msg);
     //double findInNeigLayerList(string addrN);  //returns the time of the last contact on the previous saved list of neighbors



};

#define NEIGHBORINGLAYER_SIMMODULEINFO         " NeighboringLayer>!<" << simTime() << ">!<" << getParentModule()->getFullName()

#define NEIGHBORINGLAYER_MSG_ID_HASH_SIZE      4 // in bytes
#define RESETGRAPH_EVENT_CODE    128

#endif /* NEIGHBORINGLAYER_H_ */
