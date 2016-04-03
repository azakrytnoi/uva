#pragma once

class U706
{
public:
    static const char * libname()
    {
        return "u706";
    }
    U706() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U706> pu706;
#endif
