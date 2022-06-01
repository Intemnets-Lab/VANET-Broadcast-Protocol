#include "vbp-neighbor.h"
#include <fstream>

namespace ns3 {
    NS_LOG_COMPONENT_DEFINE("VbpNeighbor");
namespace vbp {

 VbpNeighbors::VbpNeighbors ()
    : 
      m_filledFirstTime(false),
      m_currentIdx(0),
      m_NSamples(100),
      m_thisNode(NULL), 
      m_speedLogPeriod(1),
      m_neighborRemovalPeriod (0.5),
      m_neighborTimeout(2000000000),
      m_capacityPerLane(0.388888),
      m_numLanes(2),
      m_1HopNumNeighbors (0) ,
      m_1HopNumNeighborsBehind (0),
      m_1HopNumNeighborsAhead (0)


  {
    ScheduleNeighborRemoval ();
    ScheduleSpeedLogUpdate();
    
  }

 VbpNeighbors::~VbpNeighbors () {
 }
 
 TypeId VbpNeighbors::GetTypeId (void) {
   static TypeId tid = TypeId ("ns3::vbp::VbpNeighbors")
     .SetParent<Object> ()
     .AddConstructor<VbpNeighbors> ();
   return tid;
 }

 TypeId VbpNeighbors::GetInstanceTypeId (void) const {
   return GetTypeId ();
 }
 
 void VbpNeighbors::Print (std::ostream &os) const {
 }

void
VbpNeighbors::SetThisNode(Ptr<Node> n)
{
    m_thisNode = n;
}

int 
VbpNeighbors::FindNeighbor (Ipv4Address address) {
    for(uint16_t idx = 0; idx < m_1HopNumNeighbors; ++idx) {
        if (m_1HopNeighborIPs[idx] == address) {
            // if find value to add is already here, then stop
            return idx; // what index needs to be used for this ip address
        }
    }
    return -1; // if not found
}

void 
VbpNeighbors::PrintNeighbors()
{
  for (std::vector<Ipv4Address>::iterator i = m_1HopNeighborIPs.begin (); i != m_1HopNeighborIPs.end (); ++i)
  { 
    Ipv4Address addr;
    addr = *i;
  }
}

void 
VbpNeighbors::AppendNeighbor (Ipv4Address neighborAddress)
  {
     m_1HopNeighborIPs.push_back(neighborAddress); // neighbors identified by address
  }

void
VbpNeighbors::AppendQueue(const Ptr<Packet> p)
{
    std::cout << "Append Queue" << std::endl;
    m_packetQ.push_back(p);
}

Ptr<Packet>
VbpNeighbors::GetPacketQueue()
{
            std::cout << "Get Packet Queue" << std::endl;
    Ptr<Packet> p = m_packetQ.front();

    m_packetQ.erase(m_packetQ.begin());

    return p;
}

bool
VbpNeighbors::QueueEmpty()
{
    std::cout << "QueueEmpty" << std::endl;
     if (m_packetQ.size() == 0)
    {
        std::cout << "No packets in queue" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Packets in queue" << std::endl;
        return false;
    }   
}

uint16_t
VbpNeighbors::GetQueueSize()
{
    return m_packetQ.size();
}

void 
VbpNeighbors::AddDirection (uint16_t direction, Ipv4Address address) {
    m_1HopNeighborDirection.push_back(direction);
    if (direction) { // true if 1, which is ahead
        m_1HopNumNeighborsAhead++;
        m_1HopNeighborIPAhead.push_back(address);
    } else {
        m_1HopNumNeighborsBehind++;
        m_1HopNeighborIPBehind.push_back(address);
    }
}

void 
VbpNeighbors::AddNumNeighborsAhead (uint16_t numAhead) {
    m_1HopNumNeighborAhead.push_back(numAhead); 
}

void 
VbpNeighbors::AddNumNeighborsBehind (uint16_t numBehind) {
    m_1HopNumNeighborBehind.push_back(numBehind); 
}

void 
VbpNeighbors::AddLocation (float posX, float posY) {
    m_1HopPositionX.push_back(posX);
    m_1HopPositionY.push_back(posY);
}

void 
VbpNeighbors::AddSpeed(float speedX, float speedY) {
    m_1HopNeighborSpeedX.push_back(speedX); 
    m_1HopNeighborSpeedY.push_back(speedY); 
}

void 
VbpNeighbors::AddNeighborFurthestAhead (float neighborFurthestAheadX, float neighborFurthestAheadY) {
    m_neighborFurthestAheadX.push_back(neighborFurthestAheadX);
    m_neighborFurthestAheadY.push_back(neighborFurthestAheadY);
}

void 
VbpNeighbors::AddNeighborFurthestBehind (float neighborFurthestBehindX, float neighborFurthestBehindY) {
    m_neighborFurthestBehindX.push_back(neighborFurthestBehindX);
    m_neighborFurthestBehindY.push_back(neighborFurthestBehindY);
}

void 
VbpNeighbors::AddNeighborAvgSpeed (float neighborAvgSpeedX, float neighborAvgSpeedY) {
    m_neighborAvgSpeedX.push_back(neighborAvgSpeedX);
    m_neighborAvgSpeedY.push_back(neighborAvgSpeedY);
}

void 
VbpNeighbors::AddNode (Ipv4Address address, uint16_t direction, uint16_t neighborsAhead, uint16_t neighborsBehind
                            , float posX, float posY, float speedX, float speedY, float neighborFurthestAheadX
                            , float neighborFurthestAheadY, float neighborFurthestBehindX, float neighborFurthestBehindY
                            , float avgSpeedX, float avgSpeedY) {
   int idx = FindNeighbor(address);
   if (idx < 0) { // if added the neighbor, add to list
        AppendNeighbor(address);
        AddDirection(direction, address);
        AddNumNeighborsAhead(neighborsAhead);
        AddNumNeighborsBehind(neighborsBehind);
        AddLocation(posX, posY);
        AddSpeed(speedX, speedY);
        m_1HopNeighborLastTime.push_back(Simulator::Now());
        m_1HopNumNeighbors++; // update number of neighbors by one
        AddNeighborFurthestAhead (neighborFurthestAheadX, neighborFurthestAheadY);  // will add entry to m_neighborFurthestAheadX
        AddNeighborFurthestBehind (neighborFurthestBehindX, neighborFurthestBehindY);  // will add entry to m_neighborFurthestBehindX
        AddNeighborAvgSpeed (avgSpeedX, avgSpeedY);  // will add entry to m_neighborAvgSpeedX
        // start with header first then this part!!!
        //need to add x,y furthest and FurthestAhead (4) and average speed
        // when this is complete, in routing compute LOS, the value used in LOS is not accurate since average of averages
        // average will be skewed to vehicles in the middle
   }
   else {
        // update vectors if not adding a new neighbor
        m_1HopNeighborDirection[idx] = direction;
        UpdateNeighborIPAheadBehind(address, direction);
        m_1HopNumNeighborAhead[idx] = neighborsAhead;
        m_1HopNumNeighborBehind[idx] = neighborsBehind;
        m_1HopPositionX[idx] = posX; 
        m_1HopPositionY[idx] = posY; 
        m_1HopNeighborSpeedX[idx] = speedX;
        m_1HopNeighborSpeedY[idx] = speedY;
        m_1HopNeighborLastTime[idx] = Simulator::Now(); 
        m_neighborFurthestAheadX[idx] = neighborFurthestAheadX;
        m_neighborFurthestAheadY[idx] = neighborFurthestAheadY;
        m_neighborFurthestBehindX[idx] = neighborFurthestBehindX;
        m_neighborFurthestBehindY[idx] = neighborFurthestBehindY;
        m_neighborAvgSpeedX[idx] = avgSpeedX;
        m_neighborAvgSpeedY[idx] = avgSpeedY;
   }
}


void 
VbpNeighbors::UpdateNeighborIPAheadBehind (Ipv4Address address, uint16_t direction) {
    for(uint16_t idx = 0; idx < m_1HopNumNeighborsAhead; ++idx) {
        if (m_1HopNeighborIPAhead[idx] == address) {
            // if find address check if direction changed
            if (direction) {
                return; // direction stayed the same so no changes
            }
            else { //means direction changed
                m_1HopNumNeighborsAhead--;
                m_1HopNumNeighborsBehind++;
                //std::cout << "M 1 Hop Neighbor Ahead" << m_1HopNeighborIPAhead.begin() << std::endl;
                m_1HopNeighborIPAhead.erase(m_1HopNeighborIPAhead.begin()+idx);
                m_1HopNeighborIPBehind.push_back(address);
                return;
            }
        }
    }
    for(uint16_t idx = 0; idx < m_1HopNumNeighborsBehind; ++idx) {

        if (m_1HopNeighborIPBehind[idx] == address) {
            // if find address check if direction changed
            if (!direction) {
                return; // direction stayed the same so no changes
            }
            else { //means direction changed
                m_1HopNumNeighborsAhead++;
                m_1HopNumNeighborsBehind--;
                m_1HopNeighborIPAhead.push_back(address);
                m_1HopNeighborIPBehind.erase(m_1HopNeighborIPBehind.begin()+idx);
                return;
            }
        }
    } 
}

void 
VbpNeighbors::CheckForNeighborRemoval () {
    // delete neighbor if no response in TIMEOUT seconds
    if (m_1HopNumNeighbors==0) {
        return; // nothing to remove
    }
    for(uint16_t idx = 0; idx < m_1HopNumNeighbors; ++idx) {    
         if ((Simulator::Now()-m_1HopNeighborLastTime[idx]) >= Time(m_neighborTimeout)) {  //check seconds since last response
            // if find value to delete
            EraseNeighborIPAheadBehind (m_1HopNeighborIPs[idx], m_1HopNeighborDirection[idx]);
            m_1HopNeighborIPs.erase(m_1HopNeighborIPs.begin()+idx);  
            m_1HopNeighborDirection.erase(m_1HopNeighborDirection.begin()+idx);   
            m_1HopNumNeighborAhead.erase(m_1HopNumNeighborAhead.begin()+idx);
            m_1HopNumNeighborBehind.erase(m_1HopNumNeighborBehind.begin()+idx);
            m_1HopPositionX.erase(m_1HopPositionX.begin()+idx);  
            m_1HopPositionY.erase(m_1HopPositionY.begin()+idx);  
            m_1HopNeighborSpeedX.erase(m_1HopNeighborSpeedX.begin()+idx); // can't use same iterator for all since different types
            m_1HopNeighborSpeedY.erase(m_1HopNeighborSpeedY.begin()+idx); 
            m_1HopNeighborLastTime.erase(m_1HopNeighborLastTime.begin()+idx); 
            m_neighborFurthestAheadX.erase(m_neighborFurthestAheadX.begin()+idx);
            m_neighborFurthestAheadY.erase(m_neighborFurthestAheadY.begin()+idx);
            m_neighborFurthestBehindX.erase(m_neighborFurthestBehindX.begin()+idx);
            m_neighborFurthestBehindY.erase(m_neighborFurthestBehindY.begin()+idx);
            m_neighborAvgSpeedX.erase(m_neighborAvgSpeedX.begin()+idx);
            m_neighborAvgSpeedY.erase(m_neighborAvgSpeedY.begin()+idx);
            m_1HopNumNeighbors--; // decrease neighborSize count
        }
    }
}

void 
VbpNeighbors::ScheduleSpeedLogUpdate () { 
    // to schedule getSpeedValue every SPEED_LOG_PERIOD seconds
    Simulator::Schedule(Seconds(m_speedLogPeriod), &VbpNeighbors::GetSpeedValue, this);
}

void 
VbpNeighbors::GetSpeedValue () {
    // sample for individual speed to store in m_mostRecentIndividualNSpeedX 
        // and sample of current average for speed of 1 hop neighbors to store in m_mostRecentNeighborHoodNSpeedX
        // done for x and y direction
    Vector speed = Vector3D(m_thisNode->GetObject<MobilityModel>()->GetVelocity()); 
    AddSpeedSample(speed.x, speed.y, GetAvgSpeedNeighborX(speed.x), GetAvgSpeedNeighborY(speed.y));
    ScheduleSpeedLogUpdate (); // to schedule this method again in SPEED_LOG_PERIOD seconds
}

void 
VbpNeighbors::AddSpeedSample(float speedX, float speedY, float neighborhoodSpeedX, float neighborhoodSpeedY) {
    // store new speed sample for individual speed to store in m_mostRecentIndividualNSpeedX 
        // and store sample of current average for speed of 1 hop neighbors to store in m_mostRecentNeighborHoodNSpeedX
        // done for x and y direction
    m_mostRecentIndividualNSpeedX[m_currentIdx] = speedX;
    m_mostRecentIndividualNSpeedY[m_currentIdx] = speedY;
    m_mostRecentNeighborHoodNSpeedX[m_currentIdx] = neighborhoodSpeedX;
    m_mostRecentNeighborHoodNSpeedY[m_currentIdx] = neighborhoodSpeedY;
    m_currentIdx++; // increase by one for next time it is used
    if (m_currentIdx == m_NSamples) {
	    m_currentIdx = 0; // rewrite first index since it is now the oldest
	    m_filledFirstTime = true;
    }
}

void 
VbpNeighbors::ScheduleNeighborRemoval () {
    CheckForNeighborRemoval ();
    Simulator::Schedule(Seconds(m_neighborRemovalPeriod), &VbpNeighbors::ScheduleNeighborRemoval, this);
}

void 
VbpNeighbors::EraseNeighborIPAheadBehind (Ipv4Address address, uint16_t direction) {
    if (direction) { // means remove from neighbor ahead
        for(uint16_t idx = 0; idx < m_1HopNumNeighborsAhead; ++idx) {
            if (m_1HopNeighborIPAhead[idx] == address) {
                m_1HopNeighborIPAhead.erase(m_1HopNeighborIPAhead.begin()+idx);
            }
        }
        m_1HopNumNeighborsAhead--;
    }
    else { //means remove from neighbor behind
        for(uint16_t idx = 0; idx < m_1HopNumNeighborsBehind; ++idx) {
            if (m_1HopNeighborIPBehind[idx] == address) {
                m_1HopNeighborIPBehind.erase(m_1HopNeighborIPBehind.begin()+idx);
            }
        }
        m_1HopNumNeighborsBehind--;
    }   
}

uint16_t 
VbpNeighbors::Get1HopNumNeighbors () {
    return m_1HopNumNeighbors;
}

uint16_t 
VbpNeighbors::Get1HopNumNeighborsAhead () {
    return m_1HopNumNeighborsAhead;
}

uint16_t 
VbpNeighbors::Get1HopNumNeighborsBehind () {
    return m_1HopNumNeighborsBehind;
}

Ipv4Address 
VbpNeighbors::Get1HopNeighborIPAhead (uint16_t index) {
    return m_1HopNeighborIPAhead[index];
}

Ipv4Address 
VbpNeighbors::Get1HopNeighborIPBehind (uint16_t index) {
    return m_1HopNeighborIPBehind[index];
}

Ipv4Address 
VbpNeighbors::Get1HopNeighborIP(uint16_t index) {
    return m_1HopNeighborIPs[index];
}

uint16_t 
VbpNeighbors::Get1HopDirectionByIP (Ipv4Address address) {
    int idx = FindNeighbor (address); 
    if (idx >= 0) {
 	return m_1HopNeighborDirection[idx];
    }
    else {
        return 0;
    }
}

uint16_t 
VbpNeighbors::Get1HopDirection (uint16_t index) {
 	return m_1HopNeighborDirection[index];
}

uint16_t 
VbpNeighbors::Get1HopNumberOfNodesAheadOfNeighbor (uint16_t index) {
 	return m_1HopNumNeighborAhead[index];
}

uint16_t 
VbpNeighbors::Get1HopNumberOfNodesBehindOfNeighbor (uint16_t index) {
 	return m_1HopNumNeighborBehind[index];
}

float 
VbpNeighbors::GetNeighborPositionX (uint16_t index) {
 	return m_1HopPositionX[index];
}

float 
VbpNeighbors::GetNeighborPositionY (uint16_t index) {
 	return m_1HopPositionY[index];
}

float 
VbpNeighbors::GetNeighborSpeedX (uint16_t index) {
 	return m_1HopNeighborSpeedX[index];
}

float 
VbpNeighbors::GetNeighborSpeedY (uint16_t index) {
 	return m_1HopNeighborSpeedY[index];
}

std::vector<float> 
VbpNeighbors::Get1HopNeighborLocationsX () {
    return m_1HopPositionX;
}

std::vector<float> 
VbpNeighbors::Get1HopNeighborLocationsY () {
    return m_1HopPositionY;
}

std::vector<float> 
VbpNeighbors::Get1HopNeighborSpeedX () {
    return m_1HopNeighborSpeedX;
}

std::vector<float> 
VbpNeighbors::Get1HopNeighborSpeedY () {
    return m_1HopNeighborSpeedY;
}

float 
VbpNeighbors::GetNeighborFurthestAheadX(uint16_t index) {
   return m_neighborFurthestAheadX[index];
}

float 
VbpNeighbors::GetNeighborFurthestAheadY(uint16_t index) {
   return m_neighborFurthestAheadY[index];
}

float 
VbpNeighbors::GetNeighborFurthestBehindX(uint16_t index) {
   return m_neighborFurthestBehindX[index];
}

float 
VbpNeighbors::GetNeighborFurthestBehindY(uint16_t index) {
   return m_neighborFurthestBehindY[index];
}

float 
VbpNeighbors::GetNeighborAvgSpeedX(uint16_t index) {
    return m_neighborAvgSpeedX[index];
}

float 
VbpNeighbors::GetNeighborAvgSpeedY(uint16_t index) {
    return m_neighborAvgSpeedY[index];
}

float 
VbpNeighbors::GetAvgSpeedNeighborX(float speedReferenceX) {
    float avgSpeedX = speedReferenceX;
    if (m_1HopNumNeighbors > 0) {
        for(uint16_t idx = 0; idx < m_1HopNumNeighbors; idx++) { 
            avgSpeedX += m_1HopNeighborSpeedX[idx];
        }
        avgSpeedX = avgSpeedX/(m_1HopNumNeighbors+1);
    }
    return avgSpeedX;
}

float 
VbpNeighbors::GetAvgSpeedNeighborY(float speedReferenceY) {
    float avgSpeedY = speedReferenceY;
    if (m_1HopNumNeighbors > 0) {
        for(uint16_t idx = 0; idx < m_1HopNumNeighbors; idx++) { 
            avgSpeedY += m_1HopNeighborSpeedY[idx];
        }
        avgSpeedY = avgSpeedY/(m_1HopNumNeighbors+1);
    }
    return avgSpeedY;
}

int 
VbpNeighbors::GetNeighborFurthestAheadByIndex(Vector reference) {
    int furthestIdx = -1;
    if (m_1HopNumNeighborsAhead > 0) {
        furthestIdx = FindNeighbor(m_1HopNeighborIPAhead[0]); // in case only has one neighbor ahead
        Vector neighborPos = Vector3D(m_1HopPositionX[furthestIdx], m_1HopPositionY[furthestIdx],0);
        float distFurthestAhead = CalculateDistance(neighborPos, reference); // car more ahead  
        float neighborDist;
        for(uint16_t idx = 0; idx < m_1HopNumNeighbors; idx++) { 
            if (!(m_1HopNeighborDirection[idx])) { // car is behind, then skip
                continue;
            }
            neighborPos = Vector3D(m_1HopPositionX[idx], m_1HopPositionY[idx],0);
            neighborDist = CalculateDistance(neighborPos, reference);
            if (neighborDist > distFurthestAhead) { // car most head 
                distFurthestAhead = neighborDist;
                furthestIdx = idx;
            }                                           
        }                 
    }
    return furthestIdx;
}

int 
VbpNeighbors::GetNeighborFurthestBehindByIndex(Vector reference) {
    int furthestIdx = -1;
    if (m_1HopNumNeighborsBehind > 0) {
        furthestIdx = FindNeighbor(m_1HopNeighborIPBehind[0]); // in case only has one neighbor behind
        Vector neighborPos = Vector3D(m_1HopPositionX[furthestIdx], m_1HopPositionY[furthestIdx],0);
        float distFurthestBehind = CalculateDistance(neighborPos, reference); // car more ahead  
        float neighborDist;
        for(uint16_t idx = 0; idx < m_1HopNumNeighbors; idx++) { 
            if (m_1HopNeighborDirection[idx]) { // car is ahead, then skip
                continue;
            }
            neighborPos = Vector3D(m_1HopPositionX[idx], m_1HopPositionY[idx],0);
            neighborDist = CalculateDistance(neighborPos, reference);
            if (neighborDist > distFurthestBehind) {  //car most behind
                distFurthestBehind = neighborDist;
                furthestIdx = idx;
            }                                           
        }                 
    }
    return furthestIdx;
}

int 
VbpNeighbors::Get2HopDistFurthestAheadByIndex(Vector reference) {
    int furthestIndex = -1;
    float furthestDist = 0;
    if (m_1HopNumNeighborsAhead > 0) {
        uint16_t index = FindNeighbor(m_1HopNeighborIPAhead[0]);
        Vector neighborFurthestAhead = Vector3D(m_neighborFurthestAheadX[index], m_neighborFurthestAheadY[index],0);        
        if (!isnan(neighborFurthestAhead.x)) { //only need to check x because x and y will be nan
            furthestDist = CalculateDistance(reference, neighborFurthestAhead); // Dist ahead
            furthestIndex = index; // in case only has one neighbor
        }
        float neighborDist; 
        for(uint16_t idx = 0; idx < m_1HopNumNeighbors; idx++) { 
            if (!(m_1HopNeighborDirection[idx])) { // car is behind, then skip
                continue;
            }
            neighborFurthestAhead = Vector3D(m_neighborFurthestAheadX[idx], m_neighborFurthestAheadY[idx],0);
            if (isnan(neighborFurthestAhead.x)) { //only need to check x because x and y will be nan
                continue; // go to next entry
            }
            neighborDist = CalculateDistance(reference, neighborFurthestAhead); // Dist ahead
            if (neighborDist > furthestDist) { // car and car furthest head total distance
                furthestDist = neighborDist;
                furthestIndex = idx;
            }   //consider when the vector is not a number, check if nan                                  
        }                 
    } // index of 1 hop neighbor that reaches furthest ahead
    return furthestIndex; // -1 when no 2 hop neighbor ahead or no 1 hop neighbors ahead
}

int 
VbpNeighbors::Get2HopDistFurthestBehindByIndex(Vector reference) {
    int furthestIndex = -1;
    float furthestDist = 0;
    if (m_1HopNumNeighborsBehind > 0) {
        uint16_t index = FindNeighbor(m_1HopNeighborIPBehind[0]);
        Vector neighborFurthestBehind = Vector3D(m_neighborFurthestBehindX[index], m_neighborFurthestBehindY[index],0);
        if (!isnan(neighborFurthestBehind.x)) { //only need to check x because x and y will be nan
            furthestDist = CalculateDistance(reference, neighborFurthestBehind);  // dist behind
            furthestIndex = index; // in case only has one neighbor
        }
        float neighborDist; 
        for(uint16_t idx = 0; idx < m_1HopNumNeighbors; idx++) { 
            if (m_1HopNeighborDirection[idx]) { // car is ahead, then skip
                continue;
            }
            neighborFurthestBehind = Vector3D(m_neighborFurthestBehindX[idx], m_neighborFurthestBehindY[idx],0);
            if (isnan(neighborFurthestBehind.x)) { //only need to check x because x and y will be nan
                continue; // go to next entry
            }
            neighborDist = CalculateDistance(reference, neighborFurthestBehind);  // dist behind
            if (neighborDist > furthestDist) {  // car and car furthest behind total distance
                furthestDist = neighborDist;
                furthestIndex = idx;
            }                                           
        }                 
    } // index of 1 hop neighbor that reaches furthest behind
    return furthestIndex; // -1 when no 2 hop neighbor behind or no 1 hop neighbors behind
}

uint16_t 
VbpNeighbors::Get2HopCarCount(int twoHopFurthestAheadIndex, int twoHopFurthestBehindIndex, Vector reference) {
        // 3 regions, cars ahead of 2hopFurthestAheadIdx, cars behind 2hopFurthestBehindIdx, and cars between those 2 Ids
    uint16_t totalCount = m_1HopNumNeighborAhead[twoHopFurthestAheadIndex];
    totalCount += m_1HopNumNeighborBehind[twoHopFurthestBehindIndex]; // first two regions
    Vector furthestAhead = Vector3D(m_1HopPositionX[twoHopFurthestAheadIndex], m_1HopPositionY[twoHopFurthestAheadIndex],0);
    float furthestAheadDist = CalculateDistance(furthestAhead, reference);
    Vector furthestBehind = Vector3D(m_1HopPositionX[twoHopFurthestBehindIndex], m_1HopPositionY[twoHopFurthestBehindIndex],0);
    float furthestBehindDist = CalculateDistance(furthestBehind, reference);
    Vector neighborPos; 
    float neighborDist;
    for(uint16_t idx = 0; idx < m_1HopNumNeighbors; idx++) { 
        neighborPos = Vector3D(m_1HopPositionX[idx], m_1HopPositionY[idx],0);
        neighborDist = CalculateDistance(neighborPos, reference);
        if (m_1HopNeighborDirection[idx]) { // car is ahead
            if (neighborDist <= furthestAheadDist) {
                totalCount++;
            }
        }
        else { // then behind
            if (neighborDist <= furthestBehindDist) {
                totalCount++;
            }
        }                
    }
    totalCount += 1; //need to add one to account current node
    return totalCount; 
}

uint16_t 
VbpNeighbors::Get2HopCarCountSelfAhead(int twoHopFurthestAheadIndex, Vector reference) {
        // 3 regions, vehicles behind reference, vehicles in front furthest ahead, vehicles between reference and furthest ahead
    uint16_t totalCount = m_1HopNumNeighborsBehind; 
    totalCount += m_1HopNumNeighborAhead[twoHopFurthestAheadIndex];
    Vector furthestAhead = Vector3D(m_1HopPositionX[twoHopFurthestAheadIndex], m_1HopPositionY[twoHopFurthestAheadIndex],0);
    float furthestAheadDist = CalculateDistance(furthestAhead, reference);
    Vector neighborPos; 
    float neighborDist;
    for(uint16_t idx = 0; idx < m_1HopNumNeighbors; idx++) {  // also check if 1hopneighbors>0 first
        if (!(m_1HopNeighborDirection[idx])) { // car is behind, then skip
            continue;
        }
        neighborPos = Vector3D(m_1HopPositionX[idx], m_1HopPositionY[idx],0);
        neighborDist  = CalculateDistance(neighborPos, reference);
        if (neighborDist <= furthestAheadDist) {
            totalCount++;
        }               
    }
    totalCount += 1; //need to add one to account current node
    return totalCount; 
}

uint16_t 
VbpNeighbors::Get2HopCarCountSelfBehind(int twoHopFurthestBehindIndex, Vector reference) {
        // 3 regions, vehicles ahead reference, vehicles behind furthest behind, vehicles between reference and furthest behind
    uint16_t totalCount = m_1HopNumNeighborsAhead;
    totalCount += m_1HopNumNeighborBehind[twoHopFurthestBehindIndex]; // first two regions
    Vector furthestBehind = Vector3D(m_1HopPositionX[twoHopFurthestBehindIndex], m_1HopPositionY[twoHopFurthestBehindIndex],0);
    float furthestBehindDist = CalculateDistance(furthestBehind, reference);
    Vector neighborPos; 
    float neighborDist;   
    for(uint16_t idx = 0; idx < m_1HopNumNeighbors; idx++) {  // also check if 1hopneighbors>0 first
        if ((m_1HopNeighborDirection[idx])) { // car is ahead, then skip
            continue;
        }
        neighborPos = Vector3D(m_1HopPositionX[idx], m_1HopPositionY[idx],0);
        neighborDist  = CalculateDistance(neighborPos, reference);
        if (neighborDist <= furthestBehindDist) {
            totalCount++;
        }               
    }
    totalCount += 1; //need to add one to account current node
    return totalCount; 
}

float 
VbpNeighbors::GetLosCalculation(Vector referencePos, Vector referenceVel) {
    Vector furthestAhead = Vector3D(NAN,NAN,0);
    Vector furthestBehind = Vector3D(NAN,NAN,0);
    float avgSpeedX = referenceVel.x; // in case needed
    float avgSpeedY = referenceVel.y;
    int furthestAheadIndex = Get2HopDistFurthestAheadByIndex(referencePos);
    int furthestBehindIndex = Get2HopDistFurthestBehindByIndex(referencePos);
    float dist = std::numeric_limits<float>::infinity(); // to make volume go to zero, if only 1 vehicle
    uint16_t totalCount = 1;  
    int idx;
    Vector temp = referencePos; // will change if needed
    if ( (furthestAheadIndex >= 0) && (furthestBehindIndex >= 0) ) {
        furthestAhead = Vector3D(m_neighborFurthestAheadX[furthestAheadIndex], m_neighborFurthestAheadY[furthestAheadIndex],0);
        furthestBehind = Vector3D(m_neighborFurthestBehindX[furthestBehindIndex], m_neighborFurthestBehindY[furthestBehindIndex],0);
        dist = CalculateDistance(furthestBehind, furthestAhead);
        avgSpeedX = m_neighborAvgSpeedX[furthestAheadIndex] + m_neighborAvgSpeedX[furthestBehindIndex];
        avgSpeedY = m_neighborAvgSpeedY[furthestAheadIndex] + m_neighborAvgSpeedY[furthestBehindIndex];
        avgSpeedX = avgSpeedX/2; // since added 2 values
        avgSpeedY = avgSpeedY/2;
        totalCount = Get2HopCarCount(furthestAheadIndex, furthestBehindIndex, referencePos);
    } 
    else if ( (furthestAheadIndex >= 0) && (furthestBehindIndex < 0) ) {
        furthestAhead = Vector3D(m_neighborFurthestAheadX[furthestAheadIndex], m_neighborFurthestAheadY[furthestAheadIndex],0);
        idx = GetNeighborFurthestBehindByIndex(referencePos);
        if (idx >= 0) {
            temp = Vector3D(m_1HopPositionX[idx], m_1HopPositionY[idx],0);
        }   
        dist = CalculateDistance(temp, furthestAhead);
        avgSpeedX = m_neighborAvgSpeedX[furthestAheadIndex];
        avgSpeedY = m_neighborAvgSpeedY[furthestAheadIndex];     
        totalCount = Get2HopCarCountSelfAhead(furthestAheadIndex, referencePos); // make changes
    }
    else if ( (furthestAheadIndex < 0) && (furthestBehindIndex >= 0) ) {
        furthestBehind = Vector3D(m_neighborFurthestBehindX[furthestBehindIndex], m_neighborFurthestBehindY[furthestBehindIndex],0);
        idx = GetNeighborFurthestAheadByIndex(referencePos);
        if (idx >= 0) {
            temp = Vector3D(m_1HopPositionX[idx], m_1HopPositionY[idx],0);
        } 
        dist = CalculateDistance(temp, furthestBehind);
        avgSpeedX = m_neighborAvgSpeedX[furthestBehindIndex];
        avgSpeedY = m_neighborAvgSpeedY[furthestBehindIndex];    
        totalCount = Get2HopCarCountSelfBehind(furthestBehindIndex, referencePos);
    } // 4th case where both are less than zero will utilize initialized values
    float time = dist/((Vector3D(avgSpeedX, avgSpeedY,0).GetLength()));
    float volume = totalCount/time;
    float LOS = volume/(m_capacityPerLane*m_numLanes);
    /*
    std::cout << "\ntotal number of vehicles " << totalCount;
    std::cout << "\ntime " << time;
    std::cout << "\ndist " << dist;
    std::cout << "\nvolume " << volume;
    std::cout << "\ncapacity " << CAPACITY_PER_LANE*NUM_LANES;
    std::cout << "\nfurthestAhead " << furthestAheadIndex;
    std::cout << "\nfurthestBehind " << furthestBehindIndex << std::endl;
    */
    return LOS;
}

float 
VbpNeighbors::GetNeighborHoodSpeedMeanX() {
    // get average speed from vector of sampled values in m_mostRecentNeighborHoodNSpeedX
    float mean = 0;
	int max = m_currentIdx;
	if (m_filledFirstTime) { // if reached N_SAMPLES in vector, must change to be the max
		max = m_NSamples;
	}
	for(int idx = 0; idx < max; idx++) {
		mean += m_mostRecentNeighborHoodNSpeedX[idx];
	}
	mean = mean/max;
	return mean;
}

float 
VbpNeighbors::GetNeighborHoodSpeedMeanY() {
    // get average speed from vector of sampled values in m_mostRecentNeighborHoodNSpeedY
	float mean = 0;
	int max = m_currentIdx;
	if (m_filledFirstTime) { // if reached N_SAMPLES in vector, must change to be the max
		max = m_NSamples;
	}
	for(int idx = 0; idx < max; idx++) {
		mean += m_mostRecentNeighborHoodNSpeedY[idx];
	}
	mean = mean/max;
	return mean;
}

void 
VbpNeighbors::PrintNeighbors2 () {
    std::cout << "My neighbor list is now: [";
    if (m_1HopNumNeighbors > 0) {
    	for (uint16_t i = 0; i < m_1HopNumNeighbors-1; i++) {
        	std::cout << m_1HopNeighborIPs.at(i) << ", ";
    	}
    	std::cout << m_1HopNeighborIPs.at(m_1HopNumNeighbors-1) << ']';
    }
    else {
        std::cout << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::PrintNeighborsAhead () {
    std::cout << "My neighbor's ahead are: [";
    if (m_1HopNumNeighborsAhead > 0) {
    	for (uint16_t i = 0; i < m_1HopNumNeighborsAhead-1; i++) {
        	std::cout << m_1HopNeighborIPAhead.at(i) << ", ";
    	}
    	std::cout << m_1HopNeighborIPAhead.at(m_1HopNumNeighborsAhead-1) << ']';
    }
    else {
        std::cout << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::PrintNeighborsBehind () {
    std::cout << "My neighbor's behind are: [";
    if (m_1HopNumNeighborsBehind > 0) {
    	for (uint16_t i = 0; i < m_1HopNumNeighborsBehind-1; i++) {
        	std::cout << m_1HopNeighborIPBehind.at(i) << ", ";
    	}
    	std::cout << m_1HopNeighborIPBehind.at(m_1HopNumNeighborsBehind-1) << ']';
    }
    else {
        std::cout << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::PrintDirections () {
    std::cout << "My neighbor's directions are (+1=ahead, 0=behind): [";
    if (m_1HopNumNeighbors > 0) {
    	for (uint16_t i = 0; i < m_1HopNumNeighbors-1; i++) {
        	std::cout << m_1HopNeighborDirection.at(i) << ", ";
    	}
    	std::cout << m_1HopNeighborDirection.at(m_1HopNumNeighbors-1) << ']';
    }
    else {
        std::cout << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::PrintNeighborDirections () {
    std::cout << "My neighbor's have this many cars ahead of them: [";
    if (m_1HopNumNeighbors > 0) {
    	for (uint16_t i = 0; i < m_1HopNumNeighbors-1; i++) {
        	std::cout << m_1HopNumNeighborAhead.at(i) << ", ";
    	}
    	std::cout << m_1HopNumNeighborAhead.at(m_1HopNumNeighbors-1) << ']' << std::endl;
        std::cout << "My neighbor's have this many cars behind them: [";
        for (uint16_t i = 0; i < m_1HopNumNeighbors-1; i++) {
        	std::cout << m_1HopNumNeighborBehind.at(i) << ", ";
    	}
    	std::cout << m_1HopNumNeighborBehind.at(m_1HopNumNeighbors-1) << ']';
    }
    else {
        std::cout << ']'<< std::endl;
        std::cout << "My neighbor's have this many cars behind them: []";
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::PrintNumNeighborsAheadBehind () {
    std::cout << "There are: " << Get1HopNumNeighborsAhead () << " ahead and " << Get1HopNumNeighborsBehind () << " behind me" << std::endl;
}

void 
VbpNeighbors::PrintLocations () {
    std::cout << "My neighbor's locations are: ";
    if (m_1HopNumNeighbors > 0) {
	    for (uint16_t i = 0; i < m_1HopNumNeighbors; i++) {
        	std::cout << '[' << m_1HopPositionX.at(i) << ", " << m_1HopPositionY.at(i) << ']' << ' ';
        }
    }
    else {
        std::cout << '[' << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::PrintSpeeds () {
    std::cout << "My neighbor's speeds are: ";
    if (m_1HopNumNeighbors > 0) {
	    for (uint16_t i = 0; i < m_1HopNumNeighbors; i++) {
        	std::cout << '[' << m_1HopNeighborSpeedX.at(i) << ", " << m_1HopNeighborSpeedY.at(i) << ']' << ' ';
        }
    }
    else {
        std::cout << '[' << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::Print1hopFurthestAhead () {
    std::cout << "My neighbor's FurthestAhead Pos [x,y] are: ";
    if (m_1HopNumNeighbors > 0) {
	    for (uint16_t i = 0; i < m_1HopNumNeighbors; i++) {
        	std::cout << '[' << m_neighborFurthestAheadX.at(i) << ", " << m_neighborFurthestAheadY.at(i) << ']' << ' ';
        }
    }
    else {
        std::cout << '[' << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::Print1hopFurthestBehind () {
    std::cout << "My neighbor's FurthestBehind Pos [x,y] are: ";
    if (m_1HopNumNeighbors > 0) {
	    for (uint16_t i = 0; i < m_1HopNumNeighbors; i++) {
        	std::cout << '[' << m_neighborFurthestBehindX.at(i) << ", " << m_neighborFurthestBehindY.at(i) << ']' << ' ';
        }
    }
    else {
        std::cout << '[' << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::PrintAvgSpeeds () {
    std::cout << "My neighbor's avg speeds are: ";
    if (m_1HopNumNeighbors > 0) {
	    for (uint16_t i = 0; i < m_1HopNumNeighbors; i++) {
        	std::cout << '[' << m_neighborAvgSpeedX.at(i) << ", " << m_neighborAvgSpeedY.at(i) << ']' << ' ';
        }
    }
    else {
        std::cout << '[' << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::PrintTimes () {
    std::cout << "My neighbor's last updates were at: [";
    if (m_1HopNumNeighbors > 0) {
    	for (uint16_t i = 0; i < m_1HopNumNeighbors-1; i++) {
        	std::cout << m_1HopNeighborLastTime.at(i) << ", ";
    	}
    	std::cout << m_1HopNeighborLastTime.at(m_1HopNumNeighbors-1) << ']';
    }
    else {
        std::cout << ']';
    }
    std::cout << std::endl; // to end line after printing everything out horizontally
}

void 
VbpNeighbors::PrintNeighborState() {
    // NS_LOG_LOGIC("current time: " << Simulator::Now());
    //PrintNeighbors2();
    // //PrintTimes ();
    // PrintDirections();
    // PrintNeighborsAhead();
    // PrintNeighborsBehind();
    // PrintNumNeighborsAheadBehind ();
    // PrintNeighborDirections();
    // Print1hopFurthestAhead ();
    // Print1hopFurthestBehind ();
    // PrintAvgSpeeds ();
    // std::cout << std::endl;  // put space between this and next printout 
}


}
}


