#pragma once

class U350
{
public:
    static const char * libname()
    {
        return "u350";
    }
    U350() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U350> pu350;
#endif
