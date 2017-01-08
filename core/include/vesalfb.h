#ifndef VESALFB
#define VESALFB

/* There are multiple vesa standards, this one represents a linear frame buffer [aka LFB] of the vesa standard
 * It is used in virtualbox and qemu and on real hardware
 */

#include "guiprimitive.h"

class VesaLFB : public GUIPrimitive {
private:

    //vesa framebuffer initialisation
    //define type vesa header
    typedef struct VBEVesaInfo
    {
        int8_t VesaSignature[4];
        uint16_t VesaVersion __attribute__((packed));
        uint16_t OEMStringOffset __attribute__((packed));
        uint16_t OEMStringSegment __attribute__((packed));
        uint32_t Capabilities __attribute__((packed));

        uint16_t VideoModesOffset __attribute__((packed));
        uint16_t VideoModesSegment __attribute__((packed));

        uint16_t TotalMemory __attribute__((packed));
        uint16_t OEMSoftwareRevision __attribute__((packed));
        uint16_t OEMVendorNameOffset __attribute__((packed));
        uint16_t OEMVendorNameSegment __attribute__((packed));
        uint16_t OEMProductNameOffset __attribute__((packed));
        uint16_t OEMProductNameSegment __attribute__((packed));
        uint16_t OEMProductRevisionOffset __attribute__((packed));
        uint16_t OEMProductRevisionSegment __attribute__((packed));
        uint8_t Reserved[222];
        uint8_t OEMData[256];
    }VBEVesaInfo;

    //define type vesa modes supported
    typedef struct VBEModeInfo
    {
        uint16_t ModeAttributes __attribute__((packed));
        uint8_t WinAAttributes;
        uint8_t WinBAttributes;
        uint16_t WinGranularity __attribute__((packed));
        uint16_t WinSize __attribute__((packed));
        uint16_t WinASegment __attribute__((packed));
        uint16_t WinBSegment __attribute__((packed));
        uint32_t WinFuncPtr __attribute__((packed));
        uint16_t BytesPerScanLine __attribute__((packed));
        uint16_t XRes __attribute__((packed));
        uint16_t YRes __attribute__((packed));
        uint8_t XCharSize;
        uint8_t YCharSize;
        uint8_t NumberOfPlanes;
        uint8_t BitsPerPixel;
        uint8_t NumberOfBanks;
        uint8_t MemoryModel;
        uint8_t BankSize;
        uint8_t NumberOfImagePages;
        uint8_t res1;
        uint8_t RedMaskSize;
        uint8_t RedFieldPosition;
        uint8_t GreenMaskSize;
        uint8_t GreenFieldPosition;
        uint8_t BlueMaskSize;
        uint8_t BlueFieldPosition;
        uint8_t RsvedMaskSize;
        uint8_t RsvedFieldPosition;
        //VBE 2.0
        uint32_t PhysBasePtr __attribute__((packed));
        uint32_t OffScreenMemOffset __attribute__((packed));
        uint16_t OffScreenMemSize __attribute__((packed));
        //VBE 2.1
        uint16_t LinbytesPerScanLine __attribute__((packed));
        uint8_t BankNumberOfImagePages;
        uint8_t LinNumberOfImagePages;
        uint8_t LinRedMaskSize;
        uint8_t LinRedFieldPosition;
        uint8_t LingreenMaskSize;
        uint8_t LinGreenFieldPosition;
        uint8_t LinBlueMaskSize;
        uint8_t LinBlueFieldPosition;
        uint8_t LinRsvdMaskSize;
        uint8_t LinRsvdFieldPosition;
        uint8_t res2[194];
    }VBEModeInfo;

    bool checkSignature(const char * expected, int8_t * actual);
    VBEModeInfo * getModeInfo(uint16_t mode);
    void setVesaMode(uint16_t mode);

    VBEVesaInfo * vesaInfo;
    VBEModeInfo * vesaModeInfo;
    bool isActive;
    uint16_t resolutionX, resolutionY;
    uint8_t currentDepth;
    uint32_t physBaseAddress;
public:
    bool IsSupported();
    int GetMaximum(bool favourDepth = true);
    void SetResolution(uint16_t mode);

    void DrawPixel(uint16_t fromLeft, uint16_t fromTop, RGB * colour);
    void DrawLine(Rectangle * rectangle, RGB * colour);
    void DrawRectangle(Rectangle * rectangle, RGB * colour);
    void DrawChar(Rectangle * rectangle, Font * font, RGB * colour);

};


#endif // VESALFB

