#pragma once

#include "common.hpp"

#include <pqxx/pqxx>

#include <algorithm>
#include <cassert>
#include <future>
#include <memory>
#include <string_view>
#include <unordered_map>

#include <drogon/drogon.h>
#include <drogon/orm/SqlBinder.h>

namespace lb {

// class IRepository {
// public:
//     virtual ~IRepository() = default;
//
//     virtual future<result> create_lb(const LeaderboardId& lb_id) = 0;
//     virtual future<result> delete_lb(const LeaderboardId& lb_id) = 0;
//
//     virtual future<result> add_entry(const LeaderboardId& lb_id, const PlayerId& user_id, Score
//     score) = 0; virtual future<vector<entry>> read_entries(const LeaderboardId& lb_id, int
//     range_start, int range_end) = 0; virtual future<result> remove_entry(const LeaderboardId&
//     lb_id, const PlayerId& user_id) = 0;
// };

class DrogonDbRepository {
public:
    DrogonDbRepository(const std::string& conn_info) {
        m_db_client = drogon::orm::DbClient::newPgClient(conn_info, 4);
        if (!m_db_client) {
            throw std::runtime_error("Failed to create a DB client");
        }
    }

    std::future<LeaderboardId> create_lb(const std::string& lb_name, Ordering ordering) {
        std::promise<LeaderboardId> promise;
        m_db_client->execSqlAsync(
            k_create_lb_query,
            [](const drogon::orm::Result& res) {
                auto lb_id = res[0][0].as<LeaderboardId>();
                std::cout << lb_id << std::endl;
            },
            [](const drogon::orm::DrogonDbException& ex) {},
            lb_name,
            0,
            to_string(ordering).data());

        return promise.get_future();
    }
    std::future<bool> delete_lb(const LeaderboardId& lb_id);

    EntryId add_entry(const LeaderboardId& lb_id, const UserId& user_id, Score score) {}
    std::future<bool> remove_entry(const LeaderboardId& lb_id, EntryId id);
    std::future<std::vector<Entry>>
    read_entries(const LeaderboardId& lb_id, int range_start_idx, int range_end_idx);

private:
    static constexpr std::string k_create_lb_query =
        "INSERT INTO leaderboard_metadata (name, owner_id, ordering)"
        " VALUES ($1, $2, $3) RETURNING leaderboard_id;";
    std::shared_ptr<drogon::orm::DbClient> m_db_client;
};

// class PqlibDbRepository {
// public:
//     DbRepository(const std::string& conn_info)
//     : m_conn(conn_info) {
//         m_conn.prepare("insert_score",
//                       "INSERT INTO leaderboard_scores(leaderboard_id, user_id, score) "
//                       "VALUES($1,$2,$3) RETURNING entry_id");
//         m_conn.prepare("get_top_n",
//                       "SELECT user_id, score "
//                       "FROM leaderboard_scores "
//                       "WHERE leaderboard_id = $1 "
//                       "ORDER BY score DESC, entry_id ASC "
//                       "LIMIT $2");
//         m_conn.prepare("get_user_entries",
//                       "SELECT entry_id, score, created_at "
//                       "FROM leaderboard_scores "
//                       "WHERE leaderboard_id = $1 AND user_id = $2 "
//                       "ORDER BY created_at");
//     }
//
//     std::future<bool> create_lb(const LeaderboardId& lb_id, Leaderboard::Ordering ordering);
//     std::future<bool> delete_lb(const LeaderboardId& lb_id);
//
//     EntryId add_entry(const LeaderboardId& lb_id, const UserId& user_id, Score score) {
//         pqxx::work tx(m_conn);
//         pqxx::work
//         pqxx::result res = tx.exec_prepared("insert_score", lb_id, user_id, score);
//         tx.commit();
//         return res[0][0].as<int64_t>();
//     }
//     std::future<bool> remove_entry(const LeaderboardId& lb_id, EntryId id);
//     std::future<std::vector<Entry>>
//     read_entries(const LeaderboardId& lb_id, int range_start_idx, int range_end_idx);
//
// private:
//     pqxx::connection m_conn;
// };

class SimpleRepository {
public:
    bool create_lb(const LeaderboardId& lb_id) {
        auto it = m_lbs.find(lb_id);
        if (it != m_lbs.end()) {
            return false;
        }

        m_lbs.emplace(lb_id, Leaderboard{ Leaderboard::Ordering::Descending });
        return true;
    }

    bool delete_lb(const LeaderboardId& lb_id) {
        auto it = m_lbs.find(lb_id);
        if (it == m_lbs.end()) {
            return false;
        }
        m_lbs.erase(it);
        return true;
    }

    int add_entry(const LeaderboardId& lb_id, const UserId& user_id, Score score) {
        auto lb_it = m_lbs.find(lb_id);
        if (lb_it == m_lbs.end()) {
            return -1;
        }
        Leaderboard& lb = lb_it->second;
        auto insertion_pos = [&lb, score] {
            switch (lb.ordering) {
                case Leaderboard::Ordering::Ascending: {
                    return std::ranges::lower_bound(lb.entries, score);
                }
                case Leaderboard::Ordering::Descending: {
                    return std::ranges::upper_bound(lb.entries, score);
                }
            }
        }();
        lb.entries.insert(insertion_pos, score);
        return std::distance(m_lbs.begin(), insertion_pos);
    }

    std::vector<Entry>
    read_entries(const LeaderboardId& lb_id, int range_start_idx, int range_end_idx) {
        if (range_start_idx >= range_end_idx) {  // TODO add a proper error
            assert(0);
            return {};
        }

        auto lb_it = m_lbs.find(lb_id);
        if (lb_it == m_lbs.end()) {
            return {};
        }

        std::vector<Entry> result;
        const Leaderboard& lb = lb_it->second;
        const int entries_num = static_cast<int>(lb.entries.size());
        auto range_begin = lb.entries.begin() + std::min(range_start_idx, entries_num);
        auto range_end = lb.entries.begin() + std::min(range_end_idx, entries_num);
        std::copy(range_begin, range_end, std::back_inserter(result));
        return result;
    }

    bool remove_entry(const LeaderboardId& lb_id, EntryId entry_id) {
        auto lb_it = m_lbs.find(lb_id);
        if (lb_it == m_lbs.end()) {
            return false;
        }

        Leaderboard& lb = lb_it->second;
        if (entry_id >= static_cast<int>(lb.entries.size())) {
            return false;
        }

        auto entry_it = lb.entries.begin() + entry_id;
        lb.entries.erase(entry_it);
        return true;
    }

private:
    std::unordered_map<LeaderboardId, Leaderboard> m_lbs;
};

}  // namespace lb
