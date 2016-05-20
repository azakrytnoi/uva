#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u865.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <sstream>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U865 instance;
    instance();
}

namespace
{

class coder
{
public:

    explicit coder(std::ostream& out) :
        plain_(), subst_(), out_(out)
    {
    }

    friend std::istream& operator >>(std::istream& in, coder& engine)
    {
        std::getline(in, engine.plain_);
        std::getline(in, engine.subst_);
        engine.out_ << engine.subst_ << std::endl << engine.plain_ << std::endl;
        std::string line;
        while (std::getline(in, line) && !line.empty()) {
            engine.out_ << engine.encode(line) << std::endl;
        }
        engine.out_ << std::endl;
        return in;
    }

private:
    std::string plain_;
    std::string subst_;
    std::ostream& out_;

    std::string& encode(std::string& source);
};

std::string& coder::encode(std::string& source)
{
    std::transform(source.begin(), source.end(), source.begin(), [&](auto ch) {
        auto plain_pos = plain_.find(ch);
        if (plain_pos != std::string::npos) {
            return subst_[plain_pos];
        }
        return ch;
    });
    return source;
}

}  // namespace

void U865::operator()() const
{
    int N;
    std::string line;
    std::cin >> N;
    std::getline(std::cin, line);
    std::getline(std::cin, line);
    coder engine(std::cout);
    while (N--) {
        std::cin >> engine;
    }
}
