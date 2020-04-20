#pragma once

class U13203 {
public:
    static const char* libname()
    {
        return "u13203";
    }
    U13203() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U13203> pu13203;
#endif
