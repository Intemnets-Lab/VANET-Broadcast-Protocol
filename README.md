# VANET Broadcast Protocol
 VBP

VANET Broadcast Protocol (VBP) is a multi-hop routing protocol made for vehicular communications. It is built on top of an open source network simulator tool called [ns-3](https://www.nsnam.org/about/).

### Installation

```bash
git clone https://github.com/wmbjo/VANET-Broadcast-Protocol.git
cd ns-allinone-3.35
./build.py
```

VBP source files and examples can be found at
```bash
/ns-allinone-3.35/ns-3.35/scratch/vanet-broadcast-protocol
```

VBP can be added to simulation scripts in a similar manner to other routing protocols.
Users must set the broadcast area after calling '''VanetBroadcastHelper'''.

```c++
    InternetStackHelper stack;
    VanetBroadcastHelper vbp; //VanetBroadcastHelper
    vbp.SetBroadcastArea({100000, -10, 100050, 10});
    stack.SetRoutingHelper(vbp);
    stack.Install(nodes);
```

The broadcast area is defined as [insert image here]

