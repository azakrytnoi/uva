#ifdef _WIN32
    #define UVA_API_EXPORT __declspec(dllexport)
#else
    #define __cdecl
    #define UVA_API_EXPORT
#endif

#include "u452.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <map>
#include <memory>

extern "C" {
    UVA_API_EXPORT void __cdecl invoke();
}

void __cdecl invoke()
{
    U452 instance;
    instance();
}

namespace {

    struct task_t
    {
        char name_;
        size_t duration_;
        std::vector<char> predessors_;

        task_t() : name_(), duration_(), predessors_() { }

        friend std::istream& operator >>(std::istream& in, task_t& task)
        {
            in >> task.name_ >> task.duration_;
            return in;
        }
    };

    class solution_t {
    public:
        solution_t() : case_no_(std::numeric_limits<size_t>::max()), tasks_(), total_() {}

        operator bool()
        {
            return case_no_ > 0;
        }

        solution_t& operator()();

        friend std::istream& operator>>(std::istream& in, solution_t& sol);

        friend std::ostream& operator<<(std::ostream& out, const solution_t& sol);

    private:
        static const size_t N = 'Z' - 'A' + 1;
        size_t case_no_;
        std::map<char, std::shared_ptr<task_t>> tasks_;
        size_t total_;
    };

    solution_t& solution_t::operator()()
    {
        --case_no_;
        std::vector<std::vector<size_t>> matrix(N, std::vector<size_t>(N));
        std::vector<size_t> depends(N);
        std::vector<bool> defined(N);
        std::for_each(tasks_.begin(), tasks_.end(), [&](const std::pair<char, std::shared_ptr<task_t>>& task)
        {
            size_t idx = task.first - 'A';
            defined[idx] = true;

            for (int64_t ref = task.second->predessors_.size() - 1; ref >= 0; --ref)
            {
                matrix[idx][depends[idx]++] = task.second->predessors_[ref] - 'A';
            }
        });
        std::vector<bool> used(N);
        std::vector<size_t> finished(N);

        while (true)
        {
            bool flag(false);

            for (size_t i = 0; i < N; ++i)
            {
                if (defined[i] && not used[i])
                {
                    size_t max(0);
                    size_t j = 0;

                    for (; j < depends[i]; ++j)
                    {
                        if (not used[matrix[i][j]])
                        {
                            break;
                        }

                        max = std::max(max, finished[matrix[i][j]]);
                    }

                    if (j == depends[i])
                    {
                        finished[i] = max + tasks_[static_cast<char>(i + 'A')]->duration_;
                        total_ = std::max(total_, finished[i]);
                        used[i] = true;
                        flag = true;
                    }
                }
            }

            if (not flag)
            {
                break;
            }
        }

        return *this;
    }

    std::istream& operator>>(std::istream& in, solution_t& sol)
    {
        sol.tasks_.clear();
        sol.total_ = 0;

        if (sol.case_no_ == std::numeric_limits<size_t>::max())
        {
            in >> sol.case_no_;
            in.ignore();
        }

        std::string line;

        while (std::getline(in, line) && line.empty());

        do
        {
            std::stringstream sin(line);
            auto task = std::make_shared<task_t>();
            sin >> *task;
            sol.tasks_[task->name_] = task;
            char ref(0);

            while (sin >> ref)
            {
                task->predessors_.push_back(ref);
            }
        }
        while (std::getline(in, line) && not line.empty());

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const solution_t& sol)
    {
        out << sol.total_ << std::endl;
        return out;
    }

}

void U452::operator()() const
{
    solution_t sol;

    while (std::cin >> sol && sol)
    {
        std::cout << sol() << std::endl;
    }
}
