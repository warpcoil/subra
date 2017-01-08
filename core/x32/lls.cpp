#include "lls.h"
#include "kernel.h"

//#include <stdint.h>

//bool LLS::checkChecksum(void *begin, uint8_t numBytes) {

//    uint32_t sum = 0;
//    char * b = (char *)begin;

//    for (uint8_t i=0; i<numBytes; i++) {
//        sum += b[i];
//    }

//    if ((sum & 0xff) != 0)
//        return false;

//    return true;

//}

//bool LLS::scanRSDP(void *begin, void *end) {

//    for (char * i = (char *)begin; i < (char *)end; i++) {
//        if ((i[0] == 'R') && (i[1] == 'S') && (i[2] == 'D') && (i[3] == ' ') && (i[4] == 'P') && (i[5] == 'T') && (i[6] == 'R')) {
//            message("RSD Found at %x\n\n", i);

//            RSDP = (RSDPv2 *)i;

//            return true;
//        }
//    }

//    return false;
//}

//bool LLS::readRSDP() {

//    uint16_t * ebdaPtr = (uint16_t *)0x040e;

//    ebdaPtr = (uint16_t *)(ebdaPtr[0] << 4);

//    //Scan for "RSD PTR "
//    if (!scanRSDP(ebdaPtr, ebdaPtr + 0x400)) { //Extended BIOS Data Area
//        if (!scanRSDP((void *)0xe0000, (void *)0xfffff)) { //End of BIOS Data Area
//            return false;
//        }
//    }

//    //Checksum Validation
//    return checkChecksum(RSDP, sizeof(RSDPv1));

//}

//bool LLS::readRSDPExtended() {

//    //Read Version 2+ Structure [it's actually read, but not checked]
//    //Checksum Validation Extended (ACPI 2.0 or greater with XSDT support) [not qemu, yet]
//    return checkChecksum(RSDP, sizeof(RSDPv1) + sizeof(RSDPv2));

//}

//bool LLS::checkSignature(char *origin, const char *signature, int checkChars) {

//    for (int i=0; i<checkChars; i++) {
//        if (origin[i] != signature[i])
//            return false;
//    }

//    return true;
//}

//LLS::RSDType LLS::getType(RSD *rsd) {

//    if (checkSignature(rsd->Signature, "APIC", 4))
//        return APIC;

//    if (checkSignature(rsd->Signature, "BERT", 4))
//        return BERT;

//    if (checkSignature(rsd->Signature, "CPEP", 4))
//        return CPEP;

//    if (checkSignature(rsd->Signature, "DSDT", 4))
//        return DSDT;

//    if (checkSignature(rsd->Signature, "ECDT", 4))
//        return ECDT;

//    if (checkSignature(rsd->Signature, "EINJ", 4))
//        return EINJ;

//    if (checkSignature(rsd->Signature, "ERST", 4))
//        return ERST;

//    if (checkSignature(rsd->Signature, "FACP", 4))
//        return FACP;

//    if (checkSignature(rsd->Signature, "FACS", 4))
//        return FACS;

//    if (checkSignature(rsd->Signature, "HEST", 4))
//        return HEST;

//    if (checkSignature(rsd->Signature, "MSCT", 4))
//        return MSCT;

//    if (checkSignature(rsd->Signature, "MPST", 4))
//        return MPST;

//    if (checkSignature(rsd->Signature, "OEMx", 3))
//        return OEMX;

//    if (checkSignature(rsd->Signature, "PMTT", 4))
//        return PMTT;

//    if (checkSignature(rsd->Signature, "PSDT", 4))
//        return PSDT;

//    if (checkSignature(rsd->Signature, "RASF", 4))
//        return RASF;

//    if (checkSignature(rsd->Signature, "RSDT", 4))
//        return ROOT;

//    if (checkSignature(rsd->Signature, "SBST", 4))
//        return SBST;

//    if (checkSignature(rsd->Signature, "SLIT", 4))
//        return SLIT;

//    if (checkSignature(rsd->Signature, "SRAT", 4))
//        return SRAT;

//    if (checkSignature(rsd->Signature, "SSDT", 4))
//        return SSDT;

//    if (checkSignature(rsd->Signature, "XSDT", 4))
//        return XSDT;

//    return UNKN;

//}

//bool LLS::readRSD() {

//    rsd = (RSD *)RSDP->v1.RsdtAddress;

//    //Check the signature
//    RSDType type = getType(rsd);

//    if (type != ROOT)
//        return false;

//    return true;

//}

//void LLS::appendLLS(RSDType type, void *reference) {

//    if (!root) {
//        root = new Descriptor;
//        root->Type = type;
//        root->Reference = reference;
//        root->Next = 0;
//        return;
//    }

//    Descriptor * next = root;

//    while (next->Next)
//        next = next->Next;

//    next->Next = new Descriptor;

//    next = next->Next;

//    next->Type = type;
//    next->Reference = reference;

//}

//void LLS::ReadStructures(DeviceMapper * map) {

//    rsdpRead = readRSDP();

//    if (!rsdpRead)
//    {
//        return;
//    }

//    rsdpExtendedRead = readRSDPExtended();

//    //This is not a failure scenario.
//    //If the rsdp is extended use XSDT, otherwise use FADT
//    if (rsdpExtendedRead) {
//        //XSDT
//        asm volatile("hlt");
//    } else {
//        if (!readRSD())
//            return;

//        //Parse all information and store
//        int numEntries = (rsd->Length - sizeof(RSD)) / 4;

//        message("RSD Entries: %x\n\n", numEntries);

//        RSDT * next = (RSDT *)rsd;

//        int i;

//        for (i=0; i<numEntries; i++) {
//            message("Entry %i: %c%c%c%c", i, next->Rsd.Signature[0], next->Rsd.Signature[1], next->Rsd.Signature[2], next->Rsd.Signature[3]);

//            switch(getType(next)) {
//            case RSDT: //this -> ignored
//                break;
//            case FACP: //Fixed ACPI Description Table (FADT)
//                appendLLS(getType(next), next);
//                break;
//            case FACS: //Ignored
//                break;
//            default:
//                message(" : Unsupported");
//                break;
//            }

//            message("\n\n");

//            next=(RSDT *)next->Next;
//        }


//    }


//    //message("ACPI Version Detected: %x" , x)







//}
