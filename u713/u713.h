#pragma once

class  U713
{
public:
    static const char * libname()
    {
        return "u713";
    }
    U713();
    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U713> pu713;
#endif

