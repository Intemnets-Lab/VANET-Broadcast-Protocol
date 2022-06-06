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
#include "vbp-next-hop-finder.h"

namespace ns3
{

  namespace vbp
  {
    class RoutingProtocol : public Ipv4RoutingProtocol
    {
    public:
      /**
       * \brief Get the type ID.
       * \return the object TypeId
       */
      static TypeId GetTypeId(void);
      static const uint8_t PROT_NUMBER; //!< protocol number
      static const uint32_t VBP_HELLO_PORT;
      /// constructor
      RoutingProtocol();
      virtual ~RoutingProtocol();
      // Inherited from Ipv4RoutingProtocol
      Ptr<Ipv4Route> RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
      bool RouteInput(Ptr<const Packet> p,
                      const Ipv4Header &header,
                      Ptr<const NetDevice> idev,
                      UnicastForwardCallback ucb,
                      MulticastForwardCallback mcb,
                      LocalDeliverCallback lcb,
                      ErrorCallback ecb);
      virtual void SetIpv4(Ptr<Ipv4> ipv4);
      bool RoutePacket(Ptr<Packet> p, Ipv4Address dst, Ipv4Address src, bool *packetSentIndicator);
      void SetBroadcastArea(std::vector<float> broadcastArea);
      std::vector<float> GetBroadcastArea();
      virtual void PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;
      virtual void NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address);
      virtual void NotifyInterfaceDown(uint32_t interface);
      virtual void NotifyInterfaceUp(uint32_t interface);
      virtual void NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address);
      // Periodic Packets
      void SendHello(void);
      void StartHelloTx(void);

    private:
      int m_maxDistance;
      int m_txCutoffPercentage;
      float m_vcHighTraffic;
      float m_vcLowTraffic;
      //VbpNextHopFinder m_nextHopFinder;
      float m_emptyQueuePeriod;
      int m_BroadcastTime;
      /// Routing table
      RoutingTable m_routingTable;
      //Protocol parameters
      uint8_t m_helloPacketType; ///< Set packet type to hello 'h'
      uint8_t m_dataPacketType;

      Time m_activeRouteTimeout; ///< Period of time during which the route is considered to be valid.
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
      /// Raw unicast socket per each IP interface, map socket -> iface address (IP + mask)
      std::map<Ptr<Socket>, Ipv4InterfaceAddress> m_socketAddresses;
      bool FindFirstHop(Ipv4Address* nextHopAheadPtr, Ipv4Address* nextHopBehindPtr);
     // bool FindNextHop(Ipv4Address* nextHopPtr);
      bool FindNextHop(Ipv4Address* nextHopAheadPtr,Ipv4Address* nextHopBehindPtr,Vector centerBA, bool movingToBA, bool closeToBA, bool *enqueuePacketIndicator, Ipv4Address prevHopIP);
      Ipv4Address FindNextHopDownstream(Vector centerBA, bool movingToBA);
      Ipv4Address FindNextHopUpstream(Vector centerBA, bool movingToBA);
      int FindNextHopHighTrafficDownstream(Vector centerBA, Vector vehiclePos, float stopDist);
      int FindNextHopMidTrafficDownstream(float neighborhoodSpeed, Vector centerBA, Vector vehiclePos, float stopDist);
      int FindNextHopLowTrafficDownstream(float neighborhoodSpeed, Vector centerBA, Vector vehiclePos, float stopDist);
      int FindNextHopHighTrafficUpstreamToBA(Vector centerBA, Vector vehiclePos, float stopDist);
      int FindNextHopHighTrafficUpstreamAwayBA(Vector centerBA, Vector vehiclePos, float stopDist);
      int FindNextHopMidTrafficUpstreamToBA(float neighborhoodSpeed,Vector centerBA, Vector vehiclePos, float stopDist);
      int FindNextHopMidTrafficUpstreamAwayBA(float neighborhoodSpeed,Vector centerBA, Vector vehiclePos, float stopDist);
      int FindNextHopLowTrafficUpstreamToBA(float neighborhoodSpeed,Vector centerBA, Vector vehiclePos, float stopDist);
      int FindNextHopLowTrafficUpstreamAwayBA(float neighborhoodSpeed,Vector centerBA, Vector vehiclePos, float stopDist);
      void EmptyQueue();
      void ScheduleEmptyQueue();
      void SetSendFirstHop(Ipv4Address* nextHopAheadPtr,Ipv4Address* nextHopBehindPtr,Ptr<Packet> p,Ptr<NetDevice> dev ,Ipv4InterfaceAddress iface,Ipv4Address src,Ipv4Address dst);

      Ptr<Socket> FindSocketWithInterfaceAddress(Ipv4InterfaceAddress iface) const;
      /**
       * Send packet to neighbor
       * \param socket input socket
       * \param p the packet to route
       * \param destination is supposed to be IP address of my neighbor.
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
       * \param receiver is supposed to be my interface
       * \param sender is supposed to be IP address of my neighbor.
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