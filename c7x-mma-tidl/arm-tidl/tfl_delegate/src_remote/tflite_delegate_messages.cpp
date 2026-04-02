#include <set>
#include <dlfcn.h>
#include <packet.h>
#include <common.h>
#include "tensorflow/lite/c/common.h"

#include "tflite_delegate_messages.h"

void *lib = nullptr;
std::set<TfLiteDelegate *> delegates;

const char *errmsg = nullptr;
void error_report(const char *msg) { errmsg = msg; }

DECLARE_MSG(TFLITE_DELEGATE_SECTION + __COUNTER__, tflite_delegate_create_exec_delegate)
    std::unique_ptr<char *[]> keys = std::make_unique<char *[]>(m_options.size());
    std::unique_ptr<char *[]> values = std::make_unique<char *[]>(m_options.size());

    for(auto i = 0; i < m_options.size(); i++) {
        keys[i] = const_cast<char *>(m_options[i].first.c_str());
        values[i] = const_cast<char *>(m_options[i].second.c_str());
    }

    if(!lib) {
        lib = dlopen("libtidl_tfl_delegate.so", RTLD_LAZY | RTLD_GLOBAL);
        if(!lib) {
            p->m_status = -1;
            p->m_message = std::string("library load failed : ") + dlerror();
            return p;
        }
    }

    typedef TfLiteDelegate* (*create_t) (
            char** options_keys,
            char** options_values,
            size_t num_options,
            void (*error_handler)(const char*));
    create_t fxn = reinterpret_cast<create_t>(dlsym(lib, "tflite_plugin_create_delegate"));
    if(!fxn) {
        p->m_status = -1;
        p->m_message = std::string("library plugin create function load failed : ") + dlerror();
        return p;
    }

    TfLiteDelegate *delegate_ptr = fxn(keys.get(), values.get(), m_options.size(), error_report);
    std::string msg;
    if(errmsg) {
        msg = errmsg;
        errmsg = nullptr;
    }
    if(!delegate_ptr) {
        p->m_status = -1;
        p->m_message = msg;
        return p;
    }

    // We keep a record of delegates created, so that we dont fault if
    // someone sends a botched-up pointer in delete
    delegates.insert(delegate_ptr);
    p->m_status = 0;
    p->m_delegate_ptr = reinterpret_cast<uint64_t>(delegate_ptr);
    return p;
}

DECLARE_MSG(TFLITE_DELEGATE_SECTION + __COUNTER__, tflite_delegate_delete_exec_delegate)
    if(!lib) {
        lib = dlopen("libtidl_tfl_delegate.so", RTLD_LAZY | RTLD_GLOBAL);
        if(!lib) {
            p->m_status = -1;
            return p;
        }
    }

    typedef void (*delete_t)(TfLiteDelegate* delegate);
    delete_t fxn = reinterpret_cast<delete_t>(dlsym(lib, "tflite_plugin_destroy_delegate"));
    if(!fxn) {
        p->m_status = -1;
        return p;
    }
    
    TfLiteDelegate *delegate_ptr = reinterpret_cast<TfLiteDelegate *>(m_delegate_ptr);
    if(delegates.find(delegate_ptr) == delegates.end()) {
        p->m_status = -1;
        return p;
    }
    
    fxn(reinterpret_cast<TfLiteDelegate *>(m_delegate_ptr));
    delegates.erase(delegate_ptr);
    p->m_status = 0;
    return p;
}

void tflite_delegate_create_exec_delegate_req::write_to(std::ostream& output) const {
    packet::write<uint32_t>(output, m_options.size());
    for(auto it : m_options) {
        packet::write_string(output, it.first);
        packet::write_string(output, it.second);
    }
}

void tflite_delegate_create_exec_delegate_req::read_from(std::istream& input) {
    m_options = std::vector<std::pair<std::string, std::string>>(packet::read<uint32_t>(input));
    for(auto& it : m_options) {
        it.first = packet::read_string(input);
        it.second = packet::read_string(input);
    }
}
