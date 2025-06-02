#pragma once

#include "common.hpp"

#include <future>
#include <memory>

namespace lb {

class IRepository {
public:
    virtual ~IRepository() = default;

    virtual std::future<LeaderboardId> create_lb(const std::string& lb_name, Ordering ordering) = 0;
    virtual std::future<bool> delete_lb(const LeaderboardId& lb_id) = 0;

    virtual std::future<bool> set_score(const LeaderboardId& lb_id,
                                        const UserId& user_id,
                                        Score score) = 0;
    virtual std::future<bool> remove_entry(const LeaderboardId& lb_id, const UserId& user_id) = 0;
    virtual std::future<std::vector<Entry>> read_entries(const LeaderboardId& lb_id,
                                                         int range_start_idx,
                                                         int range_end_idx) = 0;
};

std::unique_ptr<IRepository> create_repository();

}  // namespace lb
