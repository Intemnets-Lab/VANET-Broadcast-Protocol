/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


// Class declaration of an application that transmits UDP packets at random times.
// The time between consecutive packets is exponentially distributed.

#ifndef VBPAPP_H
#define VBPAPP_H

#include "ns3/random-variable-stream.h"
#include "ns3/applications-module.h"
#include "vbp-data-packet-header.h"

namespace ns3
{
  class VbpApp : public Application
  {
  public:
    VbpApp();
    virtual ~VbpApp();
    void Setup (Ptr<Socket> socket, uint16_t portNum);
  private:
    virtual void StartApplication(void);
    virtual void StopApplication(void);
    Ptr<Socket> m_socketptr;
    uint16_t m_portNum;
    bool m_running;
  };

  VbpApp::VbpApp() : m_socketptr(NULL), m_portNum(0), m_running(false) {}

  VbpApp::~VbpApp() { m_socketptr = NULL; }

  void VbpApp::Setup(Ptr<Socket> socket, uint16_t portNum)
  {
    m_socketptr = socket;
    m_portNum = portNum;
  }

  void VbpApp::StartApplication(void)
  {
    m_running = true;
    // set socket to broadcast to all neighbors
    m_socketptr->Bind(InetSocketAddress(Ipv4Address::GetAny(), m_portNum));
    m_socketptr->SetAllowBroadcast(true);
    m_socketptr->Connect(InetSocketAddress(Ipv4Address("255.255.255.255"), m_portNum));
  }

  void VbpApp::StopApplication(void)
  {
    m_running = false;
    if(m_socketptr)
      m_socketptr->Close();
  }

}

#endif
