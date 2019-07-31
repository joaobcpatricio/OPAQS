# OPSLite

The Opportunistic Protocol Simulator, Light (OPSLite, pronounced as oops light!!!) is a set of
simulation models in OMNeT++ to simulate opportunistic networks. It has a
modular architecture where different protocols relevant to opportunistic networks
can be developed and plugged in. OPSLite is a scaled-down, light-weight version of  
[OPS](https://github.com/ComNets-Bremen/OPS), available at Github.

The following sections provide details of the models available and about
the installation. **Unlike [OPS](https://github.com/ComNets-Bremen/OPS) which was purely
a command-line simulator, OPSLite is meant to be run in the OMNeT++ IDE and as well as on 
the command-line**.


## Prerequisites

OPSLite requires OMNeT++ (version 5.4.1) and the INET Framework (version 4.0.0). 
The OMNeT++ simulator and the IDE can be found at the [OMNeT++ site](https://www.omnetpp.org). Once the
simulator and the IDE is installed, INET (version 4) has to be installed. It can be either 
installed through the IDE itself  or by downloading
from the [INET site](https://inet.omnetpp.org). Here are the 2 important prerequisites.

1. Install and build OMNeT++ (version 5.4.1)

2. Run the OMNet++ IDE and create a new workspace

3. Install and build INET (version 4.0.0) in the newly created workspace using
   option - `Help -> Install Simulation Models...` or another means (e.g., at workspace creation, using a downloaded archive, etc.)


## Installing OPSLite

Once OMNeT++ IDE and INET4 are installed and built, follows the following steps to install
[OPSLite](https://github.com/ComNets-Bremen/OPSLite). 

1. Download the Github repository of [OPSLite](https://github.com/ComNets-Bremen/OPSLite)

2. If [OPSLite](https://github.com/ComNets-Bremen/OPSLite) downloaded as a ZIP file, expand the ZIP file

3. Run the OMNeT++ IDE, and import [OPSLite](https://github.com/ComNets-Bremen/OPSLite) (`File -> Import... -> General -> File System`) into the OMNeT++ workspace (where INET is also a project)

4. Tick INET as a referenced project (`Properties -> Project References`). Make the exact INET version installed is ticked (remove other ticks)

5. Build the OPSLite project using - `Project -> Build Project` (should not have any errors or warnings after building)


## Running OPSLite

To run simulation, check the `omnetpp.ini` and the `OPSLiteNetwork.ned` given (in `simulations` folder), to create your own
scenario. Here are the basic steps

1. Run the OMNeT++ IDE and select the workspace where INET4 and OPSLite is installed and built

2. Modify the `omnetpp.ini` to suit your scenario parameters

3. Select the OPSLite folder (the top most folder of the OPSLite project) and run using - `Run -> Run As -> OMNeT++ Simulation`



## Node Architecture

The architecture of a node uses a number of protocol layers which can be configured
based on the scenario considered. The node model is defined in the `OPSNode.ned` file.
Generally, an OPSNode has the following protocol layers.


                           +------------------------+
                           |  +------------------+  |
                           |  |Application Layer |  |
                           |  |                  |  |
                           |  +--------+---------+  |
                           |           |            |
                           |  +--------+---------+  |
                           |  |   Opportunistic  |  |
                           |  | Networking Layer |  |
                           |  +--------+---------+  |
                           |           |            |
                           |  +--------+---------+  |
                           |  | Link Adaptation  |  |
                           |  |     Layer        |  |
                           |  +--------+---------+  |
                           |           |            |
                           |  +--------+---------+  |
                           |  |    Link Layer    |  |
                           |  | +--------------+ |  |
                           |  | |   Mobility   | |  |
                           |  | +--------------+ |  |
                           |  +--------+---------+  |
                           +-----------|------------+
                                       |

Each of the above layers can be configured through their parameters to behave as required. Here are the models
associated with each layer.

1. Application Layer consist of the `KHeraldApp` application that classifies data items as liked and non-liked
   and injects them uniformly over the simulation period.

2. Opportunistic Networking Layer conist of the `KEpidemicRoutingLayer` which is used to forward data
   in an opportunistic network

3. Link Adaptation Layer is a simple pass-through layer (`KLinkAdaptLayer`) intended to be extended in the future.

4. Link Layer consist of the `KWirelessInterface` model that performs simple wireless
   communications.
   
5. Mobility implements the movements of the mobile nodes in the scenarios. It can use any
   of the mobility models available in the INET4 Framework. In OPSLite, `omnetpp.ini` is
   configured to use BonnMotion (`BonnMotionMobility`) mobility model. Some sample traces
   with SLAW mobility (`trace-slaw-01.movements`, ...) are available in the 
   `simulations` folder.

The following picture shows an example configuration of a node as seen in the OMNeT++ IDE.

<p align="center">
  <img src="res/omnet-node-arch.png" width="268px" height="400px"/>
</p>


## Checking Results

Based on the standard configuration, the raw results (vector and scalar) collected after a simulation run 
are located in the `simulation/results` folder. Below is a brief (high-level) procedure to create your charts
using the IDE. 

1. In the `simulation/results` folder, create an `Analysis File` (`New -> Analysis File (anf)`)
2. Add the created results files (`*.vec` or `*.sca`) to the created file
3. Create datasets with the data you want plotted in every graph
4. Plot them using an appropriate chart type (Line, Bar, etc.)

For more information, check Chapter 10 of the [IDE User Guide](https://www.omnetpp.org/doc/omnetpp/UserGuide.pdf)


## Available Results (Statistics)

There are a set of network-level results collected in every simulation run. Here are those results.

### Statistics from the Application Layer

1. `likedDataBytesReceivedSum` - Liked data bytes received by all nodes

2. `nonLikedDataBytesReceivedSum` - Non-liked data bytes received by all nodes

3. `duplicateDataBytesReceivedSum` - Duplicate (liked & non-liked) data bytes received by all nodes

4. `likedDataBytesReceivableByAllNodesSum` - Liked data bytes that are possible to be received by all nodes

5. `nonLikedDataBytesReceivableByAllNodesSum` - Non-liked data bytes that are possible to be received by all nodes

6. `dataDelay` - Delivery delay of data (liked and non-liked) by all nodes

7. `deliveryRatio` - Delivery ratio of data (liked and non-liked) by all nodes


### Statistics from the Forwarding Layer

1. `dataBytesReceivedSum` - Data bytes received by all nodes in the network 
        
2. `sumVecBytesReceivedSum` - Summary vectors bytes received by all nodes

3. `dataReqBytesReceivedSum` - Data request bytes received by all nodes

4. `totalBytesReceivedSum` - Total bytes received by all nodes

5. `cacheBytesRemovedSum` - Cache removals in bytes by all nodes

6. `cacheBytesAddedSum` - Cache additions in bytes by all nodes

7. `cacheBytesUpdatedSum` - Cache updates in bytes by all nodes


### Statistics from the Wireless Interface Layer

1. `neighbourhoodSize` - Size of nodes in the wireless neighbourhood

The following graph shows the progress of the Delivery Ratio (`deliveryRatio`) for two mobility traces based on the
SLAW mobility model. The graph has been created using the statistics graphing capability in OMNeT++ IDE.


<p align="center">
  <img src="res/delivery-ratio-slaw-traces.png" width="600px" height="315px"/>
</p>




## Important Model Parameters

The following list provides some of the most important parameters of the different models 
in OPSLite. All these parameters are configurable using the `omnetpp.ini`. **Not all parameters 
are listed here**. Please check the respective model's `.ned` file to see all the parameters.

### Parameters in `OPSLiteNetwork.ned`

1. `numNodes` - The total number of nodes in the network
2. Network level statistics


### Parameters in `KHeraldApp.ned`

1. `dataGenerationInterval` - Defines how often (in seconds) a data gets injected into the network
2. `dataSizeInBytes` - The payload size in bytes of a data packet


### Parameters in `KEpidemicRoutingLayer.ned`

1. `maximumCacheSize` - The size of the cache maintained by each node in bytes
2. `maximumHopCount` - The maximum hops that a data packet is allowed to travel (be forwarded) before being discarded


### Parameters in `KWirelessLayer.ned`

1. `wirelessRange` - The wireless range of each node's wireless interface
2. `bandwidthBitRate` - Communication bit rate of the wireless interface



## Help

If you have any question or clarifications related to OMNeT++, please check the documentation provided at the [OMNeT++ site](https://www.omnetpp.org) and
[INET](https://inet.omnetpp.org) sites. Here are the important documents.

1. OMNeT++ - [Install Guide](https://www.omnetpp.org/doc/omnetpp/InstallGuide.pdf), 
   [Simulation Manual](https://www.omnetpp.org/doc/omnetpp/SimulationManual.pdf), 
   [IDE User Guide](https://www.omnetpp.org/doc/omnetpp/UserGuide.pdf)

2. INET - [Documentation](https://inet.omnetpp.org/Introduction.html)

3. Results processing in OMNeT++ IDE - Chapter 10 of the [IDE User Guide](https://www.omnetpp.org/doc/omnetpp/UserGuide.pdf)

4. INET Mobility Models - [Node Mobility](https://inet.omnetpp.org/docs/users-guide/ch-mobility.html)



## Known Problems


### Problem: IMobility headers not found

- Description: When building OPSLite, the compilation fails, complaining about the inability to find INET mobility headers.
- Solution: Since OPSLite uses the mobility headers of INET, the referenced projects section (`Properties -> Project References`) has to point to the exact INET version installed in your workspace. Sometimes, INET is installed as `inet4` or `inet`. So, make sure that the right INET entry is ticked. Untick all other unwanted projects.  


### Problem: Undefined symbols when building OPSLite on Windows

- Description: With OMNeT++ 5.4.1 IDE on Windows, the building fails with undefined symbols pointing to the IMobility interface used. We found this to be due to OMNeT++ being setup by default to use the `clang` compiler. 
- Solution: Change the configure.user file to use gcc by setting "PREFER_CLANG=no" and rebuilding OMNeT++ and all the models installed (including INET). Check [Install Guide](https://www.omnetpp.org/doc/omnetpp/InstallGuide.pdf) for further information.




## Questions or Comments

If you have any comments or suggestions, we will be very glad to hear them. Please write to us using any of the e-mail adresses below.

  - Asanga Udugama (adu@comnets.uni-bremen.de)
  - Jens Dede (jd@comnets.uni-bremen.de)
  - Anna Förster (anna.foerster@comnets.uni-bremen.de)
  - Vishnupriya Parimalam (vp@fb1.uni-bremen.de)
  
  
  
  