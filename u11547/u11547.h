#pragma once

class U11547
{
public:
    static const char * libname()
    {
        return "u11547";
    }
    U11547() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11547> pu11547;
#endif
