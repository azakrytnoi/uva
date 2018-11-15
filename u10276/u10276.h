#pragma once

class U10276 {
public:
    static const char* libname()
    {
        return "u10276";
    }
    U10276() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10276> pu10276;
#endif
