#pragma once

class U10131 {
public:
    static const char* libname()
    {
        return "u10131";
    }
    U10131() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10131> pu10131;
#endif
