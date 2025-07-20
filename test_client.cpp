#include <iostream>
#include <string>
#include <vector>

#include <sysrepo-cpp/Connection.hpp>
#include <sysrepo-cpp/Session.hpp>
#include <libyang-cpp/DataNode.hpp>

int main() {
    try {
        // 1) Connect to sysrepo and start a session on the RUNNING datastore
        sysrepo::Connection conn;
        auto session = conn.sessionStart();
        std::cout << "Session established\n";

        // 2) Load the top‑level <device> container
        const std::string container_path = "/modem-config:device";
        auto root = session.getData(container_path);
        if (!root) {
            std::cerr << "Failed to retrieve container at " << container_path << "\n";
            return 1;
        }

        // 3) Prepare the absolute XPaths of the leaves you inserted via XML
        std::vector<std::string> leaf_paths = {
            "/modem-config:device/ip-config/ip-model",
            "/modem-config:device/ip-config/ip-address",
            "/modem-config:device/ip-config/subnet-mask"
        };

        // 4) For each leaf, find it under the <device> container and print its value
        for (auto &xp : leaf_paths) {
            auto leaf = root->findPath(xp);
            if (!leaf) {
                std::cerr << "Leaf not found: " << xp << "\n";
                return 1;
            }
            // .asTerm() works only on leaf or leaf-list nodes
            std::cout << xp << " = "
                      << leaf->asTerm().valueStr()
                      << "\n";
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

