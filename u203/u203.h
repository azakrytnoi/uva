#pragma once

class U203
{
public:
    static const char * libname()
    {
        return "u203";
    }
    U203() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U203> pu203;
#endif
