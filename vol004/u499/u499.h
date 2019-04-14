#pragma once

class U499 {
public:
    static const char* libname()
    {
        return "u499";
    }
    U499() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U499> pu499;
#endif
