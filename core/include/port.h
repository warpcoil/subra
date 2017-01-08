#ifndef PORT
#define PORT

class Port {

private:
    typedef struct PortItem {
        uint32_t PortNum;
        void * Func;
        PortItem * Next;
    } PortItem;

    typedef struct PortLink {
        uint32_t PortNum;
        uint32_t PortReal;
        PortLink * Next;
    } PortLink;

public:

    //Ports are 16 bit in nature!
    //So we map the higher 16 bits to addressable devices
    //A port link could be created (via firewall) from x to higher device x
    //It's much more direct and network aware

    uint32_t AssignDynamicPort(bool IANACompatible = false) {
        //Devices are recommended to be mapped to a higher port number

    }

};

#endif // PORT

