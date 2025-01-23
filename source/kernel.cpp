//most (if not all) of this is taken straight from the WUMS loader source all credit goes to the devs for that

#include <coreinit/cache.h>
#include <coreinit/memorymap.h>
extern "C" void SC_KernelCopyData(uint32_t addr, uint32_t src, uint32_t len);

void KernelWriteU32(uint32_t addr, uint32_t value) {

    ICInvalidateRange(&value, 4);
    DCFlushRange(&value, 4);

    auto dst = (uint32_t) OSEffectiveToPhysical(addr);
    auto src = (uint32_t) OSEffectiveToPhysical((uint32_t) &value);

    SC_KernelCopyData(dst, src, 4);

    DCFlushRange((void *) addr, 4);
    ICInvalidateRange((void *) addr, 4);
}

