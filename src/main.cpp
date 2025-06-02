#include <string>
#include <vector>

#include "common.hpp"
#include "repository.hpp"

#include "drogon/HttpAppFramework.h"
#include "drogon/HttpController.h"

namespace lb {

class Service {
public:
    explicit Service(std::unique_ptr<IRepository>&& repository);

    void create_lb(const std::string& name, Ordering ordering);
    void find_lb(const std::string& name& lb_id);
    void delete_lb(const LeaderboardId& lb_id);

    void apply_score(const LeaderboardId& lb_id, const UserId& user_id, Score score);
    void read_score(const LeaderboardId& lb_id, const UserId& user_id);
    void remove_entry(const LeaderboardId& lb_id, const UserId& user_id);

    void read_scores_range(const LeaderboardId& lb_id, int range_start, int range_end);

private:
    std::unique_ptr<IRepository> m_repository;  // persistent storage
    // TODO cache
};

class HttpApi : public drogon::HttpController<HttpApi> {
public:
    explicit HttpApi(IRepository& repo)
    : m_repository(&repo) {}

    METHOD_LIST_BEGIN
    METHOD_ADD(HttpApi::create_or_find_lb, "/leaderboards/v1/", drogon::Post);
    METHOD_ADD(HttpApi::find_lb, "/leaderboards/v1?name={name}", drogon::Get);
    METHOD_ADD(HttpApi::delete_lb, "/leaderboards/{lb_id}/v1/", drogon::Delete);

    METHOD_ADD(HttpApi::apply_score, "/leaderboards/{lb_id}/scores/{user_id}/v1/", drogon::Post);
    METHOD_ADD(HttpApi::get_score, "/leaderboards/{lb_id}/scores/{user_id}/v1/", drogon::Get);
    METHOD_ADD(HttpApi::delete_score, "/leaderboards/{lb_id}/scores/{user_id}/v1/", drogon::Delete);

    METHOD_ADD(HttpApi::get_scores,
               "/leaderboards/{lb_id}/scores/v1?from={start_idx}&to={finish_idx}",
               drogon::Get);
    METHOD_LIST_END

    void initialize();  // init http serv

    void run() {
        // host endpoints
        drogon::app().addListener("127.0.0.1", 8848).run();
    }

private:
    void create_or_find_lb(const drogon::HttpRequestPtr& req,
                           std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                           std::string&& userId,
                           const std::string& password) {
        Json::Value result;

        const std::string& error = req->getJsonError();
        if (!error.empty()) {
            result["error"] = error;
        }
        else {
            const auto& body_ptr = req->getJsonObject();
            if (body_ptr) {
                const Json::Value& body = *body_ptr;
                std::string lb_name = body["name"].asString();
                std::string ordering_str = body["ordering"].asString();
                auto ordering = from_string(ordering_str);
                m_listener->create_lb(lb_name, ordering.value());
            }
        }
        auto resp = drogon::HttpResponse::newHttpJsonResponse();

    }

private:
    Service* m_listener;
    IRepository* m_repository;
};

}  // namespace lb

int main() {
    std::unique_ptr<lb::IRepository> repository = lb::create_repository();
    lb::HttpApi api(*repository);
    api.initialize();
    api.run();
    return 0;
}