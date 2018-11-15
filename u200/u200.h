#pragma once

class  U200 {
public:
    static const char* libname()
    {
        return "u200";
    }
    U200();
    ~U200();
    void operator ()() const;
};
#ifdef POPULATE_CACHE
populate <U200> pu200;
#endif

