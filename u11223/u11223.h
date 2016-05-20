#pragma once

class U11223
{
public:
    static const char * libname()
    {
        return "u11223";
    }
    U11223() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U11223> pu11223;
#endif
