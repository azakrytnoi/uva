#pragma once

#include <cstdint>
#include <functional>
class UVA_API_EXPORT U100
{
public:
    U100();
    ~U100();
    void operator()();

private:
    uint32_t cycle_length(uint32_t start, uint32_t end, std::function<uint32_t(uint32_t)> f);
};

