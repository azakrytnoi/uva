#pragma once

class U324 {
public:
    static const char* libname()
    {
        return "u324";
    }
    U324() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U324> pu324;
#endif
