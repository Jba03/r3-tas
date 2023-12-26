#ifndef stRandom_h
#define stRandom_h

#include <algorithm>

#define RND_INDEX_DEFAULT   0

struct stRandom {
    readonly uint32 tableSize;
    readonly uint32 tableIndices[50];
    readonly uint32 lastIndex;
    readonly uint32 tableMax;
    readonly float32 tableMaxInverse;
    readonly pointer<uint32_t> table;
    
    auto index(unsigned i) -> int32_t {
        if (table.offset() == 0) return 0;
        return (table[std::max(0u, i)] >> 16) & 0x7FFF;
    }
    
    auto indexRelative(unsigned idx, int offset) -> int32_t {
        if (table.offset() == 0) return 0;
        return index(tableIndices[idx] + offset);
    }

    auto call(unsigned n_calls, unsigned idx, unsigned mi, unsigned ma) -> int32_t {
        int32_t n, v = 0;
        for (n = 0; n < n_calls; n++)
            v = (mi + ((ma + 1 - mi) * indexRelative(idx, n)) / (tableMax + 1));
        return v;
    }
};

#endif /* stRandom_h */
