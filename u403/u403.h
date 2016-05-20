#pragma once

class  U403
{
public:
    static const char * libname()
    {
        return "u403";
    }
    U403();

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U403> pu403;
#endif

