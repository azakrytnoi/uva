#pragma once

class U10168
{
public:
    static const char * libname()
    {
        return "u10168";
    }
    U10168() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10168> pu10168;
#endif
