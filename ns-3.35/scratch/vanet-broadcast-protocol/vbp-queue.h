#ifndef VBPQUEUE_H
#define VBPQUEUE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/simulator.h"
#include "vanet-broadcast-protocol.h"
#include "vbp-data-packet-header.h"
#include "ns3/ipv4-l3-protocol.h"

namespace ns3
{
    namespace vbp
    {
        class VbpQueue : public Object
        {
        public:
            VbpQueue();
            virtual ~VbpQueue();
            static TypeId GetTypeId (void);
            void AppendPacket(Ptr<const Packet> p);
            // void AppendDestination(Ipv4Address destination);
            // void AppendSource(Ipv4Address source);
            // void AppendProtocol(uint8_t protocol);
            // void AppendUcb(Ipv4RoutingProtocol::UnicastForwardCallback ucb);
            // void AppendEcb(Ipv4RoutingProtocol::ErrorCallback ecb);
            void AppendHeader(Ipv4Header header);
            Ptr<const Packet> GetPacket();
            // Ipv4Address GetDestination();
            // Ipv4Address GetSource();
            Ipv4Header GetHeader();
            uint16_t GetQueueSize();
            //uint8_t GetProtocol();
            Ipv4RoutingProtocol::UnicastForwardCallback GetUcb();
            Ipv4RoutingProtocol::ErrorCallback GetEcb();
            Ptr<const Packet> PeekPacket();
            bool QueueFull();

        private:
            uint16_t m_queueSizeLimit;
            std::vector<Ptr<const Packet>> m_packetQ; // to hold queue of packet
            //std::vector<uint8_t> m_protocolQ;
            //std::vector<Ipv4Address> m_dstQ; // to hold queue of packet
            //std::vector<Ipv4Address> m_srcQ; // to hold queue of packet
            std::vector<Ipv4RoutingProtocol::UnicastForwardCallback> m_ucbQ;
            std::vector<Ipv4RoutingProtocol::ErrorCallback> m_ecbQ;
            std::vector<Ipv4Header> m_headerQ; // to hold queue of packet
        };

    } // namespace vbp
}     // namespace ns3

#endif /* VBP_QUEUE_H */
