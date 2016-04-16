#pragma once
class  U727
{
public:
    static const char * libname()
    {
        return "u727";
    }
    U727();
    ~U727();

    void operator()() const;
};
#ifdef POPULATE_CACHE
populate <U727> pu727;
#endif

