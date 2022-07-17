#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/netanim-module.h"
#include "ns3/vector.h"
//#include "ns3/aodv-module.h"
#include "ns3/v4ping-helper.h"
#include "vanet-broadcast-helper.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "MyRandomExpTrafficApp.h"
#include "ns3/constant-acceleration-mobility-model.h"
#include "VbpApp.h"


// =======================================================================================================================================================
//                                                                                                                      (100000,-10)     |----------------|
//                          Wifi 10.1.1.0                                                                                                |                |
//           *              *              *              *              *                                                               |                |
//           |    <500 m>   |   <500 m>    |   <500 m>    |   <500 m>    |                                                               |      BA        |
//       n0 (src)          n1             n2              n3             n4 (dst)                                                        |                |
//       10.1.1.1        10.1.1.2       10.1.1.3       10.1.1.4        10.1.1.5                                                          |                |
//                                                                                                                                       |                |  
//                                                                                                                                       |----------------| (100050,10)  
// =======================================================================================================================================================

#define NET_ADDRESS "10.1.1.0"
#define NET_MASK_ADDRESS "255.255.255.0"
#define NET_BROADCAST_ADDRESS "10.1.1.255"
#define UDP_PORT 8080
#define SOURCES_START_TIME 1   // seconds
#define SOURCE_START_TIME 1    // seconds
#define PK_INTERARRIVAL_TIME 1 // seconds
#define DISTANCE 5             // meters
#define SPEED 3
#define FREQ 2.4e9         // Hz
#define SYS_LOSS 1         // unitless
#define MIN_LOSS 0         // dB
#define TX_POWER 110        // dBm
#define TX_GAIN 15          // dB
#define RX_GAIN 15          // dB
#define RX_SENSITIVITY -97 // dBm
#define VBP_PORT 8081

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("wireless-grid");

// Callback executed by the sink socket when a data packet is received
void ReceivePacket(Ptr<Socket> socket)
{
    while (Ptr<Packet> packet = socket->Recv())
        
        std::cout << "Application Layer:" << packet->GetSize() << " bytes received" << std::endl;
}

// Function called to schedule the transmission of the next datagram from the source sockets
void SendDatagram(Ptr<Socket> socket, uint32_t pktsize, uint32_t numpkts, double interarrival_time)
{
    if (numpkts > 0)
    {
        socket->Send(Create<Packet>(pktsize));
        Simulator::Schedule(Seconds(interarrival_time), &SendDatagram, socket, pktsize, numpkts - 1, interarrival_time);
    }
    else
        socket->Close();
}

// Callback executed the wifi physical layer of each node
void RssCallback(std::string context, Ptr<const Packet> packet, uint16_t channelFreqMhz, WifiTxVector wifiTxVector, MpduInfo mpduInfo, SignalNoiseDbm signalNoiseDbm)
{
    NS_LOG_INFO(context << ": Received Signal Strength (RSS) = " << signalNoiseDbm.signal << " dBm, Noise Power = " << signalNoiseDbm.noise << " dBm");
}

// Simulation
int main(int argc, char *argv[])
{

    // Allow user to set the transmission mode used for RTS, data, and non-unicast packet transmissions,
    // tx gain of the primary and interfering nodes, time from the start of primary transmission to the
    // start of the interfering transmission, packet size of primary and interfering nodes, and number
    // of packets transmitted by primary and interfering nodes.
    std::string PhyMode("DsssRate1Mbps");
    uint32_t NumNodes = 22;
    double PrimaryTxGain = TX_GAIN;     // dBm
    double InterferingTxGain = TX_GAIN; // dBm
    double TimeToInterfere = 0;         // uS
    double SimulationTime = 50;        // seconds
    uint32_t PrimaryPktSize = 1024;     // bytes
    uint32_t InterferingPktSize = 1024; // bytes
    uint32_t PacketSize = 512;          // bytes
    uint32_t PRNGRunNumber = 1;
    //   uint32_t PRNGRunNumber = 1;
    //   double MinSpeed = 1; // meters per second
    //   double MaxSpeed = 20; // meters per second
    std::string AppDataRate = {'8', '1', '9', '2'}; // bits per second
    // uint32_t PacketSize = 512; // bytes
    // uint32_t PRNGRunNumber = 1;
    uint32_t NumPackets = 10;
    bool Broadcast = true;
    bool showPings = true;
    bool printRoutes = true;

    CommandLine cmd;
    cmd.AddValue("printRoutes", "Print routing table dumps.", printRoutes);
    cmd.AddValue("PhyMode", "Transmission mode used for RTS, data, and non-unicast packet transmissions", PhyMode);
    cmd.AddValue("PrimaryTxGain", "Transmission gain of the primary node (dB)", PrimaryTxGain);
    cmd.AddValue("InterferingTxGain", "Transmission gain of the interfering node (dB)", InterferingTxGain);
    cmd.AddValue("TimeToInterfere", "Time from start of primary transmission to start of interfering transmission (uS)", TimeToInterfere);
    cmd.AddValue("PrimaryPktSize", "Packet size of the primary node (bytes)", PrimaryPktSize);
    cmd.AddValue("InterferingPktSize", "Packet size of the interfering node (bytes)", InterferingPktSize);
    cmd.AddValue("NumPackets", "Number of packets transmitted by each node, primary and interfering", NumPackets);
    cmd.AddValue("Broadcast", "Enable broadcast transmissions on primary and interfering nodes", Broadcast);
    cmd.AddValue("showPings", "Show Ping6 reception", showPings);
    cmd.Parse(argc, argv);

    // Create nodes. Install Internet stack. Set location of the nodes and configure them as static (no movement).
    NodeContainer nodes;
    nodes.Create(NumNodes);

    // vbp
    InternetStackHelper stack;
    VanetBroadcastHelper vbp; //VanetBroadcastHelper
    vbp.SetBroadcastArea({100000, -10, 100050, 10});
    //vbp.SetBroadcastArea({-100050, -10, -100000, 10});
    stack.SetRoutingHelper(vbp);
    stack.Install(nodes);

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    mobility.Install(nodes);

     //Meeting June 9
     // New caravan parameters: Vehicle distance 50 meters. 21 vehicles total.
     //Set up all caravan tests as below. Multiple neighbors. Find new speeds for low, mid, high traffic level print-outs. 3 simulations
     //Confirm next hop is selected correctly, Node0 sends packet to Node250 only. Node0 can not send packet to prior vehicle such as Node150.
     //Run the same neighborlist and queue tests as before. 
     // 0 50 100 150 200 250 300 350 400 450 500 550 600 650 700 750 800 850 900 950 1000
    // float vehicleDistance = 50;
    // float vehicleSpeed = 30; //low 5, medium 15, high 30
    // for (int i = 0; i < int(NumNodes); i++)
    // {
    //     nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(vehicleDistance*i, 0, 0));
    //     std::cout << nodes.Get(i) << " Vehicle Distance " << vehicleDistance*i << std::endl;
    //     nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(vehicleSpeed, 0, 0)); 
    // }

    //Another set of simulations: BA is very-far to the left of caravan. Caravan moves away from BA.
    //Source is head of caravan (not the tail as in previous sims) Node1000 is source. Do separate sims for the three traffic levels.
    //Verify routing takes place in opposite direction. Node1000 sends packet to Node750. Node750 sends to Node500...
    //Queue should only grow in tail node (Node0).

    //experiments 1-4
    // float vehicleDistance = 50;
    // float vehicleSpeed = 15; //low 5, medium 15, high 30
    // for (int i = 0; i < int(NumNodes); i++)
    // {
    //     nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(vehicleDistance*i, 0, 0));
    //     std::cout << nodes.Get(i) << " Vehicle Distance " << vehicleDistance*i << std::endl;
    //     nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector((vehicleSpeed + 0.0001*i), 0, 0)); 
    // }

    //empty queue experiment
    float vehicleDistance = 50;
    float vehicleSpeed = 5; //low 5, medium 15, high 30
    for (int i = 0; i < int(NumNodes); i++)
    {
        if (i == NumNodes-1) //emptyqueue pos/vel
        {
            //BA to right
            nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(NumNodes*vehicleDistance + vehicleSpeed*SimulationTime/2 + 100, 0, 0));
            //std::cout << nodes.Get(i) << "EmptyQueue Vehicle Distance " << NumNodes*vehicleDistance + vehicleSpeed*SimulationTime/2 + 100 << std::endl;
            nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector((vehicleSpeed/vehicleSpeed), 0, 0)); 

            // //BA to left
            // nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(-(50 + vehicleSpeed*SimulationTime/2), 0, 0));
            // std::cout << nodes.Get(i) << "EmptyQueue Vehicle Distance " << NumNodes*vehicleDistance + vehicleSpeed*SimulationTime/2 << std::endl;
            // nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector((vehicleSpeed*2), 0, 0)); 
        }
        else //caravan pos/vel
        {
        nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetPosition(Vector(vehicleDistance*i, 0, 0));
        //std::cout << nodes.Get(i) << " Vehicle Distance " << vehicleDistance*i << std::endl;
        nodes.Get(i)->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector((vehicleSpeed + 0.0001*i), 0, 0));
        } 
    }

    //Another set of simulations: Source vehicle is middle vehicle (Node500). BA is very far to the right of the caravan. 
    //Make sure next hop's are correct. Node500 sends packets to Node250 and to Node750. Node750 sends to Node1000 and Node250 sends to Node0.
    //Queue should grow only at Node0 and Node1000. Confirm all above tests for three traffic levels. 
    

    //Another set of simulations. Source vehicle is middle vehicle (Node500). BA is very far to the LEFT of the caravan. 
    //Make sure next hop's are correct. Node500 sends packets to Node250 ONLY. Node250 sends to Node0.
    //Queue should grow only at Node0. Confirm all above tests for three traffic levels. 

    //Also clean up code. Can leave debug messages if they are significant. Comment them out and leave a comment. Get rid of insignificant debug messages.
    //Include comments on code that explains what is happening.

    //Thesis: Detailed enough so future work students won't need to talk to me to understand codebase. Talk about all issues encountered and what each code
    // code piece does. It is like a technical reference document. 
    //Also section on the above 12 simulations ran and their results.
    //Another section: Installation requirements and software requirements. How to install VANET-Broadcast-Protocol in someone's ns-3 folder.
    //Create github page for this code.



    // Create channel of constant propagation speed and Friis loss. Enable Radiotap link
    // layer information. Configure the wifi MAC layer in Ad Hoc mode. Use the 802.11b
    // standard (DSSS PHY (Clause 15) and HR/DSSS PHY (Clause 18)). Configure the transmission
    // mode specified by the user with PhyMode. Connect devices to nodes and channel using
    // the MAC and physical-layer configuration previously identified.
    YansWifiChannelHelper wifiChannelHelper = YansWifiChannelHelper::Default();
    wifiChannelHelper.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    wifiChannelHelper.AddPropagationLoss("ns3::FriisPropagationLossModel", "Frequency", DoubleValue(FREQ), "SystemLoss", DoubleValue(SYS_LOSS), "MinLoss", DoubleValue(MIN_LOSS));
    YansWifiPhyHelper wifiPhyHelper;
    wifiPhyHelper.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
    wifiPhyHelper.SetChannel(wifiChannelHelper.Create());
    WifiMacHelper wifiMacHelper;
    wifiMacHelper.SetType("ns3::AdhocWifiMac");
    WifiHelper wifiHelper;
    wifiHelper.SetStandard(WIFI_STANDARD_80211b);
    Config::SetDefault("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue(PhyMode));
    wifiHelper.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue(PhyMode), "ControlMode", StringValue(PhyMode));
    NetDeviceContainer devices = wifiHelper.Install(wifiPhyHelper, wifiMacHelper, nodes);
    // Configure frequency, transmission power, tx gain, rx gain, and sensitivity of the physical layer of n0, n1, n2
    std::ostringstream configpath;
    for (int i = 0; i < int(NumNodes); i++)
    {
        configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/ChannelNumber";
        Config::Set(configpath.str(), UintegerValue(1));
        configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/Frequency";
        Config::Set(configpath.str(), UintegerValue((uint32_t)FREQ / 1e6));
        configpath.str("");
        configpath.clear();
        configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/TxPowerEnd";
        Config::Set(configpath.str(), DoubleValue(TX_POWER));
        configpath.str("");
        configpath.clear();
        configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/TxPowerStart";
        Config::Set(configpath.str(), DoubleValue(TX_POWER));
        configpath.str("");
        configpath.clear();
        configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/TxGain";
        Config::Set(configpath.str(), DoubleValue(TX_GAIN));
        configpath.str("");
        configpath.clear();
        configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/RxGain";
        Config::Set(configpath.str(), DoubleValue(RX_GAIN));
        configpath.str("");
        configpath.clear();
        configpath << "/NodeList/" << nodes.Get(i)->GetId() << "/DeviceList/1/$ns3::WifiNetDevice/Phy/RxSensitivity";
        Config::Set(configpath.str(), DoubleValue(RX_SENSITIVITY));
        configpath.str("");
        configpath.clear();
    }

    // Set IP addresses on wifi devices
    // assigns second interface, first interface is by default. First interface is for myself
    Ipv4AddressHelper address;
    address.SetBase(NET_ADDRESS, NET_MASK_ADDRESS);
    Ipv4InterfaceContainer interfaces = address.Assign(devices); // notify methods (called from this line) will allow us to access interface to tx hello-packets

 // =============== SOURCE APP =================
    Ptr<Socket> udpSourceSocket = Socket::CreateSocket(nodes.Get(0), UdpSocketFactory::GetTypeId());
    Ptr<MyRandomExpTrafficApp> udpSourceAppPtr = CreateObject<MyRandomExpTrafficApp>();
    udpSourceAppPtr->Setup(udpSourceSocket, Ipv4Address(NET_BROADCAST_ADDRESS), VBP_PORT, PacketSize, DataRate(AppDataRate), PRNGRunNumber);
    nodes.Get(0)->AddApplication(udpSourceAppPtr);
    udpSourceAppPtr->SetStartTime(Seconds(2));
    // =============== SOURCE APP =================

    // ============== SINK APP ===================
    for(uint32_t i = 0; i < NumNodes; i++) 
    {
        // add routing app
        Ptr<Socket> vbpSocket = Socket::CreateSocket(nodes.Get(i), UdpSocketFactory::GetTypeId());
        Ptr<VbpApp> vbpAppPtr = CreateObject<VbpApp>();
        vbpAppPtr->Setup(vbpSocket, VBP_PORT);
        nodes.Get(i)->AddApplication(vbpAppPtr);
        vbpSocket->SetRecvCallback(MakeCallback(&ReceivePacket));
        vbpAppPtr->SetStartTime(Seconds(SOURCE_START_TIME));
    }
    // ============== SINK APP ===================

    // Enable promiscuous pcap tracing on sink node (n0) and enable network animation
    //wifiPhyHelper.EnablePcap("vbp-two-vehicles.pcap", nodes.Get(0)->GetDevice(1), false, true);
    AnimationInterface anim("vbp-example-4.xml");
    anim.EnablePacketMetadata(true);

    // Configure time resolution, simulation start and stop times.
    Time::SetResolution(Time::NS);
    udpSourceAppPtr->SetStartTime(Seconds(SOURCE_START_TIME));
    Simulator::Stop(Seconds(SimulationTime));

    // Execute simulation
    Simulator::Run();
    Simulator::Destroy();

    // Success
    return 0;
}