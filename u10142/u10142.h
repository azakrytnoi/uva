#pragma once

class U10142 {
public:
    static const char* libname()
    {
        return "u10142";
    }
    U10142() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10142> pu10142;
#endif
