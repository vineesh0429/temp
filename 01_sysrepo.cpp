#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    try {
        // Establish connection and session
        sysrepo::Connection conn{};
        auto session = conn.sessionStart();

        // Subscribe to module change events for "example" module
        auto subscription = session.onModuleChange(
            "example",
            [](auto sess, const std::string &module_name, const std::string &xpath,
               sysrepo::Event event, uint32_t /* request_id */) {
                
                std::cout << "[v6] Event " << int(event)
                          << " on module " << module_name
                          << " at XPath " << xpath << "\n";

                if (event == sysrepo::Event::Change) {
                    // Retrieve all changes
                    for (auto &change : sess.getChanges()) {
                        std::cout << "  Change: " << change.asString() << "\n";
                    }
                }
                return sysrepo::ErrorCode::Ok;
            }
        );

        std::cout << "Sysrepo v6 subscriber running. Press Ctrl+C to exit.\n";
        // Loop indefinitely
        while (true) std::this_thread::sleep_for(std::chrono::seconds(1));

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
