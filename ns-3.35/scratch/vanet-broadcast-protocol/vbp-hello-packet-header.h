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

      void SetData(uint8_t packetType, float posX, float posY, float speedX, float speedY, uint16_t neighborsAhead, uint16_t neighborsBehind, float neighborFurthestAheadX, float neighborFurthestAheadY, float neighborFurthestBehindX, float neighborFurthestBehindY, float avgSpeedX, float avgSpeedY);
      uint8_t GetPacketType(void) const;
      uint16_t GetNumNeighborsAhead(void) const;
      uint16_t GetNumNeighborsBehind(void) const;
      float GetPositionX(void) const; // location of forwarding node
      float GetPositionY(void) const;
      float GetSpeedX(void) const; // current speed of forwarding node
      float GetSpeedY(void) const;
      float GetNeighborFurthestAheadX(void) const; // location of node furthest ahead of forwarding node
      float GetNeighborFurthestAheadY(void) const;
      float GetNeighborFurthestBehindX(void) const; // location of node furthest behind of forwarding node
      float GetNeighborFurthestBehindY(void) const;
      float GetAvgSpeedX(void) const; // current average speed of 1 hop neighbors of forwarding node
      float GetAvgSpeedY(void) const;
      // below is inherited from Header
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