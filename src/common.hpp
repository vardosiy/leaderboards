#pragma once

#include <string>
#include <cstdint>

namespace lb {

using PlayerId = std::string;
using LeaderboardId = std::string;
using Score = uint64_t;
using Timestamp = uint64_t;

struct Entry {
    PlayerId id;
    Score score;
    Timestamp ts;
};

struct Leaderboard {
    enum Ordering {
        Ascending,
        Descending,
    };

    Ordering ordering;
    std::vector<Entry> entries;
};

} // namespace lb
