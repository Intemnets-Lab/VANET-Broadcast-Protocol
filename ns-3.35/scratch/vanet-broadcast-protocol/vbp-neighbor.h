#ifndef VBPNEIGHBOR_H
#define VBPNEIGHBOR_H
#include <vector>
#include "ns3/simulator.h"
#include "ns3/timer.h"
#include "ns3/ipv4-address.h"
#include "ns3/callback.h"
#include "ns3/arp-cache.h"
#include "vanet-broadcast-protocol.h"
#include <iostream> 
#include <algorithm>
#include "ns3/log.h"
#include "ns3/wifi-mac-header.h"
#include "vbp-data-packet-header.h"

namespace ns3 {
namespace vbp {

class VbpNeighbors : public Object {
    public:
        VbpNeighbors ();
        virtual ~VbpNeighbors ();
        static TypeId GetTypeId (void);
        virtual TypeId GetInstanceTypeId (void) const;
        virtual void Print (std::ostream &os) const;
        void AppendNeighbor(Ipv4Address neighborAddress);
        void AppendQueue(const Ptr<Packet> p);
        bool QueueEmpty();
        void RemoveQueue(VbpRoutingHeader dataHeader);
        int FindNeighbor (Ipv4Address address); // returns index for specified nodeIP, returns -1 if new nodeIP
        void AddNode (Ipv4Address address,
                      uint16_t direction,
                      uint16_t neighborsAhead,
                      uint16_t neighborsBehind,
                      float posX,
                      float posY,
                      float speedX, 
                      float speedY, 
                      float neighborFurthestAheadX, 
                      float neighborFurthestAheadY,
                      float neighborFurthestBehindX, 
                      float neighborFurthestBehindY, 
                      float avgSpeedX, 
                      float avgSpeedY);
        Ptr<Packet> GetPacketQueue();
        uint16_t GetQueueSize();
        uint16_t Get1HopNumNeighbors ();       // total number of 1 hop neighbors
        uint16_t Get1HopNumNeighborsAhead ();  // number of neighbors ahead of 1 hop neighbor
        uint16_t Get1HopNumNeighborsBehind (); // number of neighbors behind 1 hop neighbor
        Ipv4Address Get1HopNeighborIPAhead (uint16_t index);  // id of 1 hop neighbor ahead, grabbed by index
        Ipv4Address Get1HopNeighborIPBehind (uint16_t index); // id of 1 hop neighbor behind, grabbed by index
        Ipv4Address Get1HopNeighborIP (uint16_t index);  // get id based on index in vector if neighbor Ids, grabbed by index
        uint16_t Get1HopDirectionByIP (Ipv4Address address);   // if 1 hop neighbor ahead or behind, grabbed by nodeId
        uint16_t Get1HopDirection (uint16_t index);        // if 1 hop neighbor ahead or behind, grabbed by index
        uint16_t Get1HopNumberOfNodesAheadOfNeighbor (uint16_t index);  // if 1 hop neighbor has neighbor's ahead
        uint16_t Get1HopNumberOfNodesBehindOfNeighbor (uint16_t index); // if 1 hop neighbor has neighbor's behind
        float GetNeighborPositionX (uint16_t index);  // get location, grabbed by index
        float GetNeighborPositionY (uint16_t index);
        float GetNeighborSpeedX (uint16_t index);     // get speed, grabbed by index
        float GetNeighborSpeedY (uint16_t index);
        std::vector<float> Get1HopNeighborLocationsX ();   // get locations for all 1 hop neighbors
        std::vector<float> Get1HopNeighborLocationsY ();
        std::vector<float> Get1HopNeighborSpeedX ();       // get speed for all 1 hop neighbors
        std::vector<float> Get1HopNeighborSpeedY ();
        float GetNeighborFurthestAheadX (uint16_t index);  // location for furthest vehicle ahead for a specific node, grabbed by index
        float GetNeighborFurthestAheadY (uint16_t index); 
        float GetNeighborFurthestBehindX (uint16_t index); // location for furthest vehicle behind for a specific node, grabbed by index
        float GetNeighborFurthestBehindY (uint16_t index);
        float GetAvgSpeedNeighborX (float speedReferenceX);// from neighbors, find average speed
        float GetAvgSpeedNeighborY (float speedReferenceY);
        int GetNeighborFurthestAheadByIndex(Vector reference);  // find vehicle that is furthest ahead within 1 hop
        int GetNeighborFurthestBehindByIndex(Vector reference);
        float GetNeighborAvgSpeedX(uint16_t index);        // get average speed of 1 hop neighbor grabbed by index
        float GetNeighborAvgSpeedY(uint16_t index);
        int Get2HopDistFurthestAheadByIndex(Vector reference);  // discuss working cases, highlight that it handles other cases
        int Get2HopDistFurthestBehindByIndex(Vector reference);
        uint16_t Get2HopCarCount(int twoHopFurthestAheadIndex, int twoHopFurthestBehindIndex, Vector reference);
        uint16_t Get2HopCarCountSelfAhead(int twoHopFurthestAheadIndex, Vector reference);
        uint16_t Get2HopCarCountSelfBehind(int twoHopFurthestBehindIndex, Vector reference);
        float GetNeighborHoodSpeedMeanX(); // get average from sampled values in m_mostRecentNeighborHoodNSpeedX
	    float GetNeighborHoodSpeedMeanY(); // get average from sampled values in m_mostRecentNeighborHoodNSpeedY
        float GetLosCalculation(Vector referencePos, Vector referenceVel);
        void SetThisNode(Ptr<Node> n);
        void PrintNeighborState ();               // will display different info
        void PrintNeighbors();
        void PrintNeighbors2();
        void PrintDirections ();              // will display entries in m_1HopNeighborIDs
        void PrintNeighborsAhead ();          // will display neighbor IDs that are ahead
        void PrintNeighborsBehind ();         // will display neighbor IDs that are behind
        void PrintNumNeighborsAheadBehind (); // will display value in m_myNeighborsAhead and m_myNeighborsBehind
        void PrintNeighborDirections ();      // will display entries in m_neighborAhead and m_neighborBehind
        void PrintLocations ();               // will display entries in m_positionX and m_positionY
        void PrintSpeeds();                   // will display entries in m_neighborSpeedX and m_neighborSpeedY
        void PrintTimes ();                   // will display entries in m_neighborLastTime
        void Print1hopFurthestAhead ();       // will display position of neighbor furthest ahead
        void Print1hopFurthestBehind ();      // will display position of neighbor furthest behind
        void PrintAvgSpeeds ();               // will display average speed of 1 hop neighbors
    private:
        bool m_filledFirstTime;
        int m_currentIdx;
        int m_NSamples;
        Ptr<Node> m_thisNode;
        int m_speedLogPeriod;
        float m_neighborRemovalPeriod; // seconds
        int m_neighborTimeout;
        float m_capacityPerLane;
        int m_numLanes;
        uint16_t m_1HopNumNeighbors;        // total number of neighbors
        uint16_t m_1HopNumNeighborsBehind;  // number of neighbors behind
        std::vector<Ipv4Address> m_1HopNeighborIPs;       // store all 1 hop neighborIPs
        std::vector<Ipv4Address> m_1HopNeighborIPBehind;  // store each neighborIP of neighbors behind
        uint16_t m_1HopNumNeighborsAhead;   // number of neighbors ahead
        std::vector<Ipv4Address> m_1HopNeighborIPAhead;   // store each neighborIP of neighbors ahead
        std::vector<uint16_t> m_1HopNeighborDirection; // store if neighbor is ahead=+1 or behind=0
        std::vector<uint16_t> m_1HopNumNeighborAhead;  // store num of neighbors ahead for each neighborIP
        std::vector<uint16_t> m_1HopNumNeighborBehind; // store num of neighbors behind for each neighborIP
        std::vector<float> m_1HopPositionX;            // store X of all neighbors
        std::vector<float> m_1HopPositionY;            // store Y of all neighbors
        std::vector<float> m_1HopNeighborSpeedX;       // store speed in X direction of all neighbors
        std::vector<float> m_1HopNeighborSpeedY;       // store speed in Y direction of all neighbors
        std::vector<float> m_neighborFurthestAheadX;   // store X of furthest ahead neighbor for each IP
        std::vector<float> m_neighborFurthestAheadY;   // store Y of furthest ahead neighbor for each IP
        std::vector<float> m_neighborFurthestBehindX;  // store X of furthest behind neighbor for each IP
        std::vector<float> m_neighborFurthestBehindY;  // store Y of furthest behind neighbor for each IP
        std::vector<float> m_neighborAvgSpeedX;        // store average speed in X direction for each IP
        std::vector<float> m_neighborAvgSpeedY;        // store average speed in Y direction for each IP
        std::vector<Time> m_1HopNeighborLastTime;      // store last time of communication with all neighbors
        std::vector<float> m_mostRecentIndividualNSpeedX = std::vector<float>(m_NSamples,0); // initializes all values to zero
	    std::vector<float> m_mostRecentIndividualNSpeedY = std::vector<float>(m_NSamples,0);
	    std::vector<float> m_mostRecentNeighborHoodNSpeedX = std::vector<float>(m_NSamples,0); // initializes all values to zero
	    std::vector<float> m_mostRecentNeighborHoodNSpeedY = std::vector<float>(m_NSamples,0);
        std::vector<Ptr<Packet>> m_packetQ; // to hold queue of packet  
        void ScheduleSpeedLogUpdate ();  
        void GetSpeedValue ();
        void AddSpeedSample(float speedX, float speedY, float neighborhoodSpeedX, float neighborhoodSpeedY);
        void ScheduleNeighborRemoval ();  // creates call to CheckForNeighborRemoval periodically, specified by NEIGHBOR_REMOVAL_PERIOD in simulationConfiguration.h
        void CheckForNeighborRemoval ();
        void AddDirection (uint16_t direction, Ipv4Address address);       // will add entry to m_neighborDirection
        void AddNumNeighborsAhead (uint16_t numAhead);   // will add entry to m_1HopNumNeighborAhead
        void AddNumNeighborsBehind (uint16_t numBehind); // will add entry to m_1HopNumNeighborBehind
        void AddLocation (float posX, float posY);    // will add entry to m_positionX and m_positionY
        void AddSpeed (float speedX, float speedY);   // will add entry to m_neighborSpeedX and m_neighborSpeedY
        void AddNeighborFurthestAhead (float neighborFurthestAheadX, float neighborFurthestAheadY);   // will add entry to m_neighborFurthestAheadX
        void AddNeighborFurthestBehind (float neighborFurthestBehindX, float neighborFurthestBehindY);    // will add entry to m_neighborFurthestBehindX
        void AddNeighborAvgSpeed (float neighborAvgSpeedX, float neighborAvgSpeedY);  // adds entry to m_neighborFurthestBehindX
        void UpdateNeighborIPAheadBehind (Ipv4Address address, uint16_t direction); // given ID and direction, check entry is correct
        void EraseNeighborIPAheadBehind (Ipv4Address address, uint16_t direction);  // erase ID and direction from stored data
};

}
}
#endif