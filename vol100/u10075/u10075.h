#pragma once

class U10075 {
public:
    static const char* libname()
    {
        return "u10075";
    }
    U10075() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10075> pu10075;
#endif
