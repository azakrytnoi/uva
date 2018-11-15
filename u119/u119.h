#pragma once

class U119 {
public:
    static const char* libname()
    {
        return "u119";
    }
    U119() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U119> pu119;
#endif
