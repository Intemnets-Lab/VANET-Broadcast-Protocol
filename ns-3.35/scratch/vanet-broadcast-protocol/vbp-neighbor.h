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
        /**
         * Adds node as new neighbor or update a nodes information
         * 
         * \param address IP address of node
         * \param direction direction node travels [+1 ahead] [-1 behind]
         * \param neighborsAhead number of neighbors ahead
         * \param neighborsBehind number of neighbors behind
         * \param posX node's position in the x-plane
         * \param posY node's position in the y-plane
         * \param speedX node's speed in the x-plane
         * \param speedY node's speed in the y-plane
         * \param neighborFurthestAheadX x-position of neighbor furthest ahead of node 
         * \param neighborFurthestAheadY y-position of neighbor furthest ahead of node 
         * \param neighborFurthestBehindX x-position of neighbor furthest behind of node 
         * \param neighborFurthestBehindY y-position of neighbor furthest behind of node
         * \param avgSpeedX node's average speed in the x-plane
         * \param avgSpeedY node's average speed in the y-plane
        */
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
        /**
         * Gets the number of one-hop neighbors
         * 
         * \returns number of one-hop neighbors
        */
        uint16_t Get1HopNumNeighbors ();
        /**
         * Gets the number of neighbors ahead of one-hop neighbor
         * 
         * \returns number of neighbors ahead of one-hop neighbor
        */
        uint16_t Get1HopNumNeighborsAhead ();  
        /**
         * Gets the number of neighbors behind of one-hop neighbor
         * 
         * \returns number of neighbors behind of one-hop neighbor
        */
        uint16_t Get1HopNumNeighborsBehind ();
        /**
         * Gets the IP of the one-hop neighbor ahead. Grabbed by index.
         * 
         * \returns IP of the one-hop neighbor ahead
        */
        Ipv4Address Get1HopNeighborIPAhead (uint16_t index);
        /**
         * Gets the IP of the one-hop neighbor behind. Grabbed by index.
         * 
         * \returns IP of the one-hop neighbor behind
        */
        Ipv4Address Get1HopNeighborIPBehind (uint16_t index);
        /**
         * Gets the IP of a neighbor one-hop away
         * 
         * \returns IP of a neighbor one-hop away
        */
        Ipv4Address Get1HopNeighborIP (uint16_t index); 
        /**
         * Gets the direction of a one-hop neighbor
         * 
         * \returns direction of a one-hop neighbor
        */
        uint16_t Get1HopDirectionByIP (Ipv4Address address);
        /**
         * Gets the direction of one-hop neighbor ahead or behind
         * 
         * \returns direction of one-hop neighbor ahead or behind
        */
        uint16_t Get1HopDirection (uint16_t index);        // if 1 hop neighbor ahead or behind, grabbed by index
        /**
         * Gets the one-hop number of nodes ahead of neighbor
         * 
         * \returns one-hop number of nodes ahead of neighbor
        */
        uint16_t Get1HopNumberOfNodesAheadOfNeighbor (uint16_t index);  // if 1 hop neighbor has neighbor's ahead
        /**
         * Gets the one-hop number of neighbors behind the neighbor
         * 
         * \returns one-hop number of neighbors behind the neighbor
        */
        uint16_t Get1HopNumberOfNodesBehindOfNeighbor (uint16_t index); // if 1 hop neighbor has neighbor's behind
        /**
         * Gets the neighbor position in the x-plane
         * 
         * \returns neighbor position in the x-plane
        */
        float GetNeighborPositionX (uint16_t index);
        /**
         * Gets the neighbor position in the y-plane
         * 
         * \returns neighbor position in the y-plane
        */
        float GetNeighborPositionY (uint16_t index);
        /**
         * Gets the neighbor velocity in the x-plane
         * 
         * \returns neighbor velocity in the x-plane
        */
        float GetNeighborSpeedX (uint16_t index);
        /**
         * Gets the neighbor velocity in the y-plane
         * 
         * \returns neighbor velocity in the y-plane
        */
        float GetNeighborSpeedY (uint16_t index);
        /**
         * Gets the one-hop neighbor location in the x-plane
         * 
         * \returns one-hop neighbor location in the x-plane
        */
        std::vector<float> Get1HopNeighborLocationsX ();
        /**
         * Gets the one-hop neighbor location in the y-plane
         * 
         * \returns one-hop neighbor location in the y-plane
        */
        std::vector<float> Get1HopNeighborLocationsY ();
        /**
         * Gets all one-hop neighbors velocity in the x-plane
         * 
         * \returns all one-hop neighbors velocity in the x-plane
        */
        std::vector<float> Get1HopNeighborSpeedX ();
        /**
         * Gets all one-hop neighbors velocity in the y-plane
         * 
         * \returns all one-hop neighbors velocity in the y-plane
        */
        std::vector<float> Get1HopNeighborSpeedY ();
        /**
         * Gets the x-coordinate of the furthest ahead neighbor
         * 
         * \returns x-coordinate of the furthest ahead neighbor
        */
        float GetNeighborFurthestAheadX (uint16_t index);  // location for furthest vehicle ahead for a specific node, grabbed by index
        /**
         * Gets the y-coordinate of the furthest ahead neighbor
         * 
         * \returns y-coordinate of the furthest ahead neighbor
        */
        float GetNeighborFurthestAheadY (uint16_t index); 
        /**
         * Gets the x-coordinate of the furthest behind neighbor
         * 
         * \returns x-coordinate of the furthest behind neighbor
        */
        float GetNeighborFurthestBehindX (uint16_t index); // location for furthest vehicle behind for a specific node, grabbed by index
        /**
         * Gets the y-coordinate of the furthest behind neighbor
         * 
         * \returns y-coordinate of the furthest behind neighbor
        */
        float GetNeighborFurthestBehindY (uint16_t index);
        /**
         * Gets the average x-plane speed of all neighbors
         * 
         * \returns average x-plane speed of all neighbors
        */
        float GetAvgSpeedNeighborX (float speedReferenceX);// from neighbors, find average speed
        /**
         * Gets the average y-plane speed of all neighbors
         * 
         * \returns average y-plane speed of all neighbors
        */
        float GetAvgSpeedNeighborY (float speedReferenceY);
        /**
         * Gets the node that is furthest ahead within one-hop
         * 
         * \returns node that is furthest ahead within one-hop
        */
        int GetNeighborFurthestAheadByIndex(Vector reference); 
        /**
         * Gets the node that is furthest behind within one-hop
         * 
         * \returns node that is furthest behind within one-hop
        */
        int GetNeighborFurthestBehindByIndex(Vector reference);
        /**
         * Gets the average x-plane speed of one-hop neighbor 
         * 
         * \returns average x-plane speed of one-hop neighbor 
        */
        float GetNeighborAvgSpeedX(uint16_t index);        // get average speed of 1 hop neighbor grabbed by index
        /**
         * Gets the average y-plane speed of one-hop neighbor 
         * 
         * \returns average y-plane speed of one-hop neighbor 
        */
        float GetNeighborAvgSpeedY(uint16_t index);
        /**
         * Gets the furthest ahead node within a two-hop distance
         * 
         * \returns furthest ahead node within a two-hop distance
        */
        int Get2HopDistFurthestAheadByIndex(Vector reference);  // discuss working cases, highlight that it handles other cases
        /**
         * Gets the furthest behind node within a two-hop distance
         * 
         * \returns furthest behind node within a two-hop distance
        */
        int Get2HopDistFurthestBehindByIndex(Vector reference);
        /**
         * Gets the number of nodes within two-hops
         * 
         * \returns number of nodes within two-hops
        */
        uint16_t Get2HopCarCount(int twoHopFurthestAheadIndex, int twoHopFurthestBehindIndex, Vector reference);
        /**
         * Gets the number of ahead nodes within two-hops
         * 
         * \returns number of ahead nodes within two-hops
        */
        uint16_t Get2HopCarCountSelfAhead(int twoHopFurthestAheadIndex, Vector reference);
        /**
         * Gets the number of behind nodes within two-hops
         * 
         * \returns number of behind nodes within two-hops
        */
        uint16_t Get2HopCarCountSelfBehind(int twoHopFurthestBehindIndex, Vector reference);
        /**
         * Gets the average x-plane speed of vehicles within the neighborhood
         * 
         * \returns average x-plane speed of vehicles within the neighborhood
        */
        float GetNeighborHoodSpeedMeanX(); // get average from sampled values in m_mostRecentNeighborHoodNSpeedX
        /**
         * Gets the average y-plane speed of vehicles within the neighborhood
         * 
         * \returns average y-plane speed of vehicles within the neighborhood
        */
	    float GetNeighborHoodSpeedMeanY(); // get average from sampled values in m_mostRecentNeighborHoodNSpeedY
        /**
         * Gets the LOS value
         * 
         * \returns LOS value
        */
        float GetLosCalculation(Vector referencePos, Vector referenceVel);
        void SetThisNode(Ptr<Node> n);
        void PrintNeighborState ();               // will display different info
        void PrintNeighbors();
        void PrintNeighbors2();
        /**
         * Print neighbor 
         * 
         * \returns integer number of neighbors behind
        */
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
        void AppendNeighbor(Ipv4Address neighborAddress);
        int FindNeighbor (Ipv4Address address); // returns index for specified nodeIP, returns -1 if new nodeIP
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