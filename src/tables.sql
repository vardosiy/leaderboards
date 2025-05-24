CREATE TYPE leaderboard_ordering AS ENUM ('ascending', 'descending');

CREATE TABLE leaderboard_metadata (
    leaderboard_id BIGSERIAL PRIMARY KEY,
    name VARCHAR(80) NOT NULL,
    owner_id INT NOT NULL,
    ordering leaderboard_ordering NOT NULL,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP NOT NULL
);
-- TODO
-- CREATE UNIQUE INDEX IF NOT EXISTS idx_leaderboard_metadata_name ON leaderboard_metadata(name);

CREATE TABLE leaderboards (
    leaderboard_id BIGINT NOT NULL,
    user_id BIGINT NOT NULL, -- mb UUID or VARCHAR?
    score BIGINT NOT NULL,
    ts TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP NOT NULL,
    PRIMARY KEY (leaderboard_id, user_id),
    FOREIGN KEY (leaderboard_id)
        REFERENCES leaderboard_metadata(leaderboard_id)
        ON DELETE CASCADE
); -- TODO: PARTITION BY HASH/LIST(leaderboard_id);
-- TODO
-- CREATE INDEX idx_leaderboards_desc ON scores (leaderboard_id, score DESC, ts ASC);
-- CREATE INDEX idx_leaderboards_asc ON scores (leaderboard_id, score ASC, ts ASC);

-- TODO
-- CREATE TABLE leaderboards_part_1 PARTITION OF leaderboards FOR VALUES WITH (MODULUS 2, REMAINDER 0);
-- CREATE TABLE leaderboards_part_2 PARTITION OF leaderboards FOR VALUES WITH (MODULUS 2, REMAINDER 1);
