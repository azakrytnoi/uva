#pragma once

class U10191
{
public:
    static const char * libname()
    {
        return "u10191";
    }
    U10191() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10191> pu10191;
#endif
