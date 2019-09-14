//
// Generated file, do not edit! Created by nedtool 5.4 from OutsMsg.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __OUTSMSG_M_H
#define __OUTSMSG_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>OutsMsg.msg:16</tt> by nedtool.
 * <pre>
 * //
 * // Format of a Data packet.
 * //ver tese rodrigo pg55
 * //
 * // \@author : João Patrício (castanheirapatricio\@ua.pt)
 * // \@date   : 10-sep-2019
 * //
 * // example sizes in bytes
 * //
 * //               addrs | data name | group type     | payload size | payload | valid until | final dest | Addr Generator | hops	     | nMsgOrder  | destination | prev Hops | total  |
 * //               s & d | or msg ID |  (1 byte int)  | (2 byte int) | (bytes) | (4 byte int)| (dest orie)|   Node		 |  (1 byte) |            |  oriented   |  list     | (bytes)|
 * // Epidemic ->   6 + 6 .    64     .       1        .      2       . 64      .      4      .      6     .      6		 .		1    .    2       .     1       .    32     .  231+64=295
 * //
 * //Real packet size hardcoded on StorageM.cc
 * //
 * packet DataMsg
 * {
 *     // common fields
 *     string sourceAddress;		//												(6bytes)
 *     string destinationAddress;	//												(6bytes)
 *     string dataName;//sequence number?? - is the messageID em herald			(*-64 bytes)
 *     string messageID; //ID na storage->cacheList  ?  							(*-64 bytes)
 *     int msgUniqueID;	//ID da msg em int										(2)
 *     int groupType = 50;//(previouys goodness value) Groupt type 				(1byte)   
 *     simtime_t injectedTime;	//timeStamp	, generation time						(?)
 *     int realPacketSize; //tells total size of the message 						(2 byte)
 *     int realPayloadSize; //size of the data -> dataSizeInBytes 					(10 bytes? .ini)       
 *     double validUntilTime = 0; //ttl - defines when data time expires			(4b ytes)
 *     string finalDestinationNodeName; //final destination MAC - GW				(6 bytes)
 *     string originatorNodeMAC; // I can chose to store here the MACAdd 			(6 bytes)
 *     						//of the node that generated the Msg				-------
 *     //int hopsTravelled;// = 0;													------
 *     int nHops;		//															(1byte)
 *     int nMsgOrder;  	//Identifies the order of its generation on its node	(2bytes)
 *     					//could be used instead msgUniqueID or messageID
 *     bool destinationOriented = false;	//										(1 byte)
 *     string prevHopsList[];	//	list of previous nodes where the data 			(32 bytes)
 *     						//passes through - loop avoidance mechanism
 * 
 *     simtime_t sentTime;	//timeStamp
 *     simtime_t receivedTime;	//timeStamp
 * 
 *     //OTHERS old:    
 *     //string dummyPayloadContent;	
 *     //int msgType; //?? not sure 4 what
 *     // destination oriented delivery specific fields
 * 
 * }
 * </pre>
 */
class DataMsg : public ::omnetpp::cPacket
{
  protected:
    ::omnetpp::opp_string sourceAddress;
    ::omnetpp::opp_string destinationAddress;
    ::omnetpp::opp_string dataName;
    ::omnetpp::opp_string messageID;
    int msgUniqueID;
    int groupType;
    ::omnetpp::simtime_t injectedTime;
    int realPacketSize;
    int realPayloadSize;
    double validUntilTime;
    ::omnetpp::opp_string finalDestinationNodeName;
    ::omnetpp::opp_string originatorNodeMAC;
    int nHops;
    int nMsgOrder;
    bool destinationOriented;
    ::omnetpp::opp_string *prevHopsList; // array ptr
    unsigned int prevHopsList_arraysize;
    ::omnetpp::simtime_t sentTime;
    ::omnetpp::simtime_t receivedTime;

  private:
    void copy(const DataMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const DataMsg&);

  public:
    DataMsg(const char *name=nullptr, short kind=0);
    DataMsg(const DataMsg& other);
    virtual ~DataMsg();
    DataMsg& operator=(const DataMsg& other);
    virtual DataMsg *dup() const override {return new DataMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getSourceAddress() const;
    virtual void setSourceAddress(const char * sourceAddress);
    virtual const char * getDestinationAddress() const;
    virtual void setDestinationAddress(const char * destinationAddress);
    virtual const char * getDataName() const;
    virtual void setDataName(const char * dataName);
    virtual const char * getMessageID() const;
    virtual void setMessageID(const char * messageID);
    virtual int getMsgUniqueID() const;
    virtual void setMsgUniqueID(int msgUniqueID);
    virtual int getGroupType() const;
    virtual void setGroupType(int groupType);
    virtual ::omnetpp::simtime_t getInjectedTime() const;
    virtual void setInjectedTime(::omnetpp::simtime_t injectedTime);
    virtual int getRealPacketSize() const;
    virtual void setRealPacketSize(int realPacketSize);
    virtual int getRealPayloadSize() const;
    virtual void setRealPayloadSize(int realPayloadSize);
    virtual double getValidUntilTime() const;
    virtual void setValidUntilTime(double validUntilTime);
    virtual const char * getFinalDestinationNodeName() const;
    virtual void setFinalDestinationNodeName(const char * finalDestinationNodeName);
    virtual const char * getOriginatorNodeMAC() const;
    virtual void setOriginatorNodeMAC(const char * originatorNodeMAC);
    virtual int getNHops() const;
    virtual void setNHops(int nHops);
    virtual int getNMsgOrder() const;
    virtual void setNMsgOrder(int nMsgOrder);
    virtual bool getDestinationOriented() const;
    virtual void setDestinationOriented(bool destinationOriented);
    virtual void setPrevHopsListArraySize(unsigned int size);
    virtual unsigned int getPrevHopsListArraySize() const;
    virtual const char * getPrevHopsList(unsigned int k) const;
    virtual void setPrevHopsList(unsigned int k, const char * prevHopsList);
    virtual ::omnetpp::simtime_t getSentTime() const;
    virtual void setSentTime(::omnetpp::simtime_t sentTime);
    virtual ::omnetpp::simtime_t getReceivedTime() const;
    virtual void setReceivedTime(::omnetpp::simtime_t receivedTime);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const DataMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, DataMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>OutsMsg.msg:65</tt> by nedtool.
 * <pre>
 * //***********************************************************************************  /
 * // Acknowledge message.
 * //
 * // \@author : João Patrício (castanheirapatricio\@ua.pt)
 * // \@date   : 18-june-2019
 * //
 * // example sizes in bytes
 * //
 * //                addrs | msg ID hashes  | if is final | total  |
 * //                s & d | 2 bytes x count| destination | (bytes)|
 * //           ->   6 + 6 .   2 x 1        .     1       .  15
 * //Real packet size hardcoded on RoutingLayer
 * //
 * packet AckMsg
 * {
 *     // common fields
 *     string sourceAddress;
 *     string destinationAddress;
 *     int realPacketSize; // (6+6) from addr + 1 from messageID + hash size
 *     // routing specific fields
 *     string messageID;  // will inform the msg id
 *     bool isFinalDest;		//Indicates if it reached the final destination
 *     //bool reachedGW - to insert later if to use (1 byte)
 * }
 * </pre>
 */
class AckMsg : public ::omnetpp::cPacket
{
  protected:
    ::omnetpp::opp_string sourceAddress;
    ::omnetpp::opp_string destinationAddress;
    int realPacketSize;
    ::omnetpp::opp_string messageID;
    bool isFinalDest;

  private:
    void copy(const AckMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AckMsg&);

  public:
    AckMsg(const char *name=nullptr, short kind=0);
    AckMsg(const AckMsg& other);
    virtual ~AckMsg();
    AckMsg& operator=(const AckMsg& other);
    virtual AckMsg *dup() const override {return new AckMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getSourceAddress() const;
    virtual void setSourceAddress(const char * sourceAddress);
    virtual const char * getDestinationAddress() const;
    virtual void setDestinationAddress(const char * destinationAddress);
    virtual int getRealPacketSize() const;
    virtual void setRealPacketSize(int realPacketSize);
    virtual const char * getMessageID() const;
    virtual void setMessageID(const char * messageID);
    virtual bool getIsFinalDest() const;
    virtual void setIsFinalDest(bool isFinalDest);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AckMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AckMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>OutsMsg.msg:91</tt> by nedtool.
 * <pre>
 * //**********************************************************************************  /
 * // Beacon message.
 * //
 * // \@author : João Patrício (castanheirapatricio\@ua.pt)
 * // \@date   : 26-june-2019
 * //
 * // example sizes in bytes
 * //
 * //               addrs | Prob 		| MyPosX 	 | MyPosY	| neighGraph | numberVert | total  |
 * //               s & d | (4 byte)   |  (4 byte)  | (4 byte) |            |            | (bytes)|
 * // Routing	 ->   6 + 6      4             4           4        64            1           89
 * //
 * //Real packet size hardcoded on NeighboringLayer
 * //
 * packet BeaconMsg
 * {
 *     // common fields
 *     string sourceAddress;
 *     string destinationAddress;
 *     int realPacketSize;	//Hardcoded in NeighboringLayer.cc
 *     double Prob;
 *     double MyPosX;
 *     double MyPosY;
 *     string neighGraph;
 *     int numberVert;
 *     simtime_t sentTime;	//timeStamp
 *     simtime_t receivedTime;	//timeStamp
 * }
 * </pre>
 */
class BeaconMsg : public ::omnetpp::cPacket
{
  protected:
    ::omnetpp::opp_string sourceAddress;
    ::omnetpp::opp_string destinationAddress;
    int realPacketSize;
    double Prob;
    double MyPosX;
    double MyPosY;
    ::omnetpp::opp_string neighGraph;
    int numberVert;
    ::omnetpp::simtime_t sentTime;
    ::omnetpp::simtime_t receivedTime;

  private:
    void copy(const BeaconMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BeaconMsg&);

  public:
    BeaconMsg(const char *name=nullptr, short kind=0);
    BeaconMsg(const BeaconMsg& other);
    virtual ~BeaconMsg();
    BeaconMsg& operator=(const BeaconMsg& other);
    virtual BeaconMsg *dup() const override {return new BeaconMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getSourceAddress() const;
    virtual void setSourceAddress(const char * sourceAddress);
    virtual const char * getDestinationAddress() const;
    virtual void setDestinationAddress(const char * destinationAddress);
    virtual int getRealPacketSize() const;
    virtual void setRealPacketSize(int realPacketSize);
    virtual double getProb() const;
    virtual void setProb(double Prob);
    virtual double getMyPosX() const;
    virtual void setMyPosX(double MyPosX);
    virtual double getMyPosY() const;
    virtual void setMyPosY(double MyPosY);
    virtual const char * getNeighGraph() const;
    virtual void setNeighGraph(const char * neighGraph);
    virtual int getNumberVert() const;
    virtual void setNumberVert(int numberVert);
    virtual ::omnetpp::simtime_t getSentTime() const;
    virtual void setSentTime(::omnetpp::simtime_t sentTime);
    virtual ::omnetpp::simtime_t getReceivedTime() const;
    virtual void setReceivedTime(::omnetpp::simtime_t receivedTime);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const BeaconMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, BeaconMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>OutsMsg.msg:116</tt> by nedtool.
 * <pre>
 * //**********************************************************************************  /
 * // Data Request message.
 * //
 * // \@author : João Patrício (castanheirapatricio\@ua.pt)
 * // \@date   : 26-june-2019
 * //
 * //               addrs | bool send me	|Prob 		|    SSI	|total   |
 * //               s & d | 	  6 byte    |(4 byte)   | (4 bytes) | (bytes)|
 * // Routing	 ->   6 + 6       6             4           4  			26
 * //
 * packet DataReqMsg
 * {
 *     // common fields
 *     string sourceAddress;
 *     string destinationAddress;
 *     int realPacketSize;
 *     bool SendMeData;
 * 
 *     double Prob;
 *     //double MyPosX;
 *     //double MyPosY;
 *     double SSI;
 * 	//int Nic; //0=wifi, 1=Bt,
 * }
 * //**********************************************************************************  /
 * </pre>
 */
class DataReqMsg : public ::omnetpp::cPacket
{
  protected:
    ::omnetpp::opp_string sourceAddress;
    ::omnetpp::opp_string destinationAddress;
    int realPacketSize;
    bool SendMeData;
    double Prob;
    double SSI;

  private:
    void copy(const DataReqMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const DataReqMsg&);

  public:
    DataReqMsg(const char *name=nullptr, short kind=0);
    DataReqMsg(const DataReqMsg& other);
    virtual ~DataReqMsg();
    DataReqMsg& operator=(const DataReqMsg& other);
    virtual DataReqMsg *dup() const override {return new DataReqMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getSourceAddress() const;
    virtual void setSourceAddress(const char * sourceAddress);
    virtual const char * getDestinationAddress() const;
    virtual void setDestinationAddress(const char * destinationAddress);
    virtual int getRealPacketSize() const;
    virtual void setRealPacketSize(int realPacketSize);
    virtual bool getSendMeData() const;
    virtual void setSendMeData(bool SendMeData);
    virtual double getProb() const;
    virtual void setProb(double Prob);
    virtual double getSSI() const;
    virtual void setSSI(double SSI);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const DataReqMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, DataReqMsg& obj) {obj.parsimUnpack(b);}


#endif // ifndef __OUTSMSG_M_H

