# Api

REST API will be hosted for needed actions.
REST is the choice and might be the best one, because overfetching won't be happening in this project.  

## Record entity

id
score
timestamp
*custom data // this will give me an opportunity to see how to change schema with existing data in DB

## Leaderboard entity

id
ordering
entries

## create_or_find_leaderboard
lb_name
-> ?

>should there be a separate table in the DB for each leaderboard?
nope, it is inefficient, partitioning will be used instead

## post_score
lb_name: str
user_id: str
score: int64
timestamp: int64
-> status_code

## remove_entry
lb_name: str
entry_id: str // BUT! using index here is dangerous, since some other entry might be pushed in that place while request for deleting is processed
-> status_code

## download_entries
lb_name: str
range_start: int
range_end: int
-> status_code + json(entries_array)

## download_player_entry
lb_name: str
user_id: str
-> status_code + json(player_entry)

## download_entries_around_player? // outside the scope of MVP

# Parts

## DB
 
A leaderboard might have only one entry from a user.

For the beginning I want to use Postgres. I may migrate to some other DB in the future.
`libpqxx` will be used here.

### Schema:

LEADERBOARD
id: int64 PK
leaderboard_id: str
user_id: str? //  not sure that INT is the only possible identifier, let it be string for now
score: int64 // time might be encoded, therefore I want to have more range for this element
ts: timestamp

LEADERBOARD_META
leaderboard_id: str
ordering: str // enum: ascending/descending
created_at: timestamp

## Cache

???

## Http

2 libs are good to be checked: `Crow` and `Drogon`
Need to be replaceable. Drogon will be used at the beginning.

## Service

Logic will be implemented without huge libs.

# Arch

There are actions that will be performed on the storage.
The application should have some `Repository` responsible for storing data in an abstract storage.

Commands might be received from some abstract sources, too.
CLI as well as HTTP need to be supported.

Service will be injected into API.
DB client will be injected into Service.
Testability will be easy due to these injection.

API will be responsible for converting 
versioning? at the beginning it will be a part of API, later it might be re-considered