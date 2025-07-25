#include <sysrepo-cpp/Session.hpp>
#include <iostream>

int on_module_change(sr_session_ctx_t* session,
                     const char* module_name,
                     const char* xpath,
                     sr_event_t event,
                     uint32_t request_id,
                     void* private_ctx) {

    if (event == SR_EV_CHANGE) {
        S_Session sess(new sr::Session(session));
        S_ChangeIter iter = sess->get_changes_iter("/my-module:*");

        sr_change_oper_t oper;
        S_Val old_val;
        S_Val new_val;

        while (sess->get_change_next(iter, &oper, old_val, new_val) == SR_ERR_OK) {
            if (new_val && std::string(new_val->xpath()).find("my-setting") != std::string::npos) {
                std::string value = new_val->data()->get_string();

                if (value == "INVALID") {
                    std::cerr << "Rejecting invalid config: " << value << std::endl;
                    return SR_ERR_VALIDATION_FAILED;  // Reject config
                }
            }
        }
    }

    return SR_ERR_OK;  // Accept change
}

