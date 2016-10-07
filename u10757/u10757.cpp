#ifdef _WIN32
#define UVA_API_EXPORT __declspec(dllexport)
#else
#define __cdecl
#define UVA_API_EXPORT
#endif

#include "u10757.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>
#include <memory>
#include <unordered_map>
#include <sstream>

extern "C"
{
    UVA_API_EXPORT void __cdecl invoke();
}
void __cdecl invoke()
{
    U10757 instance;
    instance();
}

namespace
{
enum class datatype : char
{   I = 'I', S = 'S' };

class table
{
    std::unordered_map<std::string, datatype> meta_;
    std::vector<std::unordered_map<std::string, std::string>> data_;
    std::string name_;

public:
    table() : meta_(), data_(), name_() {}

    const std::string& name() const
    {
        return name_;
    }

    const std::unordered_map<std::string, datatype>& meta() const
    {
        return meta_;
    }

    const std::vector<std::unordered_map<std::string, std::string>>& data() const
    {
        return data_;
    }

    friend
    std::istream& operator >> (std::istream& in, table& t)
    {
        in >> t.name_;
        int nColumns, nRows;
        in >> nColumns >> nRows;
        std::vector<std::string> columns;
        columns.reserve(nColumns);
        while (nColumns--) {
            std::string name;
            std::string dt;
            in >> name >> dt;
            t.meta_[name] = datatype(dt[0]);
            columns.push_back(name);
        }
        t.data_.reserve(nRows);
        std::generate_n(std::back_inserter(t.data_), nRows, [&]() -> std::unordered_map<std::string, std::string> {
            std::unordered_map<std::string, std::string> row;
            std::for_each(columns.begin(), columns.end(), [&](auto name)
            {
                std::string value;
                in >> value;
                row.insert(std::make_pair(name, value));
            });
            return row;
        });
        return in;
    }
};

class database
{
    std::unordered_map<std::string, std::string> columns_;
    std::unordered_map<std::string, std::shared_ptr<table>> tables_;

public:
    database() : columns_(), tables_() {}
    friend
    std::istream& operator >> (std::istream& in, database& d)
    {
        int nTables;
        std::cin >> nTables;
        while (nTables--) {
            auto t = std::make_shared<table>();
            std::cin >> *t;
            d.tables_[t->name()] = t;
            std::for_each(t->meta().begin(), t->meta().end(), [&](auto col) {
                d.columns_[col.first] = t->name();
            });
        }
        return in;
    }

    const std::unordered_map<std::string, std::string>& columns() const
    {
        return columns_;
    }
    const std::unordered_map<std::string, std::shared_ptr<table>>& tables() const
    {
        return tables_;
    }

    bool compare(const std::string& column, const std::unordered_map<std::string, std::string>& r1, const std::unordered_map<std::string, std::string>& r2) const
    {
        switch (tables_.find(columns_.find(column)->second)->second->meta().find(column)->second) {
        case datatype::I:
            return std::atoi(r1.find(column)->second.c_str()) < std::atoi(r2.find(column)->second.c_str());
        case datatype::S:
            return r1.find(column)->second < r2.find(column)->second;
        default:
            return false;
        }
    }
};

class query
{
    std::vector<std::string> selector_;
    std::vector<std::unordered_map<std::string, std::string>> resultset_;
    std::vector<std::pair<std::string, bool>> sort_;
    const database& db_;

public:
    explicit query(const database& db) : selector_(), resultset_(), sort_(), db_(db) {}

    query& execute();

    friend
    std::istream& operator >> (std::istream& in, query& q)
    {
        std::string strq;
        {
            std::string line;
            while (std::getline(in, line)) {
                strq.append(line).append(" ");
            }
        }
        q.parse(strq);
        return in;
    }

    friend
    std::ostream& operator << (std::ostream& out, const query& q)
    {
        std::cout << q.selector_.size() << " " << q.resultset_.size() << std::endl;
        std::for_each(q.selector_.begin(), q.selector_.end(), [&](auto col) {
            out << col << std::endl;
        });
        std::for_each(q.resultset_.begin(), q.resultset_.end(), [&](auto row) {
            std::for_each(q.selector_.begin(), q.selector_.end(), [&](auto col) {
                out << row[col] << " ";
            });
            std::cout << std::endl;
        });
        return out;
    }

private:
    void parse(const std::string& qry);
    void parseSelect(const std::string& select_list);
    void parseFrom(const std::string& from_list);
    void parseWhere(const std::string& select_list);
    void parseOrder(const std::string& select_list);
    void parseJoin(const std::string& join_list);
};

void query::parse(const std::string& qry)
{
    auto select_pos = qry.find("SELECT ");
    if (select_pos != std::string::npos) {
        auto from_pos = qry.find(" FROM ", select_pos);
        if (from_pos != std::string::npos) {
            std::string select_list(qry.substr(select_pos + 7, from_pos - select_pos - 7));
            auto where_pos = qry.find(" WHERE ", from_pos);
            std::string from_list(qry.substr(from_pos + 6, where_pos - from_pos - 6));
            auto order_pos = qry.find(" ORDER BY ", from_pos);
            std::string where_list(qry.substr(where_pos + 7, order_pos - where_pos - 7));
            std::string order_list(qry.substr(order_pos + 10));
            parseSelect(select_list);
            parseFrom(from_list);
            parseWhere(where_list);
            parseOrder(order_list);
        }
    }
}

void query::parseSelect(const std::string & select_list)
{
    std::string source(select_list);
    source.erase(select_list.find_last_not_of(' ') + 1, std::string::npos).erase(0, select_list.find_first_not_of(' '));
    selector_.reserve(db_.columns().size());
    if (source == "*") {
        selector_.reserve(db_.columns().size());
        std::transform(db_.columns().begin(), db_.columns().end(), std::back_inserter(selector_), [](auto column) -> std::string { return column.first; });
    } else {
        std::stringstream ss(source);
        std::istream_iterator<std::string>  iss(ss);
        std::transform(iss, std::istream_iterator<std::string>(), std::back_inserter(selector_), [](auto column) -> std::string { return column.substr(0, column.find_last_not_of(',') + 1); });
    }
}

void query::parseFrom(const std::string & from_list)
{
    std::string source(from_list);
    source.erase(from_list.find_last_not_of(' ') + 1, std::string::npos).erase(0, from_list.find_first_not_of(' '));
    auto join_pos = source.find(" INNER JOIN ");
    std::string main_table_name(source);
    if (join_pos != std::string::npos) {
        main_table_name = source.substr(0, join_pos);
        main_table_name.erase(main_table_name.find_last_not_of(' ') + 1, std::string::npos).erase(0, main_table_name.find_first_not_of(' '));
        std::shared_ptr<table> main_table = db_.tables().find(main_table_name)->second;
        resultset_.reserve(main_table->data().size());
        std::copy(main_table->data().begin(), main_table->data().end(), std::back_inserter(resultset_));
        while (!source.empty()) {
            source = source.substr(join_pos + 12);
            join_pos = source.find(" INNER JOIN ");
            std::string join_spec = join_pos == std::string::npos ? source : source.substr(0, join_pos + 1);
            parseJoin(join_spec);
            if (join_pos == std::string::npos) {
                break;
            }
        }
    } else {
        std::shared_ptr<table> main_table = db_.tables().find(main_table_name)->second;
        resultset_.reserve(main_table->data().size());
        std::copy(main_table->data().begin(), main_table->data().end(), std::back_inserter(resultset_));
    }
}

void query::parseWhere(const std::string & where_list)
{
    std::string source(where_list);
    source.erase(where_list.find_last_not_of(' ') + 1, std::string::npos).erase(0, where_list.find_first_not_of(' '));
    std::string col_name = source.substr(0, source.find('='));
    std::string col_value = source.substr(source.find('=') + 1);
    col_name.erase(col_name.find_last_not_of(' ') + 1, std::string::npos).erase(0, col_name.find_first_not_of(' '));
    col_value.erase(col_value.find_last_not_of(' ') + 1, std::string::npos).erase(0, col_value.find_first_not_of(' '));
    if (col_value[0] == '"') {
        col_value = col_value.substr(1, col_value.length() - 2);
    }
    std::vector<std::unordered_map<std::string, std::string>> filter_result;
    filter_result.reserve(resultset_.size());
    std::copy_if(resultset_.begin(), resultset_.end(), std::back_inserter(filter_result), [&](auto row) -> bool { return row[col_name] == col_value; });
    resultset_ = filter_result;
    resultset_.shrink_to_fit();
}

void query::parseOrder(const std::string & order_list)
{
    std::string source(order_list);
    source.erase(order_list.find_last_not_of(' ') + 1, std::string::npos).erase(0, order_list.find_first_not_of(' '));
    if (!source.empty()) {
        source.append(",");
        std::vector<std::pair<std::string, bool>> sort_defs;
        sort_defs.reserve(std::count_if(source.begin(), source.end(), [](auto ch) {
            return ch == ',';
        }));
        while (source.find(',') != std::string::npos) {
            std::string ord = source.substr(0, source.find(','));
            ord.erase(ord.find_last_not_of(' ') + 1, std::string::npos).erase(0, ord.find_first_not_of(' '));
            source.erase(0, source.find(',') + 1);
            bool order(true);
            if (ord.find(' ') != std::string::npos) {
                order = ord.substr(ord.find(' ') + 1) == "DESCENDING";
                ord.erase(ord.find(' '), std::string::npos);
                ord.erase(ord.find_last_not_of(' ') + 1, std::string::npos).erase(0, ord.find_first_not_of(' '));
            }
            sort_defs.push_back(std::make_pair(ord, order));
        }
        std::for_each(sort_defs.rbegin(), sort_defs.rend(), [&](auto sort_def) {
            std::stable_sort(resultset_.begin(), resultset_.end(), [&](auto r1, auto r2) {
                return sort_def.second ? db_.compare(sort_def.first, r1, r2) : db_.compare(sort_def.first, r2, r1);
            });
        });
    }
}

void query::parseJoin(const std::string & join_list)
{
    std::string source(join_list);
    source.erase(join_list.find_last_not_of(' ') + 1, std::string::npos).erase(0, join_list.find_first_not_of(' '));
    auto on_pos = source.find(" ON ");
    std::string join_table_name(source.substr(0, on_pos));
    join_table_name.erase(join_table_name.find_last_not_of(' ') + 1, std::string::npos).erase(0, join_table_name.find_first_not_of(' '));
    std::string col_a = source.substr(on_pos + 4);
    std::string col_b = col_a.substr(col_a.find('=') + 1);
    col_a = col_a.substr(0, col_a.find('='));
    col_a.erase(col_a.find_last_not_of(' ') + 1, std::string::npos).erase(0, col_a.find_first_not_of(' '));
    col_b.erase(col_b.find_last_not_of(' ') + 1, std::string::npos).erase(0, col_b.find_first_not_of(' '));
    std::vector<std::unordered_map<std::string, std::string>> join_result;
    std::shared_ptr<table> join_table = db_.tables().find(join_table_name)->second;
    join_result.reserve(resultset_.size() * join_table->data().size());
    std::for_each(resultset_.begin(), resultset_.end(), [&](auto row1) {
        std::for_each(join_table->data().begin(), join_table->data().end(), [&](auto row2) {
            std::unordered_map<std::string, std::string> row(row1);
            row.insert(row2.begin(), row2.end());
            if (row[col_a] == row[col_b]) {
                join_result.push_back(row);
            }
        });
    });
    resultset_ = join_result;
    resultset_.shrink_to_fit();
}

query& query::execute()
{
    return *this;
}
}

void U10757::operator()() const
{
    int N;
    std::cin >> N;
    while (N--) {
        database db;
        std::cin >> db;
        query q(db);
        std::cin >> q;
        std::cout << q.execute() << std::endl;
    }
}
