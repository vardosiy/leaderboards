#include <string>
#include <vector>

#include "common.hpp"
#include "repository.hpp"

namespace lb {

class Service {
public:
    void create_lb(const LeaderboardId& lb_id);
    void delete_lb(const LeaderboardId& lb_id);

    void add_entry(const LeaderboardId& lb_id, const UserId& user_id, Score score);
    void read_entries(const LeaderboardId& lb_id, int range_start, int range_end);
    void remove_entry(const LeaderboardId& lb_id, const UserId& user_id);
};

class Api {
public:
    Api(Service& listener) :
    m_listener(&listener) {}

    void initialize(); // init http serv

    void run() {
        // host endpoints
        m_listener->create_lb("test");
    }

private:
    Service* m_listener;
};

}

int main() {
    lb::Service service;
    lb::Api api(service);
    api.initialize();
    api.run();
}