#ifndef VBPNEXTHOPFINDER_H
#define VBPNEXTHOPFINDER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/simulator.h"
#include "vanet-broadcast-protocol.h"


namespace ns3
{
    namespace vbp
    {
        class VbpNextHopFinder
        {
        public:
            VbpNextHopFinder ();
            virtual ~VbpNextHopFinder ();
            // static TypeId GetTypeId (void);
            // virtual TypeId GetInstanceTypeId (void) const;
            // virtual void Print (std::ostream &os) const;
            //Ipv4Address findNextHopDownstream(bool movementToBA);
            void HelloWorld();
        //private:
            // Ptr<Object> m_neighborsListPointer = CreateObject<Object>();
            // Ptr<Node> m_thisNode;
            // float m_vcHighTraffic;
            // float m_vcLowTraffic;

            };

    } // namespace vbp
}     // namespace ns3

#endif /* VBP_HOP_H */    

