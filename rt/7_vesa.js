/// <reference path="9_base_types.js" />

var VBEVesaInfo = [
                { type: baseTypes.int8_t, amount: 4, name: "VesaSignature" },
                { type: baseTypes.uint16_t, amount: 1, name: "VesaVersion" },
                { type: baseTypes.uint16_t, amount: 1, name: "OEMStringOffset" },
                { type: baseTypes.uint16_t, amount: 1, name: "OEMStringSegment" },
                { type: baseTypes.uint16_t, amount: 1, name: "Capabilities" },
                { type: baseTypes.uint16_t, amount: 1, name: "VideoModesOffset" },
                { type: baseTypes.uint16_t, amount: 1, name: "VideoModesSegment" },
                { type: baseTypes.uint16_t, amount: 1, name: "CountMemory" },
                { type: baseTypes.uint16_t, amount: 1, name: "OEMSoftwareRevision" },
                { type: baseTypes.uint16_t, amount: 1, name: "OEMVendorNameOffset" },
                { type: baseTypes.uint16_t, amount: 1, name: "OEMVendorNameSegment" },
                { type: baseTypes.uint16_t, amount: 1, name: "OEMProductNameOffset" },
                { type: baseTypes.uint16_t, amount: 1, name: "OEMProductNameSegment" },
                { type: baseTypes.uint16_t, amount: 1, name: "OEMProductRevisionOffset" },
                { type: baseTypes.uint16_t, amount: 1, name: "OEMProductRevisionSegment" },
                { type: baseTypes.uint8_t, amount: 222, name: "Reserved" },
                { type: baseTypes.uint8_t, amount: 256, name: "OEMData" }
            ];

var VBEModeInfo = [
            { type: baseTypes.uint16_t, amount: 1, name: "ModeAttributes" },
            { type: baseTypes.uint8_t, amount: 1, name: "WinAAttributes" },
            { type: baseTypes.uint8_t, amount: 1, name: "WinBAttributes" },
            { type: baseTypes.uint16_t, amount: 1, name: "WinGranularity" },
            { type: baseTypes.uint16_t, amount: 1, name: "WinSize" },
            { type: baseTypes.uint16_t, amount: 1, name: "WinASegment" },
            { type: baseTypes.uint16_t, amount: 1, name: "WinBSegment" },
            { type: baseTypes.uint32_t, amount: 1, name: "WinFuncPtr" },
            { type: baseTypes.uint16_t, amount: 1, name: "BytesPerScanLine" },
            { type: baseTypes.uint16_t, amount: 1, name: "XRes" },
            { type: baseTypes.uint16_t, amount: 1, name: "YRes" },
            { type: baseTypes.uint8_t, amount: 1, name: "XCharSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "YCharSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "NumberOfPlanes" },
            { type: baseTypes.uint8_t, amount: 1, name: "BitsPerPixel" },
            { type: baseTypes.uint8_t, amount: 1, name: "NumberOfBanks" },
            { type: baseTypes.uint8_t, amount: 1, name: "MemoryModel" },
            { type: baseTypes.uint8_t, amount: 1, name: "BankSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "NumberOfImagePages" },
            { type: baseTypes.uint8_t, amount: 1, name: "Res1" },
            { type: baseTypes.uint8_t, amount: 1, name: "RedMaskSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "RedFieldPosition" },
            { type: baseTypes.uint8_t, amount: 1, name: "GreenMaskSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "GreenFieldPosition" },
            { type: baseTypes.uint8_t, amount: 1, name: "BlueMaskSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "BlueFieldPosition" },
            { type: baseTypes.uint8_t, amount: 1, name: "RsvedMaskSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "RsvedFieldPosition" },
            //VBE 2.0
            { type: baseTypes.uint32_t, amount: 1, name: "PhysBasePtr" },
            { type: baseTypes.uint32_t, amount: 1, name: "OffScreenMemOffset" },
            { type: baseTypes.uint16_t, amount: 1, name: "OffScreenMemSize" },
            //VBE 2.1
            { type: baseTypes.uint16_t, amount: 1, name: "LinbytesPerScanLine" },
            { type: baseTypes.uint8_t, amount: 1, name: "BankNumberOfImagePages" },
            { type: baseTypes.uint8_t, amount: 1, name: "LinNumberOfImagePages" },
            { type: baseTypes.uint8_t, amount: 1, name: "LinRedMaskSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "LinRedFieldPosition" },
            { type: baseTypes.uint8_t, amount: 1, name: "LingreenMaskSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "LinGreenFieldPosition" },
            { type: baseTypes.uint8_t, amount: 1, name: "LinBlueMaskSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "LinBlueFieldPosition" },
            { type: baseTypes.uint8_t, amount: 1, name: "LinRsvdMaskSize" },
            { type: baseTypes.uint8_t, amount: 1, name: "LinRsvdFieldPosition" },
            { type: baseTypes.uint8_t, amount: 194, name: "Res2" }
];

//Get video mode
vm86_set_register_value(vm86Registers.ax, 0x4f00);
vm86_set_register_value(vm86Registers.di, 0);
vm86_set_register_value(vm86Registers.ds, 0x3a);
vm86_set_register_value(vm86Registers.es, 0x3a);
vm86_call_bios(0x6d, 0x10);

var linear = (0x3a * 16) + vm86_get_register_value(vm86Registers.di);

var vesaInfo = deserialise(VBEVesaInfo, linear, 1);

vesaInfo.data[0].VesaSignature = arrayToString(vesaInfo.data[0].VesaSignature);

message("Signature:::%s\n", vesaInfo.data[0].VesaSignature);



//Check for VESA support -- just serialisation/deserialisation test code, which works
//var szVesaInfo = sizeof(VBEVesaInfo);

//var x = {
//    data: [
//        {
//            VesaSignature: "VESA",
//            VesaVersion: 1,
//            OEMStringOffset: 1,
//            OEMStringSegment: 1,
//            Capabilities: 1,
//            VideoModesOffset: 1,
//            VideoModesSegment: 1,
//            CountMemory: 1,
//            OEMSoftwareRevision: 1,
//            OEMVendorNameOffset: 1,
//            OEMVendorNameSegment: 1,
//            OEMProductNameOffset: 1,
//            OEMProductNameSegment: 1,
//            OEMProductRevisionOffset: 1,
//            OEMProductRevisionSegment: 10,
//        }
//    ],
//    where: null
//};

////Serialise
//var ptr = serialise(VBEVesaInfo, x);

//var result = deserialise(VBEVesaInfo, ptr, 1);

//result.data[0].VesaSignature = arrayToString(result.data[0].VesaSignature);

//message("Sign:::%s\n", result.data[0].VesaSignature);
