#ifndef VBPPACKET_D
#define VBPPACKET_D

#include <iostream>
#include "ns3/header.h"
#include "ns3/enum.h"
#include "ns3/ipv4-address.h"
#include <map>
#include "ns3/nstime.h"
#include "ns3/address-utils.h"

namespace ns3
{
  namespace vbp
  {

 class VbpRoutingHeader : public Header {
 public:
 
   VbpRoutingHeader ();
   virtual ~VbpRoutingHeader ();
  uint8_t  m_packetType;
   void SetData (uint8_t packetType, Ipv4Address prevHopIP, float pos1X, float pos1Y, float pos2X, float pos2Y, float broadcastTime);
  //void SetData (uint8_t packetType, uint16_t sourceId, uint16_t prevHopId, uint16_t nextHopIdAhead, uint16_t nextHopIdBehind, float pos1X, float pos1Y, float pos2X, float pos2Y, float broadcastTime);
               // called when storing information to send in data packet
   uint8_t  GetPacketType (void) const;               // data packet is type 1
   Ipv4Address GetPrevHopIP (void) const;                // id of node who held data packet previously
   float GetPosition1X (void) const;                  // Position 1 and 2 create coordinates for broadcasting area (BA)
   float GetPosition1Y (void) const;
   float GetPosition2X (void) const;
   float GetPosition2Y (void) const;
   float GetBroadcastingTime (void) const;            // time in seconds BA will expire
   static TypeId GetTypeId (void);
   virtual TypeId GetInstanceTypeId (void) const;
   virtual void Print (std::ostream &os) const;
   virtual void Serialize (Buffer::Iterator start) const;
   virtual uint32_t Deserialize (Buffer::Iterator start);
   virtual uint32_t GetSerializedSize (void) const;
   void SetPrevHopIP(Ipv4Address prevHopIP);
   
 private:
   Ipv4Address m_prevHopIP; 
   uint8_t*  m_Position1X = new uint8_t[sizeof(float)]; //4, to be able to use float values
   uint8_t*  m_Position1Y = new uint8_t[sizeof(float)];
   uint8_t*  m_Position2X = new uint8_t[sizeof(float)];
   uint8_t*  m_Position2Y = new uint8_t[sizeof(float)];
   uint8_t*  m_BATime = new uint8_t[sizeof(float)];
 };

   }

}
#endif /* VBPACKET_D */
