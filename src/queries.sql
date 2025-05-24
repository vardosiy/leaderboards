INSERT INTO leaderboard_metadata (name, owner_id, ordering)
VALUES ($name, $ownder_id, $ordering) RETURNING leaderboard_id;

INSERT INTO leaderboards (leaderboard_id, user_id, score, ts)
VALUES ($lb_id, $user_id, $score, $ts) ON CONFLICT (leaderboard_id, user_id) DO
UPDATE SET
    score = EXCLUDED.score,
    ts = EXCLUDED.ts;

--
DELETE
FROM leaderboard_metadata
WHERE leaderboard_id = $lb_id;

DELETE
FROM leaderboards
WHERE leaderboard_id = $lb_id
  AND user_id = $user_id;

--
SELECT user_id, score, ts, position
FROM (SELECT user_id,
             score,
             ts,
             ROW_NUMBER() OVER (ORDER BY score DESC, ts ASC) as position
      FROM
          leaderboards
      WHERE
          leaderboard_id = $lb_id) AS ranked_scores
WHERE position >= $START_POS
  AND position <= $END_POS
ORDER BY position ASC;