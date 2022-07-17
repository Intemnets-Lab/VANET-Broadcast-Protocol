#ifndef RoutingProtocol_H
#define RoutingProtocol_H

#include <vector>
#include "ns3/mobility-model.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/random-variable-stream.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/udp-header.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "vbp-hello-packet-header.h"
#include "vbp-neighbor.h"
#include "vbp-rtable.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/ipv4-route.h"
#include "vbp-data-packet-header.h"
#include "vbp-queue.h"

namespace ns3
{

  namespace vbp
  {
    class RoutingProtocol : public Ipv4RoutingProtocol
    {
    public:
      /**
       * Get the type ID
       * \return the object TypeId
       */
      static TypeId GetTypeId(void);
      static const uint8_t PROT_NUMBER; //!< protocol number for VBP
      static const uint8_t UDP_PROT_NUMBER; //!< protocol number for UDP
      static const uint32_t VBP_HELLO_PORT; // Port for control (hello) packets
      static const uint32_t VBP_DATA_PORT; // Port for data (vbp) packets
      /// constructor
      RoutingProtocol();
      virtual ~RoutingProtocol();
      /**
       * Query routing cache for an existing route, for an outbound packet.
       * This lookup is used by transport protocols. It does not cause any packet to be forwarded, and is synchronous. Can be used for multicast or unicast. The Linux equivalent is ip_route_output()
       * The header input parameter may have an uninitialized value for the source address, but the destination address should always be properly set by the caller.
       * \param p packet to be routed. Note that this method may modify the packet. Callers may also pass in a null pointer.
       * \param header input parameter (used to form key to search for the route)
       * \param oif Output interface Netdevice. May be zero, or may be bound via socket options to a particular output interface.
       * \param sockerr Output parameter; socket errno
      */
      Ptr<Ipv4Route> RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
      /**
       * Route an input packet (to be forwarded or locally delivered)
       * This lookup is used in the forwarding process. The packet is handed over to the Ipv4RoutingProtocol, and will get forwarded onward by one of the callbacks.
       * The Linux equivalent is ip_route_input(). There are four valid outcomes, and a matching callbacks to handle each.
       * \param p packet to be routed by VBP
       * \param header input parameter used to form a search key for a route
       * \param idev Pointer to ingress network device
       * \param ucb Callback for the case in which the packet is to be forwarded as unicast
       * \param mcb Callback for the case in which the packet is to be forwarded as multicast
       * \param lcb Callback for the case in which the packet is to be locally delivered
       * \param ecb Callback to call if there is an error in forwarding
       * 
       * \returns true if the Ipv4RoutingProtocol takes responsibility for forwarding or delivering the packet, false otherwise
      */
      bool RouteInput(Ptr<const Packet> p,
                      const Ipv4Header &header,
                      Ptr<const NetDevice> idev,
                      UnicastForwardCallback ucb,
                      MulticastForwardCallback mcb,
                      LocalDeliverCallback lcb,
                      ErrorCallback ecb);
      /**
       * 
       * \param ipv4 the ipv4 object this routing protocol is being associated with
      */
      virtual void SetIpv4(Ptr<Ipv4> ipv4);
      /**
       * Sets broadcast area coordinates as {(x1,y1,x2,y2)} where (x1,y1) are the upper left corner of the BA
       * and (x2,y2) are the bottom right corner of the BA
       * \param broadcastArea broadcast area coordinates
      */
      void SetBroadcastArea(std::vector<float> broadcastArea);
      std::vector<float> GetBroadcastArea();
      /**
       * Print the Routing Table entries.
       * \param stream The ostream the Routing table is printed to
       * \param unit time unit to be used in the report
      */
      virtual void PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;
      /**
       * Protocols are expected to implement this method to be notified whenever a new address is added to an interface. 
       * Typically used to add a 'network route' on an interface. Can be invoked on an up or down interface.
       * \param interface the index of the interface we are being notified about
       * \param address a new address being added to an interface
      */
      virtual void NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address);
      /**
       * Protocols are expected to implement this method to be notified of the state change of an interface in a node.
       * \param interface the index of the interface we are being notified about
      */
      virtual void NotifyInterfaceDown(uint32_t interface);
      /**
       * Protocols are expected to implement this method to be notified of the state change of an interface in a node.
       * \param interface the index of the interface we are being notified about
      */
      virtual void NotifyInterfaceUp(uint32_t interface);
      /**
       * Protocols are expected to implement this method to be notified whenever a new address is removed from an interface. 
       * Typically used to remove the 'network route' of an interface. Can be invoked on an up or down interface.
       * \param interface the index of the interface we are being notified about
       * \param address a new address being added to an interface
      */
      virtual void NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address);
      void StartHelloTx(void); //Schedules transmission of hello headers

    private:
      int m_maxDistance; // maximum distance of one hop [meters]. Will not consider neighbors beyond this distance.
      int m_txCutoffPercentage; //reduces m_maxDistance by a factor
      float m_vcHighTraffic; // high traffic level based on LOS
      float m_vcLowTraffic; // low traffic level based on LOS
      float m_emptyQueuePeriod; // frequency EmptyQueue() should be called [seconds]
      int m_BroadcastTime; // amount of time broadcast area will be active [seconds]
      RoutingTable m_routingTable; // Routing table
      uint8_t m_helloPacketType; // Control packet type 'h'
      uint8_t m_dataPacketType;  // Data packet type 'd'
      Time m_activeRouteTimeout; //Period of time during which the route is considered to be valid
      // Loopback device used to defer RREQ until packet will be fully formed
      Ptr<NetDevice> m_lo;
      /// Provides uniform random variables
      Ptr<UniformRandomVariable> m_uniformRandomVariable;
      /// IP protocol
      Ptr<Ipv4> m_ipv4;
      Ptr<Node> m_thisNode;
      Ptr<Object> m_neighborsListPointer = CreateObject<Object>();
      Ptr<Object> m_queuePointer = CreateObject<Object>();
      //Ptr<Object> m_nextHopPointer = CreateObject<Object>();
      std::vector<float> m_broadcastArea = std::vector<float>(4,0);
      // Raw subnet directed broadcast socket per each IP interface, map socket -> iface address (IP + mask)
      std::map<Ptr<Socket>, Ipv4InterfaceAddress> m_socketSubnetBroadcastAddresses;
      // Raw unicast socket per each IP interface, map socket -> iface address (IP + mask)
      std::map<Ptr<Socket>, Ipv4InterfaceAddress> m_socketAddresses;

      //Routing Algorithms

      /**
       * Three cases for routing packets. 1. vehicle already in the broadcast area. 2. vehicle not in BA but will reach BA before expiration.
       * 3. calls FindNextHop().
       * 
       * \param p packet to be routed
       * \param dst destination of packet
       * \param src source of packet
       * \param packetSentIndicator set to true after packet is sent using Ipv4L3Protocol
       * 
       * \returns true if packet will be routed and lcb will be called. Otherwise, false and lcb will not be called.
      */
      bool RoutePacket(Ptr<Packet> p, Ipv4Address dst, Ipv4Address src, bool *packetSentIndicator);

      /**
       * Finds the first hop
       * \param nextHopAheadPtr 
       * \param nextHopBehindPtr
       * \returns true if first hop found
      */
      bool FindFirstHop(Ipv4Address* nextHopAheadPtr, Ipv4Address* nextHopBehindPtr);
      /**
       * Finds the next hop
       * \param nextHopAheadPtr 
       * \param nextHopBehindPtr
       * \param centerBA coordinates of center of the broadcast area
       * \param movingToBA is vehicle moving towards broadcast area
       * \param closeToBA will vehicle reach the broadcast area before expiration
       * \param enqueuePacketIndicator is packet placed in queue
       * \param prevHopIP IP address of previous vehicle that held the packet
       * \returns true if next hop found
      */
      bool FindNextHop(Ipv4Address* nextHopAheadPtr,Ipv4Address* nextHopBehindPtr,Vector centerBA, bool movingToBA, bool closeToBA, bool *enqueuePacketIndicator, Ipv4Address prevHopIP);
      /**
       * Finds the next hop downstream
       * \param centerBA coordinates of center of the broadcast area
       * \param movingToBA is vehicle moving towards broadcast area
       * \returns IP address of one-hop neighbor if neighbors found. Return 102.102.102.102 if no neighbor
      */
      Ipv4Address FindNextHopDownstream(Vector centerBA, bool movingToBA);
      /**
       * Finds the next hop upstream
       * \param centerBA coordinates of center of the broadcast area
       * \param movingToBA is vehicle moving towards broadcast area
       * \returns IP address of one-hop neighbor if neighbors found. Return 102.102.102.102 if no neighbor
      */
      Ipv4Address FindNextHopUpstream(Vector centerBA, bool movingToBA);
      /**
       * Finds the next hop index in downstream heavy traffic based on vehicle closest to broadcast area (if moving towards BA)
       * \param centerBA coordinates of center of the broadcast area
       * \param vehiclePos coordinates of vehicle
       * \param stopDist distance it takes for vehicle to stop (3 second rule)
       * 
       * \returns index of furthest ahead vehicle if moving towards broadcast area
      */
      int FindNextHopHighTrafficDownstream(Vector centerBA, Vector vehiclePos, float stopDist);
      /**
       * Finds next hop index in downstream medium traffic based on vehicle with max of sqrt(speed^2 + distToNeighbor^2)
       * \param neighborhoodSpeed average speed of a one-hop vehicle neighborhood
       * \param centerBA coordinates of center of the broadcast area
       * \param vehiclePos coordinates of vehicle
       * \param stopDist distance it takes for vehicle to stop (3 second rule)
       * 
       * \returns index of best hop if moving towards broadcast area
      */
      int FindNextHopMidTrafficDownstream(float neighborhoodSpeed, Vector centerBA, Vector vehiclePos, float stopDist);
      /**
       * Finds next hop index in downstream low traffic based on vehicle with minimum message delivery time
       * \param neighborhoodSpeed average speed of a one-hop vehicle neighborhood
       * \param centerBA coordinates of center of the broadcast area
       * \param vehiclePos coordinates of vehicle
       * \param stopDist distance it takes for vehicle to stop (3 second rule)
       * 
       * \returns index of best hop if moving towards broadcast area
      */
      int FindNextHopLowTrafficDownstream(float neighborhoodSpeed, Vector centerBA, Vector vehiclePos, float stopDist);
      /**
       * Finds the next hop index in upstream heavy traffic based on vehicle closest to broadcast area and moving towards BA
       * \param centerBA coordinates of center of the broadcast area
       * \param vehiclePos coordinates of vehicle
       * \param stopDist distance it takes for vehicle to stop (3 second rule)
       * 
       * \returns index of furthest ahead vehicle moving towards broadcast area
      */
      int FindNextHopHighTrafficUpstreamToBA(Vector centerBA, Vector vehiclePos, float stopDist);
      /**
       * Finds the next hop index in upstream heavy traffic moving away from the broadcast area based on neighbors behind that are minimum distance to the BA
       * \param centerBA coordinates of center of the broadcast area
       * \param vehiclePos coordinates of vehicle
       * \param stopDist distance it takes for vehicle to stop (3 second rule)
       * 
       * \returns index of minimum distance vehicle from broadcast area
      */
      int FindNextHopHighTrafficUpstreamAwayBA(Vector centerBA, Vector vehiclePos, float stopDist);
      /**
       * Finds next hop index in upstream medium traffic moving towards the broadcast area based on vehicle with max of sqrt(MDT^2 + distToNeighbor^2)
       * \param neighborhoodSpeed average speed of a one-hop vehicle neighborhood
       * \param centerBA coordinates of center of the broadcast area
       * \param vehiclePos coordinates of vehicle
       * \param stopDist distance it takes for vehicle to stop (3 second rule)
       * 
       * \returns index of best hop moving towards broadcast area
      */
      int FindNextHopMidTrafficUpstreamToBA(float neighborhoodSpeed,Vector centerBA, Vector vehiclePos, float stopDist);
      /**
       * Finds next hop index in upstream medium traffic moving away from the broadcast area based on vehicle with min of sqrt(speed^2 + distToNeighborToBA^2)
       * \param neighborhoodSpeed average speed of a one-hop vehicle neighborhood
       * \param centerBA coordinates of center of the broadcast area
       * \param vehiclePos coordinates of vehicle
       * \param stopDist distance it takes for vehicle to stop (3 second rule)
       * 
       * \returns index of best hop moving away from the broadcast area
      */
      int FindNextHopMidTrafficUpstreamAwayBA(float neighborhoodSpeed,Vector centerBA, Vector vehiclePos, float stopDist);
      /**
       * Finds next hop index in upstream low traffic towards the broadcast area based on neighbors behind with maximum MDT
       * \param neighborhoodSpeed average speed of a one-hop vehicle neighborhood
       * \param centerBA coordinates of center of the broadcast area
       * \param vehiclePos coordinates of vehicle
       * \param stopDist distance it takes for vehicle to stop (3 second rule)
       * 
       * \returns index of best hop moving towards broadcast area
      */
      int FindNextHopLowTrafficUpstreamToBA(float neighborhoodSpeed,Vector centerBA, Vector vehiclePos, float stopDist);
      /**
       * Finds next hop index in upstream low traffic away from the broadcast area based on neighbors behind with minimum speed
       * \param neighborhoodSpeed average speed of a one-hop vehicle neighborhood
       * \param centerBA coordinates of center of the broadcast area
       * \param vehiclePos coordinates of vehicle
       * \param stopDist distance it takes for vehicle to stop (3 second rule)
       * 
       * \returns index of best hop moving away from the broadcast area
      */
      int FindNextHopLowTrafficUpstreamAwayBA(float neighborhoodSpeed,Vector centerBA, Vector vehiclePos, float stopDist);
      /**
       * Empties the queue when queue size is greater than zero
       * 
      */
      void EmptyQueue();
      /**
       * Schedules EmptyQueue() based on definition of m_emptyQueuePeriod in the constructor
       * 
      */
      void ScheduleEmptyQueue();
      /**
       * Sets the route for data packets and sends using Ipv4L3Protocol
       * \param nextHopAheadPtr 
       * \param nextHopBehindPtr
       * \param p packet to be sent
       * \param dev device
       * \param iface device interface
       * \param src IP address of source node
       * \param dst IP address of destination node
       * 
       * \returns index of best hop moving away from the broadcast area
      */
      void SetSendFirstHop(Ipv4Address* nextHopAheadPtr,Ipv4Address* nextHopBehindPtr,Ptr<Packet> p,Ptr<NetDevice> dev ,Ipv4InterfaceAddress iface,Ipv4Address src,Ipv4Address dst);
      void SendHello(void); // Adds hello header information to packet to be sent
      /**
       * Send packet to neighbor
       * \param socket input socket
       * \param p the packet to route
       * \param destination IP address of neighbor.
       */
      void SendTo(Ptr<Socket> socket, Ptr<Packet> packet, Ipv4Address destination);
      /**
       * Receive and process packets
       * \param socket input socket
       */
      void RecvVbp(Ptr<Socket> socket);
      /**
       * Receive and process periodic packets.
       * \param p the packet to route
       * \param receiver my interface
       * \param sender IP address of neighbor.
       */
      void RecvHello(Ptr<Packet> p, Ipv4Address receiver, Ipv4Address sender);

      /**
       * Create loopback route for given header
       *
       * \param header the IP header
       * \param oif the output interface net device
       * \returns the route
       */
      Ptr<Ipv4Route> LoopbackRoute (const Ipv4Header & header, Ptr<NetDevice> oif) const;

      /**
         * Queue packet and send route request
         *
         * \param p the packet to route
         * \param header the IP header
         * \param ucb the UnicastForwardCallback function
         * \param ecb the ErrorCallback function
         */ 
        void DeferredRouteOutput (Ptr<const Packet> p, const Ipv4Header & header, UnicastForwardCallback ucb, ErrorCallback ecb);

    };

  } // namespace vbp
} // namespace ns3

#endif /* RoutingProtocol_H */