#pragma once

class  U957 {
public:
    static const char* libname()
    {
        return "u957";
    }
    U957();
    void operator()() const;
};
#ifdef POPULATE_CACHE
    populate <U957> pu957;
#endif

