
#ifndef VANET_BROADCAST_HELPER_H
#define VANET_BROADCAST_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/ipv4-routing-helper.h"

namespace ns3
{
  /**
   * \ingroup vbp
   * \brief Helper class that adds VBP routing to nodes.
   */
  class VanetBroadcastHelper : public Ipv4RoutingHelper
  {
  public:
    VanetBroadcastHelper();
    ~VanetBroadcastHelper();
    /**
     * \returns pointer to clone of this VbpHelper
     *
     * \internal
     * This method is mainly for internal use by the other helpers;
     * clients are expected to free the dynamic memory allocated by this method
     */
    VanetBroadcastHelper *Copy(void) const;
    virtual Ptr<Ipv4RoutingProtocol> Create(Ptr<Node> node) const;
    /**
     * \param name the name of the attribute to set
     * \param value the value of the attribute to set.
     *
     * This method controls the attributes of ns3::vbp::RoutingProtocol
     */
    void Set(std::string name, const AttributeValue &value);

    void SetBroadcastArea(std::vector<float> broadcastArea);
    std::vector<float> GetBroadcastArea();

  private:
    /** the factory to create VBP routing object */
    ObjectFactory m_agentFactory;
    std::vector<float> m_broadcastArea = std::vector<float>(4,0);
  };

} // namespace ns3
#endif /* VANET_BROADCAST_HELPER_H */
