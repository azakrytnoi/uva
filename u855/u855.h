#pragma once

class U855
{
public:
    static const char * libname()
    {
        return "u855";
    }
    U855() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U855> pu855;
#endif
