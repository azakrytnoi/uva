#pragma once

class U429 {
public:
    static const char* libname()
    {
        return "u429";
    }
    U429() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U429> pu429;
#endif
