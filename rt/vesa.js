
VBEVesaInfo = {
    VesaSignature: { int8_t: 4 },
    VesaVersion: { uint16_t: 1 },
    OEMStringOffset: { uint16_t: 1 },
    OEMStringSegment: { uint16_t: 1 },
    Capabilities: { uint16_t: 1 },
    VideoModesOffset: { uint16_t: 1 },
    VideoModesSegment: { uint16_t: 1 },
    TotalMemory: { uint16_t: 1 },
    OEMSoftwareRevision: { uint16_t: 1 },
    OEMVendorNameOffset: { uint16_t: 1 },
    OEMVendorNameSegment: { uint16_t: 1 },
    OEMProductNameOffset: { uint16_t: 1 },
    OEMProductNameSegment: { uint16_t: 1 },
    OEMProductRevisionOffset: { uint16_t: 1 },
    OEMProductRevisionSegment: { uint16_t: 1 },
    Reserved: { uint8_t: 222 },
    OEMData: { uint8_t: 256 }
};

VBEModeInfo = {
    ModeAttributes: { uint16_t: 1 },
    WinAAttributes: { uint8_t: 1 },
    WinBAttributes: { uint8_t: 1 },
    WinGranularity: { uint16_t: 1 },
    WinSize: { uint16_t: 1 },
    WinASegment: { uint16_t: 1 },
    WinBSegment: { uint16_t: 1 },
    WinFuncPtr: { uint32_t: 1 },
    BytesPerScanLine: { uint16_t: 1 },
    XRes: { uint16_t: 1 },
    YRes: { uint16_t: 1 },
    XCharSize: { uint8_t: 1 },
    YCharSize: { uint8_t: 1 },
    NumberOfPlanes: { uint8_t: 1 },
    BitsPerPixel: { uint8_t: 1 },
    NumberOfBanks: { uint8_t: 1 },
    MemoryModel: { uint8_t: 1 },
    BankSize: { uint8_t: 1 },
    NumberOfImagePages: { uint8_t: 1 },
    Res1: { uint8_t: 1 },
    RedMaskSize: { uint8_t: 1 },
    RedFieldPosition: { uint8_t: 1 },
    GreenMaskSize: { uint8_t: 1 },
    GreenFieldPosition: { uint8_t: 1 },
    BlueMaskSize: { uint8_t: 1 },
    BlueFieldPosition: { uint8_t: 1 },
    RsvedMaskSize: { uint8_t: 1 },
    RsvedFieldPosition: { uint8_t: 1 },
    //VBE 2.0
    PhysBasePtr: { uint32_t: 1 },
    OffScreenMemOffset: { uint32_t: 1 },
    OffScreenMemSize: { uint16_t: 1 },
    //VBE 2.1
    LinbytesPerScanLine: { uint16_t: 1 },
    BankNumberOfImagePages: { uint8_t: 1 },
    LinNumberOfImagePages: { uint8_t: 1 },
    LinRedMaskSize: { uint8_t: 1 },
    LinRedFieldPosition: { uint8_t: 1 },
    LingreenMaskSize: { uint8_t: 1 },
    LinGreenFieldPosition: { uint8_t: 1 },
    LinBlueMaskSize: { uint8_t: 1 },
    LinBlueFieldPosition: { uint8_t: 1 },
    LinRsvdMaskSize: { uint8_t: 1 },
    LinRsvdFieldPosition: { uint8_t: 1 },
    Res2: { uint8_t: 194 }
};

function func() {

}
