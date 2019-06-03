#pragma once

class U10063 {
public:
    static const char* libname()
    {
        return "u10063";
    }
    U10063() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10063> pu10063;
#endif
