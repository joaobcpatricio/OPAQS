//
// The model implementation for link adaptations between
// the used link technology (below) and the upper layer.
//
// @author : João Patrício (castanheirapatricio@ua.pt)
// @date   : 3-jul-2019
//
//

#ifndef LINKADAPTLAYER_H_
#define LINKADAPTLAYER_H_

#include <omnetpp.h>
#include <cstdlib>
#include <sstream>
#include <string>

#include "OutsMsg_m.h"
#include "InternalMsg_m.h"

using namespace omnetpp;

using namespace std;

class LinkAdaptLayer: public cSimpleModule
{
    protected:
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);
        virtual int numInitStages() const;

    private:
        string ownMACAddress;
        string ownBTMACAddress;
        string broadcastMACAddress;

};

#define LINKADAPTLAYER_SIMMODULEINFO       " LinkAdaptLayer>!<" << simTime() << ">!<" << getParentModule()->getFullName()

#endif /* LINKADAPTLAYER_H_ */
