#pragma once

class U10235 {
public:
    static const char* libname()
    {
        return "u10235";
    }
    U10235() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10235> pu10235;
#endif
