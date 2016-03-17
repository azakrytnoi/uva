#pragma once

class  U412 {
public:
    static const char * libname()
    {
        return "u412";
    }
    U412();

    void operator()();
};
#ifdef POPULATE_CACHE
populate <U412> pu412;
#endif

