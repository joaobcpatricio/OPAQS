//
// The model implementation for a Forwarding module (eg. Epidemic, spray, etc) Routing layer
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 19-march-2019
//

#ifndef ROUTINGLQE_H_
#define ROUTINGLQE_H_

#define TRUE                            1
#define FALSE                           0

#include <omnetpp.h>
#include <cstdlib>
#include <sstream>
#include <string>

#include "OutsMsg_m.h"
#include "InternalMsg_m.h"

#include "StorageM.h"
#include <fstream>
#include "GraphT.h";

//file libs
#include <iostream>
#include <chrono>
#include <ctime>


using namespace omnetpp;

using namespace std;

class RoutingLQE : public cSimpleModule
{
protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const;
    virtual void finish();

private:
    string ownMACAddress;
    string ownBTMACAddress;
    int nextAppID;
    int maximumCacheSize;
    int maximumHopCount;
    bool useTTL;
    int numEventsHandled;
    int inCache;
    string MyAddH;
    string MyAddAck;
    int nodeIndex;

    bool isSending=false;
    bool isReceiving=false;
    double waitS=0;
    double waitBFsend;
    double max_age;

    string actual_gateway;

    struct AppInfo {
        int appID;
        string appName;
        string prefixName;
    };

    list<AppInfo*> registeredAppList;


    //19/09
    string gateway_list;
    struct GatewayN{
        string nodeMACAddress;
        double untilTime;
    };
    list<GatewayN*> GatewayList;


    void handleAppRegistrationMsg(cMessage *msg);
    void handleDataMsgFromUpperLayer(cMessage *msg);
    void handleNeighbourListMsgFromLowerLayer(cMessage *msg);
    void handleDataMsgFromLowerLayer(cMessage *msg);
    void handleSummaryVectorMsgFromLowerLayer(cMessage *msg);
    void handleDataRequestMsgFromLowerLayer(cMessage *msg);
    void handleAckFromLowerLayer(cMessage *msg);
    void handleBeacon(cMessage *msg);
    void handleDataReqMsg(cMessage *msg);
    void handleBeaconInfo(cMessage *msg);
    //Cache
    StorageM Stor;
    bool msgIDexists(string messageID);
    void returnSelectMsgIDList(vector<string> & selectedMessageIDList);
    int cacheListSize();
    //Graph
    void handleGraphUpdtMsg(cMessage *msg);
    GraphT graphR;
    bool getGraph(string graphS);//, int numberVert);
    //GW
    bool setGatewayList();
    void printGatewayList();
    void updateGateway();
    void saveGraphHere();




    // stats related variables
    simsignal_t dataBytesReceivedSignal;
    simsignal_t sumVecBytesReceivedSignal;
    simsignal_t dataReqBytesReceivedSignal;
    simsignal_t totalBytesReceivedSignal;



};

#define ROUTINGLQE_SIMMODULEINFO         " RoutingLQE>!<" << simTime() << ">!<" << getParentModule()->getFullName()

#define ROUTINGLQE_MSG_ID_HASH_SIZE      4 // in bytes

#endif /* ROUTINGLQE_H_ */
