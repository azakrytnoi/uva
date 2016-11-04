#pragma once

class U10089 {
public:
    static const char* libname()
    {
        return "u10089";
    }
    U10089() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10089> pu10089;
#endif
