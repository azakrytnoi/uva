#pragma once

class U409 {
public:
    static const char* libname()
    {
        return "u409";
    }
    U409() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U409> pu409;
#endif
