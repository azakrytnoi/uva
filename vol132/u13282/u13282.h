#pragma once

class U13282 {
public:
    static const char* libname()
    {
        return "u13282";
    }
    U13282() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U13282> pu13282;
#endif
