/*
 * Logger.h
 *
 *  Created on: 04/10/2019
 *      Author: mob
 */




#ifndef LOGGER_H_
#define LOGGER_H_

#define TRUE                            1
#define FALSE                           0

#include <stdio.h>
#include <limits.h>

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

using namespace omnetpp;
using namespace std;

class Logger{
public:
    Logger();
    ~Logger();

    string ownAddress = "Wf:00:00:00:00:02";

    void initialize(string ownMACAddress);
    //--Neighboring----
    //Ener
    void saveEnerTable(string myAddr, string table);
    void saveMyEner(string myAddr, double my_ener);
    //LQE
    void saveWeight(string noS, string noN, int weigH);
    void saveResultsWeight(string myAddr,string sourceAddr, int weigH);
    void saveResultsWTime(string myAddr,string sourceAddr);
    //--Routing------------------------------
    void saveResultsLQE(string nMY, string nGW, string shrtPath, string GraphSR);   //LQE
    void saveGenDat(string ownMACAddress, string dataN, string timeMsg); //Generated data
    void saveResultsGw(string ownMACAddress, string srcMAC, string msID, string timeSMsg, string timeRMsg); //data saved GW
    void saveGraphHere(string ownMACAddress, string graf); //save graph
    void RoutingLogsInit(string ownMACAddress); //loggs init
    void saveResultsGwChk(string ownMACAddress, string actual_gateway); //current gw
    void saveMsgReachedGW(string dataName, double time, string myAddr, int nHops); //data reached gw
    void saveGwRank(int GwId, double rankk, int oldID, double oldR, int myID);
    void saveGwRankT(string ownMACAddress, int GwId, double rank, double ener);
    void saveGwRankT_time(string ownMACAddress);
    //--Storage---------------------
    //not working
    //--Wireless NIC ------------------
    void wirelessNicResultsInit(string ownMACAddress);//init
    void saveResultsSent(string ownMACAddress, string srcMAC, string dstMAC, string msNam, string msID, string timeMsg);//save all info sent
    void saveResultsSrc(string ownMACAddress, string srcMAC, string msID, string timeMsg);
    void saveResultsRec(string ownMACAddress, string srcMAC, string dstMAC, string msNam, string msID, string timeMsg);//save all info rec
    void outputResultsSent(string ownMACAddress);
    void outputResultsReceived(string ownMACAddress);
    void saveSentDataName(string ownMACAddress, string msNam);

};


#endif /* LOGGER_H_ */


