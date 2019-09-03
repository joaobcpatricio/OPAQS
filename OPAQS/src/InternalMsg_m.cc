//
// Generated file, do not edit! Created by nedtool 5.4 from InternalMsg.msg.
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
#include "InternalMsg_m.h"

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

Register_Class(RegisterAppMsg)

RegisterAppMsg::RegisterAppMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
}

RegisterAppMsg::RegisterAppMsg(const RegisterAppMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

RegisterAppMsg::~RegisterAppMsg()
{
}

RegisterAppMsg& RegisterAppMsg::operator=(const RegisterAppMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void RegisterAppMsg::copy(const RegisterAppMsg& other)
{
    this->appName = other.appName;
    this->prefixName = other.prefixName;
}

void RegisterAppMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->appName);
    doParsimPacking(b,this->prefixName);
}

void RegisterAppMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->appName);
    doParsimUnpacking(b,this->prefixName);
}

const char * RegisterAppMsg::getAppName() const
{
    return this->appName.c_str();
}

void RegisterAppMsg::setAppName(const char * appName)
{
    this->appName = appName;
}

const char * RegisterAppMsg::getPrefixName() const
{
    return this->prefixName.c_str();
}

void RegisterAppMsg::setPrefixName(const char * prefixName)
{
    this->prefixName = prefixName;
}

class RegisterAppMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    RegisterAppMsgDescriptor();
    virtual ~RegisterAppMsgDescriptor();

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

Register_ClassDescriptor(RegisterAppMsgDescriptor)

RegisterAppMsgDescriptor::RegisterAppMsgDescriptor() : omnetpp::cClassDescriptor("RegisterAppMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

RegisterAppMsgDescriptor::~RegisterAppMsgDescriptor()
{
    delete[] propertynames;
}

bool RegisterAppMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RegisterAppMsg *>(obj)!=nullptr;
}

const char **RegisterAppMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *RegisterAppMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int RegisterAppMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int RegisterAppMsgDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *RegisterAppMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "appName",
        "prefixName",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int RegisterAppMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "appName")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "prefixName")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *RegisterAppMsgDescriptor::getFieldTypeString(int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **RegisterAppMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *RegisterAppMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int RegisterAppMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    RegisterAppMsg *pp = (RegisterAppMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *RegisterAppMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RegisterAppMsg *pp = (RegisterAppMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RegisterAppMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RegisterAppMsg *pp = (RegisterAppMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getAppName());
        case 1: return oppstring2string(pp->getPrefixName());
        default: return "";
    }
}

bool RegisterAppMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    RegisterAppMsg *pp = (RegisterAppMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setAppName((value)); return true;
        case 1: pp->setPrefixName((value)); return true;
        default: return false;
    }
}

const char *RegisterAppMsgDescriptor::getFieldStructName(int field) const
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

void *RegisterAppMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    RegisterAppMsg *pp = (RegisterAppMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(NeighbourListMsg)

NeighbourListMsg::NeighbourListMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    neighbourNameList_arraysize = 0;
    this->neighbourNameList = 0;
    this->neighbourNameCount = 0;
}

NeighbourListMsg::NeighbourListMsg(const NeighbourListMsg& other) : ::omnetpp::cPacket(other)
{
    neighbourNameList_arraysize = 0;
    this->neighbourNameList = 0;
    copy(other);
}

NeighbourListMsg::~NeighbourListMsg()
{
    delete [] this->neighbourNameList;
}

NeighbourListMsg& NeighbourListMsg::operator=(const NeighbourListMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void NeighbourListMsg::copy(const NeighbourListMsg& other)
{
    delete [] this->neighbourNameList;
    this->neighbourNameList = (other.neighbourNameList_arraysize==0) ? nullptr : new ::omnetpp::opp_string[other.neighbourNameList_arraysize];
    neighbourNameList_arraysize = other.neighbourNameList_arraysize;
    for (unsigned int i=0; i<neighbourNameList_arraysize; i++)
        this->neighbourNameList[i] = other.neighbourNameList[i];
    this->neighbourNameCount = other.neighbourNameCount;
}

void NeighbourListMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    b->pack(neighbourNameList_arraysize);
    doParsimArrayPacking(b,this->neighbourNameList,neighbourNameList_arraysize);
    doParsimPacking(b,this->neighbourNameCount);
}

void NeighbourListMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    delete [] this->neighbourNameList;
    b->unpack(neighbourNameList_arraysize);
    if (neighbourNameList_arraysize==0) {
        this->neighbourNameList = 0;
    } else {
        this->neighbourNameList = new ::omnetpp::opp_string[neighbourNameList_arraysize];
        doParsimArrayUnpacking(b,this->neighbourNameList,neighbourNameList_arraysize);
    }
    doParsimUnpacking(b,this->neighbourNameCount);
}

void NeighbourListMsg::setNeighbourNameListArraySize(unsigned int size)
{
    ::omnetpp::opp_string *neighbourNameList2 = (size==0) ? nullptr : new ::omnetpp::opp_string[size];
    unsigned int sz = neighbourNameList_arraysize < size ? neighbourNameList_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        neighbourNameList2[i] = this->neighbourNameList[i];
    for (unsigned int i=sz; i<size; i++)
        neighbourNameList2[i] = 0;
    neighbourNameList_arraysize = size;
    delete [] this->neighbourNameList;
    this->neighbourNameList = neighbourNameList2;
}

unsigned int NeighbourListMsg::getNeighbourNameListArraySize() const
{
    return neighbourNameList_arraysize;
}

const char * NeighbourListMsg::getNeighbourNameList(unsigned int k) const
{
    if (k>=neighbourNameList_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", neighbourNameList_arraysize, k);
    return this->neighbourNameList[k].c_str();
}

void NeighbourListMsg::setNeighbourNameList(unsigned int k, const char * neighbourNameList)
{
    if (k>=neighbourNameList_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", neighbourNameList_arraysize, k);
    this->neighbourNameList[k] = neighbourNameList;
}

int NeighbourListMsg::getNeighbourNameCount() const
{
    return this->neighbourNameCount;
}

void NeighbourListMsg::setNeighbourNameCount(int neighbourNameCount)
{
    this->neighbourNameCount = neighbourNameCount;
}

class NeighbourListMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    NeighbourListMsgDescriptor();
    virtual ~NeighbourListMsgDescriptor();

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

Register_ClassDescriptor(NeighbourListMsgDescriptor)

NeighbourListMsgDescriptor::NeighbourListMsgDescriptor() : omnetpp::cClassDescriptor("NeighbourListMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

NeighbourListMsgDescriptor::~NeighbourListMsgDescriptor()
{
    delete[] propertynames;
}

bool NeighbourListMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<NeighbourListMsg *>(obj)!=nullptr;
}

const char **NeighbourListMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *NeighbourListMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int NeighbourListMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int NeighbourListMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *NeighbourListMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "neighbourNameList",
        "neighbourNameCount",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int NeighbourListMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighbourNameList")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighbourNameCount")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *NeighbourListMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **NeighbourListMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *NeighbourListMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int NeighbourListMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsg *pp = (NeighbourListMsg *)object; (void)pp;
    switch (field) {
        case 0: return pp->getNeighbourNameListArraySize();
        default: return 0;
    }
}

const char *NeighbourListMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsg *pp = (NeighbourListMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string NeighbourListMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsg *pp = (NeighbourListMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getNeighbourNameList(i));
        case 1: return long2string(pp->getNeighbourNameCount());
        default: return "";
    }
}

bool NeighbourListMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsg *pp = (NeighbourListMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setNeighbourNameList(i,(value)); return true;
        case 1: pp->setNeighbourNameCount(string2long(value)); return true;
        default: return false;
    }
}

const char *NeighbourListMsgDescriptor::getFieldStructName(int field) const
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

void *NeighbourListMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsg *pp = (NeighbourListMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(BeaconInfoMsg)

BeaconInfoMsg::BeaconInfoMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->realPacketSize = 0;
    this->Prob = 0;
    this->myProb = 0;
    this->MyPosX = 0;
    this->MyPosY = 0;
    this->SSI = 0;
}

BeaconInfoMsg::BeaconInfoMsg(const BeaconInfoMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

BeaconInfoMsg::~BeaconInfoMsg()
{
}

BeaconInfoMsg& BeaconInfoMsg::operator=(const BeaconInfoMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void BeaconInfoMsg::copy(const BeaconInfoMsg& other)
{
    this->sourceAddress = other.sourceAddress;
    this->destinationAddress = other.destinationAddress;
    this->realPacketSize = other.realPacketSize;
    this->Prob = other.Prob;
    this->myProb = other.myProb;
    this->MyPosX = other.MyPosX;
    this->MyPosY = other.MyPosY;
    this->SSI = other.SSI;
}

void BeaconInfoMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sourceAddress);
    doParsimPacking(b,this->destinationAddress);
    doParsimPacking(b,this->realPacketSize);
    doParsimPacking(b,this->Prob);
    doParsimPacking(b,this->myProb);
    doParsimPacking(b,this->MyPosX);
    doParsimPacking(b,this->MyPosY);
    doParsimPacking(b,this->SSI);
}

void BeaconInfoMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceAddress);
    doParsimUnpacking(b,this->destinationAddress);
    doParsimUnpacking(b,this->realPacketSize);
    doParsimUnpacking(b,this->Prob);
    doParsimUnpacking(b,this->myProb);
    doParsimUnpacking(b,this->MyPosX);
    doParsimUnpacking(b,this->MyPosY);
    doParsimUnpacking(b,this->SSI);
}

const char * BeaconInfoMsg::getSourceAddress() const
{
    return this->sourceAddress.c_str();
}

void BeaconInfoMsg::setSourceAddress(const char * sourceAddress)
{
    this->sourceAddress = sourceAddress;
}

const char * BeaconInfoMsg::getDestinationAddress() const
{
    return this->destinationAddress.c_str();
}

void BeaconInfoMsg::setDestinationAddress(const char * destinationAddress)
{
    this->destinationAddress = destinationAddress;
}

int BeaconInfoMsg::getRealPacketSize() const
{
    return this->realPacketSize;
}

void BeaconInfoMsg::setRealPacketSize(int realPacketSize)
{
    this->realPacketSize = realPacketSize;
}

double BeaconInfoMsg::getProb() const
{
    return this->Prob;
}

void BeaconInfoMsg::setProb(double Prob)
{
    this->Prob = Prob;
}

double BeaconInfoMsg::getMyProb() const
{
    return this->myProb;
}

void BeaconInfoMsg::setMyProb(double myProb)
{
    this->myProb = myProb;
}

double BeaconInfoMsg::getMyPosX() const
{
    return this->MyPosX;
}

void BeaconInfoMsg::setMyPosX(double MyPosX)
{
    this->MyPosX = MyPosX;
}

double BeaconInfoMsg::getMyPosY() const
{
    return this->MyPosY;
}

void BeaconInfoMsg::setMyPosY(double MyPosY)
{
    this->MyPosY = MyPosY;
}

double BeaconInfoMsg::getSSI() const
{
    return this->SSI;
}

void BeaconInfoMsg::setSSI(double SSI)
{
    this->SSI = SSI;
}

class BeaconInfoMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    BeaconInfoMsgDescriptor();
    virtual ~BeaconInfoMsgDescriptor();

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

Register_ClassDescriptor(BeaconInfoMsgDescriptor)

BeaconInfoMsgDescriptor::BeaconInfoMsgDescriptor() : omnetpp::cClassDescriptor("BeaconInfoMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

BeaconInfoMsgDescriptor::~BeaconInfoMsgDescriptor()
{
    delete[] propertynames;
}

bool BeaconInfoMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BeaconInfoMsg *>(obj)!=nullptr;
}

const char **BeaconInfoMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *BeaconInfoMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int BeaconInfoMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int BeaconInfoMsgDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *BeaconInfoMsgDescriptor::getFieldName(int field) const
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
        "myProb",
        "MyPosX",
        "MyPosY",
        "SSI",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int BeaconInfoMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceAddress")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationAddress")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "realPacketSize")==0) return base+2;
    if (fieldName[0]=='P' && strcmp(fieldName, "Prob")==0) return base+3;
    if (fieldName[0]=='m' && strcmp(fieldName, "myProb")==0) return base+4;
    if (fieldName[0]=='M' && strcmp(fieldName, "MyPosX")==0) return base+5;
    if (fieldName[0]=='M' && strcmp(fieldName, "MyPosY")==0) return base+6;
    if (fieldName[0]=='S' && strcmp(fieldName, "SSI")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *BeaconInfoMsgDescriptor::getFieldTypeString(int field) const
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
        "double",
        "double",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **BeaconInfoMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *BeaconInfoMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int BeaconInfoMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    BeaconInfoMsg *pp = (BeaconInfoMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *BeaconInfoMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BeaconInfoMsg *pp = (BeaconInfoMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BeaconInfoMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BeaconInfoMsg *pp = (BeaconInfoMsg *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getSourceAddress());
        case 1: return oppstring2string(pp->getDestinationAddress());
        case 2: return long2string(pp->getRealPacketSize());
        case 3: return double2string(pp->getProb());
        case 4: return double2string(pp->getMyProb());
        case 5: return double2string(pp->getMyPosX());
        case 6: return double2string(pp->getMyPosY());
        case 7: return double2string(pp->getSSI());
        default: return "";
    }
}

bool BeaconInfoMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    BeaconInfoMsg *pp = (BeaconInfoMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourceAddress((value)); return true;
        case 1: pp->setDestinationAddress((value)); return true;
        case 2: pp->setRealPacketSize(string2long(value)); return true;
        case 3: pp->setProb(string2double(value)); return true;
        case 4: pp->setMyProb(string2double(value)); return true;
        case 5: pp->setMyPosX(string2double(value)); return true;
        case 6: pp->setMyPosY(string2double(value)); return true;
        case 7: pp->setSSI(string2double(value)); return true;
        default: return false;
    }
}

const char *BeaconInfoMsgDescriptor::getFieldStructName(int field) const
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

void *BeaconInfoMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    BeaconInfoMsg *pp = (BeaconInfoMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(NeighbourListMsgBT)

NeighbourListMsgBT::NeighbourListMsgBT(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    neighbourNameList_arraysize = 0;
    this->neighbourNameList = 0;
    this->neighbourNameCount = 0;
}

NeighbourListMsgBT::NeighbourListMsgBT(const NeighbourListMsgBT& other) : ::omnetpp::cPacket(other)
{
    neighbourNameList_arraysize = 0;
    this->neighbourNameList = 0;
    copy(other);
}

NeighbourListMsgBT::~NeighbourListMsgBT()
{
    delete [] this->neighbourNameList;
}

NeighbourListMsgBT& NeighbourListMsgBT::operator=(const NeighbourListMsgBT& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void NeighbourListMsgBT::copy(const NeighbourListMsgBT& other)
{
    delete [] this->neighbourNameList;
    this->neighbourNameList = (other.neighbourNameList_arraysize==0) ? nullptr : new ::omnetpp::opp_string[other.neighbourNameList_arraysize];
    neighbourNameList_arraysize = other.neighbourNameList_arraysize;
    for (unsigned int i=0; i<neighbourNameList_arraysize; i++)
        this->neighbourNameList[i] = other.neighbourNameList[i];
    this->neighbourNameCount = other.neighbourNameCount;
}

void NeighbourListMsgBT::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    b->pack(neighbourNameList_arraysize);
    doParsimArrayPacking(b,this->neighbourNameList,neighbourNameList_arraysize);
    doParsimPacking(b,this->neighbourNameCount);
}

void NeighbourListMsgBT::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    delete [] this->neighbourNameList;
    b->unpack(neighbourNameList_arraysize);
    if (neighbourNameList_arraysize==0) {
        this->neighbourNameList = 0;
    } else {
        this->neighbourNameList = new ::omnetpp::opp_string[neighbourNameList_arraysize];
        doParsimArrayUnpacking(b,this->neighbourNameList,neighbourNameList_arraysize);
    }
    doParsimUnpacking(b,this->neighbourNameCount);
}

void NeighbourListMsgBT::setNeighbourNameListArraySize(unsigned int size)
{
    ::omnetpp::opp_string *neighbourNameList2 = (size==0) ? nullptr : new ::omnetpp::opp_string[size];
    unsigned int sz = neighbourNameList_arraysize < size ? neighbourNameList_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        neighbourNameList2[i] = this->neighbourNameList[i];
    for (unsigned int i=sz; i<size; i++)
        neighbourNameList2[i] = 0;
    neighbourNameList_arraysize = size;
    delete [] this->neighbourNameList;
    this->neighbourNameList = neighbourNameList2;
}

unsigned int NeighbourListMsgBT::getNeighbourNameListArraySize() const
{
    return neighbourNameList_arraysize;
}

const char * NeighbourListMsgBT::getNeighbourNameList(unsigned int k) const
{
    if (k>=neighbourNameList_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", neighbourNameList_arraysize, k);
    return this->neighbourNameList[k].c_str();
}

void NeighbourListMsgBT::setNeighbourNameList(unsigned int k, const char * neighbourNameList)
{
    if (k>=neighbourNameList_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", neighbourNameList_arraysize, k);
    this->neighbourNameList[k] = neighbourNameList;
}

int NeighbourListMsgBT::getNeighbourNameCount() const
{
    return this->neighbourNameCount;
}

void NeighbourListMsgBT::setNeighbourNameCount(int neighbourNameCount)
{
    this->neighbourNameCount = neighbourNameCount;
}

class NeighbourListMsgBTDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    NeighbourListMsgBTDescriptor();
    virtual ~NeighbourListMsgBTDescriptor();

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

Register_ClassDescriptor(NeighbourListMsgBTDescriptor)

NeighbourListMsgBTDescriptor::NeighbourListMsgBTDescriptor() : omnetpp::cClassDescriptor("NeighbourListMsgBT", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

NeighbourListMsgBTDescriptor::~NeighbourListMsgBTDescriptor()
{
    delete[] propertynames;
}

bool NeighbourListMsgBTDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<NeighbourListMsgBT *>(obj)!=nullptr;
}

const char **NeighbourListMsgBTDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *NeighbourListMsgBTDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int NeighbourListMsgBTDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int NeighbourListMsgBTDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *NeighbourListMsgBTDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "neighbourNameList",
        "neighbourNameCount",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int NeighbourListMsgBTDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighbourNameList")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighbourNameCount")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *NeighbourListMsgBTDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **NeighbourListMsgBTDescriptor::getFieldPropertyNames(int field) const
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

const char *NeighbourListMsgBTDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int NeighbourListMsgBTDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsgBT *pp = (NeighbourListMsgBT *)object; (void)pp;
    switch (field) {
        case 0: return pp->getNeighbourNameListArraySize();
        default: return 0;
    }
}

const char *NeighbourListMsgBTDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsgBT *pp = (NeighbourListMsgBT *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string NeighbourListMsgBTDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsgBT *pp = (NeighbourListMsgBT *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getNeighbourNameList(i));
        case 1: return long2string(pp->getNeighbourNameCount());
        default: return "";
    }
}

bool NeighbourListMsgBTDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsgBT *pp = (NeighbourListMsgBT *)object; (void)pp;
    switch (field) {
        case 0: pp->setNeighbourNameList(i,(value)); return true;
        case 1: pp->setNeighbourNameCount(string2long(value)); return true;
        default: return false;
    }
}

const char *NeighbourListMsgBTDescriptor::getFieldStructName(int field) const
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

void *NeighbourListMsgBTDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    NeighbourListMsgBT *pp = (NeighbourListMsgBT *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(routingDecisionsMsg)

routingDecisionsMsg::routingDecisionsMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->sendWifiFirst = false;
}

routingDecisionsMsg::routingDecisionsMsg(const routingDecisionsMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

routingDecisionsMsg::~routingDecisionsMsg()
{
}

routingDecisionsMsg& routingDecisionsMsg::operator=(const routingDecisionsMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void routingDecisionsMsg::copy(const routingDecisionsMsg& other)
{
    this->sendWifiFirst = other.sendWifiFirst;
}

void routingDecisionsMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sendWifiFirst);
}

void routingDecisionsMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sendWifiFirst);
}

bool routingDecisionsMsg::getSendWifiFirst() const
{
    return this->sendWifiFirst;
}

void routingDecisionsMsg::setSendWifiFirst(bool sendWifiFirst)
{
    this->sendWifiFirst = sendWifiFirst;
}

class routingDecisionsMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    routingDecisionsMsgDescriptor();
    virtual ~routingDecisionsMsgDescriptor();

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

Register_ClassDescriptor(routingDecisionsMsgDescriptor)

routingDecisionsMsgDescriptor::routingDecisionsMsgDescriptor() : omnetpp::cClassDescriptor("routingDecisionsMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

routingDecisionsMsgDescriptor::~routingDecisionsMsgDescriptor()
{
    delete[] propertynames;
}

bool routingDecisionsMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<routingDecisionsMsg *>(obj)!=nullptr;
}

const char **routingDecisionsMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *routingDecisionsMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int routingDecisionsMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int routingDecisionsMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *routingDecisionsMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sendWifiFirst",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int routingDecisionsMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sendWifiFirst")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *routingDecisionsMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "bool",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **routingDecisionsMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *routingDecisionsMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int routingDecisionsMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    routingDecisionsMsg *pp = (routingDecisionsMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *routingDecisionsMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    routingDecisionsMsg *pp = (routingDecisionsMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string routingDecisionsMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    routingDecisionsMsg *pp = (routingDecisionsMsg *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getSendWifiFirst());
        default: return "";
    }
}

bool routingDecisionsMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    routingDecisionsMsg *pp = (routingDecisionsMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSendWifiFirst(string2bool(value)); return true;
        default: return false;
    }
}

const char *routingDecisionsMsgDescriptor::getFieldStructName(int field) const
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

void *routingDecisionsMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    routingDecisionsMsg *pp = (routingDecisionsMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(NetworkGraphMsg)

NetworkGraphMsg::NetworkGraphMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->numberVert = 0;
}

NetworkGraphMsg::NetworkGraphMsg(const NetworkGraphMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

NetworkGraphMsg::~NetworkGraphMsg()
{
}

NetworkGraphMsg& NetworkGraphMsg::operator=(const NetworkGraphMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void NetworkGraphMsg::copy(const NetworkGraphMsg& other)
{
    this->numberVert = other.numberVert;
    this->graphN = other.graphN;
}

void NetworkGraphMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->numberVert);
    doParsimPacking(b,this->graphN);
}

void NetworkGraphMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->numberVert);
    doParsimUnpacking(b,this->graphN);
}

int NetworkGraphMsg::getNumberVert() const
{
    return this->numberVert;
}

void NetworkGraphMsg::setNumberVert(int numberVert)
{
    this->numberVert = numberVert;
}

const char * NetworkGraphMsg::getGraphN() const
{
    return this->graphN.c_str();
}

void NetworkGraphMsg::setGraphN(const char * graphN)
{
    this->graphN = graphN;
}

class NetworkGraphMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    NetworkGraphMsgDescriptor();
    virtual ~NetworkGraphMsgDescriptor();

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

Register_ClassDescriptor(NetworkGraphMsgDescriptor)

NetworkGraphMsgDescriptor::NetworkGraphMsgDescriptor() : omnetpp::cClassDescriptor("NetworkGraphMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

NetworkGraphMsgDescriptor::~NetworkGraphMsgDescriptor()
{
    delete[] propertynames;
}

bool NetworkGraphMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<NetworkGraphMsg *>(obj)!=nullptr;
}

const char **NetworkGraphMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *NetworkGraphMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int NetworkGraphMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int NetworkGraphMsgDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *NetworkGraphMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "numberVert",
        "graphN",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int NetworkGraphMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "numberVert")==0) return base+0;
    if (fieldName[0]=='g' && strcmp(fieldName, "graphN")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *NetworkGraphMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "string",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **NetworkGraphMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *NetworkGraphMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int NetworkGraphMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    NetworkGraphMsg *pp = (NetworkGraphMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *NetworkGraphMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NetworkGraphMsg *pp = (NetworkGraphMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string NetworkGraphMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NetworkGraphMsg *pp = (NetworkGraphMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getNumberVert());
        case 1: return oppstring2string(pp->getGraphN());
        default: return "";
    }
}

bool NetworkGraphMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    NetworkGraphMsg *pp = (NetworkGraphMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setNumberVert(string2long(value)); return true;
        case 1: pp->setGraphN((value)); return true;
        default: return false;
    }
}

const char *NetworkGraphMsgDescriptor::getFieldStructName(int field) const
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

void *NetworkGraphMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    NetworkGraphMsg *pp = (NetworkGraphMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


