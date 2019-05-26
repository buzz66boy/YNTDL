#ifndef __YNTDL_H_INCLUDED__
#define __YNTDL_H_INCLUDED__

#include <exception>
#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include <arpa/inet.h>

#include "yaml-cpp/yaml.h"

namespace yntdl {

enum class ErrorCode {
//General Error Code
    NOT_ENOUGH_ARGS,
    NO_FILE_PROVIDED,
    RUN_AS_ROOT,
    FOLDER_NOT_FOUND,
    FOLDER_NOT_CREATED,
    FILE_NOT_FOUND,
    SETTINGS_ATTR_NOT_FOUND,
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
//Generation Error Codes
    BR_CREATE_FAILURE,
    TAP_CREATE_FAILURE,
    NODE_CREATE_FAILURE,
    NODE_START_FAILURE,
    NODE_TEARDOWN_FAILURE,
    APP_INSTALL_FAILURE,
    APP_RUN_FAILURE,
    NS3_WRITE_FAILURE,
    NS3_RUN_FAILURE,
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

class AdditionalTags {
public:
    std::map<std::string, YAML::Node> additionalTags;

    /**
     * Adds unrecognized tags to the additionalTags map
     **/
    void mapAdditionalTags(std::vector<std::string> knownTags, YAML::Node mapRoot);
};

class Nameable {
public:
    std::string name;
    std::string origName;
    Nameable() {}
    Nameable(std::string name): origName(name), name(name) {}
    Nameable(std::string name, std::string origName): name(name), origName(origName) {}
    Nameable(const Nameable& temp): origName(temp.origName), name(temp.name) {}

};

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
};

class Position {
public:
    double x, y, z;
    double time;
    Position(double time, double x, double y, double z): time(time), x(x), y(y), z(z) {};
    Position() {}
    Position(const Position &p): Position(p.time, p.x, p.y, p.z) {};

    std::string str();
    std::string ns3Str();
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
};

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
};

class Node : public Positionable, public IfaceProvider, public Nameable, public AdditionalTags {
private:
    bool requiresReRef = false;
public:
    int nodeNum = -1; //For ns-3 writing, keep track of what # the node is (for ref)
    std::string type;
    std::map<std::string, Iface> ifaces;
    std::vector<Application> applications;
    std::vector<std::pair<std::string, bool> > commands;
    
    yntdl::Iface *getIface(std::string ifaceName) override; // OVERRIDE IfaceProvider

    Node(): Nameable(), Positionable(), IfaceProvider(), AdditionalTags() {};
    Node(std::string name):  Nameable(name), Positionable(), IfaceProvider() {};
    Node(yntdl::Node& temp, std::string nodeName, std::string origName);
    Node(const yntdl::Node &temp);

    void addCommand(std::string cmd, bool inherit);

    static void reRefIfaces(yntdl::Node*);
};

class Topology : public Positionable, public IfaceProvider, public IfaceAcceptor, public Nameable {
public:
    int runTime = 60;
    //std::string name = "";
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
    
    Topology();
    Topology(std::string name): Nameable(name) {};
    Topology(Topology *temp);
    Topology(std::shared_ptr<Topology> temp, std::string newName);
    Topology(const Topology&);
    ~Topology();

    static void reNumNodes(Topology*);
};

//Parse Functions

};

#endif