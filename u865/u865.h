#pragma once

class U865
{
public:
    static const char * libname()
    {
        return "u865";
    }
    U865() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U865> pu865;
#endif
