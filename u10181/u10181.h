#pragma once

class U10181 {
public:
    static const char* libname()
    {
        return "u10181";
    }
    U10181() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10181> pu10181;
#endif
