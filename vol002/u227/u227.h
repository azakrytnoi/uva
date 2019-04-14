#pragma once

class U227 {
public:
    static const char* libname()
    {
        return "u227";
    }
    U227() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U227> pu227;
#endif
