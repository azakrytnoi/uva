#pragma once

class U210
{
public:
    static const char * libname()
    {
        return "u210";
    }
    U210() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U210> pu210;
#endif
