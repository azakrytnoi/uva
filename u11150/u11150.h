#pragma once

class  U11150 {
public:
    static const char* libname()
    {
        return "u11150";
    }
    U11150();

    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U11150> pu11150;
#endif

