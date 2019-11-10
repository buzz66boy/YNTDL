#include <iostream>
#include <string>
#include <unistd.h>
#include "yntdl.h"

#define HELP_STATEMENT "usage: './print_yntdl pathToTopology [-a] [-n] [-l] [-i] [-p] [-f] [-h] [-m]'\n\
-a [app name]: print only applications/[specified app]\n\
-n [node name]: print only nodes/[specified node]\n\
-l [link name]: print only links/[specified link]\n\
-i [interface name]: prints only interfaces/[specified interface]\n\
-p [node] or [time]: print positions/[of specific node]/[all objects at specific time]\n\
-f: print in flat mode, listing all objects as one topology\n\
-h: show this help text\n\
-m: DOUBLE HELP, show the small manual"

#define DOUBLE_HELP " * Supported combinations:\n\
 *      -n -a: displays applications on all nodes\n\
 *      -n -a [app]: displays applications of name 'app' on all nodes\n\
 *      -n [node] -a: displays all applications for node 'node'\n\
 *      -n [node] -a [app]: prints the specified application's information for that node\n\
 *\n\
 *      -n -i: displays interfaces on all nodes\n\
 *      -n -i [iface]: displays interfaces of name 'iface' on all nodes\n\
 *      -n [node] -i: displays all interfaces for node 'node'\n\
 *      -n [node] -i [iface]: prints the specified interface's information for that node\n\
 *\n\
 *      -n -p: roughly same as '-p' but omits topology center location information\n\
 *      -n -p [time]: prints all node positions at time 'time'\n\
 *      -n [node] -p: same as -p [node] prints all positions for node 'node'\n\
 *      -n [node] -p [time]: print 'node's position at 'time'\n\
 *\n\
 *      -l -i: print all interfaces connected to links\n\
 *      -l -i [iface]: print all links with interfaces of name 'iface'\n\
 *      -l [link] -i: print all interfaces connected to 'link'\n\
 *      -l [link] -i [iface]: print interfaces named 'iface' connected to 'link'\n\
 *\n\
 *      Multiple combined: -n tag will be evaluated first"

using namespace std;

/**
 * Takes in a topology file and prints the requested information, default prints topology
 * hierarchy with all internal nodes and links. Flat mode (-f) prints the topology as if it
 * were one topology, lists all links, then all nodes, then all positions, then applications.
 * Other flags can be used to designate specific information to print instead of the full
 * topology (see help statements above).
 */
int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << HELP_STATEMENT << endl;
        throw yntdl::YntdlException(yntdl::ErrorCode::NOT_ENOUGH_ARGS, to_string(argc));
    }

    int opt;
    string appName, nodeName, linkName, ifaceName, posArg;
    bool flat = false, node = false, ifa = false, app = false, link = false, pos = false;
    while((opt = getopt(argc, argv, "a::n::l::i::p::hmf")) != -1){
        switch(opt){
            case 'a':
                app = true;
                if(optarg){
                    appName = optarg;
                }
                break;
            case 'f':
                flat = true;
                break;
            case 'n':
                node = true;
                if(optarg){
                    nodeName = optarg;
                }
                break;
            case 'l':
                link = true;
                if(optarg){
                    linkName = optarg;
                }
                break;
            case 'i':
                ifa = true;
                if(optarg){
                    ifaceName = optarg;
                }
                break;
            case 'p':
                pos = true;
                if(optarg){
                    posArg = optarg;
                }
                break;
            case 'm':
                cout << HELP_STATEMENT << endl;
                cout << DOUBLE_HELP << endl;
                return 2;
                break;
            case 'h':
                cout << HELP_STATEMENT << endl;
                return 1;
                break;
            case '?':
            default:
                cout << "unknown arg " << optarg << endl;
                break;
        }
    }

    // check for incompatible/ambiguous flag combinations
    if((pos && link) || (app && link)){
        cerr << "Invalid flag combination" << endl;
        return 3;
    }

    double time = -1.0;
    //convert pos arg to time
    if(posArg != ""){
        time = stod(posArg);
    }

    // optind is for the extra arguments 
    // which are not parsed 
    string filename;
    for(; optind < argc; optind++){
        filename = argv[optind];
    }

    yntdl::Topology topology;
    topology = parseTopologyFile(filename);

    cout << endl;
    if(node){
        if(nodeName == ""){
            
        } else {
            yntdl::Node *nd = findNode(splitString(nodeName), &topology).get();
            if(nd != nullptr){
                yntdl::printNode(cout, nd, 0, app, appName, ifa, ifaceName, pos, time);
            }
        }
    } else if(flat){

    } else {
        yntdl::printTopology(cout, &topology);
    }

    return 0;
}