#pragma once

#include <cstdint>
#include <string>

namespace lb {

using EntryId = int64_t;
using UserId = std::string;
using LeaderboardId = std::string;
using Score = uint64_t;
using Timestamp = uint64_t;

struct Entry {
    EntryId id;
    UserId user_id;
    Score score;
    Timestamp ts;
};


enum class Ordering {
    Ascending,
    Descending,
};
inline std::string_view to_string(const Ordering& ordering) {
    return "";
}

struct Leaderboard {
    explicit Leaderboard(Ordering ordering)
    : ordering(ordering) {}

    Ordering ordering;
    std::vector<Entry> entries;
};

}  // namespace lb