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
    std::cout << "Get Packet Queue " << m_packetQ.size() << std::endl;
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
//    if (m_headerQ.size() >= 0)
//     {
   std::cout << "Get Header Queue " << m_headerQ.size() << std::endl;
   Ipv4Header header = m_headerQ.front();
   std::cout << "Get Header Queue2" << std::endl;
   m_headerQ.erase(m_headerQ.begin());
   return header;
//    }
//   Ipv4Header dummyHeader;
//   return dummyHeader;    
}

uint16_t
VbpQueue::GetQueueSize()
{
    return m_packetQ.size();
}

}
}
