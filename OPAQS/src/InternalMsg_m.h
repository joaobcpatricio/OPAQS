//
// Generated file, do not edit! Created by nedtool 5.4 from InternalMsg.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __INTERNALMSG_M_H
#define __INTERNALMSG_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>InternalMsg.msg:13</tt> by nedtool.
 * <pre>
 * // Message to register an application
 * packet RegisterAppMsg
 * {
 *     string appName;
 *     string prefixName;
 * }
 * </pre>
 */
class RegisterAppMsg : public ::omnetpp::cPacket
{
  protected:
    ::omnetpp::opp_string appName;
    ::omnetpp::opp_string prefixName;

  private:
    void copy(const RegisterAppMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RegisterAppMsg&);

  public:
    RegisterAppMsg(const char *name=nullptr, short kind=0);
    RegisterAppMsg(const RegisterAppMsg& other);
    virtual ~RegisterAppMsg();
    RegisterAppMsg& operator=(const RegisterAppMsg& other);
    virtual RegisterAppMsg *dup() const override {return new RegisterAppMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getAppName() const;
    virtual void setAppName(const char * appName);
    virtual const char * getPrefixName() const;
    virtual void setPrefixName(const char * prefixName);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const RegisterAppMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, RegisterAppMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>InternalMsg.msg:19</tt> by nedtool.
 * <pre>
 * // Message to provide the current neighbour list
 * packet NeighbourListMsg
 * {
 *     string neighbourNameList[];
 *     int neighbourNameCount;
 * 
 *     //double neighPos[];
 * }
 * </pre>
 */
class NeighbourListMsg : public ::omnetpp::cPacket
{
  protected:
    ::omnetpp::opp_string *neighbourNameList; // array ptr
    unsigned int neighbourNameList_arraysize;
    int neighbourNameCount;

  private:
    void copy(const NeighbourListMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NeighbourListMsg&);

  public:
    NeighbourListMsg(const char *name=nullptr, short kind=0);
    NeighbourListMsg(const NeighbourListMsg& other);
    virtual ~NeighbourListMsg();
    NeighbourListMsg& operator=(const NeighbourListMsg& other);
    virtual NeighbourListMsg *dup() const override {return new NeighbourListMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual void setNeighbourNameListArraySize(unsigned int size);
    virtual unsigned int getNeighbourNameListArraySize() const;
    virtual const char * getNeighbourNameList(unsigned int k) const;
    virtual void setNeighbourNameList(unsigned int k, const char * neighbourNameList);
    virtual int getNeighbourNameCount() const;
    virtual void setNeighbourNameCount(int neighbourNameCount);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const NeighbourListMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, NeighbourListMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>InternalMsg.msg:35</tt> by nedtool.
 * <pre>
 * //*****************************************************************************************************  /
 * // \@author : João Patrício (castanheirapatricio\@ua.pt)
 * // \@date   : 26-june-2019
 * //
 * //               addrs | Prob 		| myProb	 |	MyPosX 	 | MyPosY	| SSI	    | total  |
 * //               s & d | (4 byte)   |  (4 byte)  | (4 byte)  | (4 bytes)| (4 bytes) | (bytes)
 * // Routing	 ->   6 + 6      4             4           4         4			4			32
 * //
 * // Message that sends Beacon received information to routing layer
 * packet BeaconInfoMsg
 * {
 *     string sourceAddress;
 *     string destinationAddress;
 *     int realPacketSize;
 *     double Prob;
 *     double myProb;
 *     double MyPosX;
 *     double MyPosY;
 *     double SSI;
 * 	//int Nic; //0=wifi, 1=Bt,
 * }
 * </pre>
 */
class BeaconInfoMsg : public ::omnetpp::cPacket
{
  protected:
    ::omnetpp::opp_string sourceAddress;
    ::omnetpp::opp_string destinationAddress;
    int realPacketSize;
    double Prob;
    double myProb;
    double MyPosX;
    double MyPosY;
    double SSI;

  private:
    void copy(const BeaconInfoMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BeaconInfoMsg&);

  public:
    BeaconInfoMsg(const char *name=nullptr, short kind=0);
    BeaconInfoMsg(const BeaconInfoMsg& other);
    virtual ~BeaconInfoMsg();
    BeaconInfoMsg& operator=(const BeaconInfoMsg& other);
    virtual BeaconInfoMsg *dup() const override {return new BeaconInfoMsg(*this);}
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
    virtual double getMyProb() const;
    virtual void setMyProb(double myProb);
    virtual double getMyPosX() const;
    virtual void setMyPosX(double MyPosX);
    virtual double getMyPosY() const;
    virtual void setMyPosY(double MyPosY);
    virtual double getSSI() const;
    virtual void setSSI(double SSI);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const BeaconInfoMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, BeaconInfoMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>InternalMsg.msg:52</tt> by nedtool.
 * <pre>
 * //*****************************************************************************************************  /
 * // \@author : João Patrício (castanheirapatricio\@ua.pt)
 * // \@date   : 23-july-2019
 * // Message to provide the current neighbour list by Bluetooth
 * packet NeighbourListMsgBT
 * {
 *     string neighbourNameList[];
 *     int neighbourNameCount;
 * 
 *     //double neighPos[];
 * }
 * </pre>
 */
class NeighbourListMsgBT : public ::omnetpp::cPacket
{
  protected:
    ::omnetpp::opp_string *neighbourNameList; // array ptr
    unsigned int neighbourNameList_arraysize;
    int neighbourNameCount;

  private:
    void copy(const NeighbourListMsgBT& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NeighbourListMsgBT&);

  public:
    NeighbourListMsgBT(const char *name=nullptr, short kind=0);
    NeighbourListMsgBT(const NeighbourListMsgBT& other);
    virtual ~NeighbourListMsgBT();
    NeighbourListMsgBT& operator=(const NeighbourListMsgBT& other);
    virtual NeighbourListMsgBT *dup() const override {return new NeighbourListMsgBT(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual void setNeighbourNameListArraySize(unsigned int size);
    virtual unsigned int getNeighbourNameListArraySize() const;
    virtual const char * getNeighbourNameList(unsigned int k) const;
    virtual void setNeighbourNameList(unsigned int k, const char * neighbourNameList);
    virtual int getNeighbourNameCount() const;
    virtual void setNeighbourNameCount(int neighbourNameCount);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const NeighbourListMsgBT& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, NeighbourListMsgBT& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>InternalMsg.msg:61</tt> by nedtool.
 * <pre>
 * //*****************************************************************************************************  /
 * // Message of routing commands: indicate the chosen Nic
 * packet routingDecisionsMsg
 * {
 *     bool sendWifiFirst;
 * }
 * </pre>
 */
class routingDecisionsMsg : public ::omnetpp::cPacket
{
  protected:
    bool sendWifiFirst;

  private:
    void copy(const routingDecisionsMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const routingDecisionsMsg&);

  public:
    routingDecisionsMsg(const char *name=nullptr, short kind=0);
    routingDecisionsMsg(const routingDecisionsMsg& other);
    virtual ~routingDecisionsMsg();
    routingDecisionsMsg& operator=(const routingDecisionsMsg& other);
    virtual routingDecisionsMsg *dup() const override {return new routingDecisionsMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual bool getSendWifiFirst() const;
    virtual void setSendWifiFirst(bool sendWifiFirst);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const routingDecisionsMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, routingDecisionsMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>InternalMsg.msg:67</tt> by nedtool.
 * <pre>
 * //*****************************************************************************************************  /
 * // Message to provide the current metwork graph
 * packet NetworkGraphMsg
 * {
 *     //string graphN[];
 *     int numberVert;
 *     //int countVert;
 *     string graphN;
 * 
 *     //double neighPos[];
 * }
 * </pre>
 */
class NetworkGraphMsg : public ::omnetpp::cPacket
{
  protected:
    int numberVert;
    ::omnetpp::opp_string graphN;

  private:
    void copy(const NetworkGraphMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NetworkGraphMsg&);

  public:
    NetworkGraphMsg(const char *name=nullptr, short kind=0);
    NetworkGraphMsg(const NetworkGraphMsg& other);
    virtual ~NetworkGraphMsg();
    NetworkGraphMsg& operator=(const NetworkGraphMsg& other);
    virtual NetworkGraphMsg *dup() const override {return new NetworkGraphMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getNumberVert() const;
    virtual void setNumberVert(int numberVert);
    virtual const char * getGraphN() const;
    virtual void setGraphN(const char * graphN);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const NetworkGraphMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, NetworkGraphMsg& obj) {obj.parsimUnpack(b);}


#endif // ifndef __INTERNALMSG_M_H

