//
// The model implementation for a Forwarding module (eg. Epidemic, spray, etc) Routing layer
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 19-march-2019
//

#include "RoutingLayer.h"

Define_Module(RoutingLayer);

void RoutingLayer::initialize(int stage)
{
    if (stage == 0) {
        Stor=StorageM(); //constructor
        graphR=GraphT();
        // get parameters
        nodeIndex = par("nodeIndex");
        ownMACAddress = par("ownMACAddress").stringValue();
        ownBTMACAddress = par("ownBTMACAddress").stringValue();
        nextAppID = 1;
        maximumCacheSize = par("maximumCacheSize");
        Stor.maximumCacheS(maximumCacheSize);
        maximumHopCount = par("maximumHopCount");
        useTTL = par("useTTL");
        numEventsHandled = 0;
        inCache=0;
        waitBFsend = par("waitBFsend");
        max_age = par("max_age");

    } else if (stage == 1) {
        Stor.updateMaxAge(max_age);

    } else if (stage == 2) {

        // setup statistics signals
        dataBytesReceivedSignal = registerSignal("dataBytesReceived");
        sumVecBytesReceivedSignal = registerSignal("sumVecBytesReceived");
        dataReqBytesReceivedSignal = registerSignal("dataReqBytesReceived");
        totalBytesReceivedSignal = registerSignal("totalBytesReceived");

        cacheBytesRemovedSignal = registerSignal("cacheBytesRemoved");
        cacheBytesAddedSignal = registerSignal("cacheBytesAdded");
        cacheBytesUpdatedSignal = registerSignal("cacheBytesUpdated");


        //Create File that saves Data
        /*ofstream file;
        string nameF="Results";
        string noS=ownMACAddress.substr(15,17);
        nameF.append(noS);
        nameF.append(".txt");
        file.open (nameF);
        file << "Here is information Obtained in this node\n";
        file.close();
        file.open(nameF);
        file << "Here is another information Obtained in this node\n";
                file.close();*/

        string nameF="Results";
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


    } else {
        EV_FATAL << ROUTINGLAYER_SIMMODULEINFO << "Something is radically wrong in initialisation \n";
    }
}

int RoutingLayer::numInitStages() const
{
    return 3;
}



void RoutingLayer::handleMessage(cMessage *msg)
{

    cGate *gate;
    char gateName[64];

    numEventsHandled++;

    // age the data in the cache only if needed (e.g. a message arrived)
    if (useTTL)
        Stor.ageDataInStorage();

    // self messages
    if (msg->isSelfMessage()) {
        EV_INFO << ROUTINGLAYER_SIMMODULEINFO << "Received unexpected self message" << "\n";
        delete msg;

    // messages from other layers
    } else {

       // get message arrival gate name
        gate = msg->getArrivalGate();
        strcpy(gateName, gate->getName());

        // app registration message arrived from the upper layer (app layer)
        if (strstr(gateName, "upperLayerIn") != NULL && dynamic_cast<RegisterAppMsg*>(msg) != NULL) {
            handleAppRegistrationMsg(msg);

        // data message arrived from the upper layer (app layer)
        } else if (strstr(gateName, "upperLayerIn") != NULL && dynamic_cast<DataMsg*>(msg) != NULL) {
            handleDataMsgFromUpperLayer(msg);

        // data message arrived from the lower layer (link layer)
        } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<DataMsg*>(msg) != NULL) {
            handleDataMsgFromLowerLayer(msg);

        //Added 17/06 17h
        } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<AckMsg*>(msg) != NULL) {
            handleAckFromLowerLayer(msg);

       //Added 26/06 19h06
        } else if (strstr(gateName, "neighLayerIn") != NULL && dynamic_cast<BeaconInfoMsg*>(msg) != NULL) {
            EV<<"Handling Beacon\n";
            handleBeaconInfo(msg);

         // data request message arrived from the lower layer (link layer)
        } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<DataReqMsg*>(msg) != NULL) {
            EV<<"Handling DataReqMsg\n";
            handleDataReqMsg(msg);


        // data request message arrived from the lower layer (link layer)
        } else if (strstr(gateName, "neighLayerIn") != NULL && dynamic_cast<NetworkGraphMsg*>(msg) != NULL) {
            EV<<"Handling NetworkGraphMsg\n";
            handleNetworkGraphMsg(msg);


        // received some unexpected packet
        } else {

            EV_INFO << ROUTINGLAYER_SIMMODULEINFO << "Received unexpected packet" << "\n";
            delete msg;
        }
    }
}

//Saves the received graph from neighboring here for later use in decision;
bool RoutingLayer::getGraph(string graphS){//, int numberVert){ //String:" 1->2:4;\n2->1:4;\n "
    std::string delimiter = ";";

    int i=0;//, q1=0;
    for(i=0;i<graphS.length();i++){
        int j=graphS.find(delimiter,i);
        if(j==std::string::npos){
            return false;
        }else{
            std::string token = graphS.substr(i, j-i);
            EV<<"Got the: "<<token<<" at: "<<j<<".\n";
            int q1 = graphS.find("-",i);
            int q2 = graphS.find(":",i);
            string v1=graphS.substr(i,q1-i);
            string v2=graphS.substr(q1+2,q2-(q1+2));
            string w1=graphS.substr(q2+1,j-(q2+1));
            //EV<<"Q1: "<<q1<<" V1: "<<v1<<" V2: "<<v2<<" W: "<<w1<<"\n";
            int vert1 = std::stoi (v1);
            int vert2 = std::stoi (v2);
            int weight1 = std::stod (w1);
            //EV<<" V1: "<<vert1<<" V2: "<<vert2<<" W: "<<weight1<<"\n";
            graphR.add_edge(vert1,vert2,weight1);
            //graphR.displayMatrix(3);
            //EV<<"Graph on Routing: \n";
            string GraphSR=graphR.returnGraphT();
            i =j+1;

        }
    }
}

void RoutingLayer::handleNetworkGraphMsg(cMessage *msg){
    EV<<"Routing: handleNetworkGraphMsg\n";
    NetworkGraphMsg *neighGraphMsg = dynamic_cast<NetworkGraphMsg*>(msg);
    string graphS = neighGraphMsg->getGraphN();
    //int numberVert = neighGraphMsg->getNumberVert();
    //int countVert = neighGraphMsg->getCountVert();
    EV<<"Recebeu-se: "<<graphS<<"\n";
    bool updt=getGraph(graphS);
    if(updt){ EV<<"Graph Updated\n";}else{EV<<"Graph not updated due to empty string\n";}

    delete msg;
}

//Added 25/06
//DECISION OF SENDING IS MADE HERE
/*********************************************************************************************************
 * (NOT_If the prob is good enough,) Gets List of Msgs in cache, for each MsgID of the list it searches if msg exists and gets its position,
 * pulls out the dataMsg prepared to send and sends it to lowerLayer if the destination is not on the previous Hops List of the DataMsg
 */
void RoutingLayer::handleDataReqMsg(cMessage *msg){

    EV<<"Routing: handleDataReqMsg\n";
    //pullOutMsg(msg);
    DataReqMsg *dataRequestMsg = dynamic_cast<DataReqMsg*>(msg);

    //if(dataRequestMsg->getProb()>=0.5){
    //checks list of msgs in cache
        vector<string> selectedMessageIDList;
        returnSelectMsgIDList(selectedMessageIDList);
        vector<string>::iterator iteratorMessageIDList;
        iteratorMessageIDList = selectedMessageIDList.begin();
        inCache = selectedMessageIDList.size();
        int i=0;

        if(!isSending && !isReceiving && (waitS<=simTime().dbl())){
            while (iteratorMessageIDList != selectedMessageIDList.end()) {  //checks all stored Msgs
               isSending=true;
               EV<<"SelectedMessageIDList size here is: "<<selectedMessageIDList.size()<<"\n";
               string messageID = *iteratorMessageIDList;
               bool found = Stor.msgIDExists(messageID);
               int position=Stor.msgIDListPos(messageID);
               if(found){ //if there is a stored DataMsg
                   //verify NIC:
                   string SouceDRAdd = dataRequestMsg->getSourceAddress();
                   if((SouceDRAdd.substr(0,2))=="BT"){
                       MyAddH=ownBTMACAddress;
                   }else{
                       MyAddH=ownMACAddress;
                   }
                   DataMsg *dataMsg = Stor.pullOutMsg(msg,MyAddH, position);
                   string destAdd = dataRequestMsg->getSourceAddress();
                   string gwAdd = dataMsg->getFinalDestinationNodeName();

                   //Loop Avoidance
                   int count1=0;
                   bool foundH=false, msgSent=false;
                   int sizeH = dataMsg->getPrevHopsListArraySize();
                   string HopAdd=dataMsg->getPrevHopsList(count1);
                   EV<<"Pos1: "<<dataMsg->getPrevHopsList(0)<<" source: "<<dataRequestMsg->getSourceAddress()<<"\n";
                   while(count1<sizeH){
                       HopAdd=dataMsg->getPrevHopsList(count1);
                       if(HopAdd==destAdd){
                           foundH=true;
                           EV<<"Found it \n";
                           break;
                       }
                       count1++;
                   }

                   //Verifies if DataMsg destination is this neighbor and DataMsg has not been send yet, if so, send directly with Loop Avoidance
                   if(dataMsg->getFinalDestinationNodeName()==destAdd && foundH==false){
                       EV<<"Direct Neigh is final dest. \n";
                       send(dataMsg, "lowerLayerOut");
                       msgSent = true;
                    //break;
                   }else{

                       int myID=graphR.add_element(MyAddH);
                       int gwID=graphR.add_element(gwAdd);
                       int dstID=graphR.add_element(destAdd);
                       bool isInShortPath=false;
                       EV<<"Dijkstra from myID to gwID\n";
                       graphR.dijkstra(myID, gwID);


                    //Verifies if destination is not on the prevHopList of  the Stored Msg - Loop Avoidance

                       EV<<"Sending Data Msg\n";
                       if(foundH==false && msgSent==false){
                        //Checks if there is a shortest path between me and GW
                        string sPth=graphR.returnShortestPath(myID,gwID);
                        EV<<"sPth="<<sPth<<".\n";
                        bool existsPath=true;
                        if(sPth==""){
                            EV<<"No near Path\n";
                            existsPath=false;
                        }
                        if(existsPath){
                            //Verify if  that shortest path between me and GW includes this neighbor, if so, send
                            isInShortPath=graphR.isInShortPath(myID,gwID, dstID);
                            if(isInShortPath){
                                send(dataMsg, "lowerLayerOut");
                            }
                        }
                       }
                   }
               }
               EV<<"Add++\n";
                iteratorMessageIDList++;
            }
        }

        isSending=false;
    //} else{ EV<<"Probability too low to send mensage \n"; }
    delete msg;
}


//Added 26/06
/*********************************************************************************************************
 * Get's beacon, identifies from which NIC it came, creates dataReqMsg and sends to lowerLayer
 */
void RoutingLayer::handleBeaconInfo(cMessage *msg){
    EV<<"Routing: handleBeacon\n";
    BeaconInfoMsg *beaconMsg = dynamic_cast<BeaconInfoMsg*>(msg);


    //Save Graph Matrix on RoutingLayer
    string myGraph=beaconMsg->getNeighGraph();
    //int npos=beaconMsg->getNumberVert();
    EV<<"Graph in routing: \n";
    getGraph(myGraph);



    DataReqMsg *dataRequestMsg = new DataReqMsg();
//changed 23/07/19 17h54
    string SouceAdd = beaconMsg->getSourceAddress();
    if((SouceAdd.substr(0,2))=="BT"){
        EV<<"It's BT \n";
        //dataRequestMsg->setNic(1);//APAGAR
        dataRequestMsg->setSourceAddress(ownBTMACAddress.c_str());
    //EV<<"SouceAdd= "<<SouceAdd.substr(0,2)<<"\n";

    }else{
        //dataRequestMsg->setNic(0);//APAGAR
        dataRequestMsg->setSourceAddress(ownMACAddress.c_str());
    }
    dataRequestMsg->setDestinationAddress(beaconMsg->getSourceAddress());
    int realPacketSize = 6 + 6 + 6 + 4 + 4;//(1 * ROUTINGLAYER_MSG_ID_HASH_SIZE);  //A REVER NO FUTURO
    //EV<<"Real packet size of DataMsg: "<<realPacketSize<<"\n";
    dataRequestMsg->setRealPacketSize(realPacketSize);
    dataRequestMsg->setByteLength(realPacketSize);
    dataRequestMsg->setSSI(beaconMsg->getSSI());
    dataRequestMsg->setProb(beaconMsg->getMyProb());      //VERIFICAR VALOR DE PROB QUE AQUI METO pk é o meu

    //Para retirar futuramente
    if(beaconMsg->getProb()>0.5){
        EV<<"Prob bigger than 0.5\n ";
        dataRequestMsg->setSendMeData(true);
    } else{ dataRequestMsg->setSendMeData(false);}

    send(dataRequestMsg, "lowerLayerOut");
    delete msg;
}


/********************************************************************************
 *Get's Ack, checks if the other was the final Destiny.
 */
void RoutingLayer::handleAckFromLowerLayer(cMessage *msg){
    AckMsg *ackMsg = dynamic_cast<AckMsg*>(msg);
    string messageID = ackMsg->getMessageID();
    bool isFinalDest=ackMsg->getIsFinalDest();

    if(isFinalDest){
        //Stor.deleteMsg(messageID); //PARA JÁ NAO QUERO QUE APAGUE PARA TESTAR SITUAÇÃO ATUAL DE SPREAD
        EV<<"It reached the GW! \n";
    }

    //Update previous Hops List
    bool found = Stor.msgIDExists(messageID);
    int position=Stor.msgIDListPos(messageID);
    string HopAddr = ackMsg->getSourceAddress();
    if(found){
        Stor.updatePrevHopsList(position,HopAddr);
    }

    //PAmuLQE-NACK - when received the ack it deletes the msg so only one copie is in the network

    Stor.deleteMsg(messageID);






    delete msg;
}

void RoutingLayer::handleDataMsgFromUpperLayer(cMessage *msg) //Store in cache
{
    DataMsg *upperDataMsg = dynamic_cast<DataMsg*>(msg);
    upperDataMsg->setOriginatorNodeMAC(ownMACAddress.c_str());
    Stor.saveData(msg,0);
    delete msg;
}


/****************************************************************************************************
 * Updates the number of Hops of DataMsg, if the msg has reached it's destination (I'm the Gw) or has
 * reached the maximum Hops, then don't save it in cache. Generate and send Ack informing that DataMsg
 * was received and if I am or not it's final destination (Gw). If a registered app exists of the type
 * of dataMsg received, send it to the upperLayer.
 *
 * Outputs some data for avaluation of simulation
 */
void RoutingLayer::handleDataMsgFromLowerLayer(cMessage *msg)//cache
{
    isReceiving=true;
    DataMsg *omnetDataMsg = dynamic_cast<DataMsg*>(msg);
    bool found;

    // increment the travelled hop count
    //omnetDataMsg->setHopsTravelled(omnetDataMsg->getHopsTravelled() + 1);
    omnetDataMsg->setNHops(omnetDataMsg->getNHops() + 1);

    emit(dataBytesReceivedSignal, (long) omnetDataMsg->getByteLength());
    emit(totalBytesReceivedSignal, (long) omnetDataMsg->getByteLength());

    // if destination oriented data sent around and this node is the destination or if maximum hop count is reached then cache or else don't cache
    bool cacheData = TRUE;
    if ((omnetDataMsg->getDestinationOriented()
         && strstr(getParentModule()->getFullName(), omnetDataMsg->getFinalDestinationNodeName()) != NULL)
            | omnetDataMsg->getNHops() >= maximumHopCount) {
        cacheData = FALSE;
    }

    //If i am the Msg final destination, don't store the msg
    bool imDestiny = FALSE;
    if(omnetDataMsg->getDestinationOriented()){
        if(omnetDataMsg->getFinalDestinationNodeName()==ownMACAddress){
            EV<<"Sou a final destination \n";

            //save info into file
            string nameF="Results";
            string noS=ownMACAddress.substr(15,17);
            nameF.append(noS);
            nameF.append(".txt");
            std::ofstream out(nameF, std::ios_base::app);
            //Generation Mac
            string srcMAC=omnetDataMsg->getOriginatorNodeMAC();
            string srcer="Source: ";
            srcer.append(srcMAC);
            out<<srcer;
            //MessageID
            std::string msID=std::to_string(omnetDataMsg->getNMsgOrder());//getMsgUniqueID();
            string msIDis=" | Message ID: ";
            msIDis.append(msID);
            out<<msIDis;
            //Time generated
            std::string timeMsg = std::to_string(omnetDataMsg->getInjectedTime().dbl());
            string timeGen=" | Time generated: ";
            timeGen.append(timeMsg);
            out<<timeGen;
            //time received here
            std::string timeRMsg = std::to_string(omnetDataMsg->getReceivedTime().dbl());
            string timeRec=" | Time received: ";
            timeRec.append(timeRMsg);
            out<<timeRec;
            out<<" |End \n";
            out.close();


            cacheData=FALSE;
            imDestiny=TRUE;
        }
    }



    //Saving Data
    if(cacheData) {
        Stor.saveData(msg,1);
        EV<<"Saving data in cache from lower layer \n";
    }





    //Data Treatment if I'm gateway:
    if(imDestiny){ //GwResults
        if(Stor.msgExistsInC(msg)){
            EV<<"Sava \n";
            //Save Data to file
            //Nº Msg | NodeIndex | time Gen
            string fNam, pri, tim1, ti1;
            fNam="Test: ";
            ostringstream fn, oss, tim2, ti2;
            fn <<fNam << omnetDataMsg->getNMsgOrder();
            fNam=fn.str();
            pri="Node Index: ";
            oss <<pri << nodeIndex;;
            pri=oss.str();
            simtime_t tim3;     //get generation time
            tim3=omnetDataMsg->getInjectedTime();
            tim2 << " Time: "<<tim3;
            tim1=tim2.str();
            //time received
            simtime_t ti3 = simTime().dbl();     //get generation time
            //ti3=omnetDataMsg->getInjectedTime();
            ti2 << " Time rec: "<<ti3;
            ti1=ti2.str();
            //Save Data to file
            fstream fileE;
            //file.open (fNam+".txt");
            fileE.open ("GwResults.txt",ios::app);
            fileE << "\n Running\n";
            fileE << "Num. Msg: "+fNam+" Node Index: " + pri + tim1 + ti1+ "\n";//pri+"\n";
            fileE.close();
        }
    }

    //AckMsg
    AckMsg *ackMsg = new AckMsg();
    string SouceAAdd = omnetDataMsg->getSourceAddress();
    if((SouceAAdd.substr(0,2))=="BT"){
        MyAddAck=ownBTMACAddress;
    }else{
        MyAddAck=ownMACAddress;
    }
    //ackMsg->setSourceAddress(ownMACAddress.c_str());
    ackMsg->setSourceAddress(MyAddAck.c_str());
    ackMsg->setDestinationAddress(omnetDataMsg->getSourceAddress());
    ackMsg->setIsFinalDest(imDestiny);
    ackMsg->setMessageID(omnetDataMsg->getMessageID());
    int realPacketSize = 6 + 6 + (1 * ROUTINGLAYER_MSG_ID_HASH_SIZE) + 1;
    ackMsg->setRealPacketSize(realPacketSize);
    EV<<"Sending ACK \n";
    send(ackMsg, "lowerLayerOut");

    // if registered app exist, send data msg to app
    AppInfo *appInfo = NULL;
    found = FALSE;
    list<AppInfo*>::iterator iteratorRegisteredApps = registeredAppList.begin();
    while (iteratorRegisteredApps != registeredAppList.end()) {
        appInfo = *iteratorRegisteredApps;
        if (strstr(omnetDataMsg->getDataName(), appInfo->prefixName.c_str()) != NULL) {
            found = TRUE;
            break;
        }
        iteratorRegisteredApps++;
    }
    if (found) {
        send(msg, "upperLayerOut");
    } else {
        delete msg;
    }

    isReceiving=false;
    waitS=simTime().dbl()+waitBFsend;
    EV<<"WaitS:"<<waitS<<"\n";;
}


/****************************************************************************************
*AppLayer -> Registers the AppMsg, if it's not already registered, in the list of registeredAppMsgs
****************************************************************************************/
void RoutingLayer::handleAppRegistrationMsg(cMessage *msg) //App
{
    RegisterAppMsg *regAppMsg = dynamic_cast<RegisterAppMsg*>(msg);
    AppInfo *appInfo = NULL;
    int found = FALSE;
    list<AppInfo*>::iterator iteratorRegisteredApps = registeredAppList.begin();
    while (iteratorRegisteredApps != registeredAppList.end()) {
        appInfo = *iteratorRegisteredApps;
        if (appInfo->appName == regAppMsg->getAppName()) {
            found = TRUE;
            break;
        }
        iteratorRegisteredApps++;
    }
    if (!found) {
        appInfo = new AppInfo;
        appInfo->appID = nextAppID++;
        appInfo->appName = regAppMsg->getAppName();
        registeredAppList.push_back(appInfo);
    }
    appInfo->prefixName = regAppMsg->getPrefixName();
    delete msg;
}

/**********************************************************************************************************
 * Cache functions
 **********************************************************************************************************/

/*********************************************************************************************************
 *Verifies if message ID exists in cache
 */
bool RoutingLayer::msgIDexists(string messageID){
    return Stor.msgIDExists(messageID);
}



/************************************************************************************************
 * Gets List of MsgsIDs in cache
 */
void RoutingLayer::returnSelectMsgIDList(vector<string> & selectedMessageIDList){
   selectedMessageIDList=Stor.returnSelectMsgIDList(selectedMessageIDList, maximumHopCount);
}


/***********************************************************************************************
 * Gets the size of the list oh cache Msgs (nº of Msgs in cache)
 */
int RoutingLayer::cacheListSize(){
    return Stor.cacheListSize();
}


/***************************************************************************************
 * Cleans the list of AppRegisteredApps, calls the destructor of the Cache/Storage
 */
//FINISH
void RoutingLayer::finish(){

    recordScalar("numEventsHandled", numEventsHandled);

    // clear resgistered app list
    while (registeredAppList.size() > 0) {
        list<AppInfo*>::iterator iteratorRegisteredApp = registeredAppList.begin();
        AppInfo *appInfo= *iteratorRegisteredApp;
        registeredAppList.remove(appInfo);
        delete appInfo;
    }

    // clear cache list
    Stor.~StorageM();   //Destructor


}
