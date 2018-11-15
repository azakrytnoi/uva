#pragma once

class  U10062 {
public:
    static const char* libname()
    {
        return "u10062";
    }
    U10062();

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U10062> pu10062;
#endif

