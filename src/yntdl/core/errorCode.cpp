#include <string>
#include <cstring>

#include "yntdl.h"

using namespace std;

const char* yntdl::YntdlException::what() const throw() {
    string errStr = "";
    switch(errorCode){
        case(ErrorCode::FILE_NOT_FOUND):
            errStr = "File not found: ";
            break;
        //Parse Error Codes
        //GENERAL
        case(ErrorCode::TEMPLATE_NOT_FOUND):
            errStr = "Template not found: ";
            break;
        case(ErrorCode::POSITION_ERROR):
            errStr = "Error in Positions: ";
            break;
        //TOP
        case(ErrorCode::TOPOLOGY_NOT_FOUND):
            errStr = "Topology not found: ";
            break;
        //NODES
        case(ErrorCode::NODE_NOT_FOUND):
            errStr = "Node not found: ";
            break;
        case(ErrorCode::NODE_TYPE_NOT_FOUND):
            errStr = "Node Type not Mapped: ";
            break;
        case(ErrorCode::NODE_TYPE_NOT_SPECIFIED):
            errStr = "Node Type not specified: ";
            break;
        //APPS
        case(ErrorCode::APP_NOT_FOUND):
            errStr = "Application not found: ";
            break;
        case(ErrorCode::NODE_NOT_SPECIFIED):
            errStr = "Node not specified for: ";
            break;
        //LINKS
        case(ErrorCode::LINK_NOT_FOUND):
            errStr = "Link not found: ";
            break;
        case(ErrorCode::LINK_TYPE_NOT_FOUND):
            errStr = "Link Type not mapped: ";
            break;
        case(ErrorCode::LINK_TYPE_NOT_SPECIFIED):
            errStr = "Link Type no specified";
            break;
        //IFACES
        case(ErrorCode::IFACE_NOT_FOUND):
            errStr = "Iface not found: ";
            break;
        case(ErrorCode::ACCEPTOR_NOT_FOUND):
            errStr = "Iface Acceptor not found: ";
            break;
        case(ErrorCode::PROVIDER_NOT_FOUND):
            errStr = "Iface Provider not found: ";
            break;
        case(ErrorCode::NO_IP):
            errStr = "No IP: ";
            break;
        //Validation Error Codes
        case(ErrorCode::TOPOLOGY_INVALID):
            errStr = "Invalid Topology: ";
            break;
        case(ErrorCode::LINK_INVALID):
            errStr = "Invalid Link: ";
            break;
        case(ErrorCode::NODE_INVALID):
            errStr = "Invalid Node: ";
            break;
        case(ErrorCode::APP_INVALID):
            errStr = "Invalid Application: ";
            break;
        default:
            errStr = "Error: ";
            break;
    }
    string err = errStr + relevantName;
    const char *buf = (const char *) malloc(err.length() * sizeof(char));
    strcpy((char *)buf, err.c_str());
    return buf;
}