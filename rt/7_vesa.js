
/*@include "baseTypes.js"*/

var VBEVesaInfo = [
                { type: int8_t, amount: 4, name: "VesaSignature" },
                { type: uint16_t, amount: 1, name: "VesaVersion" },
                { type: uint16_t, amount: 1, name: "OEMStringOffset" },
                { type: uint16_t, amount: 1, name: "OEMStringSegment" },
                { type: uint16_t, amount: 1, name: "Capabilities" },
                { type: uint16_t, amount: 1, name: "VideoModesOffset" },
                { type: uint16_t, amount: 1, name: "VideoModesSegment" },
                { type: uint16_t, amount: 1, name: "CountMemory" },
                { type: uint16_t, amount: 1, name: "OEMSoftwareRevision" },
                { type: uint16_t, amount: 1, name: "OEMVendorNameOffset" },
                { type: uint16_t, amount: 1, name: "OEMVendorNameSegment" },
                { type: uint16_t, amount: 1, name: "OEMProductNameOffset" },
                { type: uint16_t, amount: 1, name: "OEMProductNameSegment" },
                { type: uint16_t, amount: 1, name: "OEMProductRevisionOffset" },
                { type: uint16_t, amount: 1, name: "OEMProductRevisionSegment" },
                /*{ type: uint8_t, amount: 222, name: "Reserved" },
                { type: uint8_t, amount: 256, name: "OEMData" }*/
            ];

var VBEModeInfo = [
            { type: uint16_t, amount: 1, name: "ModeAttributes" },
            { type: uint8_t, amount: 1, name: "WinAAttributes" },
            { type: uint8_t, amount: 1, name: "WinBAttributes" },
            { type: uint16_t, amount: 1, name: "WinGranularity" },
            { type: uint16_t, amount: 1, name: "WinSize" },
            { type: uint16_t, amount: 1, name: "WinASegment" },
            { type: uint16_t, amount: 1, name: "WinBSegment" },
            { type: uint32_t, amount: 1, name: "WinFuncPtr" },
            { type: uint16_t, amount: 1, name: "BytesPerScanLine" },
            { type: uint16_t, amount: 1, name: "XRes" },
            { type: uint16_t, amount: 1, name: "YRes" },
            { type: uint8_t, amount: 1, name: "XCharSize" },
            { type: uint8_t, amount: 1, name: "YCharSize" },
            { type: uint8_t, amount: 1, name: "NumberOfPlanes" },
            { type: uint8_t, amount: 1, name: "BitsPerPixel" },
            { type: uint8_t, amount: 1, name: "NumberOfBanks" },
            { type: uint8_t, amount: 1, name: "MemoryModel" },
            { type: uint8_t, amount: 1, name: "BankSize" },
            { type: uint8_t, amount: 1, name: "NumberOfImagePages" },
            { type: uint8_t, amount: 1, name: "Res1" },
            { type: uint8_t, amount: 1, name: "RedMaskSize" },
            { type: uint8_t, amount: 1, name: "RedFieldPosition" },
            { type: uint8_t, amount: 1, name: "GreenMaskSize" },
            { type: uint8_t, amount: 1, name: "GreenFieldPosition" },
            { type: uint8_t, amount: 1, name: "BlueMaskSize" },
            { type: uint8_t, amount: 1, name: "BlueFieldPosition" },
            { type: uint8_t, amount: 1, name: "RsvedMaskSize" },
            { type: uint8_t, amount: 1, name: "RsvedFieldPosition" },
            //VBE 2.0
            { type: uint32_t, amount: 1, name: "PhysBasePtr" },
            { type: uint32_t, amount: 1, name: "OffScreenMemOffset" },
            { type: uint16_t, amount: 1, name: "OffScreenMemSize" },
            //VBE 2.1
            { type: uint16_t, amount: 1, name: "LinbytesPerScanLine" },
            { type: uint8_t, amount: 1, name: "BankNumberOfImagePages" },
            { type: uint8_t, amount: 1, name: "LinNumberOfImagePages" },
            { type: uint8_t, amount: 1, name: "LinRedMaskSize" },
            { type: uint8_t, amount: 1, name: "LinRedFieldPosition" },
            { type: uint8_t, amount: 1, name: "LingreenMaskSize" },
            { type: uint8_t, amount: 1, name: "LinGreenFieldPosition" },
            { type: uint8_t, amount: 1, name: "LinBlueMaskSize" },
            { type: uint8_t, amount: 1, name: "LinBlueFieldPosition" },
            { type: uint8_t, amount: 1, name: "LinRsvdMaskSize" },
            { type: uint8_t, amount: 1, name: "LinRsvdFieldPosition" },
            { type: uint8_t, amount: 194, name: "Res2" }
        ];

//Check for VESA support
var szVesaInfo = sizeof(VBEVesaInfo);

var x = {
    data: [
        {
            VesaSignature: "VESA",
            VesaVersion: 1,
            OEMStringOffset: 1,
            OEMStringSegment: 1,
            Capabilities: 1,
            VideoModesOffset: 1,
            VideoModesSegment: 1,
            CountMemory: 1,
            OEMSoftwareRevision: 1,
            OEMVendorNameOffset: 1,
            OEMVendorNameSegment: 1,
            OEMProductNameOffset: 1,
            OEMProductNameSegment: 1,
            OEMProductRevisionOffset: 1,
            OEMProductRevisionSegment: 10,
        }
    ],
    where: null
};

//Serialise
var ptr = serialise(VBEVesaInfo, x);

var result = deserialise(VBEVesaInfo, ptr, 1);

result.data[0].VesaSignature = arrayToString(result.data[0].VesaSignature);

message("Sign:::%s\n", result.data[0].VesaSignature);