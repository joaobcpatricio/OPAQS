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
    //ownAddress=ownMACAddress;
    //string noGenn=ownMACAddress.substr(14,4);
    //EV<<"Log init:"<<ownMACAddress<<" to:"<<noGenn<<"\n";
    //string nameGen="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/Logs/LogGen";
    string nameGen="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/LogGen";
        string noGen=ownMACAddress.substr(15,2);//17);
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


//--Neighboring------------------------------------------------------------
//ENEr
void Logger::saveEnerTable(string myAddr, string table){
    //FILE Results
    //string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/Logs/Ener/EnerTable";
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/Ener/EnerTable";
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
//my Energy
void Logger::saveMyEner(string myAddr, double my_ener){
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/Ener/MyEner";
        string noS=myAddr.substr(15,17);
        nameF.append(noS);
        nameF.append(".txt");
        std::ofstream outs(nameF, std::ios_base::app);
        //table save
        std::string eneR = std::to_string(my_ener);//getInjectedTime().dbl());
               string eneS=eneR;
               eneS.append("\n");
               outs<<eneS;
               outs.close();
}

//LQE
void Logger::saveWeight(string noS, string noN, int weigH){
    //Save weight
        //FILE Results
        string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsWeight";
        nameF.append(noS);
        nameF.append("_");
        nameF.append(noN.substr(15,17));
        nameF.append(".txt");
        std::ofstream outfile(nameF, std::ios_base::app);

        //Weight
        std::string weightH = std::to_string(100-weigH);//getReceivedTime().dbl());
        string weightB="Weight: ";
        weightB.append(weightH);
        outfile<<weightB;
        //time received here
        std::string timeRMsg = std::to_string(simTime().dbl());//getReceivedTime().dbl());
        string timeRec=" | Time now: ";
        timeRec.append(timeRMsg);
        outfile<<timeRec;
        outfile<<" |End \n";
        outfile.close();
}

void Logger::saveResultsWeight(string myAddr,string sourceAddr, int weigH){    //saves weight
    //FILE Results
    //string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/LQEweight";
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/LQEweight";
    string noS=myAddr.substr(15,17);
    string noN=sourceAddr;
    nameF.append(noS);
    nameF.append("_");
    nameF.append(noN.substr(15,17));
    nameF.append(".txt");
    std::ofstream outfile(nameF, std::ios_base::app);
    std::string weightH = std::to_string(100-weigH);//getReceivedTime().dbl());
    weightH.append("\n");
    outfile<<weightH;
    outfile.close();
}

void Logger::saveResultsWTime(string myAddr,string sourceAddr){    //saves time
    //FILE Results
    //string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/LQEwtime";
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/LQEwtime";
    string noS=myAddr.substr(15,17);
    string noN=sourceAddr;
    nameF.append(noS);
    nameF.append("_");
    nameF.append(noN.substr(15,17));
    nameF.append(".txt");
    std::ofstream outfile(nameF, std::ios_base::app);
    std::string timeRMsg = std::to_string(simTime().dbl());//getReceivedTime().dbl());
    timeRMsg.append("\n");
    outfile<<timeRMsg;
    outfile.close();
}

//--Routing-----------------------------------------------------
//Log init
void Logger::RoutingLogsInit(string ownMACAddress){
    //FILE ResultsGen
                string nameGen="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsGen";
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

                //FILE ResultsLQE
                string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsLQE";
                string noS=ownMACAddress.substr(15,17);
                nameF.append(noS);
                nameF.append(".txt");
                //EV<<"nameF: "<<nameF<<"\n";
                ofstream outfile(nameF,ios::out);
                outfile<<"RESULTS FILE \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
                outfile.close();
                std::ofstream out(nameF, std::ios_base::app);
                auto start = std::chrono::system_clock::now();
                // Some computation here
                auto end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end-start;
                std::time_t end_time = std::chrono::system_clock::to_time_t(end);
                out<< "Started simulation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
                out.close();

                //File ResultsStor
                string nameFstor="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsStor";
                string noSstor=ownMACAddress.substr(15,17);
                nameFstor.append(noSstor);
                nameFstor.append(".txt");
                //EV<<"nameF: "<<nameF<<"\n";
                ofstream outfilestor(nameFstor,ios::out);
                outfilestor<<"RESULTS FILE \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
                outfilestor.close();
                std::ofstream outstor(nameFstor, std::ios_base::app);
                auto startstor = std::chrono::system_clock::now();
                // Some computation here
                auto endstor = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_secondsstor = endstor-startstor;
                std::time_t end_timestor = std::chrono::system_clock::to_time_t(endstor);
                outstor<< "Started simulation at " << std::ctime(&end_timestor) << "elapsed time: " << elapsed_secondsstor.count() << "s\n";
                outstor.close();

                //File ResultsGW
                string nameFgw="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsGW";
                string noSgw=ownMACAddress.substr(15,17);
                nameFgw.append(noSgw);
                nameFgw.append(".txt");
                //EV<<"nameF: "<<nameF<<"\n";
                ofstream outfilegw(nameFgw,ios::out);
                outfilegw<<"RESULTS FILE \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
                outfilegw.close();
                std::ofstream outgw(nameFgw, std::ios_base::app);
                auto startgw = std::chrono::system_clock::now();
                // Some computation here
                auto endgw = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_secondsgw = endgw-startgw;
                std::time_t end_timegw = std::chrono::system_clock::to_time_t(endgw);
                outgw<< "Started simulation at " << std::ctime(&end_timegw) << "elapsed time: " << elapsed_secondsgw.count() << "s\n";
                outgw.close();
}
//LQE
void Logger::saveResultsLQE(string nMY, string nGW, string shrtPath, string GraphSR){
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsLQE";
                           nameF.append(nMY);
                           nameF.append("_");
                           nameF.append(nGW);
                           nameF.append(".txt");
                           std::ofstream out(nameF, std::ios_base::app);
                           //My Short Path
                           string stP="ShortPath: \n";
                           stP.append(shrtPath);
                           out<<stP;
                           //Graph
                           string msIDis=" | Graph: \n";
                           msIDis.append(GraphSR);
                           out<<msIDis;
                           //Time
                           std::string timeMsg = std::to_string(simTime().dbl());//getInjectedTime().dbl());
                           string timeGen=" | Time is : ";
                           timeGen.append(timeMsg);
                           out<<timeGen;
                           out<<" |End \n";
                           out.close();
}
//Gen Data
void Logger::saveGenDat(string ownMACAddress, string dataN, string timeMsg){
    //save info into file
            string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsGen";
            string noS=ownMACAddress.substr(15,17);
            nameF.append(noS);
            nameF.append(".txt");
            std::ofstream out(nameF, std::ios_base::app);
            //Graph
            string msIDis="Data Name: ";
            msIDis.append(dataN);
            out<<msIDis;
            //Time
            string timeGen=" | Generated Time is : ";
            timeGen.append(timeMsg);
            out<<timeGen;
            out<<" |End \n";
            out.close();
}
//Data reached gw
void Logger::saveResultsGw(string ownMACAddress, string srcMAC, string msID, string timeSMsg, string timeRMsg){
    //save info into file
                string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsGW";
                string noS=ownMACAddress.substr(15,17);
                                  nameF.append(noS);
                                  nameF.append(".txt");
                                  std::ofstream out(nameF, std::ios_base::app);
                                  //Data Name
                                  string srcer="Source: ";
                                  srcer.append(srcMAC);
                                  out<<srcer;
                                  //MessageID
                                  string msIDis=" | Message ID: ";
                                  msIDis.append(msID);
                                  out<<msIDis;
                                  /*//Time generated
                                  std::string timeMsg = std::to_string(omnetDataMsg->getInjectedTime().dbl());//getInjectedTime().dbl());
                                  string timeGen=" | Time generated: ";
                                  timeGen.append(timeMsg);
                                  out<<timeGen;*/
                                  //Time sent from src
                                  string timeSSrc=" | Time sentFromSrcR: ";
                                  timeSSrc.append(timeSMsg);
                                  out<<timeSSrc;
                                  //time received here
                                  string timeRec=" | Time receivedFromSrcR: ";
                                  timeRec.append(timeRMsg);
                                  out<<timeRec;
                                  out<<" |End \n";
                                  out.close();
}
//Graph save
void Logger::saveGraphHere(string ownMACAddress, string graf){
    //save info into file
                string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/GraphES";
                string noS=ownMACAddress.substr(15,17);
                nameF.append(noS);
                nameF.append(".txt");
                std::ofstream out(nameF, std::ios_base::app);
                //Time
                std::string timeMsg = std::to_string(simTime().dbl());//getInjectedTime().dbl());
                       string timeGen=" | Time : ";
                       timeGen.append(timeMsg);
                       timeGen.append("\n");
                       out<<timeGen;
                //Graph
                graf.append("\n");
                out<<graf;
                out.close();
}
//current gw
void Logger::saveResultsGwChk(string ownMACAddress, string actual_gateway){
    //save info into file
                    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/CheckGw";
                    string noS=ownMACAddress.substr(15,17);
                    nameF.append(noS);
                    nameF.append(".txt");
                    std::ofstream out(nameF, std::ios_base::app);
                    //Current Gw
                    string currGw=actual_gateway;
                    currGw.append("\n");
                    out<<currGw;

                    //Time
                    std::string timeMsg = std::to_string(simTime().dbl());//getInjectedTime().dbl());
                    string timeGen=" | Time : ";
                    timeGen.append(timeMsg);
                    timeGen.append("\n");
                    out<<timeGen;
                    out.close();
}
//data reached gw
void Logger::saveMsgReachedGW(string dataName, double time, string myAddr, int nHops){
    //save name Msg received
        string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/GW/ReachedGwName";
        nameF.append(".txt");
        std::ofstream out(nameF, std::ios_base::app);
        //Name of data
        string dName=dataName;
        dName.append("\n");
        out<<dName;
        out.close();

        //save time this Msg was sent
        string nameS="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/GW/GwTimeSent";
        nameS.append(".txt");
        std::ofstream outs(nameS, std::ios_base::app);
        //time of data sent
        std::string timeMsgS = std::to_string(time);//getInjectedTime().dbl());
        string timeGenS=timeMsgS;
        timeGenS.append("\n");
        outs<<timeGenS;
        outs.close();

        //save time this Msg reached gw
        string nameFe="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/GW/GwTimeRec";
        nameFe.append(".txt");
        std::ofstream oute(nameFe, std::ios_base::app);
        //time of data rec
        std::string timeMsg = std::to_string(simTime().dbl());//getInjectedTime().dbl());
        string timeGen=timeMsg;
        timeGen.append("\n");
        oute<<timeGen;
        oute.close();

        //save gw id that got the Msg
        string nameFa="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/GW/ReachedGwSpcs";
        nameFa.append(".txt");
        std::ofstream outA(nameFa, std::ios_base::app);
        //GW
        string addrii=myAddr;
        string addri=addrii.substr(15,17);
        addri.append("\n");
        outA<<addri;
        outA.close();


        //save time gt reached gw with id on file
        string nameFer="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/GW/Gws/GwTimeRec";
        string noSr=myAddr.substr(15,17);
        nameFer.append(noSr);
        nameFer.append(".txt");
        std::ofstream outer(nameFer, std::ios_base::app);
        //time of data rec
        std::string timeMsgr = std::to_string(simTime().dbl());//getInjectedTime().dbl());
        string timeGenr=timeMsgr;
        timeGenr.append("\n");
        outer<<timeGenr;
        outer.close();

        //save number of hops of Msg received
        string nameHop="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/GW/Gws/GwMsgHops";
        nameHop.append(".txt");
        std::ofstream outhop(nameHop, std::ios_base::app);
        //Hops
        std::string nHop = std::to_string(nHops);//getInjectedTime().dbl());
        nHop.append("\n");
        outhop<<nHop;
        outhop.close();




}
//save rank gw
void Logger::saveGwRank(int GwId, double rankk, int oldID, double oldR, int myID){
    //save rank and id
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/GW/Gws/GwRank";
    nameF.append(".txt");
    std::ofstream out(nameF, std::ios_base::app);
    //Name of data
    std::string savi = std::to_string(GwId);
    std::string savir = std::to_string(rankk);
    std::string savoi = std::to_string(oldID);
    std::string meID = std::to_string(myID);
        std::string savoir = std::to_string(oldR);
    savi.append("->");
    savi.append(savir);
    savi.append(" | ");
    savi.append(savoi);
    savi.append("->");
    savi.append(savoir);
    savi.append(" by ");
    savi.append(meID);
    savi.append(" => ");
    out<<savi;
    std::string timeMsgr = std::to_string(simTime().dbl());//getInjectedTime().dbl());
    string timeGenr=timeMsgr;
    timeGenr.append("\n");
    out<<timeGenr;
    out.close();
}
//save rank table on each node
void Logger::saveGwRankT(string ownMACAddress,int GwId, double rankk, double ener){
    //save rank and id
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/GW/Gws/Rank/GwRankT";
    string noS=ownMACAddress.substr(15,17);
    nameF.append(noS);
    nameF.append(".txt");
    std::ofstream out(nameF, std::ios_base::app);
    //Name of data
    std::string savi = std::to_string(GwId);
    std::string savir = std::to_string(rankk);
    savi.append("->");
    savi.append(savir);
    savi.append("\n");
    out<<savi;
    out.close();


    //save battery table
    string nameFe="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/Ener/EnerwithGwChk/enerTabl";
        string noSe=ownMACAddress.substr(15,17);
        nameFe.append(noSe);
        nameFe.append(".txt");
        std::ofstream oute(nameFe, std::ios_base::app);
        //Name of data
        std::string savie = std::to_string(GwId);
        std::string savire = std::to_string(ener);
        savie.append("->");
        savie.append(savire);
        savie.append("\n");
        oute<<savie;
        oute.close();
}
void Logger::saveGwRankT_time(string ownMACAddress){
    //save rank and id
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/GW/Gws/Rank/GwRankT";
    string noS=ownMACAddress.substr(15,17);
    nameF.append(noS);
    nameF.append(".txt");
    std::ofstream out(nameF, std::ios_base::app);

    std::string timeMsgr = std::to_string(simTime().dbl());//getInjectedTime().dbl());
    string timeGenr=timeMsgr;
    timeGenr.append("\n");
    out<<timeGenr;
    out.close();

    //save energy
    string nameFe="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/Ener/EnerwithGwChk/enerTabl";
        string noSe=ownMACAddress.substr(15,17);
        nameFe.append(noSe);
        nameFe.append(".txt");
        std::ofstream oute(nameFe, std::ios_base::app);

        std::string timeMsgre = std::to_string(simTime().dbl());//getInjectedTime().dbl());
        string timeGenre=timeMsgre;
        timeGenre.append("\n");
        oute<<timeGenre;
        oute.close();
}

//--STORAGE---------------------------------------------------------
//Doesn't work
//--Wireless NIC-------------------------------------------------------------
//Init
void Logger::wirelessNicResultsInit(string ownMACAddress){
    //File stores data if I'm Src
            string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsSrc";
            string noS=ownMACAddress.substr(15,17);
            nameF.append(noS);
            nameF.append(".txt");
            //EV<<"nameF: "<<nameF<<"\n";
            ofstream outfile(nameF,ios::out);
            outfile<<"RESULTS FILE \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
            outfile.close();
            std::ofstream out(nameF, std::ios_base::app);
            auto start = std::chrono::system_clock::now();
            // Some computation here
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end-start;
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            out<< "Started simulation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
            out.close();

            //File stores sent data
            string nameS="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsSent";
            nameS.append(noS);
            nameS.append(".txt");
            ofstream outfileS(nameS,ios::out);
            outfileS<<"RESULTS Sent DataMsg \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
            outfileS.close();
            std::ofstream outS(nameS, std::ios_base::app);
            auto startS = std::chrono::system_clock::now();
            // Some computation here
            auto endS = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_secondsS = endS-startS;
            std::time_t end_timeS = std::chrono::system_clock::to_time_t(endS);
            outS<< "Started simulation at " << std::ctime(&end_timeS) << "elapsed time: " << elapsed_secondsS.count() << "s\n";
            outS.close();

            //File stores Received data
            string nameR="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsReceived";
                    nameR.append(noS);
                    nameR.append(".txt");
                    ofstream outfileR(nameR,ios::out);
                    outfileR<<"RESULTS Received DataMsg \nAuthor: João Patrício (castanheirapatricio@ua.pt)"<<endl;
                    outfileR.close();
                    std::ofstream outR(nameR, std::ios_base::app);
                    auto startR = std::chrono::system_clock::now();
                    // Some computation here
                    auto endR = std::chrono::system_clock::now();
                    std::chrono::duration<double> elapsed_secondsR = endR-startR;
                    std::time_t end_timeR = std::chrono::system_clock::to_time_t(endR);
                    outR<< "Started simulation at " << std::ctime(&end_timeR) << "elapsed time: " << elapsed_secondsR.count() << "s\n";
                    outR.close();
}
//All info on sent
void Logger::saveResultsSent(string ownMACAddress, string srcMAC, string dstMAC, string msNam, string msID, string timeMsg){
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsSent";
                        string noS=ownMACAddress.substr(15,17);
                        nameF.append(noS);
                        nameF.append(".txt");
                        std::ofstream out(nameF, std::ios_base::app);
                        //Generation Mac
                        string srcer="Source: ";
                        srcer.append(srcMAC);
                        out<<srcer;
                        //Destination next
                        string dster=" | NextDest: ";
                        dster.append(dstMAC);
                        out<<dster;
                        //Message Name
                        string msNamIs=" | Message ID: ";
                        msNamIs.append(msNam);
                        out<<msNamIs;
                        //MessageID
                        string msIDis=" | Message ID: ";
                        msIDis.append(msID);
                        out<<msIDis;
                        //Time sent
                        string timeGen=" | Time SentFromNIC: ";
                        timeGen.append(timeMsg);
                        out<<timeGen;
                        out<<" | End \n";
                        out.close();
}
//
void Logger::saveResultsSrc(string ownMACAddress, string srcMAC, string msID, string timeMsg){
    //save info into file
                string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsSrc";
                string noS=ownMACAddress.substr(15,17);
                nameF.append(noS);
                nameF.append(".txt");
                std::ofstream out(nameF, std::ios_base::app);
                //Generation Mac
                string srcer="Source: ";
                srcer.append(srcMAC);
                out<<srcer;
                //MessageID
                string msIDis=" | Message ID: ";
                msIDis.append(msID);
                out<<msIDis;
                //Time generated
                string timeGen=" | Time SentFromSrc: ";
                timeGen.append(timeMsg);
                out<<timeGen;
                out<<" |End \n";
                out.close();
}
//save all info rec
void Logger::saveResultsRec(string ownMACAddress, string srcMAC, string dstMAC, string msNam, string msID, string timeMsg){
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/ResultsReceived";
                        string noS=ownMACAddress.substr(15,17);
                        nameF.append(noS);
                        nameF.append(".txt");
                        std::ofstream out(nameF, std::ios_base::app);
                        //Generation Mac
                        string srcer="Source: ";
                        srcer.append(srcMAC);
                        out<<srcer;
                        //From neighbor
                        string dster=" | From: ";
                        dster.append(dstMAC);
                        out<<dster;
                        //Message Name
                        string msNamIs=" | Message ID: ";
                        msNamIs.append(msNam);
                        out<<msNamIs;
                        //MessageID
                        string msIDis=" | Message ID: ";
                        msIDis.append(msID);
                        out<<msIDis;
                        //Time generated
                        string timeGen=" | Time Received Here: ";
                        timeGen.append(timeMsg);
                        out<<timeGen;
                        out<<" | End \n";
                        out.close();
}
//
void Logger::outputResultsSent(string ownMACAddress){
    //save info into file
                            string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/AllPktSent";
                            string noS=ownMACAddress.substr(15,17);
                            nameF.append(noS);
                            nameF.append(".txt");
                            std::ofstream out(nameF, std::ios_base::app);
                            //Generation Mac
                            string srcer="Sent ";
                            out<<srcer;
                            //Time sent
                            std::string timeMsg = std::to_string(simTime().dbl());
                            string timeGen=" | Time SentFromHere: ";
                            timeGen.append(timeMsg);
                            out<<timeGen;
                            out<<" | End \n";
                            out.close();

                            string nameFa="/home/mob/Tese/Boat/OPAQS/simulations/DanT/Logs/AllPktSent";

                                                        nameFa.append(".txt");
                                                        std::ofstream outa(nameFa, std::ios_base::app);
                                                        //Generation Mac
                                                        string srcera="Sent \n";
                                                        outa<<srcera;
}
//
void Logger::outputResultsReceived(string ownMACAddress){
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/AllPktRec";
                            string noS=ownMACAddress.substr(15,17);
                            nameF.append(noS);
                            nameF.append(".txt");
                            std::ofstream out(nameF, std::ios_base::app);
                            //Generation Mac
                            string srcer="Received ";
                            out<<srcer;
                            //Time sent
                            std::string timeMsg = std::to_string(simTime().dbl());
                            string timeGen=" | Time SentFromHere: ";
                            timeGen.append(timeMsg);
                            out<<timeGen;
                            out<<" | End \n";
                            out.close();
}
//
void Logger::saveSentDataName(string ownMACAddress, string msNam){
    string nameF="/home/mob/Tese/Boat/OPAQS/simulations/DanT/DataResults/dataNameSent";
                            string noS=ownMACAddress.substr(15,17);
                            nameF.append(noS);
                            nameF.append(".txt");
                            std::ofstream out(nameF, std::ios_base::app);
                            //Message Name
                            string msNamIs=msNam;
                            msNamIs.append("\n");
                            out<<msNamIs;
                            out.close();
}


//--EPIDEMIC--------------------------------------------------
