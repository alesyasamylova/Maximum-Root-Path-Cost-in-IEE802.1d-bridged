# Maximum-Root-Path-Cost-in-IEE802.1d-bridged


It is C++ program to calculate and print the maximum value of Root Path Cost based on Dijkstra's algorithm. 
It reads input data from file. The name of file is specified in command line. If the name of the file is not provided,
the program should get input data from the standard input. 

The input file must have the following format: 

>[BRIDGE 1] <br/>
>BridgeId = 42 
>
>[BRIDGE 1.Port 01]  <br/>
>PathCost = 10  <br/>
>ConnectedTo = LAN A 
>
>[BRIDGE 1.Port 02]  <br/>
>PathCost = 10  <br/>
>ConnectedTo = LAN B 
>
>... 
>
>[BRIDGE N]  <br/>
>BridgeId = 97 
>
>[BRIDGE N.Port 01] <br/>
>PathCost = 05  <br/>
>ConnectedTo = LAN C 
>
>... 
>
>[BRIDGE N.Port M]  <br/>
>PathCost = 10  <br/>
>ConnectedTo = LAN A 

## Functional specification 
The program does the following: 
* Takes the input file name from the command line. If the name of the file is not provided,<br/>
the program  should get input data from the standard input file (e.g. “test_bridges.txt”); 
* Reads the data from the file and parse it into suitable data structures; 
* Finds the Root Bridge; 
* Finds the Root Path Cost for each Bridge in the LAN; 
* Finds maximum value of Root Path Cost and print it to console.

## Key definitions
**The IEEE 802.1d-1998** is the IEEE MAC Bridges standard which includes Bridging, Spanning Tree and others, revised in 1998.

**The Spanning Tree Protocol (STP)** is a network protocol that builds a logical loop-free topology for Ethernet networks. 

**The Root** is the Bridge with the highest priority Bridge Identifier (for convenience of calculation, this is the identifier with the lowest numerical value) [pages 60,78 of IEEE 802.1d-1998]  

Every Bridge Port in the Bridged LAN has a **Root Path Cost**. This is the sum of the Path Costs for each Bridge Port receiving frames forwarded from the Root on the least cost path to the Bridge. 

Below is the example of an IEEE 802.1d bridged LAN (Local Area Network). 
