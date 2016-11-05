#pragma once

class U10160 {
public:
    static const char* libname()
    {
        return "u10160";
    }
    U10160() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10160> pu10160;
#endif
