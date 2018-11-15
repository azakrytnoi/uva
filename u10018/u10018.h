#pragma once

class U10018 {
public:
    static const char* libname()
    {
        return "u10018";
    }
    U10018() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10018> pu10018;
#endif
