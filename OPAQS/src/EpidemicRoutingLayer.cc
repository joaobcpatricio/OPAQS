//
// The model implementation for a Forwarding module (eg. Epidemic, spray, etc) Routing layer
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 19-march-2019
//

#include "EpidemicRoutingLayer.h"

Define_Module(EpidemicRoutingLayer);

void EpidemicRoutingLayer::initialize(int stage)
{
    if (stage == 0) {
        Stor=StorageM(); //constructor
        Log=Logger();
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


    } else if (stage == 1) {
        Log.initialize(ownMACAddress);

    } else if (stage == 2) {

        Log.RoutingLogsInit(ownMACAddress);

        // setup statistics signals
        dataBytesReceivedSignal = registerSignal("dataBytesReceived");
        sumVecBytesReceivedSignal = registerSignal("sumVecBytesReceived");
        dataReqBytesReceivedSignal = registerSignal("dataReqBytesReceived");
        totalBytesReceivedSignal = registerSignal("totalBytesReceived");




        //Create File that saves Data
                ofstream file;
                file.open ("GwResults.txt");
                file << "Here is information Obtained in Gw\n";
                file.close();


    } else {
        EV_FATAL << EPIDEMICROUTINGLAYER_SIMMODULEINFO << "Something is radically wrong in initialisation \n";
    }
}

int EpidemicRoutingLayer::numInitStages() const
{
    return 3;
}



void EpidemicRoutingLayer::handleMessage(cMessage *msg)
{

    cGate *gate;
    char gateName[64];

    numEventsHandled++;

    // age the data in the cache only if needed (e.g. a message arrived)
    if (useTTL)
        Stor.ageDataInStorage();

    // self messages
    if (msg->isSelfMessage()) {
        EV_INFO << EPIDEMICROUTINGLAYER_SIMMODULEINFO << "Received unexpected self message" << "\n";
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
            EV<<"Handling DataReqMsg f/link\n";
            handleDataReqMsg(msg);


        // data request message arrived from the neigh layer
        } else if (strstr(gateName, "neighLayerIn") != NULL && dynamic_cast<DataReqMsg*>(msg) != NULL) {
            EV<<"Handling DataReqMsg f/neigh\n";
            handleDataReqMsg(msg);

        // received some unexpected packet
        } else {

            EV_INFO << EPIDEMICROUTINGLAYER_SIMMODULEINFO << "Received unexpected packet" << "\n";
            delete msg;
        }
    }
}

//Added 25/06
//DECISION OF SENDING IS MADE HERE
/*********************************************************************************************************
 * Gets List of Msgs in cache, for each MsgID of the list it searches if msg exists and gets its position,
 * pulls out the dataMsg prepared to send and sends it to lowerLayer
 */
void EpidemicRoutingLayer::handleDataReqMsg(cMessage *msg){

    EV<<"Kepidemic: handleDataReqMsg\n";
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
        while (iteratorMessageIDList != selectedMessageIDList.end()) {
            EV<<"SelectedMessageIDList size here is: "<<selectedMessageIDList.size()<<"\n";
            string messageID = *iteratorMessageIDList;
            bool found = Stor.msgIDExists(messageID);
            int position=Stor.msgIDListPos(messageID);
            if(found){
            //EV<<"OI5 position= "<<position<<"\n";
                //verify NIC:
                string SouceDRAdd = dataRequestMsg->getSourceAddress();
                if((SouceDRAdd.substr(0,2))=="BT"){
                    MyAddH=ownBTMACAddress;
                }else{
                    MyAddH=ownMACAddress;
                }

                //DataMsg *dataMsg = Stor.pullOutMsg(msg,ownMACAddress, position);
                DataMsg *dataMsg = Stor.pullOutMsg(msg,MyAddH, position);

                /*
                //Verify if destination is not on the prevHopList of  the Stored Msg (not used on epidemic)
                int count1=0;
                bool foundH=false;
                int sizeH = dataMsg->getPrevHopsListArraySize();
                string HopAdd=dataMsg->getPrevHopsList(count1);
                string destAdd = dataRequestMsg->getSourceAddress();
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


                EV<<"Sending Data Msg\n";
                if(foundH==false){
                    send(dataMsg, "lowerLayerOut");
                }*/
                if(dataMsg->getFinalDestinationNodeName()!=MyAddH){
                EV<<"Sending Data Msg\n";
                send(dataMsg, "lowerLayerOut");
                }
            }
            iteratorMessageIDList++;
        }
    //} else{ EV<<"Probability too low to send mensage \n"; }
    delete msg;
}


/*********************************************************************************************************
 * Get's beacon, identifies from which NIC it came, creates dataReqMsg and sends to lowerLayer
 */
void EpidemicRoutingLayer::handleBeaconInfo(cMessage *msg){
    EV<<"KEpidemic: handleBeacon\n";
    BeaconInfoMsg *beaconMsg = dynamic_cast<BeaconInfoMsg*>(msg);




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
    int realPacketSize = 6 + 6 + 6 + 4 + 4;//(1 * EPIDEMICROUTINGLAYER_MSG_ID_HASH_SIZE);  //A REVER NO FUTURO
    //EV<<"Real packet size of DataMsg: "<<realPacketSize<<"\n";
    dataRequestMsg->setRealPacketSize(realPacketSize);
    dataRequestMsg->setByteLength(realPacketSize);
    dataRequestMsg->setSSI(beaconMsg->getSSI());
    dataRequestMsg->setProb(beaconMsg->getMyProb());      //VERIFICAR VALOR DE PROB QUE AQUI METO pk é o meu
    dataRequestMsg->setInjectedTime(simTime().dbl());

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
void EpidemicRoutingLayer::handleAckFromLowerLayer(cMessage *msg){
    AckMsg *ackMsg = dynamic_cast<AckMsg*>(msg);
    string messageID = ackMsg->getMessageID();
    bool isFinalDest=ackMsg->getIsFinalDest();

    if(isFinalDest){
        //Stor.deleteMsg(messageID); //PARA JÁ NAO QUERO QUE APAGUE PARA TESTAR SITUAÇÃO ATUAL DE SPREAD
    }
    delete msg;
}

/*********************************************************************************************************
 * Stores dataMsg in cache
 */
void EpidemicRoutingLayer::handleDataMsgFromUpperLayer(cMessage *msg) //Store in cache
{
    DataMsg *upperDataMsg = dynamic_cast<DataMsg*>(msg);
        //upperDataMsg->setFinalDestinationNodeName("Wf:00:00:00:00:ff");

    upperDataMsg->setOriginatorNodeMAC(ownMACAddress.c_str());
    bool var=false;
    Stor.saveData(msg,0,var);







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
void EpidemicRoutingLayer::handleDataMsgFromLowerLayer(cMessage *msg)//cache
{
    DataMsg *omnetDataMsg = dynamic_cast<DataMsg*>(msg);
    bool found;

    // increment the travelled hop count
    //omnetDataMsg->setHopsTravelled(omnetDataMsg->getHopsTravelled() + 1);
    omnetDataMsg->setNHops(omnetDataMsg->getNHops() + 1);

    emit(dataBytesReceivedSignal, (long) omnetDataMsg->getByteLength());
    emit(totalBytesReceivedSignal, (long) omnetDataMsg->getByteLength());

    // if destination oriented data sent around and this node is the destination or if maximum hop count is reached then don't cache or else cache
    bool cacheData = TRUE;
    if ((omnetDataMsg->getDestinationOriented()
         && strstr(getParentModule()->getFullName(), omnetDataMsg->getFinalDestinationNodeName()) != NULL)
        || omnetDataMsg->getNHops() >= maximumHopCount) {
        cacheData = FALSE;
    }

    //If i am the Msg final destination, don't store the msg
    bool imDestiny = FALSE;
    if(omnetDataMsg->getDestinationOriented()){
        if(omnetDataMsg->getFinalDestinationNodeName()==ownMACAddress){
            EV<<"SOU IGUAL \n";
            //cacheData=FALSE;  SAVES ANYWAY
            imDestiny=TRUE;
        }
    }


    EV<<"Msg fin:"<<omnetDataMsg->getFinalDestinationNodeName()<<" my add:"<<ownMACAddress<<"\n";

   //saves data anyway, if he's Gw, deletes it
    //Saving Data
    //Stor.saveData(msg,1);
    if(cacheData) {
        //Stor.deleteMsg(omnetDataMsg->getMessageID());
        bool val=false;
        Stor.saveData(msg,1,val);
        EV<<"Deleting data in cache from lower layer \n";
    }

    EV<<" Je suis: "<<omnetDataMsg->getFinalDestinationNodeName()<< "\n";
    EV<<" Je suis: "<<ownMACAddress<< "\n";

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


            Log.saveMsgReachedGW(omnetDataMsg->getMessageID(), omnetDataMsg->getSentTime().dbl(), ownMACAddress, 0);//omnetDataMsg->getNHops());
            //Log.saveMsgReachedGW(omnetDataMsg->getDataName(), simTime().dbl(), ownMACAddress, 0);

        }
    }

    //Ack Msg
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
    int realPacketSize = 6 + 6 + (1 * EPIDEMICROUTINGLAYER_MSG_ID_HASH_SIZE) + 1;
    ackMsg->setRealPacketSize(realPacketSize);
    ackMsg->setInjectedTime(simTime().dbl());
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
}


/****************************************************************************************
*AppLayer -> Registers the AppMsg, if it's not already registered, in the list of registeredAppMsgs
****************************************************************************************/
void EpidemicRoutingLayer::handleAppRegistrationMsg(cMessage *msg) //App
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
bool EpidemicRoutingLayer::msgIDexists(string messageID){
    return Stor.msgIDExists(messageID);
}

//nunca é chamada
/*void EpidemicRoutingLayer::pullOutMsg(cMessage *msg){
    EV<<"Pulling out requested Msg \n";

    vector<string> selectedMessageIDList;
    returnSelectMsgIDList(selectedMessageIDList);

    vector<string>::iterator iteratorMessageIDList;
    iteratorMessageIDList = selectedMessageIDList.begin();

        while (iteratorMessageIDList != selectedMessageIDList.end()) {
            string messageID = *iteratorMessageIDList;
            bool found = Stor.msgIDExists(messageID);
            EV<<"OI4\n";
            int position=Stor.msgIDListPos(messageID);
            if(found){
                EV<<"OI5 position= "<<position<<"\n";
                        DataMsg *dataMsg = Stor.pullOutMsg(msg,ownMACAddress, position);
                        EV<<"Sending Data Msg\n";
                        send(dataMsg, "lowerLayerOut");
                    }
            iteratorMessageIDList++;
        }
}*/


/************************************************************************************************
 * Gets List of MsgsIDs in cache
 */
void EpidemicRoutingLayer::returnSelectMsgIDList(vector<string> & selectedMessageIDList){
   selectedMessageIDList=Stor.returnSelectMsgIDList(selectedMessageIDList, maximumHopCount);
}

/***********************************************************************************************
 * Gets the size of the list oh cache Msgs (nº of Msgs in cache)
 */
int EpidemicRoutingLayer::cacheListSize(){
    return Stor.cacheListSize();
}

//FINISH
void EpidemicRoutingLayer::finish(){

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
