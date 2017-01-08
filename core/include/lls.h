#ifndef LLS_H
#define LLS_H

//Low Level Structures [of which there are many]

//BDS - Bios Data Area
//EBDA - Extended Bios Data Area
//RSDP - Root System Descriptor Pointer
//ACPI - Advanced Configuration and Power Management Interface
//#include <stdint.h>

//class LLS {

//protected:
//    typedef struct RSDPv1 {
//        char Signature[8];
//        uint8_t Checksum;
//        char OEMID[6];
//        uint8_t Revision;
//        uint32_t RsdtAddress;
//    } RSDPv1;

//    typedef struct RSDPv2 {
//        RSDPv1 v1;
//        uint32_t Length;
//        uint64_t XsdtAddress;
//        uint8_t ExtendedChecksum;
//        uint8_t reserved[3];
//    } RSDPv2;

//    typedef struct RSD { //The base structure
//      char Signature[4];
//      uint32_t Length;
//      uint8_t Revision;
//      uint8_t Checksum;
//      char OEMID[6];
//      char OEMTableID[8];
//      uint32_t OEMRevision;
//      uint32_t CreatorID;
//      uint32_t CreatorRevision;
//    } RSD;

//    typedef struct RSDT {
//        RSD Rsd;
//        uint32_t Next;
//    } RSDT;

//    typedef struct GenericAddressStructure
//    {
//      uint8_t AddressSpace;
//      uint8_t BitWidth;
//      uint8_t BitOffset;
//      uint8_t AccessSize;
//      uint64_t Address;
//    } GenericAddressStructure;

//    typedef struct FADTv1
//    {
//        RSD Rsd;
//        uint32_t FirmwareCtrl;
//        uint32_t Dsdt;

//        // field used in ACPI 1.0; no longer in use, for compatibility only
//        uint8_t  Reserved;

//        uint8_t  PreferredPowerManagementProfile;
//        uint16_t SCI_Interrupt;
//        uint32_t SMI_CommandPort;
//        uint8_t  AcpiEnable;
//        uint8_t  AcpiDisable;
//        uint8_t  S4BIOS_REQ;
//        uint8_t  PSTATE_Control;
//        uint32_t PM1aEventBlock;
//        uint32_t PM1bEventBlock;
//        uint32_t PM1aControlBlock;
//        uint32_t PM1bControlBlock;
//        uint32_t PM2ControlBlock;
//        uint32_t PMTimerBlock;
//        uint32_t GPE0Block;
//        uint32_t GPE1Block;
//        uint8_t  PM1EventLength;
//        uint8_t  PM1ControlLength;
//        uint8_t  PM2ControlLength;
//        uint8_t  PMTimerLength;
//        uint8_t  GPE0Length;
//        uint8_t  GPE1Length;
//        uint8_t  GPE1Base;
//        uint8_t  CStateControl;
//        uint16_t WorstC2Latency;
//        uint16_t WorstC3Latency;
//        uint16_t FlushSize;
//        uint16_t FlushStride;
//        uint8_t  DutyOffset;
//        uint8_t  DutyWidth;
//        uint8_t  DayAlarm;
//        uint8_t  MonthAlarm;
//        uint8_t  Century;

//        // reserved in ACPI 1.0; used since ACPI 2.0+
//        uint16_t BootArchitectureFlags;

//        uint8_t  Reserved2;
//        uint32_t Flags;

//        // 12 byte structure; see below for details
//        GenericAddressStructure ResetReg;

//        uint8_t  ResetValue;
//        uint8_t  Reserved3[3];

//        // 64bit pointers - Available on ACPI 2.0+
//        uint64_t                X_FirmwareControl;
//        uint64_t                X_Dsdt;

//        GenericAddressStructure X_PM1aEventBlock;
//        GenericAddressStructure X_PM1bEventBlock;
//        GenericAddressStructure X_PM1aControlBlock;
//        GenericAddressStructure X_PM1bControlBlock;
//        GenericAddressStructure X_PM2ControlBlock;
//        GenericAddressStructure X_PMTimerBlock;
//        GenericAddressStructure X_GPE0Block;
//        GenericAddressStructure X_GPE1Block;
//    } FADTv1;

//    enum RSDType {
//        APIC = 0, //Multiple APIC Description Table (MADT)
//        BERT = 1, //Boot Error Record Table (BERT)
//        CPEP = 2, //Corrected Platform Error Polling Table (CPEP)
//        DSDT = 3, //Differentiated System Description Table (DSDT)
//        ECDT = 4, //Embedded Controller Boot Resources Table (ECDT)
//        EINJ = 5, //Error Injection Table (EINJ)
//        ERST = 6, //Error Record Serialization Table (ERST)
//        FACP = 7, //Fixed ACPI Description Table (FADT)
//        FACS = 8, //Firmware ACPPI Control Structure (FACS)
//        HEST = 9, //Hardware Error Source Table (HEST)
//        MSCT = 10, //Maximum System Characteristics Table (MSCT)
//        MPST = 11, //Memory Power State Table (MPST)
//        OEMX = 12, //OEM Specific Information Tables (Any table with a signature beginning with "OEM" falls into this definition)
//        PMTT = 13, //Platform Memory Topology Table (PMTT)
//        PSDT = 14, //Persistent System Description Table (PSDT)
//        RASF = 15, //ACPI RAS FeatureTable (RASF)
//        ROOT = 16, //Root System Description Table (RSDT)
//        SBST = 17, //Smart Battery Specification Table (SBST)
//        SLIT = 18, //System Locality System Information Table (SLIT)
//        SRAT = 19, //System Resource Affinity Table (SRAT)
//        SSDT = 20, //Secondary System Description Table (SSDT)
//        XSDT = 21, //Extended System Description Table (XSDT; 64-bit version of the RSDT)

//        UNKN = 255 //Unknown
//    };

//    RSDPv2 * RSDP;
//    RSD * rsd;

//    typedef struct Descriptor {
//        RSDType Type;
//        void * Reference;
//        Descriptor * Next;
//    } Descriptor;

//    static Descriptor * root;

//private:

//    //Checksum calculation
//    bool checkChecksum(void * begin, uint8_t numBytes);

//    //Relates to the RSD Pointer Table
//    bool scanRSDP(void * begin, void * end);
//    bool readRSDP();
//    bool readRSDPExtended();

//    //Checks the signature of RSD Tables and returns a number
//    bool checkSignature(char * origin, const char * signature, int checkChars);
//    RSDType getType(RSD * rsd);
//    bool readRSD();

//    //Get descriptors/tables and append
//    void appendLLS(RSDType type, void * reference);

//    //bool readXSDT();

//    //bool readACPI();

//    //bool bdsRead;
//    //bool ebdaRead;

//    bool rsdpRead;
//    bool rsdpExtendedRead;

//    bool acpiRead;

//public:
//    void ReadStructures(DeviceMapper *map);

//};

#endif // LLS
