//
// The model implementation for a Forwarding module (eg. Epidemic, spray, etc) Routing layer
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 19-march-2019
//

#ifndef ROUTINGLAYER_H_
#define ROUTINGLAYER_H_

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


using namespace omnetpp;

using namespace std;

class RoutingLayer : public cSimpleModule
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

    struct AppInfo {
        int appID;
        string appName;
        string prefixName;
    };

    list<AppInfo*> registeredAppList;

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

    // stats related variables
    simsignal_t dataBytesReceivedSignal;
    simsignal_t sumVecBytesReceivedSignal;
    simsignal_t dataReqBytesReceivedSignal;
    simsignal_t totalBytesReceivedSignal;
    simsignal_t cacheBytesRemovedSignal;
    simsignal_t cacheBytesAddedSignal;
    simsignal_t cacheBytesUpdatedSignal;
};

#define ROUTINGLAYER_SIMMODULEINFO         " RoutingLayer>!<" << simTime() << ">!<" << getParentModule()->getFullName()

#define ROUTINGLAYER_MSG_ID_HASH_SIZE      4 // in bytes

#endif /* ROUTINGLAYER_H_ */