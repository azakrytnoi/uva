#pragma once

class U10083 {
public:
    static const char* libname()
    {
        return "u10083";
    }
    U10083() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10083> pu10083;
#endif
