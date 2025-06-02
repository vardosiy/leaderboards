#pragma once

#include <string>

namespace lb::queries {

static constexpr std::string k_create_lb_query =
    "INSERT INTO leaderboard_metadata (name, owner_id, ordering)"
    " VALUES ($1, $2, $3) RETURNING leaderboard_id;";

static constexpr std::string k_delete_lb_query =
    "DELETE FROM leaderboard_metadata WHERE leaderboard_id = $lb_id;";
static constexpr std::string k_delete_user_score_query =
    "DELETE FROM leaderboards WHERE leaderboard_id = $lb_id AND user_id = $user_id;";

static constexpr std::string k_set_user_score_query =
    "INSERT INTO leaderboards (leaderboard_id, user_id, score, ts)"
    " VALUES ($lb_id, $user_id, $score, $ts) ON CONFLICT (leaderboard_id, user_id) DO"
    " UPDATE SET score = EXCLUDED.score, ts = EXCLUDED.ts;";

static constexpr std::string k_read_entries_query = R"(
SELECT user_id, score, ts, position
FROM (SELECT user_id, score, ts,
             ROW_NUMBER() OVER (ORDER BY score DESC, ts ASC) as position
      FROM leaderboards
      WHERE leaderboard_id = $lb_id) AS ranked_scores
WHERE position >= $START_POS AND position <= $END_POS
ORDER BY position ASC;
)";

}  // namespace lb::queries