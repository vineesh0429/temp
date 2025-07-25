#include <sysrepo-cpp/Session.hpp>
#include <iostream>

int on_module_change(sysrepo::Session session,
                     const char* module_name,
                     const char*,
                     sysrepo::Event event,
                     uint32_t,
                     void*) {

    if (event == sysrepo::Event::Change) {
        auto iter = session.getChangeIter("/my-module:*");

        while (auto change = session.getChangeNext(iter)) {
            auto op = change->operation();

            // Get the new value node (may be nullptr if Deleted)
            auto node = change->node();
            if (!node) continue;

            auto path = node->path();
            auto value = node->asString();

            if (path.find("my-setting") != std::string::npos && value == "INVALID") {
                std::cerr << "Invalid value detected for " << path << ": " << value << std::endl;
                return 1;  // Reject the change
            }
        }
    }

    return SR_ERR_OK;  // Accept the change
}


