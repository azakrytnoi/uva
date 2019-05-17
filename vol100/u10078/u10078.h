#pragma once

class U10078 {
public:
    static const char* libname()
    {
        return "u10078";
    }
    U10078() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10078> pu10078;
#endif
