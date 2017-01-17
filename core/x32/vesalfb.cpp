#include "vesalfb.h"
#include "kernel.h"

void emulateInt(uint32_t intnum, struct registers r) {
    TSS *vm86_tss;
    DWORD *bos;
    DWORD isr_cs, isr_eip;
    WORD *old_esp;
    DWORD *IRQTableEntry;
    CONTEXT c = get_TR();

    vm86_tss = vm86_get_tss();
    bos = (DWORD *)vm86_tss->esp0;
    if (c == X_VM86_TSS)
    {
            old_esp = (WORD *)(*(bos - 6) + (*(bos - 5) << 4));
            r.flags = CPU_FLAG_VM | CPU_FLAG_IOPL;
            *(old_esp - 2) = (WORD)(*(bos - 8));
            *(old_esp - 3) = (WORD)(*(bos - 9));
            *(bos - 6) -= 6;

            IRQTableEntry = (DWORD *)(0L);
            isr_cs = ((IRQTableEntry[intnum]) & 0xffff0000) >> 16;
            isr_eip = ((IRQTableEntry[intnum]) & 0x0000ffff);

            *(bos - 8) = isr_cs;
            *(bos - 9) = isr_eip;
    }
}

bool VesaLFB::checkSignature(const char * expected, int8_t * actual) {

    for (int i=0; i<4; i++)
    {
        if ((int8_t)expected[i] != actual[i])
        {
            return false;
        }
    }
    return true;

}

VesaLFB::VBEModeInfo * VesaLFB::getModeInfo(uint16_t mode) {
    X_REGS16 ir, rr;
    X_SREGS16 sr;

    //Get video mode
    ir.x.ax = 0x4f01;
    ir.x.cx = mode;
    ir.x.di=0;
    sr.ds=0x60;
    sr.es=0x60;
    vm86_callBIOS (0x10, &ir, &rr, &sr);

    int linear = (0x60 * 16) + ir.x.di;
    return (VBEModeInfo *)linear;
}

bool VesaLFB::IsSupported() {

    //Check vesa linear framebuffer support
    vm86_init();

    l1_int_bind(0x6d, (void *)emulateInt);

    X_REGS16 ir, rr;
    X_SREGS16 sr;

    //Get video mode
    ir.x.ax = 0x4f00;
    ir.x.di=0;
    sr.ds=0x3a;
    sr.es=0x3a;
    vm86_callBIOS (0x10, &ir, &rr, &sr);

    long linear = (0x3a * 16) + ir.x.di;
    vesaInfo = (VBEVesaInfo *)linear;

    return checkSignature("VESA", vesaInfo->VesaSignature);

}

int VesaLFB::GetMaximum(bool favourDepth) {

    if (!IsSupported()) { //Always check first
        return GUIDRIVERUNSUPPORTED;
    }

    uint16_t maxX = 0, maxY = 0, minDepth = 0, favourResMode = 0; //favour resolution
    uint16_t minX = 0, minY = 0, maxDepth = 0, favourDepthMode = 0; //favour depth

    unsigned short * modes = (unsigned short *)((vesaInfo->VideoModesSegment * 0x10) + vesaInfo->VideoModesOffset);

    for (int i=0; modes[i] != 0xffff; i++)
    {
        VBEModeInfo * modeInfo = getModeInfo(modes[i]);

        if (modeInfo->ModeAttributes & (1 << 7)) { //Check LFB Flag

            //Favour resolution over depth?
            if ((modeInfo->XRes >= maxX) && (modeInfo->YRes >= maxY)) {
                maxX = modeInfo->XRes;
                maxY = modeInfo->YRes;
                minDepth = modeInfo->BitsPerPixel;
                favourResMode = modes[i];
            }

            //Favour depth over resolution?
            if (modeInfo->BitsPerPixel == maxDepth) {

                if ((modeInfo->XRes >= minX) && (modeInfo->YRes >= minY)) {
                    minX = modeInfo->XRes;
                    minY = modeInfo->YRes;
                    maxDepth = modeInfo->BitsPerPixel;
                    favourDepthMode = modes[i];
                }

            } else if (modeInfo->BitsPerPixel > maxDepth) {
                minX = modeInfo->XRes;
                minY = modeInfo->YRes;
                maxDepth = modeInfo->BitsPerPixel;
                favourDepthMode = modes[i];
            }

        }

    }

    if (favourDepth) {
        message("VESA best depth mode (%i x %i) x %i bpp\n\n", minX, minY, maxDepth);
        return favourDepthMode;
    }

    message("VESA best res mode (%i x %i) x %i bpp\n\n", maxX, maxY, minDepth);
    return favourResMode;

}

void VesaLFB::setVesaMode(uint16_t mode) {

    vm86_init ();
    X_REGS16 ir, rr;
    X_SREGS16 sr;

    ir.x.ax = 0x4f02;
    ir.x.bx = mode;
    vm86_callBIOS (0x10, &ir, &rr, &sr);

}

void VesaLFB::SetResolution(uint16_t mode) {

    //Refresh VESA Info Block
    IsSupported();

    message("%s\n", (char *)((vesaInfo->OEMVendorNameSegment * 16) + vesaInfo->OEMVendorNameOffset));
    message("%s\n", (char *)((vesaInfo->OEMProductNameSegment * 16) + vesaInfo->OEMProductNameOffset));
    message("%s\n\n", (char *)((vesaInfo->OEMStringSegment * 16) + vesaInfo->OEMStringOffset));

    vesaModeInfo = getModeInfo(mode);
    physBaseAddress = vesaModeInfo->PhysBasePtr;

    setVesaMode(mode | 0x4000);
    setVesaMode(mode | 0x4000); //Double push, #cough# qemu
    isActive = true;

}

void VesaLFB::DrawPixel(uint16_t fromLeft, uint16_t fromTop, RGB * colour) {

    unsigned char * offset;
    //Offset = (Y * ScreenWidth) + X;
    //unsigned char * pixel = physBaseAddress;
    unsigned char bytesPerPixel = vesaModeInfo->BitsPerPixel / 8;

    //Offset = Pixel + (384 * (3 * 1024)) + (512 * 3); //Prints a dot in the middle (1024 res)
    offset = (unsigned char *)(physBaseAddress + (fromTop * (bytesPerPixel * vesaModeInfo->XRes)) + (fromLeft * 3));// + (fromLeft * 4);

    * offset = colour->Blue;
    offset++;
    * offset = colour->Green;
    offset++;
    * offset = colour->Red;
    offset++;

}

void VesaLFB::DrawLine(Rectangle * rectangle, RGB * colour) {

    rectangle = rectangle;
    colour = colour;
    message("Not Yet Supported");

}

void VesaLFB::DrawRectangle(Rectangle * rectangle, RGB * colour) {

    rectangle = rectangle;
    colour = colour;
    message("Not Yet Supported");

}

void VesaLFB::DrawChar(Rectangle * rectangle, Font * font, RGB * colour) {

}
