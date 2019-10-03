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
        gateway_list = par("gateway_list").stringValue();
        actual_gateway = par("actual_gateway").stringValue();

    } else if (stage == 1) {
        Stor.updateMaxAge(max_age);
        setGatewayList();
        printGatewayList();
        updateGateway();

    } else if (stage == 2) {

        // setup statistics signals
        dataBytesReceivedSignal = registerSignal("dataBytesReceived");
        sumVecBytesReceivedSignal = registerSignal("sumVecBytesReceived");
        dataReqBytesReceivedSignal = registerSignal("dataReqBytesReceived");
        totalBytesReceivedSignal = registerSignal("totalBytesReceived");






        //FILE ResultsGen
        string nameGen="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsGen";
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
        string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsLQE";
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
        string nameFstor="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsStor";
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
        string nameFgw="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsGW";
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
        /*} else if (strstr(gateName, "neighLayerIn") != NULL && dynamic_cast<NetworkGraphMsg*>(msg) != NULL) {
            EV<<"Handling NetworkGraphMsg\n";
            handleNetworkGraphMsg(msg);*/


        // received some unexpected packet
        } else {

            EV_INFO << ROUTINGLAYER_SIMMODULEINFO << "Received unexpected packet" << "\n";
            delete msg;
        }
    }
}

//Saves the received graph from neighboring here for later use in decision;
bool RoutingLayer::getGraph(string graphS){//, int numberVert){ //String:" 1->2:4;\n2->1:4;\n "


    graphR.cleanGraph();

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
    return true;
}

/*void RoutingLayer::handleNetworkGraphMsg(cMessage *msg){
    EV<<"Routing: handleNetworkGraphMsg\n";
    NetworkGraphMsg *neighGraphMsg = dynamic_cast<NetworkGraphMsg*>(msg);
    string graphS = neighGraphMsg->getGraphN();
    //int numberVert = neighGraphMsg->getNumberVert();
    //int countVert = neighGraphMsg->getCountVert();
    EV<<"Recebeu-se: "<<graphS<<"\n";
    bool updt=getGraph(graphS);
    if(updt){ EV<<"Graph Updated\n";}else{EV<<"Graph not updated due to empty string\n";}

    delete msg;
}*/

//Added 25/06
//DECISION OF SENDING IS MADE HERE
/*********************************************************************************************************
 * (NOT_If the prob is good enough,) Gets List of Msgs in cache, for each MsgID of the list it searches if msg exists and gets its position,
 * pulls out the dataMsg prepared to send and sends it to lowerLayer if the destination is not on the previous Hops List of the DataMsg
 */
void RoutingLayer::handleDataReqMsg(cMessage *msg){
    updateGateway();

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

            int cnt=0;
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
                   //Update GW
                   updateGateway();
                   dataMsg->setFinalDestinationNodeName(actual_gateway.c_str());

                   //EV<<"pulled to rout Msg:"<<dataMsg->getNMsgOrder()<<" at time "<<simTime().dbl()<<"\n";
                   string destAdd = dataRequestMsg->getSourceAddress();
                   string gwAdd = dataMsg->getFinalDestinationNodeName();


                   //Save Data

                   if(cnt<=1){
                       //EV<<"gwAdd"<<gwAdd<<"\n";
                       //save info into file
                       string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsLQE";
                       string nMY=MyAddH.substr(15,17);
                       string nGW=gwAdd.substr(15,17);
                       nameF.append(nMY);
                       nameF.append("_");
                       nameF.append(nGW);
                       nameF.append(".txt");
                       std::ofstream out(nameF, std::ios_base::app);
                       //My Short Path
                       int myID=graphR.add_element(MyAddH);
                       int gwID=graphR.add_element(gwAdd);
                       string shrtPath=graphR.returnShortestPath(myID,gwID);
                       //EV<<"Epah"<<shrtPath<<"\n";
                       string stP="ShortPath: \n";
                       stP.append(shrtPath);
                       out<<stP;


                       //Graph
                       string GraphSR=graphR.returnGraphT();
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

                       cnt++;
                   }







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
                           EV<<"Loop Avoidance: Found it "<<dataMsg->getDataName() <<"\n";
                           break;
                       }
                       count1++;
                   }

                   //Verifies if DataMsg destination is this neighbor and DataMsg has not been send yet, if so, send directly with Loop Avoidance
                   if(dataMsg->getFinalDestinationNodeName()==destAdd && foundH==false){
                       int myID=graphR.add_element(MyAddH);
                       int dstID=graphR.add_element(destAdd);
                       int weightH=graphR.returnWGrapfT(myID, dstID);

                       //verifica se o vizinho direto está no grafo
                       if((weightH>0)){



                       EV<<"Direct Neigh is final dest. \n";
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


                       //Save Data
                           /*//save info into file
                           string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsLQE";
                           string noS=ownMACAddress.substr(15,17);
                           nameF.append(noS);
                           nameF.append(".txt");
                           std::ofstream out(nameF, std::ios_base::app);
                           //My Short Path
                           graphR.dijkstra(myID, gwID);
                           string sPat=graphR.returnShortestPath(myID,gwID);
                           string srcer="ShortPath Me&GW: \n";
                           srcer.append(sPat);
                           out<<srcer;
                           //Graph
                           string GraphSR=graphR.returnGraphT();
                           string msIDis="Graph: \n";
                           EV<<"LQE graph: "<<msIDis<<"\n";
                           msIDis.append(GraphSR);
                           out<<msIDis;
                           //Time
                           std::string timeMsg = std::to_string(simTime().dbl());//getInjectedTime().dbl());
                           string timeGen="Time is : ";
                           timeGen.append(timeMsg);
                           out<<timeGen;
                           out<<" |End \n";
                           out.close();*/




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
    updateGateway();

    EV<<"Routing: handleBeacon\n";
    BeaconInfoMsg *beaconMsg = dynamic_cast<BeaconInfoMsg*>(msg);


    //Save Graph Matrix on RoutingLayer
    string myGraph=beaconMsg->getNeighGraph();
    //int npos=beaconMsg->getNumberVert();
    EV<<"Graph in routing: \n";
    getGraph(myGraph);



/*    //Save Data
    //save info into file
    string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsLQE";
    string noS=ownMACAddress.substr(15,17);
    nameF.append(noS);
    nameF.append(".txt");
    std::ofstream out(nameF, std::ios_base::app);
    //My Short Path
    string MyAddh;
    string SouceDRAdd = beaconMsg->getSourceAddress();
    if((SouceDRAdd.substr(0,2))=="BT"){
        MyAddH=ownBTMACAddress;
    }else{
        MyAddH=ownMACAddress;
    }
    string gwAd=GWAddr
    int myID=graphR.add_element(MyAddH);
    int gwID=graphR.add_element(gwAdd);
    graphR.dijkstra(myID, gwID);
    string sPat=graphR.returnShortestPath;
    string srcer="My Address: ";
    srcer.append(ownMACAddress);
    out<<srcer;


    //Graph
    string GraphSR=graphR.returnGraphT();
    string msIDis="Graph: \n";
    msIDis.append(GraphSR);
    out<<msIDis;
    //Time
    std::string timeMsg = std::to_string(simTime().dbl());//getInjectedTime().dbl());
    string timeGen="Time is : ";
    timeGen.append(timeMsg);
    out<<timeGen;
    out<<" |End \n";
    out.close();*/



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
    dataRequestMsg->setInjectedTime(simTime().dbl());

    //Para retirar futuramente
    if(beaconMsg->getProb()>0.5){
        EV<<"Prob bigger than 0.5\n ";
        dataRequestMsg->setSendMeData(true);
    } else{ dataRequestMsg->setSendMeData(false);}

    EV<<"Send DataRequestMsg \n";
    send(dataRequestMsg, "lowerLayerOut");
    delete msg;
}


/********************************************************************************
 *Get's Ack, checks if the other was the final Destiny.
 */
void RoutingLayer::handleAckFromLowerLayer(cMessage *msg){
    updateGateway();
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
    updateGateway();
    DataMsg *upperDataMsg = dynamic_cast<DataMsg*>(msg);
    upperDataMsg->setFinalDestinationNodeName(actual_gateway.c_str());
    upperDataMsg->setOriginatorNodeMAC(ownMACAddress.c_str());
    bool var=false;
    Stor.saveData(msg,0, var);


        //Save Data
        //save info into file
        string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsGen";
        string noS=ownMACAddress.substr(15,17);
        nameF.append(noS);
        nameF.append(".txt");
        std::ofstream out(nameF, std::ios_base::app);

        //Graph
        string dataN=upperDataMsg->getDataName();
        string msIDis="Data Name: ";
        msIDis.append(dataN);
        out<<msIDis;
        //Time
        std::string timeMsg = std::to_string(upperDataMsg->getInjectedTime().dbl());//getInjectedTime().dbl());
        string timeGen=" | Generated Time is : ";
        timeGen.append(timeMsg);
        out<<timeGen;
        out<<" |End \n";
        out.close();




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
            EV<<"Sou a final destination \n";

            //save info into file
            string nameF="/home/mob/Documents/workspaceO/Tese/OpNetas/OPAQS/simulations/DanT/DataResults/ResultsGW";
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
                              /*//Time generated
                              std::string timeMsg = std::to_string(omnetDataMsg->getInjectedTime().dbl());//getInjectedTime().dbl());
                              string timeGen=" | Time generated: ";
                              timeGen.append(timeMsg);
                              out<<timeGen;*/
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


            //cacheData=FALSE;
            imDestiny=TRUE;
        }
    }



    //Saving Data
    if(cacheData) {
        EV<<"Saving data in cache from lower layer \n";
        bool var=false;
        Stor.saveData(msg,1,var);
    }





    //Data Treatment if I'm gateway:
    if(imDestiny){ //GwResults
        if(Stor.msgExistsInC(msg)){
            //guarda aqui nº pacotes recebidos que ja tinha recebido antes e guardado.
            //EV<<"Sava \n";
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

bool RoutingLayer::setGatewayList(){
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
            EV<<"Been here gw \n";
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

void RoutingLayer::printGatewayList(){
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

void RoutingLayer::updateGateway(){
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
            EV<<"Reached last GW set \n";
        }

        }
        itC++;
        //foreverGW=true;
    }
    actual_gateway=currentGW;
    //EV<<"Current Gateway is "<<actual_gateway<<" until "<<deadT<<" s \n";
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
