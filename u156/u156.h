#pragma once
class  U156 {
public:
    static const char* libname()
    {
        return "u156";
    }
    U156();
    ~U156();

    void operator ()() const;
};
#ifdef POPULATE_CACHE
populate <U156> pu156;
#endif

