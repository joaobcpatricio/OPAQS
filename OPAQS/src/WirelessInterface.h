//
// This program is free software: you can redistribute it and/or modify
//
// Wireless interface used in Keetchi with functionality
// to handle sending of packets considering wireless range
// and broadcast of packets. It should have access to the
// mobility management components.
//
// @date: 08-11-2015
// @author: Asanga Udugama (adu@comnets.uni-bremen.de)
//
//
// @major changes by : João Patrício (castanheirapatricio@ua.pt)
// @date :   3-jul-2019
//

#ifndef WIRELESSINTERFACE_H_
#define WIRELESSINTERFACE_H_

#define TRUE                            1
#define FALSE                           0

#include "inet/mobility/contract/IMobility.h"

#include <omnetpp.h>
#include <cstdlib>
#include <string>
#include <queue>

#include "OutsMsg_m.h"
#include "InternalMsg_m.h"

#include <sstream>

//file libs
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>

#include "Logger.h"

using namespace omnetpp;

using namespace std;

class BaseNodeInfo;

class WirelessInterface : public cSimpleModule
{
protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const;
    virtual void finish();

private:

    Logger Log;

    string ownMACAddress;
    double wirelessRange;
    string expectedNodeTypes;
    double neighbourScanInterval;
    double bandwidthBitRate;
    int wirelessHeaderSize;
    int minSSI;
    int valSSI;
    double limitQueueTime;
    double my_limit_rate;

    double calculateSSI(double x);

    string broadcastMACAddress;
    BaseNodeInfo *ownNodeInfo;
    list<BaseNodeInfo*> allNodeInfoList;
    queue<cMessage*> packetQueue;
    cMessage *sendPacketTimeoutEvent = NULL;

    list<BaseNodeInfo*> currentNeighbourNodeInfoList;
    list<BaseNodeInfo*> atTxNeighbourNodeInfoList;
    cMessage *currentPendingMsg = NULL;

    void setupSendingMsg(cMessage *msg);
    void sendPendingMsg();
    string getDestinationAddress(cMessage *msg);

    // statistics related variable
    simsignal_t neighbourhoodSizeSignal;

    void setReceivedTime(cMessage *msg);
    void setSentTime(cMessage *msg);
    double realAquaticAchievableThroughput(double x);
    void setSentTimeSrc(cMessage *msg);
    void setRecTimeGW(cMessage *msg);
    double getPacketInjectedTime(cMessage *msg);
    void outputResultsReceived();
    void outputResultsSent();
    void resultsSentDataName(cMessage *msg);

    void pcktSentMsg(double size_p, bool from_GW, double dst, bool is_sent);
    void setPckSentMsg(cMessage *msg, double dst, bool is_sent);

};


#define WIRELESSINTERFACE_SIMMODULEINFO       " WirelessInterface>!<" << simTime() << ">!<" << getParentModule()->getFullName()
#define WIRELESSINTERFACE_BIT_RATE_10Mbps     10000000
#define WIRELESSINTERFACE_NEIGH_EVENT_CODE    112
#define WIRELESSINTERFACE_PKTSEND_EVENT_CODE  114


#endif /* WIRELESSINTERFACE_H_ */
