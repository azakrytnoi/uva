#pragma once
class  U544
{
public:
    static const char * libname()
    {
        return "u544";
    }
    U544();
    ~U544();

    void operator ()();
};
#ifdef POPULATE_CACHE
populate <U544> pu544;
#endif

