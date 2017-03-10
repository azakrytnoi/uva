#pragma once

class U705 {
public:
    static const char* libname()
    {
        return "u705";
    }
    U705() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U705> pu705;
#endif
