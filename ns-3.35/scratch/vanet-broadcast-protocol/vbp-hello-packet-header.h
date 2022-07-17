#ifndef VBPPACKET_H
#define VBPPACKET_H

#include <iostream>
#include "ns3/header.h"
#include "ns3/enum.h"
#include "ns3/ipv4-address.h"
#include <map>
#include "ns3/nstime.h"

namespace ns3
{
  namespace vbp
  {
    class VbpHelloHeader : public Header
    {
    public:
      VbpHelloHeader();
      virtual ~VbpHelloHeader();
      /**
        * Sets information for control (hello) packets
        * 
        * \param packetType packet type
        * \param posX node position in the x-plane
        * \param posY node position in the y-plane
        * \param speedX node velocity in the x-plane
        * \param speedY node velocity in the y-plane
        * \param neighborsAhead number of nodes ahead within a one-hop distance
        * \param neighborsBehind number of nodes behind within a one-hop distance
        * \param neighborFurthestAheadX neighboring node furthest ahead in the x-plane
        * \param neighborFurthestAheadY neighboring node furthest ahead in the y-plane
        * \param neighborFurthestBehindX neighboring node furthest behind in the x-plane
        * \param neighborFurthestBehindY neighboring node furthest behind in the y-plane
        * \param avgSpeedX average velocity of the neighbors of a node in the x-plane
        * \param avgSpeedY average velocity of the neighbors of a node in the y-plane
      */
      void SetData(uint8_t packetType, float posX, float posY, float speedX, float speedY, uint16_t neighborsAhead, uint16_t neighborsBehind, float neighborFurthestAheadX, float neighborFurthestAheadY, float neighborFurthestBehindX, float neighborFurthestBehindY, float avgSpeedX, float avgSpeedY);
      /**
        * Gets the packet type
        * 
      */
      uint8_t GetPacketType(void) const;
      /**
        * Gets the number of neighbors ahead
        * 
        * \returns integer number of neighbors ahead
      */
      uint16_t GetNumNeighborsAhead(void) const;
      /**
        * Gets the number of neighbors behind
        * 
        * \returns integer number of neighbors behind
      */
      uint16_t GetNumNeighborsBehind(void) const;
      /**
        * Gets the x-coordinate of the forwarding node
        * 
        * \returns x-coordinate of the forwarding node
      */
      float GetPositionX(void) const; // location of forwarding node
      /**
        * Gets the y-coordinate of the forwarding node
        * 
        * \returns y-coordinate of the forwarding node
      */
      float GetPositionY(void) const;
      /**
        * Gets the current x-velocity of forwarding node
        * 
        * \returns x-velocity of forwarding node
      */
      float GetSpeedX(void) const; // current speed of forwarding node
      /**
        * Gets the current y-velocity of forwarding node
        * 
        * \returns y-velocity of forwarding node
      */
      float GetSpeedY(void) const;
      /**
        * Gets the x-location of the node furthest ahead of the forwarding node
        * 
        * \returns x-location of the node furthest ahead of the forwarding node
      */      
      float GetNeighborFurthestAheadX(void) const; // location of node furthest ahead of forwarding node
      /**
        * Gets the y-location of the node furthest ahead of the forwarding node
        * 
        * \returns y-location of the node furthest ahead of the forwarding node
      */    
      float GetNeighborFurthestAheadY(void) const;
      /**
        * Gets the x-location of the node furthest behind of the forwarding node
        * 
        * \returns x-location of the node furthest behind of the forwarding node
      */   
      float GetNeighborFurthestBehindX(void) const; // location of node furthest behind of forwarding node
      /**
        * Gets the y-location of the node furthest behind of the forwarding node
        * 
        * \returns y-location of the node furthest behind of the forwarding node
      */  
      float GetNeighborFurthestBehindY(void) const;
      /**
        * Gets the current average x-velocity of 1 hop neighbors of forwarding node
        * 
        * \returns current average x-velocity of 1 hop neighbors of forwarding node
      */ 
      float GetAvgSpeedX(void) const; // current average speed of 1 hop neighbors of forwarding node
      /**
        * Gets the current average y-velocity of 1 hop neighbors of forwarding node
        * 
        * \returns current average y-velocity of 1 hop neighbors of forwarding node
      */ 
      float GetAvgSpeedY(void) const;

      // below is inherited from Header

      //Get the type ID
      static TypeId GetTypeId(void); 
      virtual TypeId GetInstanceTypeId(void) const;
      virtual void Print(std::ostream &os) const;
      virtual void Serialize(Buffer::Iterator start) const;
      virtual uint32_t Deserialize(Buffer::Iterator start);
      virtual uint32_t GetSerializedSize(void) const;

    private:
      uint8_t m_packetType;
      uint16_t m_neighborsAhead;
      uint16_t m_neighborsBehind;
      uint8_t *m_positionX = new uint8_t[sizeof(float)];
      uint8_t *m_positionY = new uint8_t[sizeof(float)];
      uint8_t *m_speedX = new uint8_t[sizeof(float)];
      uint8_t *m_speedY = new uint8_t[sizeof(float)];
      uint8_t *m_neighborFurthestAheadX = new uint8_t[sizeof(float)];
      uint8_t *m_neighborFurthestAheadY = new uint8_t[sizeof(float)];
      uint8_t *m_neighborFurthestBehindX = new uint8_t[sizeof(float)];
      uint8_t *m_neighborFurthestBehindY = new uint8_t[sizeof(float)];
      uint8_t *m_avgSpeedX = new uint8_t[sizeof(float)];
      uint8_t *m_avgSpeedY = new uint8_t[sizeof(float)];
    };

  }

}
#endif /* VBPACKET_H */