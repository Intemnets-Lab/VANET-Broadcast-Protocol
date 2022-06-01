#include "vbp-next-hop-finder.h"
#include <fstream>

namespace ns3 {
    NS_LOG_COMPONENT_DEFINE("VbpNextHopFinder");
namespace vbp {

VbpNextHopFinder::VbpNextHopFinder ()   
    //: //m_vcHighTraffic(0.8),
      //m_vcLowTraffic(0.4)
    {
      // Ptr<VbpNeighbors> m_neighborsListPointer2 = CreateObject<VbpNeighbors>();
      // m_neighborsListPointer->AggregateObject(m_neighborsListPointer2);
    }

 VbpNextHopFinder::~VbpNextHopFinder () {
 }

//  TypeId VbpNextHopFinder::GetTypeId (void) {
//    static TypeId tid = TypeId ("ns3::vbp::VbpNextHopFinder")
//      .SetParent<VbpNextHopFinder> ()
//      .AddConstructor<ns3::vbp::VbpNextHopFinder> ();
//    return tid;
//  }

//   TypeId VbpNextHopFinder::GetInstanceTypeId (void) const {
//    return GetTypeId ();
//  }

//  void VbpNextHopFinder::Print (std::ostream &os) const {
//  }

  // Ipv4Address 
  // VbpNextHopFinder::findNextHopDownstream(bool movementToBA) {   
  //   // packet moves downstream:
  //           // vehicle moves away from BA: wont receive         
  //           // vehicle moves towards BA: 
  //               // High traffic: consider neighbors ahead, min Dist to BA
  //               // Low traffic: consider neighbors ahead, min MDT
  //               // Mid traffic: consider neighbors ahead, Max of sqrt(speed^2 + distToNeighbor^2)    
  //   std::cout << "\n\n\n********For downstream ";
  //   if (!movementToBA) {
  //       return Ipv4Address("102.102.102.102"); // BA in upstream, not downstream
  //   }    
  //   Ptr<VbpNeighbors> allNeighborInfo = m_neighborsListPointer->GetObject<VbpNeighbors>(); // turn into a pointer, also methods that use it    
  //   // check if have neighbors
  //   uint16_t numNeighbors = allNeighborInfo->Get1HopNumNeighborsAhead(); 
  //   if (numNeighbors == 0) {
  //       return Ipv4Address("102.102.102.102"); // same as receiving node since no neighbors
  //   }
  //   // check traffic conditions of receiving node
  //   Vector nodePos = m_thisNode->GetObject<MobilityModel>()->GetPosition();            
  //   Vector nodeSpeed = m_thisNode->GetObject<MobilityModel>()->GetVelocity(); 
  //   float LOS = allNeighborInfo->GetLosCalculation(nodePos, nodeSpeed); // need to check if neighbor is moving toward broadcast area, if not moving towards bA then ignore
  //   uint16_t traffic; // if 3, high traffic. if 2, medium traffic, 1 no traffic
  //   float stopDist = nodeSpeed.GetLength()*3; // stopping distance according to DMV= speed*3 seconds. Distance needed to stop   
  //       // include paramter to determine if msg moves ahead or backwards
  //   //int nextHopIdx;
  //   Ipv4Address nextHop;
  //   //float neighborHoodSpeed = Vector3D(allNeighborInfo->GetNeighborHoodSpeedMeanX(), allNeighborInfo->GetNeighborHoodSpeedMeanY(),0).GetLength();
  //   if (LOS>m_vcHighTraffic) {
  //       traffic = 3; // high traffic
  //       nextHop = allNeighborInfo->Get1HopNeighborIPAhead(0);
  //       //nextHopIdx = findNextHopHighTrafficDownstream(allNeighborInfo, center, receivingNode->GetObject<MobilityModel>()->GetPosition(), stopDist);
  //           //return node that is closest to broadcast area(Dn)
  //   } else if (LOS<m_vcLowTraffic) {
  //       traffic = 1; // no traffic
  //       nextHop = allNeighborInfo->Get1HopNeighborIPAhead(0);
  //       //nextHopIdx = findNextHopLowTrafficDownstream(neighborHoodSpeed, allNeighborInfo, center, receivingNode->GetObject<MobilityModel>()->GetPosition(), stopDist);
  //           //return node with MDT
  //   } else {
  //       traffic = 2; // medium traffic otherwise
  //       nextHop = allNeighborInfo->Get1HopNeighborIPAhead(0);
  //       //nextHopIdx = findNextHopMidTrafficDownstream(neighborHoodSpeed, allNeighborInfo, center, receivingNode->GetObject<MobilityModel>()->GetPosition(), stopDist);
  //           //return node with smallest Dn and MDT
  //   }
  //   //std::cout << "\ncurrent time: " << Simulator::Now() << std::endl;
  //   //data.Print(std::cout);           
  //   //std::cout << "Next hop speed is: " << allNeighborInfo->GetNeighborSpeedX(nextHopIdx) <<", " << allNeighborInfo->GetNeighborSpeedY(nextHopIdx)<< std::endl;
  //   //std::cout << "My position is: " << receivingNode->GetObject<MobilityModel>()->GetPosition().x <<", " << receivingNode->GetObject<MobilityModel>()->GetPosition().y<< std::endl;
  //   //std::cout << "Next hop position is: " << allNeighborInfo->GetNeighborPositionX(nextHopIdx) <<", " << allNeighborInfo->GetNeighborPositionY(nextHopIdx)<< std::endl;
  //   //std::cout << "Destination is: " << BA1.x <<", " << BA1.y<< std::endl;
  //   if (nextHop != Ipv4Address("102.102.102.102")) {
  //      // std::cout << "\nMy id is: " << receivingNode->GetId() << std::endl;  
  //       std::cout << "LOS (Volume to Capacity ratio) is: " << LOS << std::endl;
  //       std::cout << "traffic is: " << traffic << std::endl;
  //       std::cout << "My speed is: " << nodeSpeed.x <<", " << nodeSpeed.y<< ", StopDist is: " << stopDist <<std::endl;
  //       m_neighborsListPointer->GetObject<VbpNeighbors>()->PrintNeighborState();
  //       //std::cout << "In findNextHop, source is: " << data.GetSourceId() << ", prev hop is: "<< data.GetPrevHopId();                
  //       std::cout << ", next found hop is: " << m_neighborsListPointer->GetObject<VbpNeighbors>()->Get1HopNeighborIP(0) << std::endl;
  //       return m_neighborsListPointer->GetObject<VbpNeighbors>()->Get1HopNeighborIP(0);
  //   }
  //   return Ipv4Address("102.102.102.102");
  // }

void VbpNextHopFinder::HelloWorld()
{
    std::cout << "Hello World" << std::endl;
}
  
}
}