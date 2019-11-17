#include <iostream>
#include <string>
#include "yntdl.h"

using namespace std;

void yntdl::printNode(std::ostream &out, yntdl::Node *nodePtr, int indent, bool apps,\
    std::string appName, bool ifaces, std::string ifaName, bool positions, double time, bool commands){
    //FIXME: investigate making indents into a macro function
    string ind(indent, '\t');
    string ind2(indent + 1, '\t');
    string ind3(indent + 2, '\t');

    if(ifaName == "" && appName == ""){
        out << ind << " - " << *nodePtr << '\n';
    }

    if(ifaces){
        if(nodePtr->ifaces.size() > 0){
            if(ifaName == ""){
                //print all ifaces
                out << ind2 << "Interfaces:\n";
                for(auto ifacePair : nodePtr->ifaces){
                    out << ind2 << " - " << ifacePair.second << '\n';
                    if(ifacePair.second.link != nullptr){
                        out << ind3 << "Connected to " << *ifacePair.second.link << '\n';
                    }
                }
            } else {
                //print only specified interface
                for(auto ifaPair : nodePtr->ifaces){
                    if(ifaPair.first.find(ifaName) != std::string::npos){
                        out << ind << " - " << *nodePtr << '\n';
                        out << ind2 << " - " << ifaPair.second << '\n';
                        if(ifaPair.second.link != nullptr){
                            out << ind3 << "Connected to " << *ifaPair.second.link << '\n';
                        }
                    }
                }
            }
        } else {
            //no ifaces
            //out << ind2 << "No Interfaces\n";
        }
    }
    if(apps){
        if(nodePtr->applications.size() > 0){
            if(appName == ""){
                //print all apps
                out << ind2 << "Applications:\n";
                for(auto app : nodePtr->applications){
                    out << ind2 << " - " << app << '\n';
                    if(app.commands.size() > 0){
                        out << ind3 << "App Commands:\n";
                        for(auto cmdPair : app.commands){
                            out << ind3 << " - " << cmdPair.first << '\n';
                        }
                    }
                }
            } else {
                //print specific app
                for(auto app : nodePtr->applications){
                    if(app.name.find(appName) != std::string::npos){
                        out << ind << " - " << *nodePtr << '\n';
                        out << ind2 << " - " << app << '\n';
                        if(app.commands.size() > 0){
                            out << ind3 << "App Commands:\n";
                            for(auto cmdPair : app.commands){
                                out << ind3 << " - " << cmdPair.first << '\n';
                            }
                        }
                        break;
                    }
                }
            }
        } else {
            //no apps
        }
    }
    if(commands){
        if(nodePtr->commands.size() > 0){
            out << ind2 << "Commands:\n";
            for(auto cmdPair : nodePtr->commands){
                out << ind2 << " - " << cmdPair.first << '\n';
            }
        }
    }
    if(positions){
        if(nodePtr->positions.size() > 0){
            if(time < 0){
                out << ind2 << "Positions:\n";
                for(auto pos : nodePtr->positions){
                    out << ind2 << " - " << pos << '\n';
                }
            } else {
                //print position at time
                out << ind2 << "Position @ " << nodePtr->getPosition(time) << '\n';
            }
        }
    }
}