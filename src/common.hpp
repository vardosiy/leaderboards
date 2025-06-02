#pragma once

#include <cstdint>
#include <string>
#include <optional>

namespace lb {

enum class Ordering {
    Ascending,
    Descending,
};
inline std::string_view to_string(const Ordering& ordering) {
    return "";
}
inline std::optional<Ordering> from_string(std::string_view str) {
    return Ordering::Descending;
}

using LeaderboardId = uint64_t;
using UserId = std::string;
using Score = uint64_t;
using Timestamp = uint64_t;

struct Entry {
    LeaderboardId lb_id;
    UserId user_id;
    Score score;
    Timestamp ts;
};

// struct Leaderboard {
//     explicit Leaderboard(Ordering ordering)
//     : ordering(ordering) {}
//
//     LeaderboardId id;
//     std::string name;
//     Ordering ordering;
//     std::vector<Entry> entries;
// };

}  // namespace lb