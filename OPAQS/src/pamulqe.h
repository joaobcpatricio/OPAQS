//
// The model implementation for a Forwarding module (eg. Epidemic, spray, etc) Routing layer
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 19-march-2019
//

#ifndef PAMULQE_H_
#define PAMULQE_H_

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
#include "GraphT.h"
#include "Logger.h"

//file libs
#include <iostream>
#include <chrono>
#include <ctime>

#include <utility>
#include <fstream>

#define N_nodes 20//50        //HARDECODED VARIABLE of number of vertices - must be the same of NeighboringLayer
#define alfa 1  //constant to control use of Ener on rank
#define teta 0  //constant to control use of LQE in rank
#define dist_to_gw 500  //Distance to gw


using namespace omnetpp;

using namespace std;

class pamulqe : public cSimpleModule
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
    double time_actual_gw_chosen;
    double gwCheckPeriod;

    bool no_act_gw=false;
    int count_newGw_check=0;
    int temp_gw;
    int im_alone=0;
    int elect_gw;

    double kill_pcktP;

    int count_dataPkts_sent=0;      //count the number of dataPkts sent neighbor
    int count_dataPkts_sentGw=0;    //count the number of dataPkts sent to BS (I'm Gw)
    int count_allPkts_sent=0;       //counts all pkts sent (except broadcast (beacons))

    struct AppInfo {
        int appID;
        string appName;
        string prefixName;
    };

    list<AppInfo*> registeredAppList;

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
    Logger Log;
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

    void checkGwStatus();
    void checkStoredMsgs();



    //Ener table
    void pcktSentMsg(double size_p, bool from_GW);
    void handleEnerTableMsg(cMessage *msg);
    int Ener[N_nodes];
    int ener_spent=0;
    void cleanEnerTable();
    bool updateEnerTable(string tabS);
    string returnEnerTable();

    std::pair<int, int> geodisikV(int nodeID);
    //double geodisikV(int nodeID);
    double old_rank=0;

    //gw election
    int count_newElect_Gw=0;
    int returnElectedGwM1();
    int returnElectedGwM2();
    int returnElectedGwM3();
    double bestGwRank=0;
    double oldGwRank=0;
    int oldGwID=0;



    // stats related variables
    simsignal_t dataBytesReceivedSignal;
    simsignal_t sumVecBytesReceivedSignal;
    simsignal_t dataReqBytesReceivedSignal;
    simsignal_t totalBytesReceivedSignal;



};

#define PAMULQE_SIMMODULEINFO         " pamulqe>!<" << simTime() << ">!<" << getParentModule()->getFullName()

#define PAMULQE_MSG_ID_HASH_SIZE      4 // in bytes
#define CHECKGW_EVENT_CODE    118

#endif /* PAMULQE_H_ */
