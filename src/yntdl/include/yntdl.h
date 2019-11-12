#ifndef __YNTDL_H_INCLUDED__
#define __YNTDL_H_INCLUDED__

#include <exception>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <arpa/inet.h>

#include "yaml-cpp/yaml.h"

namespace yntdl {
    //Forward declarations for object string conversion
    class Position;
    class IpAddr;
    class Application;
    class Iface;
    class Topology;
    //Forward declarations of Node and Link for Iface objects & stringifying
    class Node;
    class Link;

    //main function for parsing topologies
    yntdl::Topology parseTopologyFile(std::string topPath);
}

namespace std {
    //declaration override of << operator for string conversion of objects
    std::ostream& operator<<(std::ostream &out, const yntdl::Position &pos);
    std::ostream& operator<<(std::ostream &out, const yntdl::IpAddr &addr);
    std::ostream& operator<<(std::ostream &out, const yntdl::Application &app);
    std::ostream& operator<<(std::ostream &out, const yntdl::Iface &iface);
    std::ostream& operator<<(std::ostream &out, const yntdl::Node &node);
    std::ostream& operator<<(std::ostream &out, const yntdl::Link &link);
    std::ostream& operator<<(std::ostream &out, const yntdl::Topology &top);
}

namespace yntdl {

//errorCode.h
enum class ErrorCode {
//General Error Code
    FILE_NOT_FOUND,
    NOT_ENOUGH_ARGS,
    NO_FILE_PROVIDED,
//Parse Error Codes
    //GENERAL
    TEMPLATE_NOT_FOUND,
    POSITION_ERROR,
    //TOP
    TOPOLOGY_NOT_FOUND,
    //NODES
    NODE_NOT_FOUND,
    NODE_TYPE_NOT_FOUND,
    NODE_TYPE_NOT_SPECIFIED,
    //APPS
    APP_NOT_FOUND,
    NODE_NOT_SPECIFIED,
    //LINKS
    LINK_NOT_FOUND,
    LINK_TYPE_NOT_FOUND,
    LINK_TYPE_NOT_SPECIFIED,
    //IFACES
    IFACE_NOT_FOUND,
    ACCEPTOR_NOT_FOUND,
    PROVIDER_NOT_FOUND,
    NO_IP,
    //Validation Error Codes
    TOPOLOGY_INVALID,
    LINK_INVALID,
    NODE_INVALID,
    APP_INVALID,
};

class YntdlException : public std::exception {
private:
    ErrorCode errorCode;
    std::string relevantName;
public:
    YntdlException(ErrorCode ec, std::string relName): errorCode(ec), relevantName(relName) {};
    virtual const char* what() const throw();

    YntdlException(const YntdlException& temp): errorCode(temp.errorCode), relevantName(temp.relevantName) {}
};

//additionalTags.h
class AdditionalTags {
public:
    std::map<std::string, YAML::Node> additionalTags;

    /**
     * Adds unrecognized tags to the additionalTags map
     **/
    void mapAdditionalTags(std::vector<std::string> knownTags, YAML::Node mapRoot);

    AdditionalTags(): additionalTags() {}
    AdditionalTags(const AdditionalTags& temp): additionalTags(temp.additionalTags) {}
};

//nameable.h
class Nameable {
public:
    std::string name;
    std::string origName;
    Nameable() {}
    Nameable(std::string name): origName(name), name(name) {}
    Nameable(std::string name, std::string origName): name(name), origName(origName) {}
    Nameable(const Nameable& temp): origName(temp.origName), name(temp.name) {}

};

//ipaddr.h
class IpAddr {
private:
    bool ipv4;
    union {
        uint8_t ipv6_address_8[16];
        uint32_t ipv6_address_32[4];
        uint32_t ipv4_address;
    };

public:
    IpAddr(const IpAddr&);
    IpAddr(uint32_t ipv4_addr);
    
    IpAddr(uint8_t *ipv6_addr);
    IpAddr(uint32_t *ipv6_addr) { IpAddr((uint8_t *)ipv6_addr); }

    IpAddr(int af, std::string addr);
    IpAddr(int af, int cidr);
    ~IpAddr() {}

    bool equals(IpAddr &temp);

    bool isIpv4() { return ipv4; }
    uint32_t getIpv4Addr() { return ipv4_address; }
    std::vector<uint32_t> getIpv6Addr();

    std::string str();
    int getCidr();
    void applyOffset(std::string);
    std::string getSubnet(IpAddr *subnetMask);

    friend std::ostream& std::operator<<(std::ostream &out, const yntdl::IpAddr &addr);
};

//position.h
class Position {
public:
    double x, y, z;
    double time;
    Position(double time, double x, double y, double z): time(time), x(x), y(y), z(z) {};
    Position() {}
    Position(const Position &p): Position(p.time, p.x, p.y, p.z) {};

    std::string str();
    std::string ns3Str();

    friend std::ostream& std::operator<<(std::ostream &out, const yntdl::Position &pos);
};

class Positionable {
public:
    virtual ~Positionable () {}
    Positionable() {}
    Positionable(const Positionable&);
    Positionable *parent = nullptr;

    std::vector<Position> positions; // Relative positions
    std::vector<Position> absPositions; // Absolute positions

    /**
     * Computes relative positions based on its positions and parents movement
     */
    void centerPositionsAroundParent(Positionable *par = nullptr);
    /**
     * Rotates positions (and hence movement) around 0, 0, 0 counter-clockwise
     */
    void rotatePositions(double degrees);
    /**
     * Computes position of positionable at a given time
     */
    Position getAbsPosition(double time);
    Position getPosition(double time);
};

//application.h
class Application: public Nameable, public AdditionalTags {
public:
    std::string path = "";

    /**
     * Map of string command and boolean indicating inheritance
     **/
    std::vector<std::pair<std::string, bool> > commands;
    
    /**
     * Should App be used when templated (user by higher level top)
     **/
    bool inherit = true;

    Application(std::string name, bool inherit = true): inherit(inherit), Nameable(name), AdditionalTags() {};
    Application(std::string name, std::string cmd): Nameable(name), commands({std::pair<std::string, bool> (cmd, false)}) {}
    Application(const Application& temp);

    /** 
     * Constructor to filter out non-inherited commands
     **/
    Application(Application *temp);

    void addCommand(std::string cmd, bool shouldInherit = true) {commands.push_back(std::pair<std::string, bool> (cmd, shouldInherit)); }

    friend std::ostream& std::operator<<(std::ostream &out, const yntdl::Application &app);
};

//iface.h
class Iface : public Nameable {

public:
    yntdl::Node *node = nullptr;
    yntdl::Link *link = nullptr;
    std::string bridgeName;
    std::string tapName;
    std::string macAddr;

    IpAddr *ip = nullptr;
    IpAddr *subnetMask = nullptr;

    Iface() {};
    Iface(const Iface&);
    Iface(std::string name): Nameable(name) {};
    Iface(std::string name, yntdl::Node *node): Nameable(name), node(node) {};
    ~Iface();

    void assignIp(IpAddr *ipAddr);
    void assignIp(int af, std::string ipAddr);

    void assignSubnetMask(IpAddr *subnetMaskAddr);
    void assignSubnetMask(int af, std::string subnetMaskStr);
    void assignSubnetMask(int af, int cidr);

    friend std::ostream& std::operator<<(std::ostream &out, const yntdl::Iface &iface);
};

class IfaceProvider {
protected:
    IfaceProvider() {};
public:
    std::map<std::string, std::string> ifacesProvidedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::weak_ptr<yntdl::IfaceProvider> > ifacesProvided; //keep a ref to the providers we contain
    virtual yntdl::Iface *getIface(std::string ifaceName);
};

class IfaceAcceptor {
protected:
    IfaceAcceptor() {};
public:
    std::map<std::string, std::string> ifacesAcceptedSubNames; //maps this level's iface names to lower level's
    std::map<std::string, std::weak_ptr<yntdl::IfaceAcceptor> > ifacesAccepted; //keep a ref to who below us accepts ifaces
    virtual int connectIface(std::string ifaceName, Iface *iface);
};

//link.h
class Link : public IfaceAcceptor, public Nameable, public AdditionalTags {
private:
    bool requiresReRef = false;
    std::string type;
    
public:

    std::vector<Iface *> ifaces;

    IpAddr *ip = nullptr;
    IpAddr *subnetMask = nullptr;

    std::string bandwidth;
    std::string latency;
    
    Link(std::string name, std::string type): Nameable(name), AdditionalTags(), type(type) {};
    Link(std::string name, Link& link);
    Link(std::string name): Nameable(name) {}
    Link(const Link&);
    ~Link();

    std::string getType() { return type; }
    void setType(std::string typ) { type = typ; }

    const std::vector<Iface*>& getIfaces() const { return ifaces; } 
    
    int connectIface(yntdl::Iface *iface); 
    int connectIface(std::string ifaceName, yntdl::Iface *iface) override; // OVERRIDES IfaceAccepter

    static void reRefIfaces(Link *linkPtr);

    friend std::ostream& std::operator<<(std::ostream &out, const yntdl::Link &link);
};

//node.h
class Node : public Positionable, public IfaceProvider, public Nameable, public AdditionalTags {
private:
    bool requiresReRef = false;
public:
    int nodeNum = -1; //For ns-3 writing, keep track of what # the node is (for ref)
    std::string type;
    std::map<std::string, Iface> ifaces;
    std::vector<Application> applications;
    // list of commands on the node, bool is an inherit flag, allowing children of this node (aka used as a template) to inherit a command
    std::vector<std::pair<std::string, bool> > commands;
    
    yntdl::Iface *getIface(std::string ifaceName) override; // OVERRIDE IfaceProvider

    Node(): Nameable(), Positionable(), IfaceProvider(), AdditionalTags() {};
    Node(std::string name):  Nameable(name), Positionable(), IfaceProvider() {};
    Node(yntdl::Node& temp, std::string nodeName, std::string origName);
    Node(const yntdl::Node &temp);

    void addCommand(std::string cmd, bool inherit);

    static void reRefIfaces(yntdl::Node*);

    friend std::ostream& std::operator<<(std::ostream &out, const yntdl::Node &node);
};

//topology.h
class Topology : public Positionable, public IfaceProvider, public IfaceAcceptor, public Nameable, public AdditionalTags {
public:
    //int runTime = 60;
    int curNodeNum = 0;

    IpAddr *ip = nullptr;
    IpAddr *subnetMask = nullptr;

    std::vector<std::shared_ptr<Topology> > subTopologies;
    std::vector<std::shared_ptr<Node> > nodes;
    std::vector<std::shared_ptr<Link> > links;
    std::vector<std::shared_ptr<Application> > applications;
    
    std::map<std::string, std::shared_ptr<Topology> > topMap;
    std::map<std::string, std::shared_ptr<Node> > nodeMap;
    std::map<std::string, std::shared_ptr<Link> > linkMap;
    
    Topology() {};
    Topology(std::string name): Nameable(name) {};
    Topology(Topology *temp);
    Topology(std::shared_ptr<Topology> temp, std::string newName);
    Topology(const Topology&);
    ~Topology();

    static void reNumNodes(Topology*);

    friend std::ostream& std::operator<<(std::ostream &out, const yntdl::Topology &top);
};

//linkValidator.h
void validateLink(std::shared_ptr<yntdl::Link> linkPtr);

//nodeValidator.h
void validateNode(std::shared_ptr<yntdl::Node> nodePtr);

//topologyValidator.h
void validateTopology(yntdl::Topology *top);

void printTopology(std::ostream &out, yntdl::Topology *top, int indent = 0, bool nodes = true, std::string nodeName = "", bool links = true,\
    std::string linkName = "", bool apps = true, std::string appName = "", bool ifaces = true, std::string ifaName = "",\
    bool positions = true, double time = -1.0, bool commands = true);
void printNode(std::ostream &out, yntdl::Node *nodePtr, int indent = 0, bool apps = true,\
    std::string appName = "", bool ifaces = true, std::string ifaName = "", bool positions = true, double time = -1.0, bool commands = true);
void printLink(std::ostream &out, yntdl::Link *linkPtr, int indent = 0, bool ifaces = true, std::string ifaceName = "");
//FIXME: Add back in
//void printTopology(std::ostream &out, std::ParsedTopology *parsedTop);

//topologyParser.h
class ParsedTopology {
public:
    yntdl::Topology topology;
    std::map<std::string, std::shared_ptr<yntdl::Topology> > includedTopologies;
    std::map<std::string, std::shared_ptr<yntdl::Node> > nodes;
    std::map<std::string, std::shared_ptr<yntdl::Link> > links;
    std::map<std::string, std::shared_ptr<yntdl::Application> > applications;
};

void parseTopology(YAML::Node topology, ParsedTopology *parsedTop);
void renameSubTopologies(yntdl::Topology *topology, std::string prefix="");
std::shared_ptr<yntdl::Node> findNode(std::vector<std::string> search, yntdl::Topology *top);

//applicationParser.h
void parseApplications(YAML::Node apps, ParsedTopology *parsedTop);

//commandParser.h
void parseCommands(YAML::Node cmds, ParsedTopology *parsedTop);

//ifaceParser.h
std::weak_ptr<yntdl::IfaceProvider> getProvider(std::string provider, yntdl::Topology *top);
std::weak_ptr<yntdl::IfaceAcceptor> getAcceptor(std::string acceptor, yntdl::Topology *top);
void parseIfacesProvided(YAML::Node ifaces, ParsedTopology *parsedTop);
void parseAcceptedIfaces(YAML::Node acceptedIface, ParsedTopology *top);
void parseIfacesAccepted(YAML::Node ifacesAccepted, ParsedTopology *parsedTop);

//linkParser.h
bool doesLinkExist(YAML::Node node, ParsedTopology *top);
void parseLink(YAML::Node link, ParsedTopology *top);
//FIXME: this function does nothing
void overrideLink(YAML::Node link, ParsedTopology *top);

//nodeParser.h
std::vector<std::shared_ptr<yntdl::Node> > parseNode(YAML::Node node, ParsedTopology *top);
void parseNodeApplications(YAML::Node apps, std::shared_ptr<yntdl::Node> node);
//positionParser.h
void parsePositions(YAML::Node posNode, std::shared_ptr<yntdl::Node> nodePtr);
void parsePositions(YAML::Node posNode, yntdl::Topology *topPtr);
void applyRotation(int rotation, yntdl::Topology *topPtr);
void computeAbsolutePositions(yntdl::Topology *top);

//subTopologyParser.h
std::vector<std::shared_ptr<yntdl::Topology> > parseSubTopology(YAML::Node node, ParsedTopology *top);

}; //End of yntdl namespace

//parserTags.h FIXME: Pull these tags into different file?
// NOTE: these tags can also be used plurally (checks for tag + "s")
#define TAG_INCLUDE "include" 
//#define TAG_TIME "time"
#define TAG_NODE "node" 
#define TAG_TOPOLOGY "topology"
#define TAG_APPLICATION "application"
#define TAG_COMMAND "command"
#define TAG_LINK "link"
#define TAG_BANDWIDTH "bandwidth"
#define TAG_LATENCY "latency"
#define TAG_POSITION "position"
#define TAG_IFACE "iface"
#define TAG_INTERFACE "interface"
#define TAG_TEMPLATE "template"
#define TAG_NUM "num"
#define TAG_ALL "all"
#define TAG_INHERIT "inherit"
#define TAG_ROTATION "rotation"
#define TAG_ACCEPT_IFACE "acceptIface"
#define TAG_IFACES_ACCEPTED "ifacesAccepted"
#define TAG_IFACES_PROVIDED "ifacesProvided"
#define TAG_NAME "name"
#define TAG_IP "ip"
#define TAG_CIDR "cidr"
#define TAG_SUBNET_MASK "subnetMask"
#define TAG_TYPE "type"
#define TAG_OFFSET "offset"

//Utility functions for dealing with tags and topologies
std::string pluralize(std::string str);
std::vector<std::string> splitString(std::string str);

#endif