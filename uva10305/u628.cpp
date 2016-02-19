#include "u628.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

U628::U628()
{
}
namespace {
const std::string digits[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

class generator {
public:
    generator(std::vector<std::string>& dictionary) : dictionary_(dictionary) {}

    void generate(std::string::iterator pos, std::string::iterator end, std::string s)
    {
        if(pos == end) {
            std::cout << s << std::endl;
            return;
        }
        auto self = this;
        switch(*pos) {
        case '#':
            std::for_each(dictionary_.begin(), dictionary_.end(), [&](auto dict) {
                self->generate(pos + 1, end, s + dict);
            });
            break;
        case '0':
            std::for_each(digits, digits + 10, [&](auto digit) {
                self->generate(pos + 1, end, s + digit);
            });
            break;
        }
    }

private:
    std::vector<std::string>& dictionary_;
};

}

void U628::operator()()
{
    std::vector<std::string> words;
    std::string line;
    auto load = [&]() -> std::string { std::cin >> line; return line; };
    while (true) {
        int n_words;
        std::cin >> n_words;
        if (!std::cin) {
            break;
        }
        words.clear();
        words.reserve(n_words);
        std::generate_n(std::back_inserter(words), n_words, load);
        generator gen(words);
        int n_rules;
        std::cin >> n_rules;
        std::cout << "--" << std::endl;
        while (n_rules--) {
            std::cin >> line;
            gen.generate(line.begin(), line.end(), "");
        }

    }
}
