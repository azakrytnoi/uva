#pragma once

class  U679 {
public:
    static const char * libname()
    {
        return "u679";
    }
    U679();
    void operator()();
};
#ifdef POPULATE_CACHE
populate <U679> pu679;
#endif

