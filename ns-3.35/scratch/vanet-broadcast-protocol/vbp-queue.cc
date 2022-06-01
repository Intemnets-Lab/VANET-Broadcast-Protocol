#include "vbp-queue.h"
#include <fstream>

namespace ns3 {
    NS_LOG_COMPONENT_DEFINE("VbpQueue");
namespace vbp {

VbpQueue::VbpQueue ()   
    : m_queueSizeLimit(100)
    {

    }

 VbpQueue::~VbpQueue () {
 }

 TypeId VbpQueue::GetTypeId (void) {
   static TypeId tid = TypeId ("ns3::vbp::VbpQueue")
     .SetParent<Object> ()
     .AddConstructor<VbpQueue> ();
   return tid;
 }

void
VbpQueue::AppendPacket(Ptr<const Packet> p)
{
    if (!QueueFull())
    {
        m_packetQ.push_back(p);
    }
}

// void
// VbpQueue::AppendDestination(Ipv4Address destination)
// {
//     m_dstQ.push_back(destination);
// }

// void
// VbpQueue::AppendSource(Ipv4Address source)
// {
//     m_srcQ.push_back(source);
// }

// void
// VbpQueue::AppendProtocol(uint8_t protocol)
// {
//     m_protocolQ.push_back(protocol);
// }

// void
// VbpQueue::AppendUcb(Ipv4RoutingProtocol::UnicastForwardCallback ucb)
// {
//     if (!QueueFull())
//     {
//         m_ucbQ.push_back(ucb);
//     }
// }

// void
// VbpQueue::AppendEcb(Ipv4RoutingProtocol::ErrorCallback ecb)
// {
//     if (!QueueFull())
//     {
//         m_ecbQ.push_back(ecb);
//     }
// }

void
VbpQueue::AppendHeader(Ipv4Header header)
{
    if (!QueueFull())
    {
        m_headerQ.push_back(header);
    }
}

bool
VbpQueue::QueueFull()
{
    return GetQueueSize() >= m_queueSizeLimit;
}

Ptr<const Packet>
VbpQueue::GetPacket()
{
    std::cout << "Get Packet Queue" << std::endl;
    Ptr<const Packet> p = m_packetQ.front();
    m_packetQ.erase(m_packetQ.begin());
    return p;
}

Ptr<const Packet>
VbpQueue::PeekPacket()
{
    Ptr<const Packet> p = m_packetQ.front();
    return p;
}

// Ipv4Address
// VbpQueue::GetDestination()
// {
//     Ipv4Address dst = m_dstQ.front();
//     m_dstQ.erase(m_dstQ.begin());
//     return dst;
// }

// Ipv4Address
// VbpQueue::GetSource()
// {
//     Ipv4Address src = m_srcQ.front();
//     m_srcQ.erase(m_srcQ.begin());
//     return src;
// }

// uint8_t
// VbpQueue::GetProtocol()
// {
//     uint8_t protocol = m_protocolQ.front();
//     m_protocolQ.erase(m_protocolQ.begin());
//     return protocol;
// }

Ipv4RoutingProtocol::UnicastForwardCallback
VbpQueue::GetUcb()
{
    Ipv4RoutingProtocol::UnicastForwardCallback ucb = m_ucbQ.front();
    m_ucbQ.erase(m_ucbQ.begin());
    return ucb;
}

Ipv4RoutingProtocol::ErrorCallback
VbpQueue::GetEcb()
{
    Ipv4RoutingProtocol::ErrorCallback ecb = m_ecbQ.front();
    m_ecbQ.erase(m_ecbQ.begin());
    return ecb;
}

Ipv4Header
VbpQueue::GetHeader()
{
   std::cout << "Get Header Queue" << std::endl;
   Ipv4Header header = m_headerQ.front();
   m_headerQ.erase(m_headerQ.begin());
   return header;    
}

uint16_t
VbpQueue::GetQueueSize()
{
    return m_packetQ.size();
}

}
}
