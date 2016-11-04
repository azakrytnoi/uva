#pragma once

class  U10301 {
public:
    static const char* libname()
    {
        return "u10301";
    }
    U10301();

    void operator ()() const;

protected:

private:
};
#ifdef POPULATE_CACHE
populate <U10301> pu10301;
#endif

