#pragma once

#include <unordered_map>

#include "common.hpp"

namespace lb {

class IRepository {
public:
    ~IRepository() = default;

    virtual void create_lb(const LeaderboardId& lb_id) = 0;
    virtual void delete_lb(const LeaderboardId& lb_id) = 0;

    virtual void add_entry(const LeaderboardId& lb_id, const PlayerId& player_id, Score score) = 0;
    virtual void read_entries(const LeaderboardId& lb_id, int range_start, int range_end) = 0;
    virtual void remove_entry(const LeaderboardId& lb_id, const PlayerId& player_id) = 0;
};

class Repository : public IRepository {
public:
    void create_lb(const LeaderboardId& lb_id);
    void delete_lb(const LeaderboardId& lb_id);

    void add_entry(const LeaderboardId& lb_id, const PlayerId& player_id, Score score);
    void read_entries(const LeaderboardId& lb_id, int range_start, int range_end);
    void remove_entry(const LeaderboardId& lb_id, const PlayerId& player_id);

private:
    std::unordered_map<LeaderboardId, Leaderboard> m_lbs;
};

} // namespace lb