/*
 * StorageM.cc
 *
 *  Created on: 11/03/2019
 *  Author: João Patrício
 */
// Note: later it could be an independent omnet++ module for ease of use as a simulator (thought as it is it's easier for outside integration)


#include "StorageM.h"

//Define_Module(StorageM);

//void StorageM::initialize(){//StorageM(){

//CONSTRUCTOR
StorageM::StorageM(){

    maximumCacheSize=5000000;   //It's updated later with a method
    currentCacheSize = 0;
}

//void StorageM::finish(){//~StorageM(){

//DESTRUCTOR
StorageM::~StorageM(){
    EV<<"Destructor \n";
    //clear List
    EV<<"Destructer size1: "<<cacheList.size()<<'\n';
    cacheList.clear();
    EV<<"Destructer after clean-size2: "<<cacheList.size()<<'\n';
}

/*****************************************************************************
 * Defines the maximum size of the cache
 */
void StorageM::maximumCacheS(int maximumCacheSizeValue){
    maximumCacheSize=maximumCacheSizeValue;
}

void StorageM::cleanStor(){
    while (cacheList.size() > 0) {
            auto it=cacheList.begin();
            cacheList.erase(it);
        }
}


/********************************************************************************
 * Checks if the Msg is saved in Cache - not fulyl working yet
 */
//31_07_19 15h30
bool StorageM::msgExistsInC(cMessage *msg){
    DataMsg *omnetDataMsg = dynamic_cast<DataMsg*>(msg);
        //EV<<"omnetDataMsg: "<<omnetDataMsg->getHopCount()<<'\n'; //teste para verificar se funfa check
        CacheEntry cacheEntry;//added
        auto itC = cacheList.begin();
        int found = FALSE;
        EV<<"TRUEE3 \n";
    //Verifies if da Data already exists in cache
        while (itC != cacheList.end()) {
            //EV<<"dataN: "<<iteratorCache->dataName<<'\n';
                if (itC->dataName == omnetDataMsg->getDataName()) {
                    found = TRUE;
                    EV<<"TRUEE2 \n";
                    break;
                }
                itC++;
            }
        return found;
}

/************************************************************************
 * Saves the data in the cache:
 * Verifies if it already exists in cache, applies caching policy if the
 * cache is limited or full (checking if cache date is old - removing older
 * data. Saves dataMsg in cache.
 *
 * Por limpar - já não precisa de receber a origem?
 */
void StorageM::saveData(cMessage *msg, int origin){
   EV<<"saveData: \n";

    DataMsg *omnetDataMsg = dynamic_cast<DataMsg*>(msg);
    EV<<"Saving Msg with ID "<<omnetDataMsg->getNMsgOrder()<<" at time: "<<simTime().dbl()<<" with the name:"<<omnetDataMsg->getDataName()<<"\n";
    //EV<<"omnetDataMsg: "<<omnetDataMsg->getHopCount()<<'\n'; //teste para verificar se funfa check
    CacheEntry cacheEntry;//added
    auto itC = cacheList.begin();
    int found = FALSE;
//Verifies if da Data already exists in cache
    while (itC != cacheList.end()) {
        //EV<<"dataN: "<<iteratorCache->dataName<<'\n';
            if (itC->dataName == omnetDataMsg->getDataName()) {
                found = TRUE;
                EV<<"TRUEE \n";
                break;
            }
            itC++;
        }
    if (!found) {

        //Save saved 1st time msgs
        string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsStor";
        string ownMACAddress;
        if(origin){
            ownMACAddress = omnetDataMsg->getDestinationAddress();
        }else{
            ownMACAddress = omnetDataMsg->getOriginatorNodeMAC();
        }
                   string noS=ownMACAddress.substr(15,17);
                   nameF.append(noS);
                   nameF.append(".txt");
                   std::ofstream out(nameF, std::ios_base::app);
                   //Data Name
                   string srcMAC=omnetDataMsg->getDataName();
                   string srcer="Source: ";
                   srcer.append(srcMAC);
                   out<<srcer;
                   //MessageID
                   std::string msID=std::to_string(omnetDataMsg->getNMsgOrder());//getMsgUniqueID();
                   string msIDis=" | Message ID: ";
                   msIDis.append(msID);
                   out<<msIDis;
                   //Time generated
                   std::string timeMsg = std::to_string(omnetDataMsg->getInjectedTime().dbl());//getInjectedTime().dbl());
                   string timeGen=" | Time generated: ";
                   timeGen.append(timeMsg);
                   out<<timeGen;
                   //Time sent from src
                   std::string timeSMsg = std::to_string(omnetDataMsg->getSentTimeRout().dbl());//getInjectedTime().dbl());
                   string timeSSrc=" | Time sentFromSrcR: ";
                   timeSSrc.append(timeSMsg);
                   out<<timeSSrc;
                   /*//Time sent from neigh
                   std::string timeSMsgN = std::to_string(omnetDataMsg->getSentTime().dbl());//getInjectedTime().dbl());
                   string timeSN=" | Time sentFromNeigh: ";
                   timeSN.append(timeSMsgN);
                    out<<timeSN;*/
                   //time received here
                   std::string timeRMsg = std::to_string(omnetDataMsg->getReceivedTimeRout().dbl());//getReceivedTime().dbl());
                   string timeRec=" | Time receivedFromSrcR: ";
                   timeRec.append(timeRMsg);
                   out<<timeRec;
                   out<<" |End \n";
                   out.close();


                   if(origin){
                       ownMACAddress = omnetDataMsg->getDestinationAddress();
                       string destMACAddress=omnetDataMsg->getOriginatorNodeMAC() ;

                       string nameFInd="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsStorInd";
                       string nMY=ownMACAddress.substr(15,17);
                       string nD=destMACAddress.substr(15,17);
                       nameFInd.append(nMY);
                       nameFInd.append("_");
                       nameFInd.append(nD);
                       nameFInd.append(".txt");



                       std::ofstream outInd(nameFInd, std::ios_base::app);
                                          //Data Name
                                          string srcMACInd=omnetDataMsg->getDataName();
                                          string srcerInd="Source: ";
                                          srcerInd.append(srcMACInd);
                                          outInd<<srcerInd;
                                          //MessageID
                                          std::string msIDInd=std::to_string(omnetDataMsg->getNMsgOrder());//getMsgUniqueID();
                                          string msIDisInd=" | Message ID: ";
                                          msIDisInd.append(msIDInd);
                                          outInd<<msIDisInd;
                                          //Time generated
                                          std::string timeMsgInd = std::to_string(omnetDataMsg->getInjectedTime().dbl());//getInjectedTime().dbl());
                                          string timeGenInd=" | Time generated: ";
                                          timeGenInd.append(timeMsgInd);
                                          outInd<<timeGenInd;
                                          //Time sent from src
                                          std::string timeSMsgInd = std::to_string(omnetDataMsg->getSentTimeRout().dbl());//getInjectedTime().dbl());
                                          string timeSSrcInd=" | Time sentFromSrcR: ";
                                          timeSSrcInd.append(timeSMsgInd);
                                          outInd<<timeSSrcInd;
                                          /*//Time sent from neigh
                                          std::string timeSMsgN = std::to_string(omnetDataMsg->getSentTime().dbl());//getInjectedTime().dbl());
                                          string timeSN=" | Time sentFromNeigh: ";
                                          timeSN.append(timeSMsgN);
                                           out<<timeSN;*/
                                          //time received here
                                          std::string timeRMsgInd = std::to_string(omnetDataMsg->getReceivedTimeRout().dbl());//getReceivedTime().dbl());
                                          string timeRecInd=" | Time receivedFromSrcR: ";
                                          timeRecInd.append(timeRMsgInd);
                                          outInd<<timeRecInd;
                                          outInd<<" |End \n";
                                          outInd.close();

                   }







    // Applies caching policy if limited cache and cache is full (deletes oldest)
        if (maximumCacheSize != 0 && (currentCacheSize + omnetDataMsg->getRealPayloadSize()) > maximumCacheSize && cacheList.size() > 0) {

            itC = cacheList.begin();
            auto removingCacheEntry=itC;
            itC = cacheList.begin();
            while (itC != cacheList.end()) {
                if (itC->validUntilTime < removingCacheEntry->validUntilTime) {
                    removingCacheEntry = itC;
                    EV<<"Removing from Storage:"<<itC->nMsgOrder<<"\n";
                }
                itC++;
            }
            currentCacheSize -= removingCacheEntry->realPayloadSize;
            cacheList.erase(removingCacheEntry);
            //emit(cacheBytesRemovedSignal, (long) removingCacheEntry->realPayloadSize);
        }

        //EV<<"Saving Msg with ID "<<omnetDataMsg->getNMsgOrder()<<" at time: "<<simTime().dbl()<<"\n";

        strcpy(cacheEntry.name,"God");
        // Here it saves the DataMsg in Cache
        cacheEntry.messageID = omnetDataMsg->getDataName();
//ADDED 1/07 15h24
        cacheEntry.nMsgOrder=omnetDataMsg->getNMsgOrder();
//
        //if(origin==0){ cacheEntry.hopCount = 0; }
        cacheEntry.dataName = omnetDataMsg->getDataName();
        cacheEntry.realPayloadSize = omnetDataMsg->getRealPayloadSize();
        //cacheEntry.dummyPayloadContent = omnetDataMsg->getDummyPayloadContent();
        cacheEntry.validUntilTime = simTime().dbl()+max_age;//omnetDataMsg->getValidUntilTime();    //sets new aging
        cacheEntry.realPacketSize = omnetDataMsg->getRealPacketSize();
        cacheEntry.originatorNodeMAC = omnetDataMsg->getOriginatorNodeMAC();
        cacheEntry.destinationOriented = omnetDataMsg->getDestinationOriented();
        if (omnetDataMsg->getDestinationOriented()){
            cacheEntry.finalDestinationNodeName = omnetDataMsg->getFinalDestinationNodeName();
            EV<<"DataMsg has final destination: "<<cacheEntry.finalDestinationNodeName<<"\n";
        }
        cacheEntry.groupType = omnetDataMsg->getGroupType();

        //cacheEntry.hopsTravelled = omnetDataMsg->getHopsTravelled();

        cacheEntry.msgUniqueID = omnetDataMsg->getMsgUniqueID();
        cacheEntry.injectedTime = omnetDataMsg->getInjectedTime();
        cacheEntry.nHops = omnetDataMsg->getNHops();
        cacheEntry.createdTime = simTime().dbl();
        cacheEntry.updatedTime = simTime().dbl();
        cacheEntry.sentTime = omnetDataMsg->getSentTime();
        cacheEntry.receivedTime = omnetDataMsg->getReceivedTime();
        cacheEntry.sentTimeRout = omnetDataMsg->getSentTimeRout();
        cacheEntry.receivedTimeRout = omnetDataMsg->getReceivedTimeRout();



        //Added 28/07/19 14h23
        int vi=0;
        int siz=omnetDataMsg->getPrevHopsListArraySize();
        while(vi<siz){
            cacheEntry.prevHopsList[vi] = omnetDataMsg->getPrevHopsList(vi);
            //EV<<"List prev hops: "<<cacheEntry.prevHopsList[vi]<<"\n";
            vi++;
        }
        cacheEntry.prevHopListSize = omnetDataMsg->getPrevHopsListArraySize();
        //EV<<"Size of prevHopsListSize: "<<cacheEntry.prevHopListSize<<"\n";
        EV<<"Size1 is: "<<cacheEntry.prevHopListSize<<"\n";






        cacheList.push_back(cacheEntry);//alt
        currentCacheSize += cacheEntry.realPayloadSize;
    }

    cacheEntry.lastAccessedTime = simTime().dbl();

}

void StorageM::updateMaxAge(double value){
    max_age=value;
}


/*************************************************************************************
 * Verifies if there's data older than the limited time. If so, deletes it from cache.
 */
void StorageM::ageDataInStorage(){

    EV << "KEpidemic ageDataInCache\n";
    // remove expired data items
    int expiredFound = TRUE;
    while (expiredFound) {
        expiredFound = FALSE;
        auto itC = cacheList.begin();
//Runs the list from the beggining to the end in search for an expired element of the list;
        while (itC != cacheList.end()) {
            if (itC->validUntilTime < simTime().dbl()) {
                expiredFound = TRUE;
                break;
            }
            itC++;
        }
        if (expiredFound) {
            currentCacheSize -= itC->realPacketSize;
            cacheList.erase(itC);
        }
    }
}

/*****************************************************************************************
 * Checks if msgID exists in cache (going through the cache list)
 */
bool StorageM::msgIDExists(string messageID){

    auto itC = cacheList.begin();
    bool found = FALSE;
    while (itC != cacheList.end()) {
        if (itC->messageID == messageID) {
            found = TRUE;
            EV <<"Verdade-exists \n";
            return true;
            break;
        }
        itC++;
    }
    return false;
}

/*********************************************************************************************
 * Returns the number in position of the list the element is
 */
int StorageM::msgIDListPos(string messageID){

    auto itC = cacheList.begin();
    int count=0;
    while (itC != cacheList.end()) {
        count++;
        if (itC->messageID == messageID) {
            return count;
            break;
        }
        itC++;
    }
    return count;
}


void StorageM::updatePrevHopsList(int position, string HopAddr){
    //EV<<"updatePrevHopList SM \n";
    auto itC = cacheList.begin();
    int countM=1;
    //Gets to the element of the List requested
    while (countM<position) {
        //EV<<"pullOutMsg while SM \n";
        itC++;
        countM++;
    }
    //Adds my Addr to the Hop List
    int siz=itC->prevHopListSize;
    int count1=0;
    while(count1<siz){
        string val2 = itC->prevHopsList[count1];
        if(val2==""){
                itC->prevHopsList[count1]=HopAddr;
                break;
            }
            count1++;
        }

}

/********************************************************************************************
 * Goes to the position of a stored Msg in cache and returns a copy.
 */
DataMsg* StorageM::pullOutMsg(cMessage *msg, string ownMACAddress, int count){
    EV<<"pullOutMsg SM \n";
    DataReqMsg *dataRequestMsg = dynamic_cast<DataReqMsg*>(msg);
    auto itC = cacheList.begin();
    int countM=1;
    //Gets to the element of the List requested
    while (countM<count) {
        //EV<<"counting \n";
        itC++;
        countM++;
    }
    DataMsg *dataMsg = new DataMsg();
    dataMsg->setSourceAddress(ownMACAddress.c_str());
    dataMsg->setDestinationAddress(dataRequestMsg->getSourceAddress());
    dataMsg->setDataName(itC->dataName.c_str());
    //dataMsg->setDummyPayloadContent(itC->dummyPayloadContent.c_str());
    dataMsg->setValidUntilTime(itC->validUntilTime);
    dataMsg->setRealPayloadSize(itC->realPayloadSize);
    //A  fazer: check OutsMsg.msg on sizing messages
    int realPacketSize = 6 + 6 +64 + 1 + 2 + itC->realPayloadSize + 4 + 6 + 6 + 1 + 2 + 1 + 32; //added  9/09
    dataMsg->setRealPacketSize(realPacketSize);
    dataMsg->setByteLength(realPacketSize);
    dataMsg->setOriginatorNodeMAC(itC->originatorNodeMAC.c_str());
    dataMsg->setDestinationOriented(itC->destinationOriented);
    if (itC->destinationOriented) {
        dataMsg->setFinalDestinationNodeName(itC->finalDestinationNodeName.c_str());
    }
    dataMsg->setMessageID(itC->messageID.c_str());
    //ADDED 1/07 15h24
    dataMsg->setNMsgOrder(itC->nMsgOrder);
    //
    //dataMsg->setHopCount(itC->hopCount);
    dataMsg->setGroupType(itC->groupType);
    //dataMsg->setHopsTravelled(itC->hopsTravelled);
    dataMsg->setMsgUniqueID(itC->msgUniqueID);
    dataMsg->setInjectedTime(itC->injectedTime);
    dataMsg->setNHops(itC->nHops);
    dataMsg->setSentTime(itC->sentTime);
    dataMsg->setReceivedTime(itC->receivedTime);
    dataMsg->setSentTimeRout(itC->sentTimeRout);
    dataMsg->setReceivedTimeRout(itC->receivedTimeRout);

    //Added 28/07/19 14h23
    int vi=0;
    int siz=itC->prevHopListSize;
            //->getPrevHopsListArraySize();
    //EV<<"Size 2 is: "<<siz<<" e cont: "<<countM<<"\n";
    //copies the precious Hop List
    dataMsg->setPrevHopsListArraySize(itC->prevHopListSize);
    while(vi<siz){
        dataMsg->setPrevHopsList(vi,itC->prevHopsList[vi].c_str());
        vi++;
    }

    //Adds my Addr to the Hop List
    int count2=0;

    while(count2<siz){
       // EV<<"Istoo : "<<dataMsg->getPrevHopsList(count2)<<"\n";
        string val2 =dataMsg->getPrevHopsList(count2);
        if(val2==""){
            dataMsg->setPrevHopsList(count2, ownMACAddress.c_str());
           // EV<<"Istoo 2 : "<<dataMsg->getPrevHopsList(count2)<<"\n";
            break;
        }
        count2++;
    }
    //EV<<"Eiaum:"<<dataMsg->getPrevHopsListArraySize()<<"\n";
    //dataMsg->setPrevHopsList(siz+1,ownMACAddress.c_str());




    EV<<"pull Out Msg "<<dataMsg->getNMsgOrder()<<" at time:"<<simTime().dbl()<<" with the name:"<<dataMsg->getDataName()<<"\n";
    return dataMsg;
}

/***********************************************************************************************************
 * Returns a List of MsgIDs of Msgs stored in cache
 */
vector<string> StorageM::returnSelectMsgIDList(vector<string> & selectedMessageIDList, int maximumHopCount)
{

    auto itC = cacheList.begin();
    while (itC != cacheList.end()) {
        EV<<"Maximum hop count: "<<maximumHopCount<<"\n";
        if ((itC->nHops+1)<maximumHopCount){
            EV<<"Pushed back\n";
            selectedMessageIDList.push_back(itC->messageID);
        }
        itC++;
    }
    //EV<<"Cache list size: "<<cacheList.size()<<"\n";
    //EV<<"SelectedMessageIDList size: "<<selectedMessageIDList.size()<<"\n";
    return selectedMessageIDList;
}

/*********************************************************************************************************
 * Returns the size of the List of cache Msgs (nº of saved msgs in cache)
 */
int StorageM::cacheListSize(){

   EV <<"Storage Cache List Size: "<<cacheList.size()<<'\n';
   return cacheList.size();
}


/**********************************************************************************************************
 *Deletes a message on the Storage list
 */
void StorageM::deleteMsg(string messageID){
    if(msgIDExists(messageID)){
        EV << "Delete Data In Cache\n";
        int msgFound = TRUE;
        while (msgFound) {
            msgFound = FALSE;
            auto itC = cacheList.begin();
        //Runs the list from the beggining to the end in search for the wanted element of the list;
            while (itC != cacheList.end()) {
                if (itC->messageID == messageID) {
                    msgFound = TRUE;
                    EV <<"Verdade-erase \n";
                    break;
                }
                itC++;
            }
            if (msgFound) {
                currentCacheSize -= itC->realPacketSize;
                cacheList.erase(itC);
            }
        }
    }else{
        EV<<"ERRO: MSG N EXISTE NA CACHE\n";
    }
}


