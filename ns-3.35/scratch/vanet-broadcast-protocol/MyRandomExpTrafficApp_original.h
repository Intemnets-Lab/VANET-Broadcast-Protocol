// /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
// /*
//  * This program is free software; you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License version 2 as
//  * published by the Free Software Foundation;
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with this program; if not, write to the Free Software
//  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//  */


// // Class declaration of an application that transmits UDP packets at random times.
// // The time between consecutive packets is exponentially distributed.

// #ifndef MYRANDOMEXPTRAFFICAPP_H
// #define MYRANDOMEXPTRAFFICAPP_H

// #include "ns3/random-variable-stream.h"
// #include "ns3/applications-module.h"
// #include "vbp-data-packet-header.h"

// namespace ns3
// {
//   class MyRandomExpTrafficApp : public Application
//   {
//   public:
//     MyRandomExpTrafficApp();
//     virtual ~MyRandomExpTrafficApp();
//     void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, DataRate dataRate, uint64_t prngRunNumber);
//     Ptr<Socket> GetSocketPtr(void);
//   private:
//     virtual void StartApplication(void);
//     virtual void StopApplication(void);
//     void ScheduleTx(void);
//     void SendPacket(void);
//     Ptr<Socket> m_socketptr;
//     Address m_peeraddress;
//     uint32_t m_packetSize;
//     DataRate m_appDataRate;
//     EventId m_sendEvent;
//     bool m_running;
//     Ptr<ExponentialRandomVariable> m_tNextPtr;
//     //uint64_t m_PRNGRunNumber;
//   };

//   MyRandomExpTrafficApp::MyRandomExpTrafficApp() : m_socketptr(NULL), m_peeraddress(), m_packetSize(0), m_appDataRate(0), m_sendEvent(), m_running(false) {}

//   MyRandomExpTrafficApp::~MyRandomExpTrafficApp() { m_socketptr = NULL; }

//   void MyRandomExpTrafficApp::Setup(Ptr<Socket> socket, Address address, uint32_t packetSize, DataRate dataRate, uint64_t prngRunNumber)
//   {
//     m_socketptr = socket;
//     m_peeraddress = address;
//     m_packetSize = packetSize;
//     m_appDataRate = dataRate;
//     RngSeedManager::SetRun(prngRunNumber);
//     m_tNextPtr = CreateObject<ExponentialRandomVariable>();
//     m_tNextPtr->SetAttribute("Mean", DoubleValue(m_packetSize*8/static_cast<double>(dataRate.GetBitRate())));
//   }

//   Ptr<Socket> MyRandomExpTrafficApp::GetSocketPtr(void)
//   {
//     return m_socketptr;
//   }

//   void MyRandomExpTrafficApp::StartApplication(void)
//   {
//     m_running = true;
//     m_socketptr->Bind();
//     m_socketptr->Connect(m_peeraddress);
//     SendPacket();
//   }

//   void MyRandomExpTrafficApp::StopApplication(void)
//   {
//     m_running = false;
//     if(m_sendEvent.IsRunning())
//       Simulator::Cancel(m_sendEvent);
//     if(m_socketptr)
//       m_socketptr->Close();
//   }

//   void MyRandomExpTrafficApp::SendPacket(void)
//   {
//     Ptr<Packet> packetptr = Create<Packet>(m_packetSize);
//     m_socketptr->Send(packetptr);
//     ScheduleTx();
//   }

//   void MyRandomExpTrafficApp::ScheduleTx()
//   {
//     if(m_running)
//       {
//         Time tNext(Seconds(m_tNextPtr->GetValue()));
//         m_sendEvent = Simulator::Schedule(tNext, &MyRandomExpTrafficApp::SendPacket, this);
//       }
//   }
// }

// #endif
