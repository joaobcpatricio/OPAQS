// @major changes by : João Patrício (castanheirapatricio@ua.pt)
// @date :   3-jul-2019

#ifndef BASENODEINFO_H_
#define BASENODEINFO_H_
#include "inet/mobility/contract/IMobility.h"

#include <omnetpp.h>
#include <cstdlib>
#include <string>


#include "WirelessInterface.h"
#include "BluetoothInterface.h"

using namespace omnetpp;

using namespace std;

class WirelessInterface;

class BaseNodeInfo
{
    public:
        string nodeAddress;

        cModule *nodeModule;
		inet::IMobility *nodeMobilityModule;
		WirelessInterface *nodeWirelessIfcModule;
		//ADDED 7/07/19 23h33
		BluetoothInterface *nodeBluetoothIfcModule;
};

#endif /* BASENODEINFO_H_ */
