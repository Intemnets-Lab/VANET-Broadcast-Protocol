// #include "ns3/core-module.h"
// #include "ns3/network-module.h"
// #include "ns3/mobility-module.h"
// #include "ns3/internet-module.h"
// #include "ns3/yans-wifi-helper.h"
// #include "ns3/ssid.h"
// #include "ns3/netanim-module.h"
// #include "ns3/vector.h"
// //#include "ns3/aodv-module.h"
// #include "ns3/v4ping-helper.h"
// #include "vanet-broadcast-helper.h"
// #include "ns3/constant-velocity-mobility-model.h"
// #include "MyRandomExpTrafficApp.h"
// #include "ns3/constant-acceleration-mobility-model.h"
// #include "VbpApp.h"

// //Description: Destination vehicle inside BA. Source vehicle approaches with packets in a queue.
// // =======================================================================================================================================================
// //                                                          BA
// //                                               |----------------------|                        Wifi 10.1.1.0
// //           *                                   |          *           |
// //           |                                   |          |           |
// //       n0 (src)                                |        n1 (dst)      |
// //       10.1.1.1                                |        10.1.1.2      |
// //                                               |                      |
// //                                               |----------------------|
// // =======================================================================================================================================================

// #define NET_ADDRESS "10.1.1.0"
// #define NET_MASK_ADDRESS "255.255.255.0"
// #define NET_BROADCAST_ADDRESS "10.1.1.255"
// #define UDP_PORT 8080
// #define SOURCES_START_TIME 1   // seconds
// #define SOURCE_START_TIME 1    // seconds
// #define PK_INTERARRIVAL_TIME 1 // seconds
// #define DISTANCE 5             // meters
// #define SPEED 1
// #define FREQ 2.4e9         // Hz
// #define SYS_LOSS 1         // unitless
// #define MIN_LOSS 0         // dB
// #define TX_POWER 110        // dBm
// #define TX_GAIN 15          // dB
// #define RX_GAIN 15          // dB
// #define RX_SENSITIVITY -97 // dBm
// #define VBP_PORT 8081

// using namespace ns3;

// NS_LOG_COMPONENT_DEFINE("wireless-grid");

// // Callback executed by the sink socket when a data packet is received
// void ReceivePacket(Ptr<Socket> socket)
// {
//     while (Ptr<Packet> packet = socket->Recv())

//         std::cout << "Application Layer:" << packet->GetSize() << " bytes received" << std::endl;
// }

// // Function called to schedule the transmission of the next datagram from the source sockets
// void SendDatagram(Ptr<Socket> socket, uint32_t pktsize, uint32_t numpkts, double interarrival_time)
// {
//     if (numpkts > 0)
//     {
//         socket->Send(Create<Packet>(pktsize));
//         Simulator::Schedule(Seconds(interarrival_time), &SendDatagram, socket, pktsize, numpkts - 1, interarrival_time);
//     }
//     else
//         socket->Close();
// }

// // Callback executed the wifi physical layer of each node
// void RssCallback(std::string context, Ptr<const Packet> packet, uint16_t channelFreqMhz, WifiTxVector wifiTxVector, MpduInfo mpduInfo, SignalNoiseDbm signalNoiseDbm)
// {
//     NS_LOG_INFO(context << ": Received Signal Strength (RSS) = " << signalNoiseDbm.signal << " dBm, Noise Power = " << signalNoiseDbm.noise << " dBm");
// }

// // Simulation
// int main(int argc, char *argv[])
// {

//     // Allow user to set the transmission mode used for RTS, data, and non-unicast packet transmissions,
//     // tx gain of the primary and interfering nodes, time from the start of primary transmission to the
//     // start of the interfering transmission, packet size of primary and interfering nodes, and number
//     // of packets transmitted by primary and interfering nodes.
//     std::string PhyMode("DsssRate1Mbps");
//     uint32_t NumNodes = 2;
//     double PrimaryTxGain = TX_GAIN;     // dBm
//     double InterferingTxGain = TX_GAIN; // dBm
//     double TimeToInterfere = 0;         // uS
//     double SimulationTime = 70;        // seconds
//     uint32_t PrimaryPktSize = 1024;     // bytes
//     uint32_t InterferingPktSize = 1024; // bytes
//     uint32_t PacketSize = 512;          // bytes
//     uint32_t PRNGRunNumber = 1;
//     //   uint32_t PRNGRunNumber = 1;
//     //   double MinSpeed = 1; // meters per second
//     //   double MaxSpeed = 20; // meters per second
//     std::string AppDataRate = {'8', '1', '9', '2'}; // bits per second
//     // uint32_t PacketSize = 512; // bytes
//     // uint32_t PRNGRunNumber = 1;
//     uint32_t NumPackets = 10;
//     bool Broadcast = true;
//     bool showPings = true;
//     bool printRoutes = true;

//     CommandLine cmd;
//     cmd.AddValue("printRoutes", "Print routing table dumps.", printRoutes);
//     cmd.AddValue("PhyMode", "Transmission mode used for RTS, data, and non-unicast packet transmissions", PhyMode);
//     cmd.AddValue("PrimaryTxGain", "Transmission gain of the primary node (dB)", PrimaryTxGain);
//     cmd.AddValue("InterferingTxGain", "Transmission gain of the interfering node (dB)", InterferingTxGain);
//     cmd.AddValue("TimeToInterfere", "Time from start of primary transmission to start of interfering transmission (uS)", TimeToInterfere);
//     cmd.AddValue("PrimaryPktSize", "Packet size of the primary node (bytes)", PrimaryPktSize);
//     cmd.AddValue("InterferingPktSize", "Packet size of the interfering node (bytes)", InterferingPktSize);
//     cmd.AddValue("NumPackets", "Number of packets transmitted by each node, primary and interfering", NumPackets);
//     cmd.AddValue("Broadcast", "Enable broadcast transmissions on primary and interfering nodes", Broadcast);
//     cmd.AddValue("showPings", "Show Ping6 reception", showPings);
//     cmd.Parse(argc, argv);

//     // Create nodes. Install Internet stack. Set location of the nodes and configure them as static (no movement).
//     NodeContainer nodes;
//     nodes.Create(NumNodes);

//     // vbp
//     InternetStackHelper stack;
//     VanetBroadcastHelper vbp; //VanetBroadcastHelper
//     vbp.SetBroadcastArea({50, -10, 80, 10});
//     stack.SetRoutingHelper(vbp);
//     stack.Install(nodes);

//     MobilityHelper mobility;
//     mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
//     mobility.Install(nodes);


//         for (int i = 0; i < int(NumNodes-1); i++)
//     {
//         if (i == 1)
//         {
//             nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector((float)51, 0, 0));
//             nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector((float)0, 0, 0)); // 0.1*i because vehicles will lose neighbors about 50 seconds into simulation. SPEED is original variable here.        
//         }
//         else
//         {
//         nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector((float)DISTANCE * (NumNodes - i), 0, 0));
//         nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector((float)SPEED, 0, 0)); // 0.1*i because vehicles will lose neighbors about 50 seconds into simulation. SPEED is original variable here.
//         }
//     }


//     //n0 source, n1 destination
//     //  nodes.Get(0)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(0, 0, 0));
//     //  nodes.Get(0)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(5, 0, 0));
//     //  nodes.Get(1)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(50, 0, 0));
//     //  nodes.Get(1)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(2, 0, 0));

//     //  nodes.Get(0)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(0, 0, 0));
//     //  nodes.Get(0)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(5, 0, 0));
//     //  nodes.Get(1)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(51, 0, 0));
//     //  nodes.Get(1)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(0, 0, 0));

//     // Create channel of constant propagation speed and Friis loss. Enable Radiotap link
//     // layer information. Configure the wifi MAC layer in Ad Hoc mode. Use the 802.11b
//     // standard (DSSS PHY (Clause 15) and HR/DSSS PHY (Clause 18)). Configure the transmission
//     // mode specified by the user with PhyMode. Connect devices to nodes and channel using
//     // the MAC and physical-layer configuration previously identified.
//     YansWifiChannelHelper wifiChannelHelper = YansWifiChannelHelper::Default();
//     wifiChannelHelper.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
//     wifiChannelHelper.AddPropagationLoss("ns3::FriisPropagationLossModel", "Frequency", DoubleValue(FREQ), "SystemLoss", DoubleValue(SYS_LOSS), "MinLoss", DoubleValue(MIN_LOSS));
//     YansWifiPhyHelper wifiPhyHelper;
//     wifiPhyHelper.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
//     wifiPhyHelper.SetChannel(wifiChannelHelper.Create());
//     WifiMacHelper wifiMacHelper;
//     wifiMacHelper.SetType("ns3::AdhocWifiMac");
//     WifiHelper wifiHelper;
//     wifiHelper.SetStandard(WIFI_STANDARD_80211b);
//     Config::SetDefault("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue(PhyMode));
//     wifiHelper.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue(PhyMode), "ControlMode", StringValue(PhyMode));
//     NetDeviceContainer devices = wifiHelper.Install(wifiPhyHelper, wifiMacHelper, nodes);
//     // Configure frequency, transmission power, tx gain, rx gain, and sensitivity of the physical layer of n0, n1, n2
//     std::ostringstream configpath;
//     for (int i = 0; i < int(NumNodes); i++)
//     {
//         configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/ChannelNumber";
//         Config::Set(configpath.str(), UintegerValue(1));
//         configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/Frequency";
//         Config::Set(configpath.str(), UintegerValue((uint32_t)FREQ / 1e6));
//         configpath.str("");
//         configpath.clear();
//         configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/TxPowerEnd";
//         Config::Set(configpath.str(), DoubleValue(TX_POWER));
//         configpath.str("");
//         configpath.clear();
//         configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/TxPowerStart";
//         Config::Set(configpath.str(), DoubleValue(TX_POWER));
//         configpath.str("");
//         configpath.clear();
//         configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/TxGain";
//         Config::Set(configpath.str(), DoubleValue(TX_GAIN));
//         configpath.str("");
//         configpath.clear();
//         configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/RxGain";
//         Config::Set(configpath.str(), DoubleValue(RX_GAIN));
//         configpath.str("");
//         configpath.clear();
//         configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/RxSensitivity";
//         Config::Set(configpath.str(), DoubleValue(RX_SENSITIVITY));
//         configpath.str("");
//         configpath.clear();
//     }

//     // Set IP addresses on wifi devices
//     // assigns second interface, first interface is by default. First interface is for myself
//     Ipv4AddressHelper address;
//     address.SetBase(NET_ADDRESS, NET_MASK_ADDRESS);
//     Ipv4InterfaceContainer interfaces = address.Assign(devices); // notify methods (called from this line) will allow us to access interface to tx hello-packets

//  // =============== SOURCE APP =================
//     Ptr<Socket> udpSourceSocket = Socket::CreateSocket(nodes.Get(NumNodes - 1), UdpSocketFactory::GetTypeId());
//     Ptr<MyRandomExpTrafficApp> udpSourceAppPtr = CreateObject<MyRandomExpTrafficApp>();
//     udpSourceAppPtr->Setup(udpSourceSocket, Ipv4Address(NET_BROADCAST_ADDRESS), VBP_PORT, PacketSize, DataRate(AppDataRate), PRNGRunNumber);
//     nodes.Get(NumNodes - 1)->AddApplication(udpSourceAppPtr);
//     //udpSourceSocket->SetRecvCallback(MakeCallback(&ReceivePacket));
//     udpSourceAppPtr->SetStartTime(Seconds(2));
//     // =============== SOURCE APP =================

//     // ============== SINK APP ===================
//     for(uint32_t i = 0; i < NumNodes-1; i++) 
//     {
//         // add routing app
//         Ptr<Socket> vbpSocket = Socket::CreateSocket(nodes.Get(i), UdpSocketFactory::GetTypeId());
//         Ptr<VbpApp> vbpAppPtr = CreateObject<VbpApp>();
//         vbpAppPtr->Setup(vbpSocket, VBP_PORT);
//         nodes.Get(i)->AddApplication(vbpAppPtr);
//         vbpSocket->SetRecvCallback(MakeCallback(&ReceivePacket));
//         vbpAppPtr->SetStartTime(Seconds(SOURCE_START_TIME));
//     }
//     // ============== SINK APP ===================

//     // Enable promiscuous pcap tracing on sink node (n0) and enable network animation
//     //wifiPhyHelper.EnablePcap("vbp-two-vehicles.pcap", nodes.Get(0)->GetDevice(1), false, true);
//     AnimationInterface anim("vbp-example1.xml");
//     anim.EnablePacketMetadata(true);

//     // Configure time resolution, simulation start and stop times.
//     Time::SetResolution(Time::NS);
//     udpSourceAppPtr->SetStartTime(Seconds(SOURCE_START_TIME));
//     Simulator::Stop(Seconds(SimulationTime));

//     // Execute simulation
//     Simulator::Run();
//     Simulator::Destroy();

//     // Success
//     return 0;
// }
