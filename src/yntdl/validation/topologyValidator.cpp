#include <string>
#include <vector>
#include <iostream>

#include "yntdl.h"

using namespace std;

void yntdl::validateTopology(yntdl::Topology *top){
    bool fail = false;
    if(top->name == ""){
        cerr << "A Topology has no name" << endl;
        fail = true;
    }

    for(auto nodePtr : top->nodes){
        if(top->nodeMap.at(nodePtr->origName) != nodePtr){
            cerr << "Node " + nodePtr->name + " not mapped correctly" << endl;
            fail = true;
        }
        yntdl::validateNode(nodePtr);
    }
    for(auto linkPtr : top->links){
        if(top->linkMap.at(linkPtr->origName) != linkPtr){
            cerr << "Link " + linkPtr->name + " not mapped correctly" << endl;
            fail = true;
        }
        yntdl::validateLink(linkPtr);
    }
    for(auto subTopPtr : top->subTopologies){
        if(top->topMap.at(subTopPtr->origName) != subTopPtr){
            cerr << "Sub Topology " + subTopPtr->name + " not mapped properly" << endl;
            fail = true;
        }
        yntdl::validateTopology(subTopPtr.get());
    }

    if(fail){
        throw yntdl::YntdlException(yntdl::ErrorCode::TOPOLOGY_INVALID, top->name);
    }
}