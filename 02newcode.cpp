int onModuleChange(sr::Session session, const char *module_name,
                   sr::XPath xpath, sr::Event event, uint32_t request_id) {
    if (event == sr::Event::Change) {
        // Get changes for the specified module (XPath may be useful for filtering)
        auto changes = session.get_changes_iter("/my-module:*");

        for (auto &change : changes) {
            auto oper = change.oper;     // What operation (created, modified, etc.)
            auto newVal = change.new_value;

            // Example: Reject if "my-setting" has an invalid value
            if (newVal && std::string(newVal->xpath).find("my-setting") != std::string::npos) {
                std::string val = newVal->data()->get_string();
                if (val == "INVALID") {
                    std::cerr << "Invalid config detected! Rejecting..." << std::endl;
                    return 1; // Reject this change!
                }
            }
        }
    }

    return SR_ERR_OK; // Accept all other changes
}
