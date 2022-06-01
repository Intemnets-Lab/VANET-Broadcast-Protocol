#include "vanet-broadcast-helper.h"
#include "vanet-broadcast-protocol.h"

namespace ns3
{
    NS_LOG_COMPONENT_DEFINE("VbpHelper");
  VanetBroadcastHelper::~VanetBroadcastHelper()
  {
  }

  VanetBroadcastHelper::VanetBroadcastHelper() : Ipv4RoutingHelper()
  {
    m_agentFactory.SetTypeId("ns3::vbp::RoutingProtocol");
    m_broadcastArea[0] = NAN;
    m_broadcastArea[1] = NAN;
    m_broadcastArea[2] = NAN;
    m_broadcastArea[3] = NAN;
  }

  VanetBroadcastHelper *
  VanetBroadcastHelper::Copy(void) const
  {
    return new VanetBroadcastHelper(*this);
  }

  Ptr<Ipv4RoutingProtocol>
  VanetBroadcastHelper::Create(Ptr<Node> node) const
  {
    NS_ASSERT_MSG(!(isnan(m_broadcastArea[0]) || isnan(m_broadcastArea[1]) || isnan(m_broadcastArea[2]) || isnan(m_broadcastArea[3])),"Need to set coordinates of broadcast area with VanetBroadcastHelper");
  
    Ptr<vbp::RoutingProtocol> agent = m_agentFactory.Create<vbp::RoutingProtocol>();
    node->AggregateObject(agent);
    Ptr<Ipv4> ipv4 = node->GetObject<Ipv4>();
    agent->SetIpv4(ipv4);
    agent->StartHelloTx();
    agent->SetBroadcastArea(m_broadcastArea);
    return agent;
  }

  void
  VanetBroadcastHelper::Set(std::string name, const AttributeValue &value)
  {
    m_agentFactory.Set(name, value);
  }

  std::vector<float> 
  VanetBroadcastHelper::GetBroadcastArea()
    {
        return m_broadcastArea;
    }

  void
  VanetBroadcastHelper::SetBroadcastArea(std::vector<float> broadcastArea)
    {
        m_broadcastArea[0] = broadcastArea[0];
        m_broadcastArea[1] = broadcastArea[1];
        m_broadcastArea[2] = broadcastArea[2];
        m_broadcastArea[3] = broadcastArea[3];
    }


} // namespace ns3