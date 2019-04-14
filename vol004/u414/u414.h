#pragma once

class U414 {
public:
    static const char* libname()
    {
        return "u414";
    }
    U414() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U414> pu414;
#endif
