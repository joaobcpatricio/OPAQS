//
// Generated file, do not edit! Created by nedtool 5.4 from OutsMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "OutsMsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(DataMsg)

DataMsg::DataMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->msgUniqueID = 0;
    this->groupType = 50;
    this->injectedTime = 0;
    this->realPacketSize = 0;
    this->realPayloadSize = 0;
    this->validUntilTime = 0;
    this->nHops = 0;
    this->nMsgOrder = 0;
    this->destinationOriented = false;
    prevHopsList_arraysize = 0;
    this->prevHopsList = 0;
    this->sentTimeRout = 0;
    this->receivedTimeRout = 0;
    this->sentTime = 0;
    this->receivedTime = 0;
    this->reached_gw = false;
}

DataMsg::DataMsg(const DataMsg& other) : ::omnetpp::cPacket(other)
{
    prevHopsList_arraysize = 0;
    this->prevHopsList = 0;
    copy(other);
}

DataMsg::~DataMsg()
{
    delete [] this->prevHopsList;
}

DataMsg& DataMsg::operator=(const DataMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void DataMsg::copy(const DataMsg& other)
{
    this->sourceAddress = other.sourceAddress;
    this->destinationAddress = other.destinationAddress;
    this->dataName = other.dataName;
    this->messageID = other.messageID;
    this->msgUniqueID = other.msgUniqueID;
    this->groupType = other.groupType;
    this->injectedTime = other.injectedTime;
    this->realPacketSize = other.realPacketSize;
    this->realPayloadSize = other.realPayloadSize;
    this->validUntilTime = other.validUntilTime;
    this->finalDestinationNodeName = other.finalDestinationNodeName;
    this->originatorNodeMAC = other.originatorNodeMAC;
    this->nHops = other.nHops;
    this->nMsgOrder = other.nMsgOrder;
    this->destinationOriented = other.destinationOriented;
    delete [] this->prevHopsList;
    this->prevHopsList = (other.prevHopsList_arraysize==0) ? nullptr : new ::omnetpp::opp_string[other.prevHopsList_arraysize];
    prevHopsList_arraysize = other.prevHopsList_arraysize;
    for (unsigned int i=0; i<prevHopsList_arraysize; i++)
        this->prevHopsList[i] = other.prevHopsList[i];
    this->sentTimeRout = other.sentTimeRout;
    this->receivedTimeRout = other.receivedTimeRout;
    this->sentTime = other.sentTime;
    this->receivedTime = other.receivedTime;
    this->reached_gw = other.reached_gw;
}

void DataMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sourceAddress);
    doParsimPacking(b,this->destinationAddress);
    doParsimPacking(b,this->dataName);
    doParsimPacking(b,this->messageID);
    doParsimPacking(b,this->msgUniqueID);
    doParsimPacking(b,this->groupType);
    doParsimPacking(b,this->injectedTime);
    doParsimPacking(b,this->realPacketSize);
    doParsimPacking(b,this->realPayloadSize);
    doParsimPacking(b,this->validUntilTime);
    doParsimPacking(b,this->finalDestinationNodeName);
    doParsimPacking(b,this->originatorNodeMAC);
    doParsimPacking(b,this->nHops);
    doParsimPacking(b,this->nMsgOrder);
    doParsimPacking(b,this->destinationOriented);
    b->pack(prevHopsList_arraysize);
    doParsimArrayPacking(b,this->prevHopsList,prevHopsList_arraysize);
    doParsimPacking(b,this->sentTimeRout);
    doParsimPacking(b,this->receivedTimeRout);
    doParsimPacking(b,this->sentTime);
    doParsimPacking(b,this->receivedTime);
    doParsimPacking(b,this->reached_gw);
}

void DataMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceAddress);
    doParsimUnpacking(b,this->destinationAddress);
    doParsimUnpacking(b,this->dataName);
    doParsimUnpacking(b,this->messageID);
    doParsimUnpacking(b,this->msgUniqueID);
    doParsimUnpacking(b,this->groupType);
    doParsimUnpacking(b,this->injectedTime);
    doParsimUnpacking(b,this->realPacketSize);
    doParsimUnpacking(b,this->realPayloadSize);
    doParsimUnpacking(b,this->validUntilTime);
    doParsimUnpacking(b,this->finalDestinationNodeName);
    doParsimUnpacking(b,this->originatorNodeMAC);
    doParsimUnpacking(b,this->nHops);
    doParsimUnpacking(b,this->nMsgOrder);
    doParsimUnpacking(b,this->destinationOriented);
    delete [] this->prevHopsList;
    b->unpack(prevHopsList_arraysize);
    if (prevHopsList_arraysize==0) {
        this->prevHopsList = 0;
    } else {
        this->prevHopsList = new ::omnetpp::opp_string[prevHopsList_arraysize];
        doParsimArrayUnpacking(b,this->prevHopsList,prevHopsList_arraysize);
    }
    doParsimUnpacking(b,this->sentTimeRout);
    doParsimUnpacking(b,this->receivedTimeRout);
    doParsimUnpacking(b,this->sentTime);
    doParsimUnpacking(b,this->receivedTime);
    doParsimUnpacking(b,this->reached_gw);
}

const char * DataMsg::getSourceAddress() const
{
    return this->sourceAddress.c_str();
}

void DataMsg::setSourceAddress(const char * sourceAddress)
{
    this->sourceAddress = sourceAddress;
}

const char * DataMsg::getDestinationAddress() const
{
    return this->destinationAddress.c_str();
}

void DataMsg::setDestinationAddress(const char * destinationAddress)
{
    this->destinationAddress = destinationAddress;
}

const char * DataMsg::getDataName() const
{
    return this->dataName.c_str();
}

void DataMsg::setDataName(const char * dataName)
{
    this->dataName = dataName;
}

const char * DataMsg::getMessageID() const
{
    return this->messageID.c_str();
}

void DataMsg::setMessageID(const char * messageID)
{
    this->messageID = messageID;
}

int DataMsg::getMsgUniqueID() const
{
    return this->msgUniqueID;
}

void DataMsg::setMsgUniqueID(int msgUniqueID)
{
    this->msgUniqueID = msgUniqueID;
}

int DataMsg::getGroupType() const
{
    return this->groupType;
}

void DataMsg::setGroupType(int groupType)
{
    this->groupType = groupType;
}

::omnetpp::simtime_t DataMsg::getInjectedTime() const
{
    return this->injectedTime;
}

void DataMsg::setInjectedTime(::omnetpp::simtime_t injectedTime)
{
    this->injectedTime = injectedTime;
}

int DataMsg::getRealPacketSize() const
{
    return this->realPacketSize;
}

void DataMsg::setRealPacketSize(int realPacketSize)
{
    this->realPacketSize = realPacketSize;
}

int DataMsg::getRealPayloadSize() const
{
    return this->realPayloadSize;
}

void DataMsg::setRealPayloadSize(int realPayloadSize)
{
    this->realPayloadSize = realPayloadSize;
}

double DataMsg::getValidUntilTime() const
{
    return this->validUntilTime;
}

void DataMsg::setValidUntilTime(double validUntilTime)
{
    this->validUntilTime = validUntilTime;
}

const char * DataMsg::getFinalDestinationNodeName() const
{
    return this->finalDestinationNodeName.c_str();
}

void DataMsg::setFinalDestinationNodeName(const char * finalDestinationNodeName)
{
    this->finalDestinationNodeName = finalDestinationNodeName;
}

const char * DataMsg::getOriginatorNodeMAC() const
{
    return this->originatorNodeMAC.c_str();
}

void DataMsg::setOriginatorNodeMAC(const char * originatorNodeMAC)
{
    this->originatorNodeMAC = originatorNodeMAC;
}

int DataMsg::getNHops() const
{
    return this->nHops;
}

void DataMsg::setNHops(int nHops)
{
    this->nHops = nHops;
}

int DataMsg::getNMsgOrder() const
{
    return this->nMsgOrder;
}

void DataMsg::setNMsgOrder(int nMsgOrder)
{
    this->nMsgOrder = nMsgOrder;
}

bool DataMsg::getDestinationOriented() const
{
    return this->destinationOriented;
}

void DataMsg::setDestinationOriented(bool destinationOriented)
{
    this->destinationOriented = destinationOriented;
}

void DataMsg::setPrevHopsListArraySize(unsigned int size)
{
    ::omnetpp::opp_string *prevHopsList2 = (size==0) ? nullptr : new ::omnetpp::opp_string[size];
    unsigned int sz = prevHopsList_arraysize < size ? prevHopsList_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        prevHopsList2[i] = this->prevHopsList[i];
    for (unsigned int i=sz; i<size; i++)
        prevHopsList2[i] = 0;
    prevHopsList_arraysize = size;
    delete [] this->prevHopsList;
    this->prevHopsList = prevHopsList2;
}

unsigned int DataMsg::getPrevHopsListArraySize() const
{
    return prevHopsList_arraysize;
}

const char * DataMsg::getPrevHopsList(unsigned int k) const
{
    if (k>=prevHopsList_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", prevHopsList_arraysize, k);
    return this->prevHopsList[k].c_str();
}

void DataMsg::setPrevHopsList(unsigned int k, const char * prevHopsList)
{
    if (k>=prevHopsList_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", prevHopsList_arraysize, k);
    this->prevHopsList[k] = prevHopsList;
}

::omnetpp::simtime_t DataMsg::getSentTimeRout() const
{
    return this->sentTimeRout;
}

void DataMsg::setSentTimeRout(::omnetpp::simtime_t sentTimeRout)
{
    this->sentTimeRout = sentTimeRout;
}

::omnetpp::simtime_t DataMsg::getReceivedTimeRout() const
{
    return this->receivedTimeRout;
}

void DataMsg::setReceivedTimeRout(::omnetpp::simtime_t receivedTimeRout)
{
    this->receivedTimeRout = receivedTimeRout;
}

::omnetpp::simtime_t DataMsg::getSentTime() const
{
    return this->sentTime;
}

void DataMsg::setSentTime(::omnetpp::simtime_t sentTime)
{
    this->sentTime = sentTime;
}

::omnetpp::simtime_t DataMsg::getReceivedTime() const
{
    return this->receivedTime;
}

void DataMsg::setReceivedTime(::omnetpp::simtime_t receivedTime)
{
    this->receivedTime = receivedTime;
}

bool DataMsg::getReached_gw() const
{
    return this->reached_gw;
}

void DataMsg::setReached_gw(bool reached_gw)
{
    this->reached_gw = reached_gw;
}

class DataMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    DataMsgDescriptor();
    virtual ~DataMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(DataMsgDescriptor)

DataMsgDescriptor::DataMsgDescriptor() : omnetpp::cClassDescriptor("DataMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

DataMsgDescriptor::~DataMsgDescriptor()
{
    delete[] propertynames;
}

bool DataMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DataMsg *>(obj)!=nullptr;
}

const char **DataMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *DataMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int DataMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 21+basedesc->getFieldCount() : 21;
}

unsigned int DataMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<21) ? fieldTypeFlags[field] : 0;
}

const char *DataMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sourceAddress",
        "destinationAddress",
        "dataName",
        "messageID",
        "msgUniqueID",
        "groupType",
        "injectedTime",
        "realPacketSize",
        "realPayloadSize",
        "validUntilTime",
        "finalDestinationNodeName",
        "originatorNodeMAC",
        "nHops",
        "nMsgOrder",
        "destinationOriented",
        "prevHopsList",
        "sentTimeRout",
        "receivedTimeRout",
        "sentTime",
        "receivedTime",
        "reached_gw",
    };
    return (field>=0 && field<21) ? fieldNames[field] : nullptr;
}

int DataMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceAddress")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationAddress")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataName")==0) return base+2;
    if (fieldName[0]=='m' && strcmp(fieldName, "messageID")==0) return base+3;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgUniqueID")==0) return base+4;
    if (fieldName[0]=='g' && strcmp(fieldName, "groupType")==0) return base+5;
    if (fieldName[0]=='i' && strcmp(fieldName, "injectedTime")==0) return base+6;
    if (fieldName[0]=='r' && strcmp(fieldName, "realPacketSize")==0) return base+7;
    if (fieldName[0]=='r' && strcmp(fieldName, "realPayloadSize")==0) return base+8;
    if (fieldName[0]=='v' && strcmp(fieldName, "validUntilTime")==0) return base+9;
    if (fieldName[0]=='f' && strcmp(fieldName, "finalDestinationNodeName")==0) return base+10;
    if (fieldName[0]=='o' && strcmp(fieldName, "originatorNodeMAC")==0) return base+11;
    if (fieldName[0]=='n' && strcmp(fieldName, "nHops")==0) return base+12;
    if (fieldName[0]=='n' && strcmp(fieldName, "nMsgOrder")==0) return base+13;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationOriented")==0) return base+14;
    if (fieldName[0]=='p' && strcmp(fieldName, "prevHopsList")==0) return base+15;
    if (fieldName[0]=='s' && strcmp(fieldName, "sentTimeRout")==0) return base+16;
    if (fieldName[0]=='r' && strcmp(fieldName, "receivedTimeRout")==0) return base+17;
    if (fieldName[0]=='s' && strcmp(fieldName, "sentTime")==0) return base+18;
    if (fieldName[0]=='r' && strcmp(fieldName, "receivedTime")==0) return base+19;
    if (fieldName[0]=='r' && strcmp(fieldName, "reached_gw")==0) return base+20;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *DataMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "string",
        "string",
        "int",
        "int",
        "simtime_t",
        "int",
        "int",
        "double",
        "string",
        "string",
        "int",
        "int",
        "bool",
        "string",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "bool",
    };
    return (field>=0 && field<21) ? fieldTypeStrings[field] : nullptr;
}

const char **DataMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *DataMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int DataMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    DataMsg *pp = (DataMsg *)object; (void)pp;
    switch (field) {
        case 15: return pp->getPrevHopsListArraySize();
        default: return 0;
    }
}

const char *DataMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DataMsg *pp = (DataMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DataMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DataMsg *pp = (DataMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getSourceAddress());
        case 1: return oppstring2string(pp->getDestinationAddress());
        case 2: return oppstring2string(pp->getDataName());
        case 3: return oppstring2string(pp->getMessageID());
        case 4: return long2string(pp->getMsgUniqueID());
        case 5: return long2string(pp->getGroupType());
        case 6: return simtime2string(pp->getInjectedTime());
        case 7: return long2string(pp->getRealPacketSize());
        case 8: return long2string(pp->getRealPayloadSize());
        case 9: return double2string(pp->getValidUntilTime());
        case 10: return oppstring2string(pp->getFinalDestinationNodeName());
        case 11: return oppstring2string(pp->getOriginatorNodeMAC());
        case 12: return long2string(pp->getNHops());
        case 13: return long2string(pp->getNMsgOrder());
        case 14: return bool2string(pp->getDestinationOriented());
        case 15: return oppstring2string(pp->getPrevHopsList(i));
        case 16: return simtime2string(pp->getSentTimeRout());
        case 17: return simtime2string(pp->getReceivedTimeRout());
        case 18: return simtime2string(pp->getSentTime());
        case 19: return simtime2string(pp->getReceivedTime());
        case 20: return bool2string(pp->getReached_gw());
        default: return "";
    }
}

bool DataMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    DataMsg *pp = (DataMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourceAddress((value)); return true;
        case 1: pp->setDestinationAddress((value)); return true;
        case 2: pp->setDataName((value)); return true;
        case 3: pp->setMessageID((value)); return true;
        case 4: pp->setMsgUniqueID(string2long(value)); return true;
        case 5: pp->setGroupType(string2long(value)); return true;
        case 6: pp->setInjectedTime(string2simtime(value)); return true;
        case 7: pp->setRealPacketSize(string2long(value)); return true;
        case 8: pp->setRealPayloadSize(string2long(value)); return true;
        case 9: pp->setValidUntilTime(string2double(value)); return true;
        case 10: pp->setFinalDestinationNodeName((value)); return true;
        case 11: pp->setOriginatorNodeMAC((value)); return true;
        case 12: pp->setNHops(string2long(value)); return true;
        case 13: pp->setNMsgOrder(string2long(value)); return true;
        case 14: pp->setDestinationOriented(string2bool(value)); return true;
        case 15: pp->setPrevHopsList(i,(value)); return true;
        case 16: pp->setSentTimeRout(string2simtime(value)); return true;
        case 17: pp->setReceivedTimeRout(string2simtime(value)); return true;
        case 18: pp->setSentTime(string2simtime(value)); return true;
        case 19: pp->setReceivedTime(string2simtime(value)); return true;
        case 20: pp->setReached_gw(string2bool(value)); return true;
        default: return false;
    }
}

const char *DataMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *DataMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    DataMsg *pp = (DataMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(AckMsg)

AckMsg::AckMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->realPacketSize = 0;
    this->isFinalDest = false;
    this->injectedTime = 0;
}

AckMsg::AckMsg(const AckMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

AckMsg::~AckMsg()
{
}

AckMsg& AckMsg::operator=(const AckMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void AckMsg::copy(const AckMsg& other)
{
    this->sourceAddress = other.sourceAddress;
    this->destinationAddress = other.destinationAddress;
    this->realPacketSize = other.realPacketSize;
    this->messageID = other.messageID;
    this->isFinalDest = other.isFinalDest;
    this->injectedTime = other.injectedTime;
}

void AckMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sourceAddress);
    doParsimPacking(b,this->destinationAddress);
    doParsimPacking(b,this->realPacketSize);
    doParsimPacking(b,this->messageID);
    doParsimPacking(b,this->isFinalDest);
    doParsimPacking(b,this->injectedTime);
}

void AckMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceAddress);
    doParsimUnpacking(b,this->destinationAddress);
    doParsimUnpacking(b,this->realPacketSize);
    doParsimUnpacking(b,this->messageID);
    doParsimUnpacking(b,this->isFinalDest);
    doParsimUnpacking(b,this->injectedTime);
}

const char * AckMsg::getSourceAddress() const
{
    return this->sourceAddress.c_str();
}

void AckMsg::setSourceAddress(const char * sourceAddress)
{
    this->sourceAddress = sourceAddress;
}

const char * AckMsg::getDestinationAddress() const
{
    return this->destinationAddress.c_str();
}

void AckMsg::setDestinationAddress(const char * destinationAddress)
{
    this->destinationAddress = destinationAddress;
}

int AckMsg::getRealPacketSize() const
{
    return this->realPacketSize;
}

void AckMsg::setRealPacketSize(int realPacketSize)
{
    this->realPacketSize = realPacketSize;
}

const char * AckMsg::getMessageID() const
{
    return this->messageID.c_str();
}

void AckMsg::setMessageID(const char * messageID)
{
    this->messageID = messageID;
}

bool AckMsg::getIsFinalDest() const
{
    return this->isFinalDest;
}

void AckMsg::setIsFinalDest(bool isFinalDest)
{
    this->isFinalDest = isFinalDest;
}

::omnetpp::simtime_t AckMsg::getInjectedTime() const
{
    return this->injectedTime;
}

void AckMsg::setInjectedTime(::omnetpp::simtime_t injectedTime)
{
    this->injectedTime = injectedTime;
}

class AckMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    AckMsgDescriptor();
    virtual ~AckMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(AckMsgDescriptor)

AckMsgDescriptor::AckMsgDescriptor() : omnetpp::cClassDescriptor("AckMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

AckMsgDescriptor::~AckMsgDescriptor()
{
    delete[] propertynames;
}

bool AckMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<AckMsg *>(obj)!=nullptr;
}

const char **AckMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *AckMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int AckMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int AckMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *AckMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sourceAddress",
        "destinationAddress",
        "realPacketSize",
        "messageID",
        "isFinalDest",
        "injectedTime",
    };
    return (field>=0 && field<6) ? fieldNames[field] : nullptr;
}

int AckMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceAddress")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationAddress")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "realPacketSize")==0) return base+2;
    if (fieldName[0]=='m' && strcmp(fieldName, "messageID")==0) return base+3;
    if (fieldName[0]=='i' && strcmp(fieldName, "isFinalDest")==0) return base+4;
    if (fieldName[0]=='i' && strcmp(fieldName, "injectedTime")==0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *AckMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "int",
        "string",
        "bool",
        "simtime_t",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **AckMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *AckMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int AckMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    AckMsg *pp = (AckMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *AckMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    AckMsg *pp = (AckMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string AckMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    AckMsg *pp = (AckMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getSourceAddress());
        case 1: return oppstring2string(pp->getDestinationAddress());
        case 2: return long2string(pp->getRealPacketSize());
        case 3: return oppstring2string(pp->getMessageID());
        case 4: return bool2string(pp->getIsFinalDest());
        case 5: return simtime2string(pp->getInjectedTime());
        default: return "";
    }
}

bool AckMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    AckMsg *pp = (AckMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourceAddress((value)); return true;
        case 1: pp->setDestinationAddress((value)); return true;
        case 2: pp->setRealPacketSize(string2long(value)); return true;
        case 3: pp->setMessageID((value)); return true;
        case 4: pp->setIsFinalDest(string2bool(value)); return true;
        case 5: pp->setInjectedTime(string2simtime(value)); return true;
        default: return false;
    }
}

const char *AckMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *AckMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    AckMsg *pp = (AckMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(BeaconMsg)

BeaconMsg::BeaconMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->realPacketSize = 0;
    this->Prob = 0;
    this->MyPosX = 0;
    this->MyPosY = 0;
    this->numberVert = 0;
    this->sentTime = 0;
    this->receivedTime = 0;
    this->injectedTime = 0;
}

BeaconMsg::BeaconMsg(const BeaconMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

BeaconMsg::~BeaconMsg()
{
}

BeaconMsg& BeaconMsg::operator=(const BeaconMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void BeaconMsg::copy(const BeaconMsg& other)
{
    this->sourceAddress = other.sourceAddress;
    this->destinationAddress = other.destinationAddress;
    this->realPacketSize = other.realPacketSize;
    this->Prob = other.Prob;
    this->MyPosX = other.MyPosX;
    this->MyPosY = other.MyPosY;
    this->neighGraph = other.neighGraph;
    this->numberVert = other.numberVert;
    this->sentTime = other.sentTime;
    this->receivedTime = other.receivedTime;
    this->injectedTime = other.injectedTime;
    this->neighEner = other.neighEner;
}

void BeaconMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sourceAddress);
    doParsimPacking(b,this->destinationAddress);
    doParsimPacking(b,this->realPacketSize);
    doParsimPacking(b,this->Prob);
    doParsimPacking(b,this->MyPosX);
    doParsimPacking(b,this->MyPosY);
    doParsimPacking(b,this->neighGraph);
    doParsimPacking(b,this->numberVert);
    doParsimPacking(b,this->sentTime);
    doParsimPacking(b,this->receivedTime);
    doParsimPacking(b,this->injectedTime);
    doParsimPacking(b,this->neighEner);
}

void BeaconMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceAddress);
    doParsimUnpacking(b,this->destinationAddress);
    doParsimUnpacking(b,this->realPacketSize);
    doParsimUnpacking(b,this->Prob);
    doParsimUnpacking(b,this->MyPosX);
    doParsimUnpacking(b,this->MyPosY);
    doParsimUnpacking(b,this->neighGraph);
    doParsimUnpacking(b,this->numberVert);
    doParsimUnpacking(b,this->sentTime);
    doParsimUnpacking(b,this->receivedTime);
    doParsimUnpacking(b,this->injectedTime);
    doParsimUnpacking(b,this->neighEner);
}

const char * BeaconMsg::getSourceAddress() const
{
    return this->sourceAddress.c_str();
}

void BeaconMsg::setSourceAddress(const char * sourceAddress)
{
    this->sourceAddress = sourceAddress;
}

const char * BeaconMsg::getDestinationAddress() const
{
    return this->destinationAddress.c_str();
}

void BeaconMsg::setDestinationAddress(const char * destinationAddress)
{
    this->destinationAddress = destinationAddress;
}

int BeaconMsg::getRealPacketSize() const
{
    return this->realPacketSize;
}

void BeaconMsg::setRealPacketSize(int realPacketSize)
{
    this->realPacketSize = realPacketSize;
}

double BeaconMsg::getProb() const
{
    return this->Prob;
}

void BeaconMsg::setProb(double Prob)
{
    this->Prob = Prob;
}

double BeaconMsg::getMyPosX() const
{
    return this->MyPosX;
}

void BeaconMsg::setMyPosX(double MyPosX)
{
    this->MyPosX = MyPosX;
}

double BeaconMsg::getMyPosY() const
{
    return this->MyPosY;
}

void BeaconMsg::setMyPosY(double MyPosY)
{
    this->MyPosY = MyPosY;
}

const char * BeaconMsg::getNeighGraph() const
{
    return this->neighGraph.c_str();
}

void BeaconMsg::setNeighGraph(const char * neighGraph)
{
    this->neighGraph = neighGraph;
}

int BeaconMsg::getNumberVert() const
{
    return this->numberVert;
}

void BeaconMsg::setNumberVert(int numberVert)
{
    this->numberVert = numberVert;
}

::omnetpp::simtime_t BeaconMsg::getSentTime() const
{
    return this->sentTime;
}

void BeaconMsg::setSentTime(::omnetpp::simtime_t sentTime)
{
    this->sentTime = sentTime;
}

::omnetpp::simtime_t BeaconMsg::getReceivedTime() const
{
    return this->receivedTime;
}

void BeaconMsg::setReceivedTime(::omnetpp::simtime_t receivedTime)
{
    this->receivedTime = receivedTime;
}

::omnetpp::simtime_t BeaconMsg::getInjectedTime() const
{
    return this->injectedTime;
}

void BeaconMsg::setInjectedTime(::omnetpp::simtime_t injectedTime)
{
    this->injectedTime = injectedTime;
}

const char * BeaconMsg::getNeighEner() const
{
    return this->neighEner.c_str();
}

void BeaconMsg::setNeighEner(const char * neighEner)
{
    this->neighEner = neighEner;
}

class BeaconMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    BeaconMsgDescriptor();
    virtual ~BeaconMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(BeaconMsgDescriptor)

BeaconMsgDescriptor::BeaconMsgDescriptor() : omnetpp::cClassDescriptor("BeaconMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

BeaconMsgDescriptor::~BeaconMsgDescriptor()
{
    delete[] propertynames;
}

bool BeaconMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BeaconMsg *>(obj)!=nullptr;
}

const char **BeaconMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *BeaconMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int BeaconMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount() : 12;
}

unsigned int BeaconMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
}

const char *BeaconMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sourceAddress",
        "destinationAddress",
        "realPacketSize",
        "Prob",
        "MyPosX",
        "MyPosY",
        "neighGraph",
        "numberVert",
        "sentTime",
        "receivedTime",
        "injectedTime",
        "neighEner",
    };
    return (field>=0 && field<12) ? fieldNames[field] : nullptr;
}

int BeaconMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceAddress")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationAddress")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "realPacketSize")==0) return base+2;
    if (fieldName[0]=='P' && strcmp(fieldName, "Prob")==0) return base+3;
    if (fieldName[0]=='M' && strcmp(fieldName, "MyPosX")==0) return base+4;
    if (fieldName[0]=='M' && strcmp(fieldName, "MyPosY")==0) return base+5;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighGraph")==0) return base+6;
    if (fieldName[0]=='n' && strcmp(fieldName, "numberVert")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "sentTime")==0) return base+8;
    if (fieldName[0]=='r' && strcmp(fieldName, "receivedTime")==0) return base+9;
    if (fieldName[0]=='i' && strcmp(fieldName, "injectedTime")==0) return base+10;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighEner")==0) return base+11;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *BeaconMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "int",
        "double",
        "double",
        "double",
        "string",
        "int",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "string",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : nullptr;
}

const char **BeaconMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BeaconMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BeaconMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    BeaconMsg *pp = (BeaconMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *BeaconMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BeaconMsg *pp = (BeaconMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BeaconMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BeaconMsg *pp = (BeaconMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getSourceAddress());
        case 1: return oppstring2string(pp->getDestinationAddress());
        case 2: return long2string(pp->getRealPacketSize());
        case 3: return double2string(pp->getProb());
        case 4: return double2string(pp->getMyPosX());
        case 5: return double2string(pp->getMyPosY());
        case 6: return oppstring2string(pp->getNeighGraph());
        case 7: return long2string(pp->getNumberVert());
        case 8: return simtime2string(pp->getSentTime());
        case 9: return simtime2string(pp->getReceivedTime());
        case 10: return simtime2string(pp->getInjectedTime());
        case 11: return oppstring2string(pp->getNeighEner());
        default: return "";
    }
}

bool BeaconMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    BeaconMsg *pp = (BeaconMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourceAddress((value)); return true;
        case 1: pp->setDestinationAddress((value)); return true;
        case 2: pp->setRealPacketSize(string2long(value)); return true;
        case 3: pp->setProb(string2double(value)); return true;
        case 4: pp->setMyPosX(string2double(value)); return true;
        case 5: pp->setMyPosY(string2double(value)); return true;
        case 6: pp->setNeighGraph((value)); return true;
        case 7: pp->setNumberVert(string2long(value)); return true;
        case 8: pp->setSentTime(string2simtime(value)); return true;
        case 9: pp->setReceivedTime(string2simtime(value)); return true;
        case 10: pp->setInjectedTime(string2simtime(value)); return true;
        case 11: pp->setNeighEner((value)); return true;
        default: return false;
    }
}

const char *BeaconMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *BeaconMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    BeaconMsg *pp = (BeaconMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(DataReqMsg)

DataReqMsg::DataReqMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->realPacketSize = 0;
    this->SendMeData = false;
    this->Prob = 0;
    this->SSI = 0;
    this->beaconSentT = 0;
    this->injectedTime = 0;
    this->sentTime = 0;
    this->receivedTime = 0;
}

DataReqMsg::DataReqMsg(const DataReqMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

DataReqMsg::~DataReqMsg()
{
}

DataReqMsg& DataReqMsg::operator=(const DataReqMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void DataReqMsg::copy(const DataReqMsg& other)
{
    this->sourceAddress = other.sourceAddress;
    this->destinationAddress = other.destinationAddress;
    this->realPacketSize = other.realPacketSize;
    this->SendMeData = other.SendMeData;
    this->Prob = other.Prob;
    this->SSI = other.SSI;
    this->beaconSentT = other.beaconSentT;
    this->injectedTime = other.injectedTime;
    this->sentTime = other.sentTime;
    this->receivedTime = other.receivedTime;
}

void DataReqMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sourceAddress);
    doParsimPacking(b,this->destinationAddress);
    doParsimPacking(b,this->realPacketSize);
    doParsimPacking(b,this->SendMeData);
    doParsimPacking(b,this->Prob);
    doParsimPacking(b,this->SSI);
    doParsimPacking(b,this->beaconSentT);
    doParsimPacking(b,this->injectedTime);
    doParsimPacking(b,this->sentTime);
    doParsimPacking(b,this->receivedTime);
}

void DataReqMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceAddress);
    doParsimUnpacking(b,this->destinationAddress);
    doParsimUnpacking(b,this->realPacketSize);
    doParsimUnpacking(b,this->SendMeData);
    doParsimUnpacking(b,this->Prob);
    doParsimUnpacking(b,this->SSI);
    doParsimUnpacking(b,this->beaconSentT);
    doParsimUnpacking(b,this->injectedTime);
    doParsimUnpacking(b,this->sentTime);
    doParsimUnpacking(b,this->receivedTime);
}

const char * DataReqMsg::getSourceAddress() const
{
    return this->sourceAddress.c_str();
}

void DataReqMsg::setSourceAddress(const char * sourceAddress)
{
    this->sourceAddress = sourceAddress;
}

const char * DataReqMsg::getDestinationAddress() const
{
    return this->destinationAddress.c_str();
}

void DataReqMsg::setDestinationAddress(const char * destinationAddress)
{
    this->destinationAddress = destinationAddress;
}

int DataReqMsg::getRealPacketSize() const
{
    return this->realPacketSize;
}

void DataReqMsg::setRealPacketSize(int realPacketSize)
{
    this->realPacketSize = realPacketSize;
}

bool DataReqMsg::getSendMeData() const
{
    return this->SendMeData;
}

void DataReqMsg::setSendMeData(bool SendMeData)
{
    this->SendMeData = SendMeData;
}

double DataReqMsg::getProb() const
{
    return this->Prob;
}

void DataReqMsg::setProb(double Prob)
{
    this->Prob = Prob;
}

double DataReqMsg::getSSI() const
{
    return this->SSI;
}

void DataReqMsg::setSSI(double SSI)
{
    this->SSI = SSI;
}

::omnetpp::simtime_t DataReqMsg::getBeaconSentT() const
{
    return this->beaconSentT;
}

void DataReqMsg::setBeaconSentT(::omnetpp::simtime_t beaconSentT)
{
    this->beaconSentT = beaconSentT;
}

::omnetpp::simtime_t DataReqMsg::getInjectedTime() const
{
    return this->injectedTime;
}

void DataReqMsg::setInjectedTime(::omnetpp::simtime_t injectedTime)
{
    this->injectedTime = injectedTime;
}

::omnetpp::simtime_t DataReqMsg::getSentTime() const
{
    return this->sentTime;
}

void DataReqMsg::setSentTime(::omnetpp::simtime_t sentTime)
{
    this->sentTime = sentTime;
}

::omnetpp::simtime_t DataReqMsg::getReceivedTime() const
{
    return this->receivedTime;
}

void DataReqMsg::setReceivedTime(::omnetpp::simtime_t receivedTime)
{
    this->receivedTime = receivedTime;
}

class DataReqMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    DataReqMsgDescriptor();
    virtual ~DataReqMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(DataReqMsgDescriptor)

DataReqMsgDescriptor::DataReqMsgDescriptor() : omnetpp::cClassDescriptor("DataReqMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

DataReqMsgDescriptor::~DataReqMsgDescriptor()
{
    delete[] propertynames;
}

bool DataReqMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DataReqMsg *>(obj)!=nullptr;
}

const char **DataReqMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *DataReqMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int DataReqMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 10+basedesc->getFieldCount() : 10;
}

unsigned int DataReqMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<10) ? fieldTypeFlags[field] : 0;
}

const char *DataReqMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sourceAddress",
        "destinationAddress",
        "realPacketSize",
        "SendMeData",
        "Prob",
        "SSI",
        "beaconSentT",
        "injectedTime",
        "sentTime",
        "receivedTime",
    };
    return (field>=0 && field<10) ? fieldNames[field] : nullptr;
}

int DataReqMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceAddress")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationAddress")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "realPacketSize")==0) return base+2;
    if (fieldName[0]=='S' && strcmp(fieldName, "SendMeData")==0) return base+3;
    if (fieldName[0]=='P' && strcmp(fieldName, "Prob")==0) return base+4;
    if (fieldName[0]=='S' && strcmp(fieldName, "SSI")==0) return base+5;
    if (fieldName[0]=='b' && strcmp(fieldName, "beaconSentT")==0) return base+6;
    if (fieldName[0]=='i' && strcmp(fieldName, "injectedTime")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "sentTime")==0) return base+8;
    if (fieldName[0]=='r' && strcmp(fieldName, "receivedTime")==0) return base+9;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *DataReqMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "int",
        "bool",
        "double",
        "double",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
    };
    return (field>=0 && field<10) ? fieldTypeStrings[field] : nullptr;
}

const char **DataReqMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *DataReqMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int DataReqMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    DataReqMsg *pp = (DataReqMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *DataReqMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DataReqMsg *pp = (DataReqMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DataReqMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    DataReqMsg *pp = (DataReqMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getSourceAddress());
        case 1: return oppstring2string(pp->getDestinationAddress());
        case 2: return long2string(pp->getRealPacketSize());
        case 3: return bool2string(pp->getSendMeData());
        case 4: return double2string(pp->getProb());
        case 5: return double2string(pp->getSSI());
        case 6: return simtime2string(pp->getBeaconSentT());
        case 7: return simtime2string(pp->getInjectedTime());
        case 8: return simtime2string(pp->getSentTime());
        case 9: return simtime2string(pp->getReceivedTime());
        default: return "";
    }
}

bool DataReqMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    DataReqMsg *pp = (DataReqMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourceAddress((value)); return true;
        case 1: pp->setDestinationAddress((value)); return true;
        case 2: pp->setRealPacketSize(string2long(value)); return true;
        case 3: pp->setSendMeData(string2bool(value)); return true;
        case 4: pp->setProb(string2double(value)); return true;
        case 5: pp->setSSI(string2double(value)); return true;
        case 6: pp->setBeaconSentT(string2simtime(value)); return true;
        case 7: pp->setInjectedTime(string2simtime(value)); return true;
        case 8: pp->setSentTime(string2simtime(value)); return true;
        case 9: pp->setReceivedTime(string2simtime(value)); return true;
        default: return false;
    }
}

const char *DataReqMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *DataReqMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    DataReqMsg *pp = (DataReqMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


