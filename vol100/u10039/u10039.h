#pragma once

class U10039 {
public:
    static const char* libname()
    {
        return "u10039";
    }
    U10039() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U10039> pu10039;
#endif
