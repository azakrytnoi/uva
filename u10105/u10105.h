#pragma once

class U10105 {
public:
    static const char* libname()
    {
        return "u10105";
    }
    U10105() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10105> pu10105;
#endif
