#pragma once

class U492 {
public:
    static const char * libname()
    {
        return "u492";
    }
    U492() {}

    void operator()();

};
#ifdef POPULATE_CACHE
populate <U492> pu492;
#endif

