/*
    Author: Aman Pandey
    E-mail: amanpandey1235@gmail.com
*/

#ifndef __GDT_H
#define __GDT_H

#include <common/types.h>

class GlobalDescriptorTable {
    public:
        class SegmentDescriptor {
            private:
                uint16_t limit_lo;
                uint16_t base_lo;
                uint8_t base_hi;
                uint8_t type;
                uint8_t flags_limit_hi;
                uint8_t base_vhi;

            public:
                SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
                uint32_t Base();
                uint32_t Limit();
        } __attribute__((packed));

    private:
        SegmentDescriptor nullSegmentPointer;
        SegmentDescriptor unusedSegmentPointer;
        SegmentDescriptor codeSegmentPointer;
        SegmentDescriptor dataSegmentPointer;
    
    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        uint16_t codeSegmentOffset();
        uint16_t dataSegmentOffset();
};

#endif