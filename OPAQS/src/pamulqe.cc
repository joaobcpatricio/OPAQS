//
// The model implementation for a Forwarding module (eg. Epidemic, spray, etc) Routing layer
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 19-march-2019
//

#include "pamulqe.h"

Define_Module(pamulqe);

void pamulqe::initialize(int stage)
{
    if (stage == 0) {
        Log=Logger();
        Stor=StorageM(); //constructor
        graphR=GraphT();
        //Log=Logger();
        // get parameters
        nodeIndex = par("nodeIndex");
        EV<<"Hi, I'm "<<nodeIndex<<"\n";
        ownMACAddress = par("ownMACAddress").stringValue();
        EV<<"Hi, My add "<<ownMACAddress<<"\n";
        ownBTMACAddress = par("ownBTMACAddress").stringValue();
        nextAppID = 1;
        maximumCacheSize = par("maximumCacheSize");
        Stor.maximumCacheS(maximumCacheSize);
        maximumHopCount = par("maximumHopCount");
        useTTL = par("useTTL");
        numEventsHandled = 0;
        inCache=0;
        waitBFsend = par("waitBFsend"); //NOT BEING USED
        max_age = par("max_age");
        gateway_list = par("gateway_list").stringValue();
        actual_gateway = par("actual_gateway").stringValue();
        gwCheckPeriod = par("gwCheckPeriod");
        kill_pcktP = par("kill_pcktP");

    } else if (stage == 1) {
        Stor.updateMaxAge(max_age);
        Stor.updateKillPcktP(kill_pcktP);
        Log.initialize(ownMACAddress);
        setGatewayList();
        printGatewayList();
        updateGateway();

    } else if (stage == 2) {

        // setup statistics signals
        dataBytesReceivedSignal = registerSignal("dataBytesReceived");
        sumVecBytesReceivedSignal = registerSignal("sumVecBytesReceived");
        dataReqBytesReceivedSignal = registerSignal("dataReqBytesReceived");
        totalBytesReceivedSignal = registerSignal("totalBytesReceived");



        // setup next event to check gw and check if there's an initialized gw
        cMessage *checkGW = new cMessage("Send Check Gw Event");
        EV<<"Checking GW status ini \n";
        checkGW->setKind(CHECKGW_EVENT_CODE);
        if(actual_gateway==""){
            no_act_gw=true;
        } //no_act_gw=true;
        scheduleAt(simTime() + gwCheckPeriod, checkGW);

        Log.RoutingLogsInit(ownMACAddress);



    } else {
        EV_FATAL << PAMULQE_SIMMODULEINFO << "Something is radically wrong in initialisation \n";
    }
}

int pamulqe::numInitStages() const
{
    return 3;
}

void pamulqe::handleMessage(cMessage *msg)
{

    cGate *gate;
    char gateName[64];

    numEventsHandled++;

    // age the data in the cache only if needed (e.g. a message arrived)
    if (useTTL)
        Stor.ageDataInStorage();

    // trigger to send pending packet and setup new send
    if (msg->isSelfMessage() && msg->getKind() == CHECKGW_EVENT_CODE){//WIRELESSINTERFACE_NEIGH_EVENT_CODE) {
        checkGwStatus();
        delete msg;
    // self messages
    }else if (msg->isSelfMessage()) {
        EV_INFO << PAMULQE_SIMMODULEINFO << "Received unexpected self message" << "\n";
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

        } else if (strstr(gateName, "lowerLayerIn") != NULL && dynamic_cast<AckMsg*>(msg) != NULL) {
            handleAckFromLowerLayer(msg);

        } else if (strstr(gateName, "neighLayerIn") != NULL && dynamic_cast<BeaconInfoMsg*>(msg) != NULL) {
            EV<<"Handling Beacon\n";
            handleBeaconInfo(msg);

         // data request message arrived from the lower layer (link layer)
        } else if (strstr(gateName, "neighLayerIn") != NULL && dynamic_cast<DataReqMsg*>(msg) != NULL) {
            EV<<"Handling DataReqMsg\n";
            handleDataReqMsg(msg);


        // Graph message arrived from the neighLayer (before lower layer (link layer))
        } else if (strstr(gateName, "neighLayerIn") != NULL && dynamic_cast<GraphUpdtMsg*>(msg) != NULL) {
            EV<<"Handling GraphUpdtMsg\n";
            handleGraphUpdtMsg(msg);

        // Ener table message arrived from the neighLayer (before lower layer (link layer))
        } else if (strstr(gateName, "neighLayerIn") != NULL && dynamic_cast<EnerTableMsg*>(msg) != NULL) {

            EV<<"Handling EnerTableMsg\n";

            handleEnerTableMsg(msg);




        // received some unexpected packet
        } else {

            EV_INFO << PAMULQE_SIMMODULEINFO << "Received unexpected packet" << "\n";
            delete msg;
        }
    }
}


//--HANDLE--------------------------------------------------------------------------------------
void pamulqe::handleGraphUpdtMsg(cMessage *msg){
    EV<<"Routing: handleNetworkGraphMsg\n";
    GraphUpdtMsg *neighGraphMsg = dynamic_cast<GraphUpdtMsg*>(msg);


    if(neighGraphMsg->getNoNeighs()){
        graphR.cleanGraph();
    }else{

        string graphS = neighGraphMsg->getGraph();
        //int numberVert = neighGraphMsg->getNumberVert();
        //int countVert = neighGraphMsg->getCountVert();
        //EV<<"Graph rec: "<<graphS<<"\n";
        bool updt=getGraph(graphS);
        if(updt){ EV<<"Graph Updated\n";}else{EV<<"Graph not updated due to empty string\n";}
    }
    delete msg;
}

//DECISION OF SENDING IS MADE HERE
/*********************************************************************************************************
 * (NOT_If the prob is good enough,) Gets List of Msgs in cache, for each MsgID of the list it searches if msg exists and gets its position,
 * pulls out the dataMsg prepared to send and sends it to lowerLayer if the destination is not on the previous Hops List of the DataMsg
 */
void pamulqe::handleDataReqMsg(cMessage *msg){
    updateGateway();

    if(actual_gateway!=""){ //if it has no gw, no sending decision will be made

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

        if(!isSending && !isReceiving){// && (waitS<=simTime().dbl())){

            int cnt=0;
            while (iteratorMessageIDList != selectedMessageIDList.end()) {  //checks all stored Msgs
               isSending=true;
               //EV<<"SelectedMessageIDList size here is: "<<selectedMessageIDList.size()<<"\n";
               string messageID = *iteratorMessageIDList;
               bool found = Stor.msgIDExists(messageID);
               int position=Stor.msgIDListPos(messageID);
               string SouceDRAdd = dataRequestMsg->getSourceAddress();
               if((SouceDRAdd.substr(0,2))=="BT"){
                   MyAddH=ownBTMACAddress;
               }else{

                   MyAddH=ownMACAddress;
               }
               bool itsOk=false;
               if(found){
                   DataMsg *dataMsg = Stor.pullOutMsg(msg,MyAddH, position);
                   if(dataMsg->getReached_gw()){
                       //EV<<"Reached gateway at"<<MyAddH<<" name:"<<dataMsg->getDataName()<<"\n";
                    itsOk=false;
                   }else{
                       itsOk=true;
                   }
                   delete dataMsg;
               }



               if(itsOk){ //if there is a stored DataMsg
                   //verify NIC:
                   DataMsg *dataMsg = Stor.pullOutMsg(msg,MyAddH, position);
                   //Update GW
                   //updateGateway();

                   dataMsg->setFinalDestinationNodeName(actual_gateway.c_str());

                   EV<<"pulled to rout Msg:"<<dataMsg->getNMsgOrder()<<" at time "<<simTime().dbl()<<"\n";
                   string destAdd = dataRequestMsg->getSourceAddress();
                   string gwAdd = dataMsg->getFinalDestinationNodeName();


                   //Save Data

                   if(cnt<=1){
                       //EV<<"gwAdd"<<gwAdd<<"\n";

                       //save LOGS info into file
                       string nMY=MyAddH.substr(15,17);
                       string nGW=gwAdd.substr(15,17);
                       //My Short Path
                       int myID=graphR.add_element(MyAddH);
                       int gwID=graphR.add_element(gwAdd);
                       string shrtPath=graphR.returnShortestPath(myID,gwID);
                       //Graph
                       string GraphSR=graphR.returnGraphT();
                       Log.saveResultsLQE(nMY, nGW, shrtPath, GraphSR);
                       cnt++;
                   }



                   //Loop Avoidance
                   int count1=0;
                   bool foundH=false, msgSent=false;
                   int sizeH = dataMsg->getPrevHopsListArraySize();
                   string HopAdd=dataMsg->getPrevHopsList(count1);
                   //EV<<"Pos1: "<<dataMsg->getPrevHopsList(0)<<" source: "<<dataRequestMsg->getSourceAddress()<<"\n";
                   while(count1<sizeH){
                       HopAdd=dataMsg->getPrevHopsList(count1);
                       if(HopAdd==destAdd){
                           foundH=true;
                           //EV<<"Loop Avoidance: Found it "<<dataMsg->getDataName() <<"\n";
                           break;
                       }
                       count1++;
                   }

                   //Anul loop avoidance
                   foundH=false;


                   //Verifies if DataMsg destination is this neighbor and DataMsg has not been send yet, if so, send directly with Loop Avoidance
                   if(dataMsg->getFinalDestinationNodeName()==destAdd && foundH==false){
                       int myID=graphR.add_element(MyAddH);
                       int dstID=graphR.add_element(destAdd);
                       int weightH=graphR.returnWGrapfT(myID, dstID);

                       //verifica se o vizinho direto está no grafo
                       if((weightH>0)){



                       //EV<<"Direct Neigh is final dest. \n";
                       EV<<"Sent Direct Msg from Rout:"<<dataMsg->getNMsgOrder()<<" at time:"<<simTime().dbl()<<"\n";

                       //If this is my generated data Msg i set the time I sent it from here
                       if(dataMsg->getOriginatorNodeMAC()==ownMACAddress || dataMsg->getOriginatorNodeMAC()==ownBTMACAddress){
                           dataMsg->setSentTimeRout(simTime().dbl());
                       }


                       send(dataMsg, "lowerLayerOut");
                       msgSent = true;
                    //break;
                       }
                   }else{

                       int myID=graphR.add_element(MyAddH);
                       int gwID=graphR.add_element(gwAdd);
                       int dstID=graphR.add_element(destAdd);
                       bool isInShortPath=false;
                       EV<<"Dijkstra from myID to gwID\n";
                       graphR.dijkstra(myID, gwID);
                       //string sPat=graphR.returnShortestPath;







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
                                dataMsg->setSentTimeRout(simTime().dbl());
                                EV<<"Sent Msg from Rout:"<<dataMsg->getNMsgOrder()<<" at time:"<<simTime().dbl()<<"\n";
                                send(dataMsg, "lowerLayerOut");
                            }
                        }
                       }else{EV<<"Ups, shouldn't end up here \n"; }
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
}

/*********************************************************************************************************
 * Get's beacon, identifies from which NIC it came, creates dataReqMsg and sends to lowerLayer
 */
void pamulqe::handleBeaconInfo(cMessage *msg){
    updateGateway();

    EV<<"Routing: handleBeacon\n";
    BeaconInfoMsg *beaconMsg = dynamic_cast<BeaconInfoMsg*>(msg);


    //Save Graph Matrix on pamulqe
    string myGraph=beaconMsg->getNeighGraph();
    //int npos=beaconMsg->getNumberVert();
    //EV<<"Graph in routing: \n";
    getGraph(myGraph);

    //Log.saveEnerTable(ownMACAddress, returnEnerTable());






    DataReqMsg *dataRequestMsg = new DataReqMsg();
//changed 23/07/19 17h54
    string SouceAdd = beaconMsg->getSourceAddress();
    if((SouceAdd.substr(0,2))=="BT"){
        //EV<<"It's BT \n";
        //dataRequestMsg->setNic(1);//APAGAR
        dataRequestMsg->setSourceAddress(ownBTMACAddress.c_str());
    //EV<<"SouceAdd= "<<SouceAdd.substr(0,2)<<"\n";

    }else{
        //dataRequestMsg->setNic(0);//APAGAR
        dataRequestMsg->setSourceAddress(ownMACAddress.c_str());
    }
    dataRequestMsg->setDestinationAddress(beaconMsg->getSourceAddress());
    int realPacketSize = 6 + 6 + 6 + 4 + 4;//(1 * PAMULQE_MSG_ID_HASH_SIZE);  //A REVER NO FUTURO
    //EV<<"Real packet size of DataMsg: "<<realPacketSize<<"\n";
    dataRequestMsg->setRealPacketSize(realPacketSize);
    dataRequestMsg->setByteLength(realPacketSize);
    dataRequestMsg->setSSI(beaconMsg->getSSI());
    dataRequestMsg->setProb(beaconMsg->getMyProb());      //VERIFICAR VALOR DE PROB QUE AQUI METO pk é o meu
    dataRequestMsg->setInjectedTime(simTime().dbl());
    dataRequestMsg->setBeaconSentT(beaconMsg->getSentTime().dbl());

    //Para retirar futuramente
    if(beaconMsg->getProb()>0.5){
        //EV<<"Prob bigger than 0.5\n ";
        dataRequestMsg->setSendMeData(true);
    } else{ dataRequestMsg->setSendMeData(false);}

    //EV<<"Send DataRequestMsg \n";
    send(dataRequestMsg, "lowerLayerOut");
    delete msg;
}

/********************************************************************************
 *Get's Ack, checks if the other was the final Destiny.
 */
void pamulqe::handleAckFromLowerLayer(cMessage *msg){
    updateGateway();
    AckMsg *ackMsg = dynamic_cast<AckMsg*>(msg);
    string messageID = ackMsg->getMessageID();
    bool isFinalDest=ackMsg->getIsFinalDest();
    EV<<"Received ack for sent "<<ackMsg->getMessageID()<<"\n";

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
    //EV<<"Here deletes:"<<messageID<<"\n";
    Stor.deleteMsg(messageID);


    delete msg;
}

void pamulqe::handleDataMsgFromUpperLayer(cMessage *msg) //Store in cache
{
    updateGateway();
    DataMsg *upperDataMsg = dynamic_cast<DataMsg*>(msg);
    upperDataMsg->setFinalDestinationNodeName(actual_gateway.c_str());
    upperDataMsg->setOriginatorNodeMAC(ownMACAddress.c_str());

    bool reached_gwH=false;
    Stor.saveData(msg,0, reached_gwH);


        //Save Data into Log
        //Graph
        string dataN=upperDataMsg->getDataName();
        //Time
        std::string timeMsg = std::to_string(upperDataMsg->getInjectedTime().dbl());//getInjectedTime().dbl());
        Log.saveGenDat(ownMACAddress, dataN, timeMsg);
        string graf=graphR.returnGraphT();
        Log.saveGraphHere(ownMACAddress, graf);
        Log.saveEnerTable(ownMACAddress, returnEnerTable());


       // EV<<"actGw:"<<actual_gateway<<" me:"<<ownMACAddress<<"\n";
        if(actual_gateway==ownMACAddress){ //If I am the Gw, I delete this Msg from storage
            //EV<<"Here from up deletes:"<<upperDataMsg->getDataName()<<"\n";
            bool delet = Stor.deleteMsg(upperDataMsg->getDataName());
            EV<<"Deleting my msg cause I'm GW "<<upperDataMsg->getDataName()<<" \n";

            if(delet){
               // EV<<"Delete:"<<upperDataMsg->getDataName()<<"\n";
                Log.saveMsgReachedGW(upperDataMsg->getDataName(), simTime().dbl(), ownMACAddress, 0);
                pcktSentMsg(upperDataMsg->getRealPacketSize(), true);
            }
        }
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
void pamulqe::handleDataMsgFromLowerLayer(cMessage *msg)//cache
{
    isReceiving=true;
    DataMsg *omnetDataMsg = dynamic_cast<DataMsg*>(msg);

    //Set updated GW address
    updateGateway();
    omnetDataMsg->setFinalDestinationNodeName(actual_gateway.c_str());

    //EV<<"Rout: received msg:"<<omnetDataMsg->getNMsgOrder()<<" at time:"<<simTime().dbl()<<"\n";
    bool found;
    omnetDataMsg->setReceivedTimeRout(simTime().dbl());

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
            //EV<<"Sou a final destination \n";


            //save info into LOG file
            //Data Name
            string srcMAC=omnetDataMsg->getDataName();
            //MessageID
            std::string msID=std::to_string(omnetDataMsg->getNMsgOrder());//getMsgUniqueID();
            //Time sent from src
            std::string timeSMsg = std::to_string(omnetDataMsg->getSentTimeRout().dbl());//getInjectedTime().dbl());
            //time received here
            std::string timeRMsg = std::to_string(omnetDataMsg->getReceivedTimeRout().dbl());//getReceivedTime().dbl());
            Log.saveResultsGw(ownMACAddress, srcMAC,  msID, timeSMsg, timeRMsg);

            //cacheData=FALSE;
            imDestiny=TRUE;
        }
    }


    bool stored=false;
    //Saving Data
    if(actual_gateway==ownMACAddress) {
       // EV<<"Saving data in cache from lower layer, I'm Gw \n";
        stored=Stor.saveData(msg,1, true);
        pcktSentMsg(omnetDataMsg->getRealPacketSize(), true);
    }else{
        //EV<<"Saving data in cache from lower layer, I'm not Gw \n";
        stored=Stor.saveData(msg,1,false);
    }





    //Data Treatment if I'm gateway:
    if(imDestiny){ //GwResults
        if(Stor.msgExistsInC(msg)){

            //guarda aqui nº pacotes recebidos que ja tinha recebido antes e guardado.
            //EV<<"Sava \n";
        }

        //bool delt=false;
        /*if(actual_gateway==ownMACAddress){  //If I'm GW, delete Msg
            //delt=Stor.deleteMsg(omnetDataMsg->getDataName());
        //}

            if(stored){

                string MsgId=omnetDataMsg->getDataName();

                //Log.saveMsgReachedGW(MsgId, omnetDataMsg->getSentTimeRout().dbl());

            }
        }*/
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
    int realPacketSize = 6 + 6 + (1 * PAMULQE_MSG_ID_HASH_SIZE) + 1;
    ackMsg->setRealPacketSize(realPacketSize);
    ackMsg->setInjectedTime(simTime().dbl());
    //EV<<"Sending ACK \n";
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
    waitS=simTime().dbl()+waitBFsend;   //NOT BEING USED
    //EV<<"WaitS:"<<waitS<<"\n";;
}

/****************************************************************************************
*AppLayer -> Registers the AppMsg, if it's not already registered, in the list of registeredAppMsgs
****************************************************************************************/
void pamulqe::handleAppRegistrationMsg(cMessage *msg) //App
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


//--CACHE------------------------------------------------------------------------------
void pamulqe::checkStoredMsgs(){   //deletes stored Msgs if I'm the gw
    EV<<"checkStoredMsgs \n";
    //Goes through the cache list to send the stored Msgs
    vector<string> selectedMessageIDList;
    returnSelectMsgIDList(selectedMessageIDList);
    vector<string>::iterator iteratorMessageIDList;
    iteratorMessageIDList = selectedMessageIDList.begin();
    inCache = selectedMessageIDList.size();
    int i=0;

    DataReqMsg *dataRequestMsg = new DataReqMsg();
    dataRequestMsg->setSourceAddress(ownMACAddress.c_str());

    if(!isSending && !isReceiving){// && (waitS<=simTime().dbl())){
        int cnt=0;
        while (iteratorMessageIDList != selectedMessageIDList.end()) {  //checks all stored Msgs
            isSending=true;
            //EV<<"SelectedMessageIDList size here is: "<<selectedMessageIDList.size()<<"\n";
            string messageID = *iteratorMessageIDList;
            bool found = Stor.msgIDExists(messageID);
            int position=Stor.msgIDListPos(messageID);


            bool itsOk=false;
            if(found){
                DataMsg *dataMsg = Stor.pullOutMsg(dataRequestMsg,MyAddH, position);
                if(dataMsg->getReached_gw()){
                    //EV<<"Reached gateway at"<<MyAddH<<" name:"<<dataMsg->getDataName()<<"\n";
                    itsOk=false;
                }else{
                    itsOk=true;
                }
                delete dataMsg;
            }



            if(itsOk){//itsOk){ //if there is a stored DataMsg
                DataMsg *dataMsg = Stor.pullOutMsg(dataRequestMsg,MyAddH, position);
                //EV<<"pull out on check \n";
                //if(dataMsg->getReached_gw()){EV<<"Break on check stored msgs \n";break;}
                double time_f_sent=dataMsg->getSentTimeRout().dbl();

                if(actual_gateway==ownMACAddress){ //If I am the Gw, I delete this Msg from storage
                    //EV<<"Here from up deletes:"<<upperDataMsg->getDataName()<<"\n";
                    bool delt=Stor.deleteMsg(messageID);
                    //EV<<"Deleting cause I'm GW \n";

                    if(delt){
                        Log.saveMsgReachedGW(messageID, time_f_sent, ownMACAddress, dataMsg->getNHops());
                        //EV<<"Noted \n";
                    }
                }
                delete dataMsg;
            }
            iteratorMessageIDList++;
        }
    }
    delete dataRequestMsg;
}

/**********************************************************************************************************
 * Cache functions
 **********************************************************************************************************/

/*********************************************************************************************************
 *Verifies if message ID exists in cache
 */
bool pamulqe::msgIDexists(string messageID){
    return Stor.msgIDExists(messageID);
}


/************************************************************************************************
 * Gets List of MsgsIDs in cache
 */
void pamulqe::returnSelectMsgIDList(vector<string> & selectedMessageIDList){
   selectedMessageIDList=Stor.returnSelectMsgIDList(selectedMessageIDList, maximumHopCount);
}


/***********************************************************************************************
 * Gets the size of the list oh cache Msgs (nº of Msgs in cache)
 */
int pamulqe::cacheListSize(){
    return Stor.cacheListSize();
}

//--GRAPH---------------------------------------------------------------------------
//Saves the received graph from neighboring here for later use in decision;
bool pamulqe::getGraph(string graphS){//, int numberVert){ //String:" 1->2:4;\n2->1:4;\n "


    graphR.cleanGraph();

    std::string delimiter = ";";

    int i=0;//, q1=0;
    for(i=0;i<graphS.length();i++){
        int j=graphS.find(delimiter,i);
        if(j==std::string::npos){
            return false;
        }else{
            std::string token = graphS.substr(i, j-i);
            //EV<<"Got the: "<<token<<" at: "<<j<<".\n";
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
    return true;
}

//--ENER table
void pamulqe::pcktSentMsg(double size_p, bool from_GW){    //to be used when simulating sending through
    PcktIsSentMsg *sentMsg = new PcktIsSentMsg();
    sentMsg->setOwnAddr(ownMACAddress.c_str());
    sentMsg->setBit_size(size_p);
    sentMsg->setTo_Gw(from_GW);
    sentMsg->setDistance(dist_to_gw);
    sentMsg->setSentTime(simTime().dbl());
    sentMsg->setIs_sent(true);

    send(sentMsg,"neighLayerOut");
}

void pamulqe::cleanEnerTable(){    //cleans table exept my value
      int myID=graphR.add_element(ownMACAddress.c_str());
      int my_Ener=Ener[myID];
      //int count=0;
      for(int i = 0; i < N_nodes; i++) {
          Ener[i]=-1;
          //EV<<"Removed:"<<i<<"\n";
      }
      Ener[myID]=my_Ener;
}

void pamulqe::handleEnerTableMsg(cMessage *msg){
    EnerTableMsg *EnerTableRec = dynamic_cast<EnerTableMsg*>(msg);
    string EnerS=EnerTableRec->getTable();
    updateEnerTable(EnerS);
    delete msg;
}

bool pamulqe::updateEnerTable(string tabS){
    cleanEnerTable();

    std::string delimiter = ";";
    int i=0;//, q1=0;
    for(i=0;i<tabS.length();i++){
        int j=tabS.find(delimiter,i);
        if(j==std::string::npos){
            EV<<"false \n";
            return false;
        }else{
           // EV<<"Here I am \n";
            std::string token = tabS.substr(i, j-i);
            int q1 = tabS.find("-",i);
            int q2 = tabS.find(";",i);
            string v=tabS.substr(i,q1-i);
            string w=tabS.substr(q1+2,q2-(q1+2));
            int vert = std::stoi (v);
            int weight = std::stoi (w);
            //EV<<"WEI: "<<w<<"\n";
            Ener[vert]=weight;
            i =j+1;
            //EV<<"i="<<i<<"j="<<j<<"q1:"<<q1<<"q2:"<<q2<<"v:"<<v<<"w:"<<w<<"\n";
        }
    }
    return true;
}

string pamulqe::returnEnerTable(){
    //Returns the table on a string
    std::string tableS;
    int k,l;
    for(k = 0; k < N_nodes; k++) {
        if(Ener[k]!=0 && Ener[k]!=(-1)){
            tableS=tableS+std::to_string(k)+"->"+std::to_string(Ener[k])+";\n";
        }
    }
    //EV<<"Table Ener: \n"<<tableS<<"\n";
    return tableS;
}

//--GATEWAY----------------------------------------------------------------------
//gw list
bool pamulqe::setGatewayList(){

    GatewayN *gatewayN = NULL;
    std::string delimiter = "|";
    int i=0;//, q1=0;
    for(i=0;i<gateway_list.length();i++){
        int j=gateway_list.find(delimiter,i);
        if(j==std::string::npos){
            return false;
        }else{
            int q1 = gateway_list.find(";",i);
            string GWis=gateway_list.substr(i,q1-i);
            int q2 = gateway_list.find("|",i);
            string timeIs=gateway_list.substr(q1+1,q2-(q1+1));
            //EV<<"Gw is: "<<GWis<<" until time: "<<timeIs<<"\n";
            //

            gatewayN = new GatewayN;
            gatewayN->nodeMACAddress = GWis;
            gatewayN->untilTime = std::stod(timeIs);
            GatewayList.push_back(gatewayN);
            i =j;
        }
    }
    //printGatewayList();
    return true;
}

void pamulqe::printGatewayList(){
    //EV<<"Print here \n";
    GatewayN *gatewayN = NULL;
    auto itC= GatewayList.begin();
    //EV<<"Print gw "<<GatewayList.size() <<" \n";
    while (itC != GatewayList.end()) {
        gatewayN = *itC;
        EV<<"Gateway is "<<gatewayN->nodeMACAddress<<" at "<<gatewayN->untilTime<<" s \n";
          itC++;
      }
}

void pamulqe::updateGateway(){
    GatewayN *gatewayN = NULL;
    string currentGW;
    double deadT;
    bool hereFirst=true;
    bool foreverGW=true;
    auto itC= GatewayList.begin();
    //EV<<"Print gw "<<GatewayList.size() <<" \n";
   double timeG;
    while (itC != GatewayList.end()) {
        gatewayN = *itC;
        timeG=gatewayN->untilTime;
        if(timeG >simTime().dbl() && hereFirst){
            currentGW=gatewayN->nodeMACAddress;
            deadT=gatewayN->untilTime;
            hereFirst=false;
            foreverGW=false;

        }else if(timeG >simTime().dbl() && (timeG<deadT)){
            currentGW=gatewayN->nodeMACAddress;
            deadT=gatewayN->untilTime;
            foreverGW=false;
        }else{ if(timeG==0 && foreverGW){
            currentGW=gatewayN->nodeMACAddress;
            deadT=gatewayN->untilTime;
            //EV<<"Reached last GW set \n";
        }

        }
        itC++;
        //foreverGW=true;
    }

    if(currentGW!=""){
        actual_gateway=currentGW;
        //EV<<"Current Gateway is "<<actual_gateway<<" until "<<deadT<<" s \n";
    }else{ EV<<"No update from the GW list \n"; }
}
//GW CALCULATION---
void pamulqe::checkGwStatus(){
    EV<<"checkGwStatus \n";

    bool is=false;
if(is){

/*    if(!no_act_gw){
        oldGwID=graphR.add_element(actual_gateway);
        //check if there's path to gw
        if(graphR.returnShortestPath(graphR.add_element(ownMACAddress), oldGwID)==""){
            EV<<"No path to gw \n";
            oldGwRank=0;
            oldGwID=-1;
        /*}else{
            oldGwRank=bestGwRank;
        *//*}
    }

*/


    //calculate Gw rank
/*    int nVert=graphR.returnVvalue();
    //EV<<"Já contei:"<<nVert<<"\n";
    double gwMat[nVert][2];
    for(int g=0;g<nVert;g++){
        gwMat[g][0]=-1;
        gwMat[g][1]=-1;
    }*/


    /**Check if I have no neighbor. If so, check again 5 times with a period of gwCheckPeriod (5s) before electing myself GW*********/
    //verifies if it has any direct neigh
    if(graphR.returnGraphT()==""){
        im_alone++;
        if(im_alone>=5){
            im_alone=0;
            actual_gateway=ownMACAddress;
            no_act_gw=false;
            //checkStoredMsgs();
        }
        cMessage *checkGW = new cMessage("Send Check Gw Event");
        //EV<<"Checking GW status \n";
        checkGW->setKind(CHECKGW_EVENT_CODE);
        scheduleAt(simTime() + gwCheckPeriod, checkGW);
        EV<<"Set check in 5s 1st\n";

    }else{//if it has direct neigh then

        //int bestGwId=0;
        /*Calculate the best GW to be elected*****************/
        //bestGwId=returnElectedGwM3();
/*METHOD 3**********************************************************************************/
        //calculate Gw rank
            int nVert=graphR.returnVvalue();
            //EV<<"Já contei:"<<nVert<<"\n";
            double gwMat[nVert][2];
            for(int g=0;g<nVert;g++){
                gwMat[g][0]=-0;//1;
                gwMat[g][1]=0;//-1;
            }

            //Evaluation table- saves rank and centrality
            int n_connect=0;
            double central=0, imN_path=0, n_path=0, sumLqe=0, nSum=0, wei=0;
            for(int i=0;i<nVert;i++){
                if(graphR.returnShortestPath(graphR.add_element(ownMACAddress),i)!=""){
                    //Centrality
                    std::pair<int, int> result=geodisikV(i);
                    EV<<"For->"<<i<<" nPaths:"<<result.first<<" ImNpaths:"<<result.second<<"\n";
                    n_path=result.first;
                    imN_path=result.second;
                    central=imN_path/n_path;
                    sumLqe=0, nSum=0, wei=0;
                    for(int j=0;j<nVert;j++){
                        wei = graphR.returnWGrapfT(i,j);
                        if(wei>0){
                            sumLqe=sumLqe+(100-wei);
                            nSum++;
                        }
                    }
                    //central=nSum/nVert;
                    double ener_spent=Ener[i];
                    if(nSum!=0){
                        //gwMat[i][0]=teta*sumLqe*nSum+alfa*ener_spent;   //RANK EQUATION
                        gwMat[i][0]=(1-central)*sumLqe/nSum+central*ener_spent;   //RANK EQUATION
                        //EV<<"Rank is:"<<gwMat[i][0]<<"central:"<<central<<"lqe:"<<sumLqe/nSum<<"ener:"<<ener_spent<<"\n";
                        EV<<"Rank is:"<<gwMat[i][0]<<" central "<<central<<" i: "<<i<<" lqe:"<<sumLqe/nSum<<"ener:"<<ener_spent<<"\n";
                        Log.saveGwRankT(ownMACAddress,i, gwMat[i][0], ener_spent);

                    }else{
                        gwMat[i][0]=0;
                    }
                    gwMat[i][1]=central;//nSum;   //nº somas=no neighs
                    n_connect=n_connect+nSum;
                }
            }
            n_connect=n_connect/2;
            EV<<"Conec:"<<n_connect<<"\n";
            Log.saveGwRankT_time(ownMACAddress);

            /**Methods that chose GW to be elected from the node's rank************************************************************************/
            //--METHOD 2 ----------------------------------

            //compare by the rank -- RANK ALGORITHM ON CENTRALITY AND ENERGY - best rank value----------

            double Rl=0; int bestGwId=-1;
            for(int uR=0;uR<nVert;uR++){
                if(0.99*gwMat[uR][0]>Rl){
                    EV<<"bestr:"<<gwMat[uR][0]<<"with id:"<<uR<<" prevR:"<<Rl<<"with id:"<<bestGwId<<"\n";
                    Rl=gwMat[uR][0];
                    bestGwId=uR;
                }else if(gwMat[uR][0]==Rl){  //if same rank lvl, check who's got more neighs
                    if(gwMat[uR][1]>gwMat[bestGwId][1]){
                        EV<<"==>bestr:"<<gwMat[uR][0]<<" prevR:"<<Rl<<"\n";
                        Rl=gwMat[uR][0];
                        bestGwId=uR;
                    }
                }
            }

            if(gwMat[oldGwID][0]<0){
                oldGwRank=0;
            }else{
                oldGwRank=gwMat[oldGwID][0];
            }
            //oldGwRank=gwMat[oldGwID][0];
            bestGwRank=Rl;


/*END METHOD3********************************************************************************/
    //EV<<"Chosen GW is "<<bestGwId<<" with "<<nN<<"neighs \n";
    int IDadd;
    if(actual_gateway!=""){
        IDadd=std::stoi( actual_gateway.substr(15,17));
    }else{
        IDadd=-1;
    }

    EV<<"best gw is:"<<bestGwId<<"\n";

    //insert here method that compares elected GW with current GW parameters and compare within a range if a new GW should be elected
    /*Control 1*******+ S/Control ativo*/
    /*double oldR=0, newR=0;
    if(bestGwId!=IDadd){
        oldR=gwMat[IDadd][0];
        newR=gwMat[bestGwId][0];
        EV<<"oldR:"<<oldR<<" newR"<<newR<<"\n";
        //if(old_rank>0 && 0.9*newR<oldR){
        if(oldR>0 && 0.9*newR<oldR){
            bestGwId=IDadd;
        }
    }*/


    /**CONTROL********************************/
    if(!no_act_gw && bestGwId!=IDadd && bestGwId!=(-1)){

    /*S/control****/
    if(bestGwId!=IDadd){
        //EV<<"ACTUAL GW IS different than best ranked \n";
        if(bestGwId!=(-1)){
            string addDf="Wf:00:00:00:00:";
            if(bestGwId<10){
                addDf.append("0");
                addDf.append(std::to_string(bestGwId));
            }else if(bestGwId>=10){
                addDf.append(std::to_string(bestGwId));
            }
            //Log.saveGwRank(bestGwId, gwMat[bestGwId][0], IDadd, gwMat[IDadd][0]);
            EV<<"Updated Gw \n";
                               actual_gateway=addDf;
                               no_act_gw=false;
                               old_rank=gwMat[bestGwId][0];
        }
    }
    // setup next event to check gw
                    cMessage *checkGW = new cMessage("Send Check Gw Event");
                    //EV<<"Checking GW status \n";
                    checkGW->setKind(CHECKGW_EVENT_CODE);
                    scheduleAt(simTime() + gwCheckPeriod, checkGW);
                    EV<<"Set check in 5s \n";

    /*End S/Control*/

    //if GW elected now is different than the current GW, check it 3 more times on periods of 500ms

    /*Control 2**************/
    /*if(bestGwId!=IDadd){
        //EV<<"ACTUAL GW IS different than best ranked \n";
        if(bestGwId!=(-1)){
            if(elect_gw==bestGwId){
                count_newElect_Gw++;
            }else{
                count_newElect_Gw=0;
            }
            if(count_newElect_Gw>1){

                string addDf="Wf:00:00:00:00:";
                if(bestGwId<10){
                    addDf.append("0");
                    addDf.append(std::to_string(bestGwId));
                }else if(bestGwId>=10){
                    addDf.append(std::to_string(bestGwId));
                }
                actual_gateway=addDf;
                old_rank=gwMat[bestGwId][0];
            }else{
                elect_gw=bestGwId;
                // setup next event to confirm no check gw
                cMessage *checkGW = new cMessage("Send Check Gw Event");
                //EV<<"Checking GW status- no gw chosen \n";
                //EV<<"Temp gw id:"<<temp_gw<<"\n";
                checkGW->setKind(CHECKGW_EVENT_CODE);
            scheduleAt(simTime() + 1, checkGW);
            }
        }
    }*/

    /*Control 3**************/

    //if(!no_act_gw && bestGwId!=IDadd && bestGwId!=(-1)){
        /*double oldR=0, newR=0;
        //if(bestGwId!=IDadd){
        EV<<"elec:"<<elect_gw<<"\n";
            EV<<"ACTUAL GW IS different than best ranked \n";
            //if(bestGwId!=(-1)){
            if(elect_gw==bestGwId){
                EV<<"counting \n";
                count_newElect_Gw++;
            }else{
                EV<<"count 0 \n";
                count_newElect_Gw=0;
            }
            EV<<"count is:"<<count_newElect_Gw<<"\n";
            if(count_newElect_Gw>3){
                oldR=gwMat[IDadd][0];
                newR=gwMat[bestGwId][0];
                EV<<"oldR:"<<oldR<<" newR"<<newR<<"\n";
                //if(old_rank>0 && 0.9*newR<oldR){
                if(oldR>=0 && 0.9*newR>oldR){
                    string addDf="Wf:00:00:00:00:";
                    if(bestGwId<10){
                        addDf.append("0");
                        addDf.append(std::to_string(bestGwId));
                    }else if(bestGwId>=10){
                        addDf.append(std::to_string(bestGwId));
                    }
                    EV<<"Updated Gw \n";
                    actual_gateway=addDf;
                    no_act_gw=false;
                    Log.saveGwRank(bestGwId, gwMat[bestGwId][0], IDadd, oldR);
                    old_rank=gwMat[bestGwId][0];
                }
                // setup next event to check gw
                cMessage *checkGW = new cMessage("Send Check Gw Event");
                //EV<<"Checking GW status \n";
                checkGW->setKind(CHECKGW_EVENT_CODE);
                scheduleAt(simTime() + gwCheckPeriod, checkGW);
                EV<<"Set check in 5s \n";
            }else{
                elect_gw=bestGwId;
                // setup next event to confirm no check gw
                cMessage *checkGW = new cMessage("Send Check Gw Event");
                //EV<<"Checking GW status- no gw chosen \n";
                //EV<<"Temp gw id:"<<temp_gw<<"\n";
                checkGW->setKind(CHECKGW_EVENT_CODE);
                scheduleAt(simTime() + 5, checkGW); //Bastante tempo para verificar se nao é um no de passagem a efetuar alteraçao da gw
                EV<<"Set check in 5s \n";
            }*/
    }else


    /**If there's no gw check 3 times (period 500ms), if it still choses the same GW, then it elects it ************/

        if(no_act_gw){  //if there's no gw gonna check 3 times (period 500ms) if i chose the same gw
              string actual_gateway_temp;
              if(temp_gw==bestGwId){
                count_newGw_check++;
              }else{
                count_newGw_check=0;
              }
              if(count_newGw_check>2){
                count_newGw_check=0;
                string addF="Wf:00:00:00:00:";
                if(bestGwId<10){
                    addF.append("0");
                    addF.append(std::to_string(bestGwId));
                }else if(bestGwId>=10){
                    addF.append(std::to_string(bestGwId));
                }
                actual_gateway_temp=addF;
                EV<<"Actual gw temp:"<<actual_gateway_temp<<"\n";
                actual_gateway=addF;//"Wf:00:00:00:00:02";
                no_act_gw=false;
              }
              temp_gw=bestGwId;
              // setup next event to confirm no check gw
              cMessage *checkGW = new cMessage("Send Check Gw Event");
              //EV<<"Checking GW status- no gw chosen \n";
              //EV<<"Temp gw id:"<<temp_gw<<"\n";
              checkGW->setKind(CHECKGW_EVENT_CODE);
            scheduleAt(simTime() + 0.5, checkGW);
            EV<<"Set check in 0.5s \n";
        }else{


            // setup next event to check gw
            cMessage *checkGW = new cMessage("Send Check Gw Event");
            //EV<<"Checking GW status \n";
            checkGW->setKind(CHECKGW_EVENT_CODE);
            scheduleAt(simTime() + gwCheckPeriod, checkGW);
            EV<<"Set check in 5s \n";
        }
    }

    EV<<"Saving Gw current \n";
    Log.saveResultsGwChk(ownMACAddress, actual_gateway);
    if(actual_gateway==ownMACAddress){
        checkStoredMsgs(); //if i'm gw save msgs stored
    }
}
    actual_gateway="Wf:00:00:00:00:02";
}

//--RANK GW---
/*If node is within my graph, compare node with another (with higher id to not repeat myself),
 *  if there's a shrtpath count it, check if the nodeID is within and count it too****/
std::pair<int, int> pamulqe::geodisikV(int nodeID){
//double pamulqe::geodisikV(int nodeID){
    EV<<"GeodisikV \n";
    //std::pair<int, int> r(std::string fn)
    int myID=graphR.add_element(ownMACAddress);

    if(graphR.returnShortestPath(myID,nodeID)==""){
        return std::make_pair(0, 0);
    }

    int n_paths=0, imN_paths=0;
    string spath;
    //EV<<"testing:"<<graphR.returnVvalue()<<"\n";
    int max=graphR.returnVvalue();
    for(int a=0; a<max; a++){
        for(int b=0; b<max; b++){
            //EV<<"a:"<<a<<"b:"<<b<<"\n";
            if(b>a){
                spath = graphR.returnShortestPath(a,b);
                if(spath!=""){
                    //EV<<"sspath is:"<<spath<<"\n";
                    n_paths++;
                    EV<<"ShrtP s o:"<<spath<< "with length "<<spath.length()<<"\n";
                    int posi=0;
                    for(int st=0; st<spath.length();st++){
                        //EV<<"st:"<<spath[1]<<"\n";
                        posi= spath.find(">",st);
                        //EV<<"p1:"<<posi<<"\n";
                        int vID = std::stoi (spath.substr(st,posi-1-st));
                        //EV<<"Me:"<<a<<"him"<<vID<<"\n";
                        if(vID==nodeID){
                            EV<<"I'm in path \n";
                            imN_paths++;
                        }
                        st=posi;
                    }
                }
            }
        }
    }
    //n_paths=1; imN_paths=2;
    EV<<"n_paths:"<<n_paths<<" imN_paths"<<imN_paths<<"\n";
    return std::make_pair(n_paths, imN_paths);
}

//----------------------------------------------------------------------------------------

int pamulqe::returnElectedGwM1(){

    //calculate Gw rank
    int nVert=graphR.returnVvalue();
    //EV<<"Já contei:"<<nVert<<"\n";
    double gwMat[nVert][2];
    for(int g=0;g<nVert;g++){
        gwMat[g][0]=-1;
        gwMat[g][1]=-1;
    }

    //Evaluation table- saves rank and centrality
    int n_connect=0;
    double central=0, imN_path=0, n_path=0, sumLqe=0, nSum=0, wei=0;
    for(int i=0;i<nVert;i++){
        if(graphR.returnShortestPath(graphR.add_element(ownMACAddress),i)!=""){
            //Centrality
            std::pair<int, int> result=geodisikV(i);
            EV<<"For->"<<i<<" nPaths:"<<result.first<<" ImNpaths:"<<result.second<<"\n";
            n_path=result.first;
            imN_path=result.second;
            central=imN_path/n_path;
            sumLqe=0, nSum=0, wei=0;
            for(int j=0;j<nVert;j++){
                wei = graphR.returnWGrapfT(i,j);
                if(wei>0){
                    sumLqe=sumLqe+(100-wei);
                    nSum++;
                }
            }
            double ener_spent=Ener[i];
            if(nSum!=0){
                //gwMat[i][0]=teta*sumLqe*nSum+alfa*ener_spent;   //RANK EQUATION
                gwMat[i][0]=(1-central)*sumLqe/nSum+central*ener_spent;   //RANK EQUATION
                //EV<<"Rank is:"<<gwMat[i][0]<<"central:"<<central<<"lqe:"<<sumLqe/nSum<<"ener:"<<ener_spent<<"\n";
                EV<<"Rank is:"<<gwMat[i][0]<<" central "<<central<<" i: "<<i<<" lqe:"<<sumLqe/nSum<<"ener:"<<ener_spent<<"\n";
            }else{
                gwMat[i][0]=0;
            }
            gwMat[i][1]=nSum;   //nº somas=no neighs
            n_connect=n_connect+nSum;
        }
    }
    n_connect=n_connect/2;
    EV<<"Conec:"<<n_connect<<"\n";

    /**Methods that chose GW to be elected from the node's rank************************************************************************/
    //--METHOD 1 ----------------------------------
    //compare by the number of direct-neighs (used on early-beta) --RANK ALGORITHM ONLY ON CENTRALITY
    EV<<"id0:"<<gwMat[0][1]<<"id1:"<<gwMat[1][1]<<"id2:"<<gwMat[2][1]<<"id3:"<<gwMat[3][1]<<"id4:"<<gwMat[4][1]<<"\n";
    EV<<"id0:"<<gwMat[0][0]<<"id1:"<<gwMat[1][0]<<"id2:"<<gwMat[2][0]<<"id3:"<<gwMat[3][0]<<"id4:"<<gwMat[4][0]<<"\n";
    double nN=0; int bestGwId=0;
    for(int u=0;u<nVert;u++){
        if(gwMat[u][1]>nN){
            nN=gwMat[u][1];
            bestGwId=u;
        }else if(gwMat[u][1]==nN){  //if same number of neighs, check who's got the lowest rank (lower the better)
            //EV<<"Comparing "<<u<<":"<<gwMat[u][0]<<" to "<<bestGwId<<":"<<gwMat[nN][0]<<"\n";
            if(gwMat[u][0]>gwMat[bestGwId][0]){
                //EV<<"Even! Won "<<u<<"\n";
                nN=gwMat[u][1];
                bestGwId=u;
            }
        }
    }
    bestGwRank=nN;
    oldGwRank=gwMat[oldGwID][0];
    return bestGwId;
}

int pamulqe::returnElectedGwM2(){

    //calculate Gw rank
    int nVert=graphR.returnVvalue();
    //EV<<"Já contei:"<<nVert<<"\n";
    double gwMat[nVert][2];
    for(int g=0;g<nVert;g++){
        gwMat[g][0]=-1;
        gwMat[g][1]=-1;
    }

    //Evaluation table- saves rank and centrality
    int n_connect=0;
    double central=0, imN_path=0, n_path=0, sumLqe=0, nSum=0, wei=0;
    for(int i=0;i<nVert;i++){
        if(graphR.returnShortestPath(graphR.add_element(ownMACAddress),i)!=""){
            //Centrality
            std::pair<int, int> result=geodisikV(i);
            EV<<"For->"<<i<<" nPaths:"<<result.first<<" ImNpaths:"<<result.second<<"\n";
            n_path=result.first;
            imN_path=result.second;
            central=imN_path/n_path;
            sumLqe=0, nSum=0, wei=0;
            for(int j=0;j<nVert;j++){
                wei = graphR.returnWGrapfT(i,j);
                if(wei>0){
                    sumLqe=sumLqe+(100-wei);
                    nSum++;
                }
            }
            double ener_spent=Ener[i];
            if(nSum!=0){
                //gwMat[i][0]=teta*sumLqe*nSum+alfa*ener_spent;   //RANK EQUATION
                gwMat[i][0]=(1-central)*sumLqe/nSum+central*ener_spent;   //RANK EQUATION
                //EV<<"Rank is:"<<gwMat[i][0]<<"central:"<<central<<"lqe:"<<sumLqe/nSum<<"ener:"<<ener_spent<<"\n";
                EV<<"Rank is:"<<gwMat[i][0]<<" central "<<central<<" i: "<<i<<" lqe:"<<sumLqe/nSum<<"ener:"<<ener_spent<<"\n";
            }else{
                gwMat[i][0]=0;
            }
            gwMat[i][1]=central;//nSum;   //nº somas=no neighs
            n_connect=n_connect+nSum;
        }
    }
    n_connect=n_connect/2;
    EV<<"Conec:"<<n_connect<<"\n";

    /**Methods that chose GW to be elected from the node's rank************************************************************************/
    //--METHOD 1 ----------------------------------
    //compare by the number of direct-neighs (used on early-beta) --RANK ALGORITHM ONLY ON CENTRALITY
    EV<<"id0:"<<gwMat[0][1]<<"id1:"<<gwMat[1][1]<<"id2:"<<gwMat[2][1]<<"id3:"<<gwMat[3][1]<<"id4:"<<gwMat[4][1]<<"\n";
    EV<<"id0:"<<gwMat[0][0]<<"id1:"<<gwMat[1][0]<<"id2:"<<gwMat[2][0]<<"id3:"<<gwMat[3][0]<<"id4:"<<gwMat[4][0]<<"\n";
    double nN=0; int bestGwId=0;
    for(int u=0;u<nVert;u++){
        if(gwMat[u][1]>nN){
            nN=gwMat[u][1];
            bestGwId=u;
        }else if(gwMat[u][1]==nN){  //if same number of neighs, check who's got the lowest rank (lower the better)
            //EV<<"Comparing "<<u<<":"<<gwMat[u][0]<<" to "<<bestGwId<<":"<<gwMat[nN][0]<<"\n";
            if(gwMat[u][0]>gwMat[bestGwId][0]){
                //EV<<"Even! Won "<<u<<"\n";
                nN=gwMat[u][1];
                bestGwId=u;
            }
        }
    }
    bestGwRank=nN;
    oldGwRank=gwMat[oldGwID][0];
    return bestGwId;
}

int pamulqe::returnElectedGwM3(){

    //calculate Gw rank
    int nVert=graphR.returnVvalue();
    //EV<<"Já contei:"<<nVert<<"\n";
    double gwMat[nVert][2];
    for(int g=0;g<nVert;g++){
        gwMat[g][0]=-1;
        gwMat[g][1]=-1;
    }

    //Evaluation table- saves rank and centrality
    int n_connect=0;
    double central=0, imN_path=0, n_path=0, sumLqe=0, nSum=0, wei=0;
    for(int i=0;i<nVert;i++){
        if(graphR.returnShortestPath(graphR.add_element(ownMACAddress),i)!=""){
            //Centrality
            std::pair<int, int> result=geodisikV(i);
            EV<<"For->"<<i<<" nPaths:"<<result.first<<" ImNpaths:"<<result.second<<"\n";
            n_path=result.first;
            imN_path=result.second;
            central=imN_path/n_path;
            sumLqe=0, nSum=0, wei=0;
            for(int j=0;j<nVert;j++){
                wei = graphR.returnWGrapfT(i,j);
                if(wei>0){
                    sumLqe=sumLqe+(100-wei);
                    nSum++;
                }
            }
            double ener_spent=Ener[i];
            if(nSum!=0){
                //gwMat[i][0]=teta*sumLqe*nSum+alfa*ener_spent;   //RANK EQUATION
                gwMat[i][0]=(1-central)*sumLqe/nSum+central*ener_spent;   //RANK EQUATION
                //EV<<"Rank is:"<<gwMat[i][0]<<"central:"<<central<<"lqe:"<<sumLqe/nSum<<"ener:"<<ener_spent<<"\n";
                EV<<"Rank is:"<<gwMat[i][0]<<" central "<<central<<" i: "<<i<<" lqe:"<<sumLqe/nSum<<"ener:"<<ener_spent<<"\n";
            }else{
                gwMat[i][0]=0;
            }
            gwMat[i][1]=central;//nSum;   //nº somas=no neighs
            n_connect=n_connect+nSum;
        }
    }
    n_connect=n_connect/2;
    EV<<"Conec:"<<n_connect<<"\n";

    /**Methods that chose GW to be elected from the node's rank************************************************************************/
    //--METHOD 2 ----------------------------------

    //compare by the rank -- RANK ALGORITHM ON CENTRALITY AND ENERGY - best rank value----------

    double Rl=0; int bestGwId=-1;
    for(int uR=0;uR<nVert;uR++){
        if(0.95*gwMat[uR][0]>Rl){
            EV<<"bestr:"<<gwMat[uR][0]<<"with id:"<<uR<<" prevR:"<<Rl<<"with id:"<<bestGwId<<"\n";
            Rl=gwMat[uR][0];
            bestGwId=uR;
        }else if(gwMat[uR][0]==Rl){  //if same rank lvl, check who's got more neighs
            if(gwMat[uR][1]>gwMat[bestGwId][1]){
                EV<<"==>bestr:"<<gwMat[uR][0]<<" prevR:"<<Rl<<"\n";
                Rl=gwMat[uR][0];
                bestGwId=uR;
            }
        }
    }

    if(gwMat[oldGwID][0]<0){
        oldGwRank=0;
    }else{
        oldGwRank=gwMat[oldGwID][0];
    }
    //oldGwRank=gwMat[oldGwID][0];
    bestGwRank=Rl;
    return bestGwId;
}


/***************************************************************************************
 * Cleans the list of AppRegisteredApps, calls the destructor of the Cache/Storage
 */
//FINISH
void pamulqe::finish(){

    recordScalar("numEventsHandled", numEventsHandled);

    // clear resgistered app list
    while (registeredAppList.size() > 0) {
        list<AppInfo*>::iterator iteratorRegisteredApp = registeredAppList.begin();
        AppInfo *appInfo= *iteratorRegisteredApp;
        registeredAppList.remove(appInfo);
        delete appInfo;
    }


    // clear gateway list

    while (GatewayList.size() > 0) {
            list<GatewayN*>::iterator iteratorRegisteredApp = GatewayList.begin();
            GatewayN *gatewayN= *iteratorRegisteredApp;
            GatewayList.remove(gatewayN);
            delete gatewayN;
        }


    // clear cache list
    Stor.~StorageM();   //Destructor


}
