#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "crow.h"

#define IPADDRESS 9095

std::vector<std::string> split(const std::string& s, char delim);

int main() {
    crow::SimpleApp app;

    std::mutex mtx;
    ;
    std::unordered_map<crow::websocket::connection*, std::string> userMap;

    CROW_ROUTE(app, "/ws")
        .websocket()
        .onopen([&](crow::websocket::connection& conn) {
            CROW_LOG_INFO << "New websocket connection";
            std::lock_guard<std::mutex> _(mtx);
            userMap[&conn] = conn.get_remote_ip();
        })
        .onclose([&](crow::websocket::connection& conn, const std::string& /*reason*/) {
            CROW_LOG_INFO << "Websocket connection closed";
            std::lock_guard<std::mutex> _(mtx);
            userMap.erase(&conn);
        })
        .onmessage([&](crow::websocket::connection& /*conn*/, const std::string& data, bool is_binary) {
            std::lock_guard<std::mutex> _(mtx);
            for (auto& user : userMap) {
                if (is_binary)
                    user.first->send_binary(data);
                else
                    // broadcast username along with the message
                    user.first->send_text(user.second + ": " + data);
            }
        });

    CROW_ROUTE(app, "/")
    ([] {
        char name[256];
        gethostname(name, 256);
        crow::mustache::context x;
        x["servername"] = name;
        auto page = crow::mustache::load("ws.html");
        return page.render(x);
    });

    /*
     *   TODO: add a route that loads the htmx.html web-page
     */
    CROW_ROUTE(app, "/htmx")
    ([] {
        char name[256];
        gethostname(name, 256);
        crow::mustache::context x;
        x["servername"] = name;
        auto page = crow::mustache::load("htmx.html");
        return page.render(x);
    });

    /*
     *   TODO: add a route that return an HTML string that says `hello from server`
     *   How can Crow just return a string and not a full HTML page?
     */
    CROW_ROUTE(app, "/send-message").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        std::unordered_map<std::string, std::string> body_map;

        // Split form data into key-value pairs for each input
        for (std::string& input : split(req.body, '&')) {
            std::vector<std::string> input_split = split(input, '=');
            std::pair<std::string, std::string> p(input_split[0], input_split[1]);
            body_map.insert(p);
        }

        // Instead of returning 'hello from server', I've made the app display
        // the user's name and message in the text box, similar to the
        // WebSockets implementation. The message won't show up if you log in
        // through another browser, though.
        return body_map["username"] + ": " + body_map["message"] + "\n";
    });

    app.port(IPADDRESS)
        .multithreaded()
        .run();
}

// https://stackoverflow.com/a/46931770
std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}