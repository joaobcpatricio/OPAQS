/*
 *
 *
 *  Created on: 04/10/2019
 *  Author: João Patrício
 */

#include "Logger.h"

/***********************************************************************************************************
 * Saves Data for later analysis
 */

Logger::Logger(){
}

Logger::~Logger(){
}

void Logger::initialize(string ownMACAddress){
    ownAddress=ownMACAddress;
    string nameGen="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/Logs/LogGen";
        string noGen=ownMACAddress.substr(15,17);
        nameGen.append(noGen);
        nameGen.append(".txt");
        //EV<<"nameF: "<<nameF<<"\n";
        ofstream outfileGen(nameGen,ios::out);
        outfileGen<<"Generated Data \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
        outfileGen.close();
        std::ofstream outGen(nameGen, std::ios_base::app);
        auto startGen = std::chrono::system_clock::now();
        // Some computation here
        auto endGen = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_secondsGen = endGen-startGen;
        std::time_t end_timeGen = std::chrono::system_clock::to_time_t(endGen);
        outGen<< "Started simulation at " << std::ctime(&end_timeGen) << "elapsed time: " << elapsed_secondsGen.count() << "s\n";
        outGen.close();

}


