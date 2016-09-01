#pragma once

class U913
{
public:
    static const char * libname()
    {
        return "u913";
    }
    U913() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U913> pu913;
#endif
