#pragma once

class U10086 {
public:
    static const char* libname()
    {
        return "u10086";
    }
    U10086() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10086> pu10086;
#endif
