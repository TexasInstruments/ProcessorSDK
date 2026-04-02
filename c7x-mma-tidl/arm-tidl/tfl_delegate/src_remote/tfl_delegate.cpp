#include <vector>
#include <string>
#include <functional>
#include <experimental/filesystem>
#include <common.h>
#include <fs_utils.h>
#include "tensorflow/lite/c/common.h"

#include "tflite_delegate_messages.h"

namespace tflite {
namespace tfl_delegate {

/* Just a dummy function so that we can populate delegate->prepare
 * This information will be used by dladdr in tflite core to determine
 * who is this delegate for
 */
static TfLiteStatus DelegatePrepareDummy(TfLiteContext* context, TfLiteDelegate* delegate) {
    return kTfLiteOk;
}    

extern "C"{
TfLiteDelegate* tflite_plugin_create_delegate(char** options_keys,
                                              char** options_values,
                                              size_t num_options,
                                              void (*error_handler)(const char*)) {
    std::vector<std::pair<std::string, std::string>> options;
    std::string user_artifacts_folder;

    for (uint32_t idx = 0; idx < num_options; idx++) 
    {
        std::string option_key = options_keys[idx];
        std::string option_value = options_values[idx];

        /*
         * We just need to be a bit careful about what we pass as artifacts_folder
         * so convert the user supplied path to a canonical path and store it as
         * user supplied artifacts_folder. This is what we will use in send_dir() below
         *
         * However, when the delegate on the remote tries to access this path it should
         * be prepended by BASE because all paths are stored relative to it 
         */
        if (option_key == "artifacts_folder") {
            user_artifacts_folder = std::experimental::filesystem::canonical(option_value);
            option_value = std::string(BASE) + "/" + user_artifacts_folder;
        }

        /* create the option list from user supplied options, no judgements */
        options.push_back(std::make_pair<std::string, std::string>(std::move(option_key), std::move(option_value)));
    }

    if (user_artifacts_folder.empty()) {
        if (error_handler) error_handler("artifacts_folder must be provided");
        return nullptr;
    }

    try {
        send_dir(user_artifacts_folder.c_str());
        roundtrip(tflite_delegate_create_exec_delegate, options);
        if(resp.status()) {
            if (error_handler) error_handler(resp.message().c_str());
            return nullptr;
        }

        TfLiteDelegate* delegate = new TfLiteDelegate;
        delegate->data_ = reinterpret_cast<void*>(resp.delegate_ptr());
        delegate->flags = kTfLiteDelegateFlagsNone;
        delegate->Prepare = DelegatePrepareDummy;
        delegate->CopyFromBufferHandle = nullptr;
        delegate->CopyToBufferHandle = nullptr;
        delegate->FreeBufferHandle = nullptr;
        return delegate;
    } catch(std::exception &e) {
        if (error_handler) error_handler("internal error");
        return nullptr;
    }
}

void tflite_plugin_destroy_delegate(TfLiteDelegate* delegate) {
    if(delegate) {
        try {
            roundtrip(tflite_delegate_delete_exec_delegate, reinterpret_cast<uint64_t>(delegate->data_));
        } catch (std::exception &e) {
        }
    }
    delete delegate;
}

}
}
}
