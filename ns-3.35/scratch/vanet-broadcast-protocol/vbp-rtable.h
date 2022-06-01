#ifndef VBP_RTABLE_H
#define VBP_RTABLE_H

#include <stdint.h>
#include <cassert>
#include <map>
#include <sys/types.h>
#include "ns3/ipv4.h"
#include "ns3/ipv4-route.h"
#include "ns3/timer.h"
#include "ns3/net-device.h"
#include "ns3/output-stream-wrapper.h"

namespace ns3 {
namespace vbp {

/**
 * \ingroup vbp
 * \brief Route record states
 */
enum RouteFlags
{
  VALID = 0,          //!< VALID
  INVALID = 1,        //!< INVALID
  IN_SEARCH = 2,      //!< IN_SEARCH
};

/**
 * \ingroup vbp
 * \brief Routing table entry
 */
class RoutingTableEntry
{
public:
  /**
   * constructor
   *
   * \param dev the device
   * \param dst the destination IP address
   * \param vSeqNo verify sequence number flag
   * \param seqNo the sequence number
   * \param iface the interface
   * \param hops the number of hops
   * \param nextHop the IP address of the next hop
   * \param lifetime the lifetime of the entry
   */
  RoutingTableEntry (Ptr<NetDevice> dev = 0,Ipv4Address dst = Ipv4Address (), bool vSeqNo = false, uint32_t seqNo = 0,
                     Ipv4InterfaceAddress iface = Ipv4InterfaceAddress (), uint16_t  hops = 0,
                     Ipv4Address nextHop = Ipv4Address (), Time lifetime = Simulator::Now ());

  ~RoutingTableEntry ();


  /**
   * Mark entry as "down" (i.e. disable it)
   * \param badLinkLifetime duration to keep entry marked as invalid
   */
  void Invalidate (Time badLinkLifetime);

  // Fields
  /**
   * Get destination address function
   * \returns the IPv4 destination address
   */
  Ipv4Address GetDestination () const
  {
    return m_ipv4Route->GetDestination ();
  }
  /**
   * Get route function
   * \returns The IPv4 route
   */
  Ptr<Ipv4Route> GetRoute () const
  {
    return m_ipv4Route;
  }
  /**
   * Set route function
   * \param r the IPv4 route
   */
  void SetRoute (Ptr<Ipv4Route> r)
  {
    m_ipv4Route = r;
  }
  /**
   * Set next hop address
   * \param nextHop the next hop IPv4 address
   */
  void SetNextHop (Ipv4Address nextHop)
  {
    m_ipv4Route->SetGateway (nextHop);
  }
  /**
   * Get next hop address
   * \returns the next hop address
   */
  Ipv4Address GetNextHop () const
  {
    return m_ipv4Route->GetGateway ();
  }
  /**
   * Set output device
   * \param dev The output device
   */
  void SetOutputDevice (Ptr<NetDevice> dev)
  {
    m_ipv4Route->SetOutputDevice (dev);
  }
  /**
   * Set the Ipv4InterfaceAddress
   * \param iface The Ipv4InterfaceAddress
   */
  void SetInterface (Ipv4InterfaceAddress iface)
  {
    m_iface = iface;
  }

  /**
   * Get the lifetime
   * \returns the lifetime
   */
  Time GetLifeTime () const
  {
    return m_lifeTime - Simulator::Now ();
  }
  /**
   * Set the route flags
   * \param flag the route flags
   */
  void SetFlag (RouteFlags flag)
  {
    m_flag = flag;
  }
  /**
   * Get the route flags
   * \returns the route flags
   */
  RouteFlags GetFlag () const
  {
    return m_flag;
  }

  /// RREP_ACK timer
  Timer m_ackTimer;

  /**
   * \brief Compare destination address
   * \param dst IP address to compare
   * \return true if equal
   */
  bool operator== (Ipv4Address const  dst) const
  {
    return (m_ipv4Route->GetDestination () == dst);
  }
  /**
   * Print packet to trace file
   * \param stream The output stream
   * \param unit The time unit to use (default Time::S)
   */
  void Print (Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;

private:
  /// Hop Count (number of hops needed to reach destination)
  uint16_t m_hops;
  /**
  * \brief Expiration or deletion time of the route
  *	Lifetime field in the routing table plays dual role:
  *	for an active route it is the expiration time, and for an invalid route
  *	it is the deletion time.
  */
  Time m_lifeTime;
  /** Ip route, include
   *   - destination address
   *   - source address
   *   - next hop address (gateway)
   *   - output device
   */
  Ptr<Ipv4Route> m_ipv4Route;
  /// Output interface address
  Ipv4InterfaceAddress m_iface;
  /// Routing flags: valid, invalid or in search
  RouteFlags m_flag;

  /// List of precursors
  std::vector<Ipv4Address> m_precursorList;
  /// When I can send another request
  Time m_routeRequestTimout;
  /// Number of route requests
  uint8_t m_reqCount;
  /// Time for which the node is put into the blacklist
  Time m_blackListTimeout;
};

/**
 * \ingroup vbp
 * \brief The Routing table used by VBP protocol
 */
class RoutingTable
{
public:
  /**
   * constructor
   * \param t the routing table entry lifetime
   */
  RoutingTable (Time t);
  ///\name Handle lifetime of invalid route
  //\{
  /**
   * Get the lifetime of a bad link
   *
   * \return the lifetime of a bad link
   */
  Time GetBadLinkLifetime () const
  {
    return m_badLinkLifetime;
  }
  /**
   * Set the lifetime of a bad link
   *
   * \param t the lifetime of a bad link
   */
  void SetBadLinkLifetime (Time t)
  {
    m_badLinkLifetime = t;
  }
  //\}


  /// Delete all entries from routing table
  void Clear ()
  {
    m_ipv4AddressEntry.clear ();
  }
  /// Delete all outdated entries and invalidate valid entry if Lifetime is expired
  void Purge ();

  /**
   * Print routing table
   * \param stream the output stream
   * \param unit The time unit to use (default Time::S)
   */
  void Print (Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;

private:
  /// The routing table
  std::map<Ipv4Address, RoutingTableEntry> m_ipv4AddressEntry;
  /// Deletion time for invalid routes
  Time m_badLinkLifetime;
  /**
   * const version of Purge, for use by Print() method
   * \param table the routing table entry to purge
   */
  void Purge (std::map<Ipv4Address, RoutingTableEntry> &table) const;
};

}  // namespace vbp
}  // namespace ns3

#endif /* VBP_RTABLE_H */
