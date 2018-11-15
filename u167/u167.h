#pragma once

class  U167 {
public:
    static const char* libname()
    {
        return "u167";
    }
    U167();

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U167> pu167;
#endif

