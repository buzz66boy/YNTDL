# YNTDL Topology Printer
## Overview
The printer enables concise viewing and searching of a topology for easier debugging of topology files (and the parser itself).

## Usage
`./print_yntdl pathToTopologyFile [-a] [-n] [-l] [-i] [-p] [-h] [-m]`

`-a[app name]`: print only applications/[specified app]

`-n[node name]`: print only nodes/[specified node]

`-l[link name]`: print only links/[specified link]

`-i[interface name]`: prints only interfaces/[specified interface]

`-p[time]`: print positions/[at specific time]

`-h`: show this help text

`-m`: DOUBLE HELP, show the small manual

For each argument, if you want to search names that have spaces, wrap the string in quotes (e.g. -n"node name with spaces").

## How flags work
The app, node, link, and interface flags (a, n, l, i), when associated with a search will return all of the objects with a partial name match.

For example, in a topology with nodes: "node1", "node2", and "connect", if you ran the printer with `-nnode` it would return "node1" and "node2".

### Combining flags
If all those nodes had application "sl", `-nnode -al` would print all the application information for "sl" (it matched the 'l' in the name) on nodes 1 and 2. If there was no application containing the letter 'l', nothing would print.
Similarly "-ieth -n -l" would print all nodes and links with an interface containing "eth" in the name, and you could add additional criteria (such as a node search term) to narrow your results.
