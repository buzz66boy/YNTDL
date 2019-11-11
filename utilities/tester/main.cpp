#include <iostream>
#include <arpa/inet.h>
#include "yntdl.h"


using namespace std;

int main(int argc, char *argv[]) {
    cout << "it worked!" << endl;
    if(argc > 1){
        yntdl::Topology topology = parseTopologyFile(argv[1]);
    }
    // yntdl::Node node("test");
    // yntdl::Position pos(0,1,2,3);
    
    // cout << pos << endl;

    // yntdl::IpAddr addr(4294967295);
    // yntdl::IpAddr addr2(AF_INET6, "2001:0db8:85a3:0000:0000:8a2e:0370:7334");
    // cout << addr << endl;
    // cout << addr2 << endl;
    // cout << node << endl;

    // yntdl::Application app("applicate", "ls");
    // cout << app << endl;
    // yntdl::Iface ifa("ifa");
    // cout << ifa << endl;

    // ifa.assignIp(&addr);
    // cout << ifa << endl;

    // ifa.assignSubnetMask(AF_INET, 24);
    // cout << ifa << endl;

    // yntdl::Link lank("lank", "cdma");
    // cout << lank << endl;

    // lank.bandwidth = "500kbps";
    // lank.latency = "30ms";
    // cout << lank << endl;

    // yntdl::Topology top("myTop");
    // top.ip = new yntdl::IpAddr(addr);
    // top.subnetMask = new yntdl::IpAddr(AF_INET, 4);

    // cout << top << endl;
    return 0;
}