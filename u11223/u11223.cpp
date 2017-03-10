#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u11223.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <sstream>
#include <unordered_map>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U11223 instance;
    instance();
}

namespace {

    class morse {
    public:
        morse() : code_(), buffer_()
        {
            code_[".-"] = 'A';
            code_["-..."] = 'B';
            code_["-.-."] = 'C';
            code_["-.."] = 'D';
            code_["."] = 'E';
            code_["..-."] = 'F';
            code_["--."] = 'G';
            code_["...."] = 'H';
            code_[".."] = 'I';
            code_[".---"] = 'J';
            code_["-.-"] = 'K';
            code_[".-.."] = 'L';
            code_["--"] = 'M';
            code_["-."] = 'N';
            code_["---"] = 'O';
            code_[".--."] = 'P';
            code_["--.-"] = 'Q';
            code_[".-."] = 'R';
            code_["..."] = 'S';
            code_["-"] = 'T';
            code_["..-"] = 'U';
            code_["...-"] = 'V';
            code_[".--"] = 'W';
            code_["-..-"] = 'X';
            code_["-.--"] = 'Y';
            code_["--.."] = 'Z';
            code_["-----"] = '0';
            code_[".----"] = '1';
            code_["..---"] = '2';
            code_["...--"] = '3';
            code_["....-"] = '4';
            code_["....."] = '5';
            code_["-...."] = '6';
            code_["--..."] = '7';
            code_["---.."] = '8';
            code_["----."] = '9';
            code_[".-.-.-"] = '.';
            code_["--..--"] = ',';
            code_["..--.."] = '?';
            code_[".----."] = '\'';
            code_["-.-.--"] = '!';
            code_["-..-."] = '/';
            code_["-.--."] = '(';
            code_["-.--.-"] = ')';
            code_[".-..."] = '&';
            code_["---..."] = ':';
            code_["-.-.-."] = ';';
            code_["-...-"] = '=';
            code_[".-.-."] = '+';
            code_["-....-"] = '-';
            code_["..--.-"] = '_';
            code_[".-..-."] = '"';
            code_[".--.-."] = '@';
        }

        morse& operator << (const std::string& input);
        morse& operator >> (std::string& output);

    private:
        std::unordered_map<std::string, char> code_;
        std::string buffer_;
    };

    morse& morse::operator<<(const std::string& input)
    {
        std::stringstream iss(input);
        std::stringstream oss;
        std::string code;

        while (iss >> code) {
            oss << code_[code];

            if (iss) {
                iss.get();
                char ch = iss.peek();

                if (ch == ' ') {
                    oss << ' ';
                }
            }
        }

        buffer_.assign(oss.str());
        return *this;
    }

    morse& morse::operator>>(std::string& output)
    {
        output.assign(buffer_);
        return *this;
    }

}

void U11223::operator()() const
{
    int N;
    std::cin >> N;
    int caseNo(0);
    std::string line;
    std::getline(std::cin, line);
    morse coder;

    while (N--) {
        std::getline(std::cin, line);
        (coder << line) >> line;
        std::cout << "Message #" << (++caseNo) << std::endl << line << std::endl << std::endl;
    }
}
