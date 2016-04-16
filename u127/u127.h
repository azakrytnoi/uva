#pragma once

class  U127
{
public:
    static const char * libname()
    {
        return "u127";
    }
    /** Default constructor */
    U127();

    void operator()() const;

protected:

private:
};
#ifdef POPULATE_CACHE
populate <U127> pu127;
#endif

