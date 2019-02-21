#pragma once

class  U401 {
public:
    static const char* libname()
    {
        return "u401";
    }
    U401();

    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U401> pu401;
#endif

