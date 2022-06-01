// #include <iostream>
// #include <cmath>
// #include "ns3/core-module.h"
// #include "ns3/network-module.h"
// #include "ns3/aodv-module.h"
// #include "ns3/mobility-module.h"
// #include "ns3/internet-module.h"
// #include "ns3/yans-wifi-helper.h"
// #include "ns3/ssid.h"
// #include "ns3/netanim-module.h"
// #include "ns3/vector.h"
// #include "MyRandomExpTrafficApp.h"

// // ==========================================================================
// //
// // ELEC 673 Final Project:
// // ==========================================================================
// //
// //                   Wifi 10.1.1.0
// //
// //       *           *           *           *
// //       |           |           |           |
// //       n0          n1          n2          n3
// //
// //
// //       *           *           *           *
// //       |           |           |           |
// //       n4          n5          n6          n7
// //
// //
// //       *           *           *           *
// //       |           |           |           |
// //       n8          n9          n10         n11
// //
// //
// //       *           *           *           *
// //       |           |           |           |
// //       n12         n13         n14         n15
// //
// // The user can add more wifi nodes beyond n15. The wifi network operates in
// // ad-hoc mode. The last node sends data randomly to n0 via UDP. All nodes
// // are mobile (2D Random Walk), and they are initially placed on a grid as
// // shown above. The 2D random walk is configured with two parameters: speed,
// // distance. The speed is uniformly distributed between a minimum and maximum
// // value which can be specified by the user. The distance is fixed at the
// // following value: initial distance between adjacent nodes divided by 20.
// // The initial distance between adjacent nodes is set by the user.
// //
// // Given that the nodes are mobile and the network is configured in ad-hoc
// // mode, packets are delivered from source to destination on a multi-hop
// // basis. The routing algorithm is Ad hoc On-Demand Distance Vector Routing,
// // which is a widely known algorithm to route data in ad-hoc networks. The
// // purpose of the final project is to evaluate the ability of this routing
// // algorithm to deliver packets as the nodes move faster and the number
// // of nodes increases.
// //
// // The following results are to be collected:
// //
// // - Network animation in ELEC673_final_project.xml
// //
// // - Printout for every packet received at the application layer of the last
// //   that indicates the number of bytes received
// //
// // The previous results need to be analyzed for the following scenarios in
// // which the application data rate is always 8192 bps and packets carry 4096
// // bits (i.e., 512 bytes), so the packet rate is 2 packet/s. The scenarios
// // considered include the cases of 16 and 25 nodes. In each scenario the
// // nodes increase their speed in every simulation. The second result listed
// // above shows that as the speed increases, the number of packets
// // successfully delivered to the last node decreases.
// //
// // The following simulations need to be run:
// //
// //  - As the speed increases, less packets are delivered to the destination.
// //    There are 16 nodes in this case:
// //
// //       sudo ./waf --run "scratch/ELEC673_final_project --NumNodes=16
// //                             --MinSpeed=0 --MaxSpeed=0.1 --NodeDistance=30"
// //
// //       sudo ./waf --run "scratch/ELEC673_final_project --NumNodes=16
// //                             --MinSpeed=32 --MaxSpeed=64 --NodeDistance=30"
// //
// //       sudo ./waf --run "scratch/ELEC673_final_project --NumNodes=16
// //                            --MinSpeed=64 --MaxSpeed=128 --NodeDistance=30"
// //
// //  - As the speed increases, less packets are delivered to the destination.
// //    There are 25 nodes in this case, and the number of packets delivered
// //    to the destination decreases faster with the speed.
// //
// //       sudo ./waf --run "scratch/ELEC673_final_project --NumNodes=25
// //                             --MinSpeed=0 --MaxSpeed=0.1 --NodeDistance=30"
// //
// //       sudo ./waf --run "scratch/ELEC673_final_project --NumNodes=25
// //                             --MinSpeed=10 --MaxSpeed=20 --NodeDistance=30"
// //
// //       sudo ./waf --run "scratch/ELEC673_final_project --NumNodes=25
// //                             --MinSpeed=32 --MaxSpeed=64 --NodeDistance=30"
// //
// //       sudo ./waf --run "scratch/ELEC673_final_project --NumNodes=25
// //                            --MinSpeed=64 --MaxSpeed=128 --NodeDistance=30"
// //
// // ==========================================================================


// #define NET_ADDRESS "10.1.1.0"
// #define NET_MASK_ADDRESS "255.255.255.0"
// #define UDP_PORT 8080
// #define SOURCE_START_TIME 1 // seconds

// using namespace ns3;

// // Callback that connects to UDP socket of the destination node to print the
// // number of packets delivered to the application layer of the destination
// // node
// void ReceivePacket(Ptr<Socket> socket)
// {
//   while(Ptr<Packet> packet = socket->Recv())
//   {
//     std::cout << "Application Layer:" << packet->GetSize() << " bytes received" << std::endl;
//   }
// }

// // Simulation
// int main (int argc, char **argv)
// {
//   // Allow user to set the number of wifi nodes, distance between adjacent
//   // nodes, simulation time, applications' data rate, packet size, run number
//   // of the pseudo random number generator (PRNG),and minimum and maximum
//   // speed of the uniformly distributed node speed.
//   uint32_t NumNodes = 16;
//   double NodeDistance = 30; // meters
//   double SimulationTime = 100; // seconds
//   std::string AppDataRate = {'8', '1', '9', '2'}; // bits per second
//   uint32_t PacketSize = 512; // bytes
//   uint32_t PRNGRunNumber = 1;
//   double MinSpeed = 64; // meters per second
//   double MaxSpeed = 128; // meters per second
//   CommandLine cmd;
//   cmd.AddValue ("NumNodes", "Number of nodes", NumNodes);
//   cmd.AddValue ("NodeDistance", "Distance between adjacent nodes (meters)", NodeDistance);
//   cmd.AddValue ("SimulationTime", "Simulation time (seconds)", SimulationTime);
//   cmd.AddValue("AppDataRate", "Application's average data rate (bps)", AppDataRate);
//   cmd.AddValue("PacketSize", "Application's packet size (bytes)", PacketSize);
//   cmd.AddValue("PRNGRunNumber", "Run number of the pseudo random number generator (PRNG) that determines packet transmission times", PRNGRunNumber);
//   cmd.AddValue("MinSpeed", "The speed of nodes is uniformly distributed from MinSpeed to MaxSpeed (m/s)", MinSpeed);
//   cmd.AddValue("MaxSpeed", "The speed of nodes is uniformly distributed from MinSpeed to MaxSpeed (m/s)", MaxSpeed);
//   cmd.Parse (argc, argv);


//   // Create nodes and keep them on a container. Names are added to the nodes.
//   // Install the Internet stack and set the routing protocol to AODV.
//   NodeContainer nodes;
//   nodes.Create(NumNodes);
//   for(uint32_t i = 0; i < NumNodes; i++)
//   {
//     std::ostringstream nodename;
//     nodename << "node_" << i;
//     Names::Add(nodename.str(), nodes.Get(i));
//   }
//   AodvHelper aodv;
//   InternetStackHelper stack;
//   stack.SetRoutingHelper(aodv);
//   stack.Install(nodes);


//   // Nodes are mobile (2D random walk). The initial location of the nodes is
//   // a grid. The area of movement is the square that encloses the grid. The
//   // side length of the square is specified by the user. The 2D random walk
//   // is configured with two parameters: speed, distance. The speed is
//   // uniformly distributed between a minimum and maximum value which can be
//   // specified by the user. The distance is fixed at the following value:
//   // initial distance between adjacent nodes divided by 20. The initial
//   // distance between adjacent nodes is set by the user.
//   MobilityHelper mobility;
//   mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
//   mobility.Install(nodes);
//   Ptr<GridPositionAllocator> MyPositionAllocator = CreateObject<GridPositionAllocator>();
//   MyPositionAllocator->SetAttribute("MinX", DoubleValue(0.0));
//   MyPositionAllocator->SetAttribute("MinY", DoubleValue(0.0));
//   MyPositionAllocator->SetAttribute("DeltaX", DoubleValue(NodeDistance));
//   MyPositionAllocator->SetAttribute("DeltaY", DoubleValue(NodeDistance));
//   int GridWidth = (int) std::ceil(std::sqrt((double) NumNodes));
//   MyPositionAllocator->SetAttribute("GridWidth", UintegerValue(GridWidth));
//   MyPositionAllocator->SetAttribute("LayoutType", StringValue("RowFirst"));
//   for(uint32_t i = 0; i < NumNodes; i++)
//   {
//     //nodes.Get(i)->GetObject<RandomWalk2dMobilityModel>()->SetAttribute("Bounds", RectangleValue(Rectangle(0, GridWidth*NodeDistance, 0, GridWidth*NodeDistance)));
//     //nodes.Get(i)->GetObject<RandomWalk2dMobilityModel>()->SetPosition(MyPositionAllocator->GetNext());
//     RngSeedManager::SetRun(PRNGRunNumber+i);
//     Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable>();
//     x->SetAttribute ("Min", DoubleValue (MinSpeed));
//     x->SetAttribute ("Max", DoubleValue (MaxSpeed));
//     nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(i*1, 0, 0));
//     nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(10, 0, 0));
//     //nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetAttribute("Speed", PointerValue(x));
//     //nodes.Get(i)->GetObject<RandomWalk2dMobilityModel>()->SetAttribute("Distance", DoubleValue(NodeDistance/20));
//   }


//   // Install the channel and wifi net devices on all nodes. The MAC layer is
//   // configured in ad-hoc mode. Its rate-control algorithm is
//   // ConstantRateWifiManager with the data-mode set to OfdmRate6Mbps, and the
//   // RtsCtsThreshold is set to 0.
//   YansWifiChannelHelper wifiChannelHelper = YansWifiChannelHelper::Default();
//   YansWifiPhyHelper wifiPhyHelper;
//   WifiMacHelper wifiMacHelper;
//   WifiHelper wifiHelper;
//   NetDeviceContainer devices;
//   wifiPhyHelper.SetChannel(wifiChannelHelper.Create());
//   wifiMacHelper.SetType("ns3::AdhocWifiMac");
//   wifiHelper.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue("OfdmRate6Mbps"), "RtsCtsThreshold", UintegerValue(0));
//   devices = wifiHelper.Install(wifiPhyHelper, wifiMacHelper, nodes);


//   // Assign IP addresses to wife network devices
//   Ipv4AddressHelper address;
//   address.SetBase(NET_ADDRESS, NET_MASK_ADDRESS);
//   Ipv4InterfaceContainer interfaces = address.Assign(devices);


//   // Create and bind the socket on the destination node. Set the receive
//   // callback that prints the number of data bytes received in every packet.
//   Ptr<Socket> udpSinkSocket = Socket::CreateSocket(nodes.Get(0), UdpSocketFactory::GetTypeId());
//   udpSinkSocket->Bind(InetSocketAddress(Ipv4Address::GetAny(), UDP_PORT));
//   udpSinkSocket->SetRecvCallback(MakeCallback(&ReceivePacket));


//   // Create and install the source application over UDP on the last node that
//   // generates random traffic directed to the sink (n0)
//   Address udpSinkAddress(InetSocketAddress(interfaces.GetAddress(0), UDP_PORT));
//   Ptr<MyRandomExpTrafficApp> udpSourceAppPtr;
//   Ptr<Socket> udpSourceSocket = Socket::CreateSocket(nodes.Get(NumNodes-1), UdpSocketFactory::GetTypeId());
//   udpSourceAppPtr = CreateObject<MyRandomExpTrafficApp>();
//   udpSourceAppPtr->Setup(udpSourceSocket, udpSinkAddress, PacketSize, DataRate(AppDataRate), PRNGRunNumber);
//   nodes.Get(NumNodes-1)->AddApplication(udpSourceAppPtr);

//   // Configure time resolution, simulation start and stop times.
//   Time::SetResolution(Time::NS);
//   udpSourceAppPtr->SetStartTime(Seconds(SOURCE_START_TIME));
//   Simulator::Stop(Seconds(SimulationTime));

//   // Enable network animation
//   AnimationInterface anim("ELEC673_final_project.xml");
//   anim.EnablePacketMetadata(true);

//   // Execute simulation
//   Simulator::Run();
//   Simulator::Destroy();

//   // Success
//   return 0;
// }
