#include "vbp-data-packet-header.h"

namespace ns3
{
     namespace vbp
     {

 VbpRoutingHeader::VbpRoutingHeader ()
{
   // we must provide a public default constructor, 
   // implicit or explicit, but never private.
 }

 VbpRoutingHeader::~VbpRoutingHeader () {
 }
 
 TypeId VbpRoutingHeader::GetTypeId (void) {
   static TypeId tid = TypeId ("ns3::VbpRoutingHeader")
     .SetParent<Header> ()
     .AddConstructor<VbpRoutingHeader> ();
   return tid;
 }

 TypeId VbpRoutingHeader::GetInstanceTypeId (void) const {
   return GetTypeId ();
 }
 
 void VbpRoutingHeader::Print (std::ostream &os) const {
   // This method is invoked by the packet printing
   // routines to print the content of my header.
   // os << "Packet type: " << GetPacketType() << std::endl;
    os << "Previous Hop: " << GetPrevHopIP() << std::endl;
    //os << "point 1: " << GetPosition1X() << ", " << GetPosition1Y() << std::endl;
    //os << "point 2: " << GetPosition2X() << ", " << GetPosition2Y() << std::endl;
    //os << "BATime is: " << GetBroadcastingTime() << " seconds" << std::endl;
 }

 uint32_t VbpRoutingHeader::GetSerializedSize (void) const {
   // we reserve bytes for our header.
   uint32_t totalSize = sizeof(m_packetType) + sizeof(m_prevHopIP) + sizeof(m_Position1X) + sizeof(m_Position1Y) + sizeof(m_Position2X) + sizeof(m_Position2Y) + sizeof(m_BATime);
   //uint32_t totalSize = sizeof(m_packetType) + sizeof(m_sourceIP) + sizeof(m_prevHopIP) + sizeof(m_nextHopIdAhead) + sizeof(m_nextHopIdBehind) + sizeof(m_position1X) + sizeof(m_position1Y) + sizeof(m_Position2X) + sizeof(m_Position2Y) + sizeof(m_BATime);
   //size of header in bytes
   return totalSize;
 }

 void VbpRoutingHeader::Serialize (Buffer::Iterator start) const {
   // we write them in network byte order.
   start.WriteU8 (m_packetType);   
   WriteTo(start,m_prevHopIP);
   // use for loop to move 8 bits at a time to store each float
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        start.WriteU8(m_Position1X[byteNum]);
   }
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        start.WriteU8(m_Position1Y[byteNum]);
   }
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        start.WriteU8(m_Position2X[byteNum]);
   }
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        start.WriteU8(m_Position2Y[byteNum]);
   }
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        start.WriteU8(m_BATime[byteNum]);
   }
 }

 uint32_t VbpRoutingHeader::Deserialize (Buffer::Iterator start) {
   // we read them in network byte order and store them
   //std::cout << "VBPRoutingHeader Deserialize "  << std::endl; 
   m_packetType = start.ReadU8 ();
   //uint16_t count; count = 0;
   while (m_packetType != 'd')
   {
      //std::cout << "Count: " << ++count << std::endl;
      m_packetType = start.ReadU8 ();
   }

//    if (count != 8)
//    {
//         std::cout << "Deserialize not working" << std::endl;
//    }

   ReadFrom(start, m_prevHopIP);
   // use for loop to move 8 bits at a time to read each float
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_Position1X[byteNum] = start.ReadU8 ();
   }
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_Position1Y[byteNum] = start.ReadU8 ();
   }
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_Position2X[byteNum] = start.ReadU8 ();
   }
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_Position2Y[byteNum] = start.ReadU8 ();
   }
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_BATime[byteNum] = start.ReadU8 ();
   }
   // we return the number of bytes effectively read.
   return GetSerializedSize();
 }
 
 //void VbpRoutingHeader::SetData (uint8_t packetType, Ipv4Address sourceIP, Ipv4Address prevHopIP, uint16_t nextHopIdAhead, uint16_t nextHopIdBehind, float pos1X, float pos1Y, float pos2X, float pos2Y, float broadcastTime) {
 void VbpRoutingHeader::SetData (uint8_t packetType, Ipv4Address prevHopIP, float pos1X, float pos1Y, float pos2X, float pos2Y, float broadcastTime) {
   m_packetType = packetType;
   m_prevHopIP = prevHopIP;
   // use for loop to move 8 bits at a time to store float into 4 bytes
   uint8_t* temp = (uint8_t*) &pos1X;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_Position1X[byteNum] = temp[byteNum];
   }
   temp = (uint8_t*) &pos1Y;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_Position1Y[byteNum] = temp[byteNum];
   }
   temp = (uint8_t*) &pos2X;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_Position2X[byteNum] = temp[byteNum];
   }
   temp = (uint8_t*) &pos2Y;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_Position2Y[byteNum] = temp[byteNum];
   }
   temp = (uint8_t*) &broadcastTime;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        m_BATime[byteNum] = temp[byteNum];
   }
 }

 uint8_t VbpRoutingHeader::GetPacketType (void) const {
   return m_packetType;
 }
 


 Ipv4Address VbpRoutingHeader::GetPrevHopIP (void) const {
   return m_prevHopIP;
 }

 float VbpRoutingHeader::GetPosition1X (void) const {
   // use for loop to move 8 bits at a time to store 4 bytes into a float
   float temp;
   uint8_t* asByte = (uint8_t*) &temp;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        asByte[byteNum] = m_Position1X[byteNum];
   }
   return temp;
 }

 float VbpRoutingHeader::GetPosition1Y (void) const {
   // use for loop to move 8 bits at a time to store 4 bytes into a float
   float temp;
   uint8_t* asByte = (uint8_t*) &temp;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        asByte[byteNum] = m_Position1Y[byteNum];
   }
   return temp;
 }

float VbpRoutingHeader::GetPosition2X(void) const {
   // use for loop to move 8 bits at a time to store 4 bytes into a float
   float temp;
   uint8_t* asByte = (uint8_t*) &temp;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        asByte[byteNum] = m_Position2X[byteNum];
   }
   return temp;
 }

float VbpRoutingHeader::GetPosition2Y(void) const {
   // use for loop to move 8 bits at a time to store 4 bytes into a float
   float temp;
   uint8_t* asByte = (uint8_t*) &temp;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        asByte[byteNum] = m_Position2Y[byteNum];
   }
   return temp;
}

float VbpRoutingHeader::GetBroadcastingTime(void) const {
   // use for loop to move 8 bits at a time to store 4 bytes into a float
   float temp;
   uint8_t* asByte = (uint8_t*) &temp;
   for (uint32_t byteNum = 0; byteNum < sizeof(float); byteNum++) {
        asByte[byteNum] = m_BATime[byteNum];
   }
   return temp;
}

void
VbpRoutingHeader::SetPrevHopIP(Ipv4Address prevHopIP)
{
     m_prevHopIP = prevHopIP;
}

} //namespace vbp

} //namespace ns3