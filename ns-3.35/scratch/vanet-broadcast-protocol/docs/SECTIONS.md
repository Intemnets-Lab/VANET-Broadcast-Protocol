SECTIONS





###Minimum Requirements for a Routing Protocol

ns-3 includes implmentations of common routing protocols such as rip, olsr and aodv. It is recommended to also look at these routing protocols if you wish to view examples of other protocols built in this framework.

If you have a general question about ns-3 it is recommended to search the group conversations at ns-3-users(https://groups.google.com/g/ns-3-users)


VBP Requirements:
Requirements must be set before creating a custom routing protocol. For VBP our requirements included:
1. Nodes have mobility, meaning they can move within the 'x' and 'y' plane

2. Nodes can transmit and receive two types of packets: hello and data

3. Hello packets allow nodes to gather information about their neighbors (up to two hops). These are generated, transmitted and received on every node.

4. Data packets will be generated on the source node and hop until it reaches its destination

5. Multi-hop communication of data packets

6. Determine traffic levels

7. Based on traffic level pick an algorithm to find the next best hop

8. Queue packets in node that will most likely reach broadcast area

9. Define broadcast area, which can be thought of as a destination



VBP's routing protocol class (ns3::vbp::RoutingProtocol) can be found in vanet-broadcast-protocol.cc and vanet-broadcast-protocol.h. 

Class RoutingProtocol inherits from Ipv4RoutingProtocol. To create any custom routing protocol in ns-3 the following public member functions found in Ipv4RoutingProtocol must be implemented in your custom routing protocol:

    1. NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
    2. NotifyInterfaceDown (uint32_t interface)
    3. NotifyInterfaceUp (uint32_t interface)
    4. NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
    5. PrintRoutingTable (Ptr< OutputStreamWrapper > stream, Time::Unit unit=Time::S) 
    6. RouteInput (Ptr< const Packet > p, const Ipv4Header &header, Ptr< const NetDevice > idev, UnicastForwardCallback ucb, MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb)
    7. RouteOutput (Ptr< Packet > p, const Ipv4Header &header, Ptr< NetDevice > oif, Socket::SocketErrno &sockerr)


The following information is taken from http://tommy-ns-3-corner.blogspot.com/2014/03/how-to-develop-new-protocol.html:
    In a routing protocol, a routing table is built. A routing table contains information about the 1. destination 2. next hop 3. interface 4. prefix to use.
    Routing tables are used in RouteOutput and RouteInput. 

    RouteOutput is called when a packet is initially sent from the source node.
    RouteInput is called when a packet is received. This is where we need to decide if the packet should be forwarded to the next node or if it is meant to stay in this node.


VBP has two types of packets implemented: control and data.
Control packets (also known as hello packets) are contained in the class `VbpRoutingHeader`.
The high-level goal of this class is the following:
    1. Setting the data
    2. Getting the data
    3. Data storing and transferring

Data packets deal (also known as VBP packets) are found in the class `VbpRoutingHeader`. 




### Logging

Logging is used to monitor the codebase or for debugging purposes.

To enable all logging messages:
```bash
NS_LOG="*" ./waf --run vanet-broadcast-protocol
```

#### Logging terminal output
For logging messages from a specific class, search for NS_LOG_COMPONENT_DEFINE in the protocol files.
If you wish to enable log outputs for VBP you will find at the top of the script
```NS_LOG_COMPONENT_DEFINE("VanetBroadcastProtocol");```

In the terminal type
```bash
NS_LOG="VanetBroadcastProtocol" ./waf --run vanet-broadcast-protocol
```

#### Logging Syntax
When writing code