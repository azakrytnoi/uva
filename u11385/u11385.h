#pragma once

class U11385 {
public:
    static const char* libname()
    {
        return "u11385";
    }
    U11385() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U11385> pu11385;
#endif
