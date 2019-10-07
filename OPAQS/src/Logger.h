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
    void saveResultsWeight(string myAddr,string sourceAddr, string weightH);
    void saveResultsWTime(string myAddr,string sourceAddr, string timeRMsg);
    void saveEnerTable(string myAddr, string table);



};


#endif /* LOGGER_H_ */


