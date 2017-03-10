#pragma once

#include <cstdint>
#include <functional>
class U100 {
public:
    static const char* libname()
    {
        return "u100";
    }
    U100();
    void operator()() const;

private:
    static uint32_t cycle_length(uint32_t start, uint32_t end, std::function<uint32_t(uint32_t)> f);
};

#ifdef POPULATE_CACHE
populate <U100> pu100;
#endif

