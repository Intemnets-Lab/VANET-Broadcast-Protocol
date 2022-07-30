
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
    //m_socketptr->SetAllowBroadcast(true);
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
