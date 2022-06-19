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
   /**
    * Sets information for data (VBP) packets
    * 
    * \param packetType packet type
    * \param prevHopIP IP address of packet's previous hop
    * \param pos1X x-coordinate of upper-left point of the broadcast area
    * \param pos1Y y-coordinate of upper-left point of the broadcast area
    * \param pos2X x-coordinate of bottom-right point of the broadcast area
    * \param pos2Y y-coordinate of bottom-right point of the broadcast area
    * \param broadcastTime amount of time broadcast area will be active [seconds]
   */
   void SetData (uint8_t packetType, Ipv4Address prevHopIP, float pos1X, float pos1Y, float pos2X, float pos2Y, float broadcastTime);
   /**
    * Gets the packet type
    * 
   */
   uint8_t  GetPacketType (void) const;    
   /**
    * Gets the IP address of packet's previous hop
    * 
   */           
   Ipv4Address GetPrevHopIP (void) const;  
   /**
    * Gets the x-coordinate of upper-left point of the broadcast area
    * 
   */  
   float GetPosition1X (void) const;
   /**
    * Gets the y-coordinate of upper-left point of the broadcast area
    * 
   */                    
   float GetPosition1Y (void) const;
   /**
    * Gets the x-coordinate of bottom-right point of the broadcast area
    * 
   */  
   float GetPosition2X (void) const;
   /**
    * Gets the y-coordinate of bottom-right point of the broadcast area
    * 
   */  
   float GetPosition2Y (void) const;
   /**
    * Gets the amount of time broadcast area will be active [seconds]
    * 
   */  
   float GetBroadcastingTime (void) const;            
   static TypeId GetTypeId (void);
   virtual TypeId GetInstanceTypeId (void) const;
   /**
    * Prints contents of header
    * 
    * \returns Print-out of: Previous hop IP, broadcast area coordinates and BA expiration time
   */  
   virtual void Print (std::ostream &os) const;
   virtual void Serialize (Buffer::Iterator start) const;
   virtual uint32_t Deserialize (Buffer::Iterator start);
   virtual uint32_t GetSerializedSize (void) const;
   
 private:
   Ipv4Address m_prevHopIP; 
   uint8_t*  m_Position1X = new uint8_t[sizeof(float)]; 
   uint8_t*  m_Position1Y = new uint8_t[sizeof(float)];
   uint8_t*  m_Position2X = new uint8_t[sizeof(float)];
   uint8_t*  m_Position2Y = new uint8_t[sizeof(float)];
   uint8_t*  m_BATime = new uint8_t[sizeof(float)];
 };

   }

}
#endif /* VBPACKET_D */
