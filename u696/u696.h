#pragma once

class U696
{
public:
    static const char * libname()
    {
        return "u696";
    }
    U696() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U696> pu696;
#endif
