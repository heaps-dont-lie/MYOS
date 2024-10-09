#ifndef __VGADRIVER_H
#define __VGADRIVER_H

#include <hwcomms/ports.h>
#include <common/stdio.h>

class VideoGraphicsArrayDriver {
    protected:
        Port8Bit _misc_port;
        Port8Bit _crtcIndex_port;
        Port8Bit _crtcData_port;
        Port8Bit _seqIndex_port;
        Port8Bit _seqData_port;
        Port8Bit _gctrlIndex_port;
        Port8Bit _gctrlData_port;
        Port8Bit _attrctrlIndex_port;
        Port8Bit _attrctrlRead_port;
        Port8Bit _attrctrlWrite_port;
        Port8Bit _attrctrlReset_port;

        void writeRegister(uint8_t* reg);
        uint8_t* getSegmentBaseAddress();   // Video memory is divided in banks.
        
        virtual uint8_t getColorIndexFromRGB(uint8_t red, uint8_t green, uint8_t blue);

    public:
        VideoGraphicsArrayDriver();
        ~VideoGraphicsArrayDriver();

        virtual bool setMode(uint32_t width, uint32_t height, uint32_t colorDepth);
        virtual bool isModeSupported(uint32_t width, uint32_t height, uint32_t colorDepth);
        virtual void putPixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue);
        virtual void putPixel(uint32_t x, uint32_t y, uint8_t colorIndex);
};

#endif