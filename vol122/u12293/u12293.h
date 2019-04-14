#pragma once

class U12293 {
public:
    static const char* libname()
    {
        return "u12293";
    }
    U12293() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U12293> pu12293;
#endif
