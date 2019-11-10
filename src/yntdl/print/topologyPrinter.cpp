#include <iostream>
#include <string>
#include "yntdl.h"

using namespace std;

void yntdl::printLink(std::ostream &out, yntdl::Link *linkPtr, int indent){
    string ind(indent, '\t');
    string ind2(indent + 1, '\t');

    out << ind << " - " << *linkPtr << '\n';
    if(linkPtr->ifaces.size() > 0){
        out << ind2 << "Connected Interfaces:\n";
        for(auto ifacePtr : linkPtr->ifaces){
            out << ind2 << " - " << *ifacePtr << '\n';
        }
    }
}

void yntdl::printTopology(std::ostream &out, yntdl::Topology *top, int indent){
    out << "Topology " << top->name << '\n';
    if(top->ip != nullptr){
        out << "\tIp: " << *top->ip << '\n';
    }
    if(top->subnetMask != nullptr){
        out << "\tSubnet Mask: " << *top->subnetMask << '\n';
    }
    if(top->positions.size() > 0){
        out << "\tPositions:\n";
        for(auto pos : top->positions){
            out << "\t - " << pos << '\n';
        }
    }
    if(top->links.size() > 0){
        out << "\tLinks:\n";
        for(auto linkPtr : top->links){
            // out << "\t\t" << *linkPtr.get() << '\n';
            yntdl::printLink(out, linkPtr.get(), 1);
        }
    }
    if(top->nodes.size() > 0){
        out << "\tNodes:\n";
        for(auto nodePtr : top->nodes){
            // out << "\t\t" << *nodePtr.get() << '\n';
            yntdl::printNode(out, nodePtr.get(), 1);
        }
    }
    for(auto topPtr : top->subTopologies){
        yntdl::printTopology(out, topPtr.get());
    }
}

//FIXME: Add Back in
/*void yntdl::printTopology(std::ostream &out, ParsedTopology *parsedTop){
    out << "Parsed Topology " << parsedtop->topology.name << '\n';
    out << '\t' << "Included Topologies:" << '\n';
    for(auto incTopPair : parsedtop->includedTopologies){
        out << "\t\t" << incTopPair.first << '\n';
    }
    out << '\t' << "Included Nodes:" << '\n';
    for(auto incNodePair : parsedtop->nodes){
        out << "\t\t" << incNodePair.first << '\n';
    }
    out << '\t' << "Included Links:" << '\n';
    for(auto incLinkPair : parsedtop->links){
        out << "\t\t" << incLinkPair.first << '\n';
    }
    printTopology(out, parsedtop->topology);
}*/