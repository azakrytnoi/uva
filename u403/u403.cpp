#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u403.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <regex>

namespace {
    enum class action_t {
        print, flush
    };

    enum class alignment_t : char {
        none = 'P', left = 'L', right = 'R', center = 'C', end = 'E'
    };

    enum class font_t : char {
        C1 = '1', C5 = '5'
    };

    struct command_t {
        action_t action_;
        alignment_t alignment_;
        font_t font_;
        size_t row_, col_;
        std::string text_;

        command_t () : action_(action_t::print), alignment_(), font_(), row_(), col_(), text_() {}

        friend std::istream& operator>>(std::istream& in, command_t& command)
        {
            std::string word;

            if (in >> word && word[0] == '.') {
                command.alignment_ = static_cast<alignment_t>(word[1]);

                if (command.alignment_ != alignment_t::end) {
                    command.text_.clear();
                    in >> word;
                    command.font_ = static_cast<font_t>(word[1]);
                    in >> command.row_;

                    if (command.alignment_ == alignment_t::none) {
                        in >> command.col_;
                    }

                    char ch(0);

                    while (in >> ch && ch != '|');

                    while (in >> ch && ch != '|') {
                        command.text_.append(1, ch);
                    }
                }
            }

            return in;
        }

        command_t& operator << (const std::string& src)
        {
            text_.clear();

            if (src == ".EOP") {
                action_ = action_t::flush;
                return *this;
            }

            action_ = action_t::print;
            auto ch = src.begin();

            if (*ch++ == '.') {
                alignment_ = static_cast<alignment_t>(*ch++);

                while (*ch++ != 'C');

                font_ = static_cast<font_t>(*ch++);

                auto parse = [&]() -> size_t {
                    size_t result(0);

                    while (not std::isdigit(*ch))
                    {
                        ++ch;
                    }

                    while (std::isdigit(*ch))
                    {
                        result *= 10;
                        result += *ch++ - '0';
                    }

                    return result - 1;
                };
                row_ = parse();

                if (alignment_ == alignment_t::none) {
                    col_ = parse();
                }

                while (*ch++ != '|');

                while (*ch != '|') {
                    text_ += *ch++;
                }
            }

            return *this;
        }
    };

    class solution_t {
    public:
        solution_t() : page_(60, std::string(60, '.')), letters_()
        {
            letters_['A'] = {".***..",
                             "*...*.",
                             "*****.",
                             "*...*.",
                             "*...*."
                            };
            letters_['B'] = {"****..",
                             "*...*.",
                             "****..",
                             "*...*.",
                             "****.."
                            };
            letters_['C'] = {".****.",
                             "*...*.",
                             "*.....",
                             "*.....",
                             ".****."
                            };
            letters_['D'] = {"****..",
                             "*...*.",
                             "*...*.",
                             "*...*.",
                             "****.."
                            };
            letters_['E'] = {"*****.",
                             "*.....",
                             "***...",
                             "*.....",
                             "*****."
                            };
            letters_['F'] = {"*****.",
                             "*.....",
                             "***...",
                             "*.....",
                             "*....."
                            };
            letters_['G'] = {".****.",
                             "*.....",
                             "*..**.",
                             "*...*.",
                             ".***.."
                            };
            letters_['H'] = {"*...*.",
                             "*...*.",
                             "*****.",
                             "*...*.",
                             "*...*."
                            };
            letters_['I'] = {"*****.",
                             "..*...",
                             "..*...",
                             "..*...",
                             "*****."
                            };
            letters_['J'] = {"..***.",
                             "...*..",
                             "...*..",
                             "*..*..",
                             ".**..."
                            };
            letters_['K'] = {"*...*.",
                             "*..*..",
                             "***...",
                             "*..*..",
                             "*...*."
                            };
            letters_['L'] = {"*.....",
                             "*.....",
                             "*.....",
                             "*.....",
                             "*****."
                            };
            letters_['M'] = {"*...*.",
                             "**.**.",
                             "*.*.*.",
                             "*...*.",
                             "*...*."
                            };
            letters_['N'] = {"*...*.",
                             "**..*.",
                             "*.*.*.",
                             "*..**.",
                             "*...*."
                            };
            letters_['O'] = {".***..",
                             "*...*.",
                             "*...*.",
                             "*...*.",
                             ".***.."
                            };
            letters_['P'] = {"****..",
                             "*...*.",
                             "****..",
                             "*.....",
                             "*....."
                            };
            letters_['Q'] = {".***..",
                             "*...*.",
                             "*...*.",
                             "*..**.",
                             ".****."
                            };
            letters_['R'] = {"****..",
                             "*...*.",
                             "****..",
                             "*..*..",
                             "*...*."
                            };
            letters_['S'] = {".****.",
                             "*.....",
                             ".***..",
                             "....*.",
                             "****.."
                            };
            letters_['T'] = {"*****.",
                             "*.*.*.",
                             "..*...",
                             "..*...",
                             ".***.."
                            };
            letters_['U'] = {"*...*.",
                             "*...*.",
                             "*...*.",
                             "*...*.",
                             ".***.."
                            };
            letters_['V'] = {"*...*.",
                             "*...*.",
                             ".*.*.",
                             ".*.*..",
                             "..*..."
                            };
            letters_['W'] = {"*...*.",
                             "*...*.",
                             "*.*.*.",
                             "**.**.",
                             "*...*."
                            };
            letters_['X'] = {"*...*.",
                             ".*.*..",
                             "..*...",
                             ".*.*..",
                             "*...*."
                            };
            letters_['Y'] = {"*...*.",
                             ".*.*..",
                             "..*...",
                             "..*...",
                             "..*..."
                            };
            letters_['Z'] = {"*****.",
                             "...*..",
                             "..*...",
                             ".*....",
                             "*****."
                            };
        }

        operator bool()
        {
            return true;
        }

        solution_t& operator()(std::istream& in, std::ostream& out);

    private:
        std::vector<std::string> page_;
        std::map<char, std::vector<std::string>> letters_;

        void print_text(size_t row, size_t col, const std::string& text, size_t start_from, size_t n_chars, font_t font);
        void do_print(const command_t& command);
        void do_flush(std::ostream& out);
    };

    void solution_t::do_flush(std::ostream& out)
    {
        std::ostream_iterator<std::string> pout(out, "\n");
        std::copy(page_.begin(), page_.end(), pout);
        out << std::endl << std::string(60, '-') << std::endl << std::endl;
        page_ = std::vector<std::string>(60, std::string(60, '.'));
    }

    void solution_t::do_print(const command_t& command)
    {
        auto length = command.text_.length();

        if (command.font_ == font_t::C5) {
            length *= 6;
        }

        if (length > 60) {
            length = 60;
        }

        auto n_chars = (command.font_ == font_t::C1 ? length : length / 6);

        switch (command.alignment_) {
        case alignment_t::center: {
            size_t col = (60 - length) / 2.0 + 0.5;
            size_t start_from(0);

            if (command.text_.length() > (command.font_ == font_t::C1 ? 60 : 10)) {
                start_from = (command.text_.length() - (command.font_ == font_t::C1 ? 60 : 10)) / 2.0 + 0.5;
                col = 0;
            }

            print_text(command.row_, col, command.text_, start_from, n_chars, command.font_);
        }
        break;

        case alignment_t::left: {
            print_text(command.row_, 0, command.text_, 0, n_chars, command.font_);
        }
        break;

        case alignment_t::right: {
            size_t col = 60 - length;
            print_text(command.row_, col, command.text_, 0, n_chars, command.font_);
        }
        break;

        case alignment_t::none: {
            if (command.font_ == font_t::C1) {
                n_chars = std::min(60 - command.col_, n_chars);
            } else {
                n_chars = std::min((60 - command.col_) / 5, n_chars);
            }

            print_text(command.row_, command.col_, command.text_, 0, n_chars,
                       command.font_);
        }
        break;

        default:
            break;
        }
    }

    solution_t& solution_t::operator()(std::istream& in, std::ostream& out)
    {
        std::string line;

        while (std::getline(in, line)) {
            command_t command;
            command << line;

            switch (command.action_) {
            case action_t::flush:
                do_flush(out);
                break;

            case action_t::print:
                do_print(command);
                break;

            default:
                break;
            }
        }

        return *this;
    }

    void solution_t::print_text(size_t row, size_t col, const std::string& text, size_t start_from, size_t n_chars, font_t font)
    {
    	switch (font) {
			case font_t::C1:
	            for (size_t idx = 0; idx < n_chars; ++idx) {
	                if (text[idx + start_from] != ' ') {
	                    page_[row][col + idx] = text[idx + start_from];
	                }
	            }
				break;
			case font_t::C5:
	            for (size_t drow = 0; drow < 5 && row + drow < 60; ++drow) {
	                for (size_t idx = 0; idx < n_chars; ++idx) {
	                    if (text[idx + start_from] != ' ') {
	                        auto letter = letters_.find(text[idx + start_from]);

	                        if (letter != letters_.end()) {
	                            for (size_t dcol = 0; dcol < 5 && col + idx * 6 + dcol < 60; dcol++) {
	                                if (letter->second[drow][dcol] != '.') {
	                                    page_[row + drow][col + idx * 6 + dcol] = letter->second[drow][dcol];
	                                }
	                            }
	                        }
	                    }
	                }
	            }
				break;
			default:
				break;
		}
    }
}

U403::U403() {}

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U403 instance;
    instance();
}
void U403::operator()() const
{
    solution_t sol;
    sol(std::cin, std::cout);
}
