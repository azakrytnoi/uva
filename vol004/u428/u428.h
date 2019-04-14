#pragma once

class U428 {
public:
    static const char* libname()
    {
        return "u428";
    }
    U428() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
    populate <U428> pu428;
#endif
