//
// The model implementation for the Herald application.
//
// @author : Asanga Udugama (adu@comnets.uni-bremen.de), Anna Förster (afoerster@uni-bremen.de)
// @date   : 15-aug-2016, updated 6-febr-2018
//
// @major changes by : João Patrício (castanheirapatricio@ua.pt)
// @date :   3-jul-2019
//

#include "AppLayer.h"

Define_Module(AppLayer);

vector<int> popularityList;


void AppLayer::initialize(int stage)
{
    if (stage == 0) {

        // get parameters
        nodeIndex = par("nodeIndex");
        usedRNG = par("usedRNG");
        dataGenerationInterval = par("dataGenerationInterval");
        popularityAssignmentPercentage = par("popularityAssignmentPercentage");
        dataSizeInBytes = par("dataSizeInBytes");
        ttl = par("ttl");
        totalSimulationTime = SimTime::parse(getEnvir()->getConfig()->getConfigValue("sim-time-limit")).dbl();
        notificationCount = totalSimulationTime/dataGenerationInterval;
        totalNumNodes = getParentModule()->getParentModule()->par("numNodes");

        //Adicionei:
        destinationAddr=par("destinationAddr").stringValue();
        destinationBTAddr=par("destinationBTAddr").stringValue();

        //added 1/07
        nMsgOrder=0;
        generateMsg = false;
        //nodeGenMsg=par("nodeGenMsg");
        nodesGenMsg=par("nodesGenMsg").stringValue();
        nodesThatGen = par("nodesThatGen");

        // setup prefix
        strcpy(prefixName, "/herald");

        // The following procedure is followed when assigning goodness values to each notification
        // - introduce popularity of messages. The “pop” value represents the percentage of the complete
        //   network which will love this packet (e.g. joke).
        // - at initialisation, create pop-values for all messages. 90% of the messages get a pop-value of
        //   0, 10% of the messages get a random pop-value between 1 and 20.
        // - after creating all the messages with their pop-values, allow each user to decide whether she
        //   likes the message or not. There are only two decisions possible: IGNORE (corresponds to a value
        //   of 0) and LOVE (correponds to a value of 100).
        // - the decision itself is taken according to Fig. 3b and Equation (2) of the UBM paper. The
        //   resulting value is compared to 90: if the value is greater or equal to 90, LOVE (100). If not, IGNORE (0).
        // - The final “like” values (only 0 and 100s possible) are stored and used later at simulation time
        //   to represent the reaction of the user and to send to Keetchi as goodness values.


        // assign popularity values for the popularityAssignmentPercentage of notificationCount
        if (popularityList.size() == 0) {
            for (int i = 0; i < notificationCount; i++) {
                double perc = uniform(0.0, 100.0, usedRNG);
                int popularity = 0;

                if (perc <= popularityAssignmentPercentage) {
                    popularity = 100;
                }

                popularityList.push_back(popularity);
            }
        }


        // setup the event notification array
        for (int i = 0; i < notificationCount; i++)
        {
            int like = 0;
            if (popularityList[i] > 90)
                like = 100;
            myLikenesses.push_back(like);       //TO DELETE
            timesMessagesReceived.push_back(0);
        }

        nextGenerationNotification = 0;

    } else if (stage == 1) {

/*********************************************************************************************
* Here it decides which nodes will be generating or that will not be generating
*/
        string nList = nodesGenMsg;
        string chos;
        string tt=",";
        int nodeC[]={};//[5];
        int iss=0;
        int t=0;
        int nA=0;
        while(iss<nList.length()){
            if(nList.compare(iss,1,",")==0){
                chos=nList.substr(t,iss-t);
                t=iss+1;
                nodeC[nA]=stoi(chos);
                nA++;
            }else if((iss==nList.length()-1)){
                chos=nList.substr(t,iss-t+1);
                t=iss+1;
                nodeC[nA]=stoi(chos);
                nA++;
            }
            iss++;
        }
        int var=0;
        while(var<nA){
            nodeCh[var]=nodeC[var];
            var++;
        }
        nAr=nA;
        int vrr=0;
        while(vrr<nA){
            EV<<"nodeCh is: "<<nodeCh[vrr]<<" \n";
            vrr++;
        }

        //check if node index is in the array
        bool confirm=false;
        vrr=0;
        while(vrr<nA){
            if(nodeCh[vrr]==nodeIndex){
                if(nodesThatGen){
                    generateMsg=true;
                }else{
                    //nodeGenMsg=nodeIndex+3; //make sure it is diferent so that after it is no chosen to generate
                    generateMsg=false;
                    confirm=true;
                }
                break;
            }
            vrr++;
        }
        if(!nodesThatGen && !confirm){
            generateMsg=true;
        }
//----------------------------------------------------------------------------------------------------------------



    } else if (stage == 2) {

        // create and setup app registration trigger
        appRegistrationEvent = new cMessage("App Registration Event");
        appRegistrationEvent->setKind(APPLAYER_REGISTRATION_EVENT);
        scheduleAt(simTime(), appRegistrationEvent);

        // this is a round-robin scheduling of traffic: in a row, everybody gets a chance to send the next packet.

        dataTimeoutEvent = new cMessage("Data Timeout Event");
        dataTimeoutEvent->setKind(APPLAYER_DATASEND_EVENT);
        //ADDED 01/07 15h21



        //if(nodeGenMsg==nodeIndex){
        if(generateMsg){
                    generateMsg=true;
                    EV<<"It's first node0 msg:"<<nMsgOrder<<" my index= "<<nodeIndex<<" \n";
                    scheduleAt(simTime() + 0 + 0.1, dataTimeoutEvent); //ou apagar "dataGenerationInterval*nodeIndex" para gerar logo uma msg no inicio


                // add 0.1 secs to the first sending to avoid the simulation to send one more message than expected.
                //scheduleAt(simTime() + dataGenerationInterval*nodeIndex + 0.1, dataTimeoutEvent);
                nextGenerationNotification = nodeIndex;
                nMsgOrder++;
                }
        // setup statistics signals
        likedDataBytesReceivedSignal = registerSignal("likedDataBytesReceived");
        nonLikedDataBytesReceivedSignal = registerSignal("nonLikedDataBytesReceived");
        duplicateDataBytesReceivedSignal = registerSignal("duplicateDataBytesReceived");
        likedDataBytesReceivableByAllNodesSignal = registerSignal("likedDataBytesReceivableByAllNodes");
        nonLikedDataBytesReceivableByAllNodesSignal = registerSignal("nonLikedDataBytesReceivableByAllNodes");

        totalDataBytesReceivedSignal = registerSignal("totalDataBytesReceived");
        totalDataBytesReceivableByAllNodesSignal = registerSignal("totalDataBytesReceivableByAllNodes");

        dataDelaySignal = registerSignal("dataDelay");


//Create File that saves App Data
        ofstream file;
        file.open ("test.txt");
        file << "Please writr this text to a file.\n this text is written using C++\n";
        file.close();
    } else {
        EV_FATAL << APPLAYER_SIMMODULEINFO << "Something is radically wrong\n";
    }

}

int AppLayer::numInitStages() const
{
    return 3;
}

void AppLayer::handleMessage(cMessage *msg)
{


    // check message


/********************************************************************************************************
 * If it is a self message of type APP Registration, create new RegisterAppMsg and send to lowerLayer
 */
    if (msg->isSelfMessage() && msg->getKind() == APPLAYER_REGISTRATION_EVENT) {
        // send app registration message the forwarding layer
        RegisterAppMsg *regAppMsg = new RegisterAppMsg("Herald App Registration");
        regAppMsg->setAppName("Herald");
        regAppMsg->setPrefixName(prefixName);

        send(regAppMsg, "lowerLayerOut");

/***********************************************************************************************************
 * If it is self message od type AppDataMsg, create DataMsg, send it to lowerLayer and schedule another self message of type AppDataMsg
 */


    } else if (msg->isSelfMessage() && msg->getKind() == APPLAYER_DATASEND_EVENT) {

        // mark this message as received by this node
        timesMessagesReceived[nextGenerationNotification]++;

        //setup the data message for sending down to forwarding layer
        char tempString[128];
        sprintf(tempString, "D item-%0d", APPLAYER_START_ITEM_ID + nextGenerationNotification);

        DataMsg *dataMsg = new DataMsg(tempString);

        dataMsg->setSourceAddress("");
        dataMsg->setDestinationAddress("");

        //I added a final Destination and set Msg as Destination Oriented
        dataMsg->setFinalDestinationNodeName(destinationAddr.c_str());
        //EV<<"Destino set1:"<<dataMsg->getFinalDestinationNodeName()<<"\n";
        if(dataMsg->getFinalDestinationNodeName()!=NULL){
        //EV<<"Origem set:"<<dataMsg->getFinalDestinationNodeName()<<"\n";
            EV<<"Destino set2:"<<dataMsg->getFinalDestinationNodeName()<<"\n";
            dataMsg->setDestinationOriented(true);
        }
        //--

        sprintf(tempString, "/app/item-%0d", APPLAYER_START_ITEM_ID + nextGenerationNotification);
        dataMsg->setDataName(tempString);
        dataMsg->setGroupType(myLikenesses[nextGenerationNotification]);
        dataMsg->setRealPayloadSize(dataSizeInBytes);
        dataMsg->setMsgUniqueID(nextGenerationNotification);
        //EV<<"Data name: "<<tempString<<" uniqueID: "<<nextGenerationNotification<<" \n";
        dataMsg->setNMsgOrder(nMsgOrder);
        //EV<<"nMsgOrder= "<<nMsgOrder<<" \n";
        sprintf(tempString, "Details of item-%0d", APPLAYER_START_ITEM_ID + nextGenerationNotification);
        dataMsg->setDummyPayloadContent(tempString);
        dataMsg->setByteLength(dataSizeInBytes);
        dataMsg->setMsgType(0);
        dataMsg->setValidUntilTime(ttl);
        dataMsg->setInjectedTime(simTime().dbl());
        dataMsg->setNHops(0);
        dataMsg->setHopsTravelled(0);
        send(dataMsg, "lowerLayerOut");

        // emit stat signals
        if (myLikenesses[nextGenerationNotification] == 100) {
            emit(likedDataBytesReceivableByAllNodesSignal, (1 * totalNumNodes * dataSizeInBytes));
        } else {
            emit(nonLikedDataBytesReceivableByAllNodesSignal, (1 * totalNumNodes * dataSizeInBytes));
        }
        emit(totalDataBytesReceivableByAllNodesSignal, (1 * totalNumNodes * dataSizeInBytes));

        // schedule again after a complete round robin of all nodes
        nextGenerationNotification=nMsgOrder;//added 2/07 00h += totalNumNodes;

        //ADDED 1/07 15h44
        if(generateMsg){
            EV<<"It's node0 msg:"<<nMsgOrder<<" \n";
            scheduleAt(simTime() + dataGenerationInterval, msg);
            //scheduleAt(simTime() + dataGenerationInterval*totalNumNodes, msg);
            nMsgOrder++;
        }
//-------------------------------------------------------------------------------------------------------------------

//Save Data to file
        //Nº Msg | NodeIndex | time Gen
        string fNam, pri, tim1;
        fNam="Test: ";
        ostringstream fn, oss, tim2;
        fn <<fNam << nMsgOrder;;
        fNam=fn.str();
        pri="Node Index: ";
        oss <<pri << nodeIndex;;
        pri=oss.str();

        simtime_t tim3;     //get generation time
        tim3=dataMsg->getInjectedTime();
        tim2 << " Time: "<<tim3;
        tim1=tim2.str();

        //Save Data to file
        fstream fileE;
        //file.open (fNam+".txt");
        fileE.open ("test.txt",ios::app);
        fileE << "\n Running\n";
        fileE << "Num. Msg: "+fNam+" Node Index: " + pri + tim1 + "\n";//pri+"\n";
        fileE.close();


/*************************************************************************************************
 * If it received a DataMsg, process it.
 */

    } else if (dynamic_cast<DataMsg*>(msg) != NULL) {

        // message received from outside so, process received data message
        DataMsg *dataMsg = check_and_cast<DataMsg *>(msg);

        // increment number of times seen
        timesMessagesReceived[dataMsg->getMsgUniqueID()]++;

        // check and emit stat signals
        if (timesMessagesReceived[dataMsg->getMsgUniqueID()] > 1) {
            emit(duplicateDataBytesReceivedSignal, (long) dataSizeInBytes);
        } else {
            if (myLikenesses[dataMsg->getMsgUniqueID()] == 100) {
                emit(likedDataBytesReceivedSignal, (long) dataSizeInBytes);
            } else {
                emit(nonLikedDataBytesReceivedSignal, (long) dataSizeInBytes);
            }
            emit(totalDataBytesReceivedSignal, (long) dataSizeInBytes);
            emit(dataDelaySignal, (simTime().dbl() - dataMsg->getInjectedTime()));
        }

        delete msg;

    } else {

        EV_INFO << APPLAYER_SIMMODULEINFO << ">!<Received unexpected packet \n";
        delete msg;
    }

}

void AppLayer::finish()
{
    if (appRegistrationEvent->isScheduled())
        cancelEvent(appRegistrationEvent);
    delete appRegistrationEvent;

    if (dataTimeoutEvent->isScheduled())
        cancelEvent(dataTimeoutEvent);
    delete dataTimeoutEvent;

    myLikenesses.clear();
    if (popularityList.size() > 0)
        popularityList.clear();
    timesMessagesReceived.clear();

}

