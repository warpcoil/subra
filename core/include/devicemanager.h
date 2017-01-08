#ifndef DEVICEMANAGER
#define DEVICEMANAGER

////A generic device, we collect information about devices from a range of sources
////including (but not limited to) ACPI, EBDA, bus scans/probes etc
////Regardless of what the device is, this contains generics that apply to all devices

////InputDevice isa typeof Device remember class InputDevice : public Device

//class Device {
//    friend class DeviceManager;
//private:
//    Device * next;
//protected:
//    char * Name;
//    uint32_t Port; //Assign Port =< 65525 of Network Access is a requirement, port is NOT 16 bit
//    Device * Link[];
//public:

//} Device;

////Ok, things have been grouped together for simplicity and organisation
////It doesn't have to be specific to PCI, the same structure follows USB or otherwise
////Including on board system devices

//class DeviceManager {

//private:

//    typedef struct DeviceTree {
//        Device * Input; //Keyboard, Mouse, Touchpad, whatever
//        Device * Display; //VGA, VESA, Proprietory etc
//        Device * MassStorage; //Hard Disks, Floppy, USB Mass Storage
//        Device * Peripheral; //Cameras, Scanners, TViVo etc
//        Device * Communication; //Network, via any means, serial, ethernet etc
//        Device * Bridge; //Interconnections -- bridges, buses, hubs
//        Device * Other; //Can't think of anything specific
//    } DeviceTree;

//    static DeviceTree * devices;

//    enum DeviceType {
//        INPUT = 0,
//        DISPLAY = 1,
//        MASSSTORAGE = 2,
//        COMMUNICATION = 3,
//        PERIPHERAL = 4,
//        OTHER = 5,
//        BUS = 255
//    };

//public:

//    void Init() {
//        devices = new DeviceTree;

//        devices->Bridge = new Device();
//        devices->Bridge->Name = "Bridge Devices";
//        devices->Bridge->Port = 65536;

//        devices->Communication = new Device();
//        devices->Communication->Name = "Communication Devices";
//        devices->Bridge->Port = 65537;

//        devices->Display = new Device();
//        devices->Display->Name = "Display Devices";
//        devices->Display->Port = 65538;

//        devices->Input = new Device();
//        devices->MassStorage = new Device();
//        devices->Other = new Device();
//        devices->Peripheral = new Device();

//    }

//    bool RegisterDevice(DeviceType type, char * name, uint32_t port) {

//    }

//};

#endif // DEVICEMANAGER

