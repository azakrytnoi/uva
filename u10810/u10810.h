#pragma once

class  U10810 {
public:
    static const char* libname()
    {
        return "u10810";
    }
    U10810();

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U10810> pu10810;
#endif

