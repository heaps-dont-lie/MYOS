#include <drivers/vgaDriver.h>

//Graphics Mode
unsigned char g_320x200x256[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

VideoGraphicsArrayDriver::VideoGraphicsArrayDriver() 
:   _misc_port(0x3C2),
    _crtcIndex_port(0x3D4),
    _crtcData_port(0x3D5),
    _seqIndex_port(0x3C4),
    _seqData_port(0x3C5),
    _gctrlIndex_port(0x3CE),
    _gctrlData_port(0x3CF),
    _attrctrlIndex_port(0x3C0),
    _attrctrlRead_port(0x3C1),
    _attrctrlWrite_port(0x3C0),
    _attrctrlReset_port(0x3DA) 
{

}

VideoGraphicsArrayDriver::~VideoGraphicsArrayDriver() {

}

void VideoGraphicsArrayDriver::writeRegister(uint8_t* reg) {
    _misc_port.Write(*(reg++));

    for(uint8_t i = 0; i < 5; i++) {
        _seqIndex_port.Write(i); // First write at index i
        _seqData_port.Write(*(reg++));
    }

    // Did not understand this part!
    _crtcIndex_port.Write(0x03);
    _crtcData_port.Write(_crtcData_port.Read() | 0x80);
    _crtcIndex_port.Write(0x11);
    _crtcData_port.Write(_crtcData_port.Read() & ~0x80);

    reg[0x03] = reg[0x03] | 0x80;
    reg[0x11] = reg[0x11] & ~0x80;
    for(uint8_t i = 0; i < 25; i++) {
        _crtcIndex_port.Write(i); // First write at index i
        _crtcData_port.Write(*(reg++));
    }

    for(uint8_t i = 0; i < 9; i++) {
        _gctrlIndex_port.Write(i); // First write at index i
        _gctrlData_port.Write(*(reg++));
    }

    for(uint8_t i = 0; i < 21; i++) {
        _attrctrlReset_port.Read(); // First reset before we set data
        _attrctrlIndex_port.Write(i); // First write at index i
        _attrctrlWrite_port.Write(*(reg++));
    }

    //Did not understand
    _attrctrlReset_port.Read();
    _attrctrlIndex_port.Write(0x20);
}

uint8_t* VideoGraphicsArrayDriver::getSegmentBaseAddress() {
    _gctrlIndex_port.Write(0x06);
    uint8_t segmentNumber = (_gctrlData_port.Read() >> 2) & 0x03;

    switch(segmentNumber) {
        default:
        case 0: return (uint8_t*)0x00000;
        case 1: return (uint8_t*)0xA0000;
        case 2: return (uint8_t*)0xB0000;
        case 3: return (uint8_t*)0xB8000;
    }
}
        
void VideoGraphicsArrayDriver::putPixel(uint32_t x, uint32_t y, uint8_t colorIndex) {
    uint8_t* pixelAddress = getSegmentBaseAddress() + 320*y + x;
    *pixelAddress = colorIndex;
}

uint8_t VideoGraphicsArrayDriver::getColorIndexFromRGB(uint8_t red, uint8_t green, uint8_t blue) {
    if(red == 0 && green == 0 && blue == 0xA8)
        return 0x01;
}



bool VideoGraphicsArrayDriver::setMode(uint32_t width, uint32_t height, uint32_t colorDepth) {
    if (!isModeSupported(width, height, colorDepth))
        return false;

    writeRegister(g_320x200x256);
    return true;
}

bool VideoGraphicsArrayDriver::isModeSupported(uint32_t width, uint32_t height, uint32_t colorDepth) {
    return width == 320 && height == 200 && colorDepth == 8;
}

void VideoGraphicsArrayDriver::putPixel(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue) {
    putPixel(x, y, getColorIndexFromRGB(red, green, blue));
}