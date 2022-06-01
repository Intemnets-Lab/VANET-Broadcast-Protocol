#include "vbp-hello-packet-header.h"

namespace ns3
{
     namespace vbp
     {

          VbpHelloHeader::VbpHelloHeader()
          {
               // we must provide a public default constructor,
               // implicit or explicit, but never private.
          }

          VbpHelloHeader::~VbpHelloHeader()
          {
          }

          TypeId VbpHelloHeader::GetTypeId(void)
          {
               static TypeId tid = TypeId("ns3::vbp::VbpHelloHeader")
                                       .SetParent<Header>()
                                       .AddConstructor<VbpHelloHeader>();
               return tid;
          }

          TypeId VbpHelloHeader::GetInstanceTypeId(void) const
          {
               return GetTypeId();
          }

          void VbpHelloHeader::Print(std::ostream &os) const
          {
          }

          uint32_t VbpHelloHeader::GetSerializedSize(void) const
          {
               // we reserve bytes for our header.
               uint32_t totalSize = sizeof(m_packetType) + sizeof(m_neighborsAhead) + sizeof(m_neighborsBehind) + sizeof(m_positionX) + sizeof(m_positionY) + sizeof(m_speedX) + sizeof(m_speedY) + sizeof(m_neighborFurthestAheadX) + sizeof(m_neighborFurthestAheadY) + sizeof(m_neighborFurthestBehindX) + sizeof(m_neighborFurthestBehindY) + sizeof(m_avgSpeedX) + sizeof(m_avgSpeedY);
               // size of header in bytes
               return totalSize;
          }

          void VbpHelloHeader::Serialize(Buffer::Iterator start) const
          {
               // we write them in network byte order.
               start.WriteU8(m_packetType);

               start.WriteHtonU16(m_neighborsAhead);
               start.WriteHtonU16(m_neighborsBehind);
               // use for loop to move 8 bits at a time to store each float
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_positionX[byteNum]);
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_positionY[byteNum]);
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_speedX[byteNum]);
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_speedY[byteNum]);
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_neighborFurthestAheadX[byteNum]);
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_neighborFurthestAheadY[byteNum]);
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_neighborFurthestBehindX[byteNum]);
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_neighborFurthestBehindY[byteNum]);
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_avgSpeedX[byteNum]);
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    start.WriteU8(m_avgSpeedY[byteNum]);
               }
          }

          uint32_t VbpHelloHeader::Deserialize(Buffer::Iterator start)
          {
               // we read them in network byte order and store them
               m_packetType = start.ReadU8();
               m_neighborsAhead = start.ReadNtohU16();
               m_neighborsBehind = start.ReadNtohU16();
               // use for loop to move 8 bits at a time to read each float
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_positionX[byteNum] = start.ReadU8();
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_positionY[byteNum] = start.ReadU8();
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_speedX[byteNum] = start.ReadU8();
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_speedY[byteNum] = start.ReadU8();
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_neighborFurthestAheadX[byteNum] = start.ReadU8();
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_neighborFurthestAheadY[byteNum] = start.ReadU8();
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_neighborFurthestBehindX[byteNum] = start.ReadU8();
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_neighborFurthestBehindY[byteNum] = start.ReadU8();
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_avgSpeedX[byteNum] = start.ReadU8();
               }
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_avgSpeedY[byteNum] = start.ReadU8();
               }
               // we return the number of bytes effectively read.
               return GetSerializedSize();
          }

          void VbpHelloHeader::SetData(uint8_t packetType, float posX, float posY, float speedX, float speedY, uint16_t neighborsAhead, uint16_t neighborsBehind, float neighborFurthestAheadX, float neighborFurthestAheadY, float neighborFurthestBehindX, float neighborFurthestBehindY, float avgSpeedX, float avgSpeedY)
          {
               m_packetType = packetType;
               m_neighborsAhead = neighborsAhead;
               m_neighborsBehind = neighborsBehind;
               // use for loop to move 8 bits at a time to store float into 4 bytes
               uint8_t *temp = (uint8_t *)&posX;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_positionX[byteNum] = temp[byteNum];
               }
               temp = (uint8_t *)&posY;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_positionY[byteNum] = temp[byteNum];
               }
               temp = (uint8_t *)&speedX;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_speedX[byteNum] = temp[byteNum];
               }
               temp = (uint8_t *)&speedY;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_speedY[byteNum] = temp[byteNum];
               }
               temp = (uint8_t *)&neighborFurthestAheadX;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_neighborFurthestAheadX[byteNum] = temp[byteNum];
               }
               temp = (uint8_t *)&neighborFurthestAheadY;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_neighborFurthestAheadY[byteNum] = temp[byteNum];
               }
               temp = (uint8_t *)&neighborFurthestBehindX;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_neighborFurthestBehindX[byteNum] = temp[byteNum];
               }
               temp = (uint8_t *)&neighborFurthestBehindY;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_neighborFurthestBehindY[byteNum] = temp[byteNum];
               }
               temp = (uint8_t *)&avgSpeedX;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_avgSpeedX[byteNum] = temp[byteNum];
               }
               temp = (uint8_t *)&avgSpeedY;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    m_avgSpeedY[byteNum] = temp[byteNum];
               }
          }

          uint8_t VbpHelloHeader::GetPacketType(void) const
          {
               // std::cout << unsigned(m_packetType) << std::endl;
               return m_packetType;
          }

          uint16_t VbpHelloHeader::GetNumNeighborsAhead(void) const
          {
               return m_neighborsAhead;
          }

          uint16_t VbpHelloHeader::GetNumNeighborsBehind(void) const
          {
               return m_neighborsBehind;
          }

          float VbpHelloHeader::GetPositionX(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_positionX[byteNum];
               }
               return temp;
          }

          float VbpHelloHeader::GetPositionY(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_positionY[byteNum];
               }
               return temp;
          }

          float VbpHelloHeader::GetSpeedX(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_speedX[byteNum];
               }
               return temp;
          }

          float VbpHelloHeader::GetSpeedY(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_speedY[byteNum];
               }
               return temp;
          }

          float VbpHelloHeader::GetNeighborFurthestAheadX(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_neighborFurthestAheadX[byteNum];
               }
               return temp;
          }

          float VbpHelloHeader::GetNeighborFurthestAheadY(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_neighborFurthestAheadY[byteNum];
               }
               return temp;
          }

          float VbpHelloHeader::GetNeighborFurthestBehindX(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_neighborFurthestBehindX[byteNum];
               }
               return temp;
          }

          float VbpHelloHeader::GetNeighborFurthestBehindY(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_neighborFurthestBehindY[byteNum];
               }
               return temp;
          }

          float VbpHelloHeader::GetAvgSpeedX(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_avgSpeedX[byteNum];
               }
               return temp;
          }

          float VbpHelloHeader::GetAvgSpeedY(void) const
          {
               // use for loop to move 8 bits at a time to store 4 bytes into a float
               float temp;
               uint8_t *asByte = (uint8_t *)&temp;
               for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++)
               {
                    asByte[byteNum] = m_avgSpeedY[byteNum];
               }
               return temp;
          }

     }
}