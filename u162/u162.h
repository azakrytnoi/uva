#pragma once

class U162 {
public:
    static const char* libname()
    {
        return "u162";
    }
    U162() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U162> pu162;
#endif
