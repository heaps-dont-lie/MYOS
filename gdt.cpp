#include "gdt.h"

/*
*   Initialise the Global descriptor table
*   and all the segment entries and then
*   load the gdt in the lgdt register.
*   Note: lgdt registers expects a 6 byte
*   address.
*/
GlobalDescriptorTable::GlobalDescriptorTable() : 
nullSegment(0,0,0), 
unusedSegment(0,0,0), 
dataSegment(0,64*1024*1024,0x9A), 
codeSegment(0,64*1024*1024,0x92) {
    uint32_t gdtp[2];
    gdtp[0] = (uint32_t)this;
    gdtp[1] = sizeof(GlobalDescriptorTable) << 16;

    //Did not understand completely how we are loading into "lgdt"
    asm volatile("lgdt (%0)": :"p" (((uint8_t *) gdtp) + 2));
}

uint16_t GlobalDescriptorTable::codeSegmentOffset() {
    return (uint8_t*)&codeSegment - (uint8_t*)this;
}


uint16_t GlobalDescriptorTable::dataSegmentOffset() {
    return (uint8_t*)&dataSegment - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flag) {
    uint8_t* target = (uint8_t *)this;

    /* 
    *   If limit is a 16-bit entry then set
    *   the 6th byte accordingly
    */
    if (limit <= 65536)
        target[6] = 0x40;
    else { // Did not understand the code under else statement
        if ((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;
        target[6] = 0xC0;
    }
    
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;
    
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    target[5] = flag;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base() {
    uint8_t* target = (uint8_t*)this;
    uint32_t base = target[7];
    base = (base << 8) + target[4];
    base = (base << 8) + target[3];
    base = (base << 8) + target[2];

    return base;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit() {
    uint8_t* target = (uint8_t*)this;
    uint32_t limit = target[6] & 0xF;
    limit = (limit << 8) + target[1];
    limit = (limit << 8) + target[0];

    if((target[6] & 0xC0) == 0xC0)
        limit = (limit << 12) | 0xFFF;

    return limit;
}