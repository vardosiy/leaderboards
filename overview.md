# Leaderboard service (LB)

## Overview

Targeting highly loaded LBs, like for Trackmania or generic implementations like Steam LBs.

Usage:
1. Creating/deleting LBs
2. Adding/removing/modifying entries
3. Reading in batches

For MVP, support for custom data is not necessary. However, when the service will grow, users should be able to attach some user data (like reference to a recording, like in trackmania).
Auth is also outside of the scope of this service. Mb some auth mechanism will be introduced in the future.

The target platform is Linux, but the service need to be runnable on Windows, too. // This requirement might be removed if some Linux-specific networking will be used for the implementation.

## Non-functional requirements / constrains / performance assumptions

The entries at the top of LB should be updated quickly and instantly visible to any reader. // I want to try both: strict consistency and eventual consistency
// This part gives a motivation to experiment with implementation

More relaxed handling for big LBs.
What is considered a large LB? >100k entries?
Strategy adjustments should be done automatically or set on creation of a LB?

5-10k active users submitting data to the LB
Amount of data per record is tiny, not more than 1kb.

## Assumptions about the data

Write requests are expected to be spread across the whole range of a LB.
Reads will mostly be happening for the top of a LB.
Occasional reads for getting records around a player.

## System design overview

Cache for the most frequently accessed records.
Relational DB, because the data will have a consistent format.
Services executing requests from users.
Load balancers will not be used in the MVP.

Service will be accessible via HTTP.

## Tech stack

The latest version of C++ and Rust should be used for implementation of this service.
RDB?

C++: anything
Rust: tokio

## Tech requirements

Ability to work under heavy load.
Performance testing.