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
            /**
             * Appends packet to queue
             * 
            */
            void AppendPacket(Ptr<const Packet> p);
            /**
             * Appends header to queue
             * 
            */
            void AppendHeader(Ipv4Header header);
            /**
             * Gets the packets in the queue (FIFO)
             * 
            */
            Ptr<const Packet> GetPacket();
            /**
             * Gets the headers in the queue (FIFO)
             * 
            */
            Ipv4Header GetHeader();
            /**
             * Gets the number of packets in the queue
             * 
             * \returns number of packets in the queue
            */
            uint16_t GetQueueSize();
            /**
             * Calls error callback
             * 
            */
            Ipv4RoutingProtocol::ErrorCallback GetEcb();
            /**
             * Peeks the front packet of the queue
             * 
             * \returns front packet of the queue
            */
            Ptr<const Packet> PeekPacket();
            /**
             * Determins if queue is full
             * 
             * \returns true if queue full. Otherwise, false.
            */
            bool QueueFull();

        private:
            uint16_t m_queueSizeLimit;
            std::vector<Ptr<const Packet>> m_packetQ; // to hold queue of packet
            std::vector<Ipv4RoutingProtocol::ErrorCallback> m_ecbQ;
            std::vector<Ipv4Header> m_headerQ; // to hold queue of packet
        };

    } // namespace vbp
}     // namespace ns3

#endif /* VBP_QUEUE_H */
