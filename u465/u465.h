#pragma once

class U465
{
public:
    static const char * libname()
    {
        return "u465";
    }
    U465() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U465> pu465;
#endif
