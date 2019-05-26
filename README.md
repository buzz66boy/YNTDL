# The YAML Network Topology Description Language (YNTDL)
## Overview
YNTDL (Entitle) was created to allow for simple, understandable, and scalable text descriptions of network topologies. Much like an hardware description language (HDL) is to hardware, YNTDL describes the structure and behavior of networks. YNTDL was created as part of the ns3-lxc project (https://github.com/buzz66boy/ns3-lxc) and separated to allow for further development and usage in other applications.

Resources, including example topology files and a readme on how to write topology files can be found in the resources folder.

## How it works
YNTDL describes networks in terms of:
- links: connects to n interfaces
- interfaces: connects to one link, assigned an IP address (later adding mac addresses)
- nodes: has associated physical position, applications, and n interfaces
- applications: exist on associated nodes
These four objects are combined to form a network topology. A network topology can also consist of multiple other network topologies (subtopologies).

## Scalability
The true power of YNTDL is the ability to instantiate network topologies inside of other network topologies. By explicitly naming exposed interfaces and links (network edges) in an included topology, there's no need to dive into a subtopology's, subtopology's definition to create a connection. Using address and position offsets mitigates the need for hard-coding IP addresses.

# Links
- yaml-cpp - https://github.com/jbeder/yaml-cpp

# Setup
## Dependencies
- yaml-cpp - libyaml-cpp-dev

- CMake - cmake

- GCC - gcc

### YAML
- Download yaml-cpp (libyaml-cpp-dev) and follow the install instructions.
- You may have to download and install the Boost libraries as well (libboost-dev).

# Pronunciation
Should this "language" be adopted by a substantial following, it is important that the calamity of the word "gif" not be repeated. You may pronounce this language's name/abbreviation as you like, "Entitle", "En-tidal", "Young-teedle", have at it.
