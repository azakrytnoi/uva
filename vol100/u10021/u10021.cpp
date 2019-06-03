#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u10021.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <memory>
#include <limits>
#include <queue>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U10021 instance;
    instance();
}

namespace {

    enum class direction_t : int8_t
    {
        right = 0, left, up, down
    };

    class cube_t {
    public:
        cube_t(int16_t x = 0, int16_t y = 0) : x_(x), y_(y), faces_()
        {
            faces_.resize(6);
            std::iota(faces_.begin(), faces_.end(), 0);
        }
        ~cube_t() {}

        cube_t roll(direction_t dir)
        {
            static std::map<direction_t, int8_t> dx = {{direction_t::right, 0}, {direction_t::left, 0}, {direction_t::up, 1}, {direction_t::down, -1}};
            static std::map<direction_t, int8_t> dy = {{direction_t::right, 1}, {direction_t::left, -1}, {direction_t::up, 0}, {direction_t::down, 0}};
            cube_t r (*this);
            auto t (r.faces_[0]);

            if (dir == direction_t::right)   // roll right
            {
                r.faces_[0] = r.faces_[5], r.faces_[5] = r.faces_[2], r.faces_[2] = r.faces_[4];
                r.faces_[4] = t;
            }
            else if (dir == direction_t::left)     // roll left
            {
                r.faces_[0] = r.faces_[4], r.faces_[4] = r.faces_[2], r.faces_[2] = r.faces_[5];
                r.faces_[5] = t;
            }
            else if (dir == direction_t::up)     // roll up
            {
                r.faces_[0] = r.faces_[3], r.faces_[3] = r.faces_[2], r.faces_[2] = r.faces_[1];
                r.faces_[1] = t;
            }
            else if (dir == direction_t::down)     // roll down
            {
                r.faces_[0] = r.faces_[1], r.faces_[1] = r.faces_[2], r.faces_[2] = r.faces_[3];
                r.faces_[3] = t;
            }

            r.x_ += dx[dir], r.y_ += dy[dir];
            return r;
        }

        int16_t x_, y_;
        std::vector<int8_t> faces_;
    };

    class solution_t {
    public:
        solution_t() : N_(std::numeric_limits<size_t>::max()), X_(), Y_(), A_(), B_(), C_(), D_(),
            field_(), result_() { }

        friend std::istream& operator >>(std::istream& in, solution_t& sol);
        friend std::ostream& operator <<(std::ostream& out, const solution_t& sol);

        operator bool()
        {
            return N_-- > 0;
        }
        solution_t& operator()();

    private:
        size_t N_;
        int16_t X_, Y_, A_, B_, C_, D_;
        std::vector<std::vector<std::vector<int8_t>>> field_;
        int16_t result_;

        void bfs();
    };

    std::istream& operator >> (std::istream& in, solution_t& sol)
    {
        if (sol.N_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.N_;
        }

        if (in)
        {
            in >> sol.X_ >> sol.Y_ >> sol.A_ >> sol.B_ >> sol.C_ >> sol.D_;
            in.ignore();
            sol.field_.clear();
            sol.field_ = std::vector<std::vector<std::vector<int8_t>>>(16, std::vector<std::vector<int8_t>>(16, std::vector<int8_t>(4)));
            char kind(0);
            std::string temp;

            while (std::getline(in, temp) && not temp.empty())
            {
                switch (temp[0])
                {
                case 'v':
                case 'h':
                    kind = temp[0];
                    break;

                default:
                {
                    std::stringstream ss(temp);
                    int16_t x(0), y(0);
                    ss >> x >> y;

                    if (kind == 'v')
                    {
                        sol.field_[x][y][2] = sol.field_[x + 1][y][3] = 1;
                    }
                    else
                    {
                        sol.field_[x][y][0] = sol.field_[x][y + 1][1] = 1;
                    }
                }
                break;
                }
            }
        }

        return in;
    }

    std::ostream& operator << (std::ostream& out, const solution_t& sol)
    {
        if (sol.result_ < 0)
        {
            out << "No solution";
        }
        else
        {
            out << sol.result_;
        }

        return out;
    }

    solution_t& solution_t::operator()()
    {
        result_ = -1;
        bfs();
        return *this;
    }

    void solution_t::bfs()
    {
        int8_t dist[16][16][16][16] = {};
        std::queue<cube_t> work;
        cube_t u(A_, B_);
        work.push(u), dist[u.x_][u.y_][u.faces_[0]][u.faces_[1]] = 0;

        while (!work.empty())
        {
            u = work.front(), work.pop();

            if (u.x_ == C_ && u.y_ == D_ && u.faces_[0] == 0)
            {
                result_ = dist[C_][D_][0][u.faces_[1]];
                return ;
            }

            static std::vector<direction_t> dirs ({direction_t::right, direction_t::left, direction_t::up, direction_t::down});

            for (auto d : dirs)
            {
                if (field_[u.x_][u.y_][static_cast<int16_t>(d)] == 0)
                {
                    auto v = u.roll(d);

                    if (v.x_ <= 0 || v.x_ > X_ || v.y_ <= 0 || v.y_ > Y_)
                    {
                        continue;
                    }

                    if (dist[v.x_][v.y_][v.faces_[0]][v.faces_[1]] == 0)
                    {
                        dist[v.x_][v.y_][v.faces_[0]][v.faces_[1]] = dist[u.x_][u.y_][u.faces_[0]][u.faces_[1]] + 1;
                        work.push(v);
                    }
                }
            }
        }
    }

}

void U10021::operator()() const
{
    solution_t sol;

    std::string separator;

    while (true)
    {
        std::cin >> sol;

        if (not sol)
        {
            break;
        }

        std::cout << separator << sol() << std::endl;
        separator = "\n";
    }
}
