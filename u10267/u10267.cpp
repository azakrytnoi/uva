#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10267.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10267 instance;
    instance();
}

namespace
{

class device
{
    std::vector<std::string> pixels_;
    size_t N_;
    size_t M_;
public:
    device() : pixels_(), N_(0), M_(0) {}

    friend std::istream& operator >> (std::istream& in, device& dev);
    friend std::ostream& operator << (std::ostream& out, const device& dev);

private:
    void reset ()
    {
        pixels_.clear();
        pixels_.reserve(N_);
        std::generate_n(std::back_inserter(pixels_), N_, [&]() -> std::string {
            return std::string(M_, 'O');
        });
    }

    void colorPoint(size_t col, size_t row, char color)
    {
        pixels_[row - 1][col - 1] = color;
    }

    void vLine (size_t col, size_t row_s, size_t row_e, char color)
    {
        for (size_t r = row_s - 1; r <= row_e - 1; r++) {
            pixels_[r][col - 1] = color;
        }
    }

    void hLine(size_t col_s, size_t col_e, size_t row, char color)
    {
        for (size_t c = col_s - 1; c <= col_e - 1; c++) {
            pixels_[row - 1][c] = color;
        }
    }

    void rectangle(size_t col_s, size_t col_e, size_t row_s, size_t row_e, char color)
    {
        for (size_t r = row_s - 1; r <= row_e - 1; r++) {
            for (size_t c = col_s - 1; c <= col_e - 1; c++) {
                pixels_[r][c] = color;
            }
        }
    }

    void fillRegion(size_t col, size_t row, char new_color, char orig_color)
    {
        if (pixels_[row][col] == orig_color) {
            pixels_[row][col] = new_color;
            if (col >= 1) {
                fillRegion(col - 1, row, new_color, orig_color);
            }
            if (col <= M_ - 2) {
                fillRegion(col + 1, row, new_color, orig_color);
            }
            if (row >= 1) {
                fillRegion(col, row - 1, new_color, orig_color);
            }
            if (row <= N_ - 2) {
                fillRegion(col, row + 1, new_color, orig_color);
            }
        }
    }

    void fillRegion(size_t col, size_t row, char new_color)
    {
        if (pixels_[row - 1][col - 1] != new_color) {
            fillRegion(col - 1, row - 1, new_color, pixels_[row - 1][col - 1]);
        }
    }
};

class solution
{
public:
    solution(std::ostream& out) : out_(out), command_(), device_() {}

    friend std::istream& operator >>(std::istream& in, solution& sol)
    {
        std::getline(in, sol.command_);
        sol.running_ = !sol.command_.empty();
        return in;
    }

    operator bool();

private:
    std::ostream& out_;
    std::string command_;
    device device_;
    bool running_ = true;

};

solution::operator bool()
{
    if (running_) {
        switch (command_[0]) {
        case 'I':
        case 'C':
        case 'L':
        case 'V':
        case 'H':
        case 'K':
        case 'F': {
            std::stringstream iss(command_);
            iss >> device_;
        }
        break;
        case 'S':
            out_ << command_.substr(2) << std::endl << device_;
            break;
        case 'X':
            running_ = false;
            break;
        default:
            break;
        }
    }
    return running_;
}

std::istream& operator >> (std::istream& in, device& dev)
{
    char command;
    in >> command;
    switch (command) {
    case 'I':
        in >> dev.M_ >> dev.N_;
        dev.reset();
        break;
    case 'C':
        dev.reset();
        break;
    case 'L': {
        size_t x, y;
        char c;
        in >> x >> y >> c;
        dev.colorPoint(x, y, c);
    }
    break;
    case 'V': {
        size_t x, y1, y2;
        char c;
        in >> x >> y1 >> y2 >> c;
        dev.vLine(x, y1, y2, c);
    }
    break;
    case 'H': {
        size_t x1, x2, y;
        char c;
        in >> x1 >> x2 >> y >> c;
        dev.hLine(x1, x2, y, c);
    }
    break;
    case 'K': {
        size_t x1, x2, y1, y2;
        char c;
        in >> x1 >> x2 >> y1 >> y2 >> c;
        dev.rectangle(x1, x2, y1, y2, c);
    }
    break;
    case 'F': {
        size_t x, y;
        char c;
        in >> x >> y >> c;
        dev.fillRegion(x, y, c);
    }
    break;
    default:
        break;
    }
    return in;
}

std::ostream& operator << (std::ostream& out, const device& dev)
{
    std::ostream_iterator<std::string> oit(out, "\n");
    std::copy(dev.pixels_.begin(), dev.pixels_.end(), oit);
    return out;
}

}

void U10267::operator()()
{
    solution sol(std::cout);
    while (std::cin >> sol && sol) {}
    std::cout << std::endl;
}
