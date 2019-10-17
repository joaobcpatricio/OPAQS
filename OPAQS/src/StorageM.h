/*
 * StorageM.h
 *
 *  Created on: 11/03/2019
 *  Author: João Patrício
 */

#ifndef STORAGEM_H_
#define STORAGEM_H_

#define TRUE                            1
#define FALSE                           0
#include <omnetpp.h>
#include <cstdlib>
#include <sstream>
#include <string>
#include<iostream>
#include <list>
#include "OutsMsg_m.h"
#include "InternalMsg_m.h"

//file libs
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>


using namespace omnetpp;
using namespace std;


class StorageM{// : public cSimpleModule{
public:
    StorageM();
    ~StorageM();

//Functions:

    void cleanStor();
    void maximumCacheS(int maximumCacheSizeValue);
    bool saveData(cMessage *msg, int origin, bool reachedGW);
    void ageDataInStorage();
    bool msgIDExists(string messageID);
    int msgIDListPos(string messageID);
    DataMsg* pullOutMsg(cMessage *msg,string ownMACAddress, int count);
    vector<string> returnSelectMsgIDList(vector<string> & selectedMessageIDList, int maximumHopCount);
    int cacheListSize();
    bool deleteMsg(string messageID);
    bool msgExistsInC(cMessage *msg);
    //28/08/19
    void updatePrevHopsList(int position, string HopAddr);

//Variables:
    int maximumCacheSize;
    int currentCacheSize;

    struct CacheEntry {
        char name[20];  //teste - apagar
        string messageID;
        string dataName;
        int realPayloadSize;
        //string dummyPayloadContent;
        double validUntilTime;
        int realPacketSize;
        bool destinationOriented;
        string originatorNodeMAC;
        string finalDestinationNodeName;
        int groupType;
        //int hopsTravelled;
        int msgUniqueID;
        simtime_t injectedTime;
        double createdTime;
        double updatedTime;
        double lastAccessedTime;
        int nMsgOrder;
        int nHops;
        string prevHopsList[200];
        int prevHopListSize=1;
        simtime_t sentTime; //timeStamp
        simtime_t receivedTime; //timeStamp
        simtime_t sentTimeRout; //timeStamp
        simtime_t receivedTimeRout; //timeStamp

        bool reached_gw=false;
        double waitingToDel;
    };

    list<CacheEntry> cacheList;

    double max_age=9999;
    void updateMaxAge(double value);
    void saveResultsStorTimeRec(cMessage *msg, int origin);
    void saveResultsStorTimeSent(cMessage *msg,int origin );
    double kill_pcktP=9999;
    void updateKillPcktP(double kill_pckt);
    void ageFlaggedDataInStorage();
    void saveMsgReachedGw(string dataName, double time, string myAddr, int nHops);

};





#endif /* STORAGEM_H_ */
