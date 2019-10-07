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

void Logger::saveResultsWeight(string myAddr,string sourceAddr, string weightH){    //called on neighboring
    //FILE Results
    string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/LQEweight";
    string noS=myAddr.substr(15,17);
    string noN=sourceAddr;
    nameF.append(noS);
    nameF.append("_");
    nameF.append(noN.substr(15,17));
    nameF.append(".txt");
    std::ofstream outfile(nameF, std::ios_base::app);
    weightH.append("\n");
    outfile<<weightH;
    outfile.close();
}
void Logger::saveResultsWTime(string myAddr,string sourceAddr, string timeRMsg){    //called on neighboring
    //FILE Results
    string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/LQEwtime";
    string noS=myAddr.substr(15,17);
    string noN=sourceAddr;
    nameF.append(noS);
    nameF.append("_");
    nameF.append(noN.substr(15,17));
    nameF.append(".txt");
    std::ofstream outfile(nameF, std::ios_base::app);
    timeRMsg.append("\n");
    outfile<<timeRMsg;
    outfile.close();
}

void Logger::saveEnerTable(string myAddr, string table){
    //FILE Results
    string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/Logs/Ener/EnerTable";
    string noS=myAddr.substr(15,17);
    nameF.append(noS);
    nameF.append(".txt");
    std::ofstream outfile(nameF, std::ios_base::app);
    //table save
    outfile<<table;


    std::string timeMsg = std::to_string(simTime().dbl());//getInjectedTime().dbl());
    string timeGen="\n Time: ";
    timeGen.append(timeMsg);
    timeGen.append("\n");
    outfile<<timeGen;
    outfile.close();
}

