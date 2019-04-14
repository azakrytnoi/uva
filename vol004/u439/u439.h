#pragma once

class U439 {
public:
    static const char* libname()
    {
        return "u439";
    }
    U439() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U439> pu439;
#endif
