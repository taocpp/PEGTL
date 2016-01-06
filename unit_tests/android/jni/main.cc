#include <jni.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <android/log.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>

#include <pegtl.hh>

namespace pegtl
{
   std::size_t failed = 0;
   std::vector< std::pair< std::string, std::string > > applied;

} // pegtl

#include "test_action.hh"
#include "test_assert.hh"
#include "test_control.hh"
#include "test_failed.hh"

#include "verify_rule.hh"
#include "verify_char.hh"
#include "verify_fail.hh"

#include "verify_analyze.hh"

#include "unittest.cc"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "PEGTL", __VA_ARGS__ ))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "PEGTL", __VA_ARGS__ ))

void android_main(struct android_app* state)
{
    app_dummy();

    ANativeActivity* nativeActivity = state->activity;
    const char* externalDataPath = nativeActivity->externalDataPath;
    const char* internalDataPath = nativeActivity->internalDataPath;
    std::string inDataPath( internalDataPath );
    std::string exDataPath( externalDataPath );

    LOGI("Copying asset files.");
    const int buffer_size = 255;
    AAssetManager* assetManager = state->activity->assetManager;
    AAssetDir* assetDir = AAssetManager_openDir(assetManager, "");
    const char* filename = (const char*)NULL;
    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) {
        LOGI("Asset file %s.", filename);
        AAsset* asset = AAssetManager_open(assetManager, filename, AASSET_MODE_STREAMING);
        char buf[buffer_size];
        int nb_read = 0;
        std::string filename_str(filename);
        std::string filepath = inDataPath + "/" + filename_str;
        FILE* out = fopen(filepath.c_str(), "w");
        while ((nb_read = AAsset_read(asset, buf, buffer_size)) > 0) {
            fwrite(buf, nb_read, 1, out);
        }
        fclose(out);
        AAsset_close(asset);
    }
    AAssetDir_close(assetDir);
    LOGI("Copying of asset files completed.");

    LOGI("Starting unit test.");
    pegtl::unit_test(inDataPath);
    if (pegtl::failed) {
        LOGI("Unittest failed.");
    }
    else {
        LOGI("Unittest passed.");
    }
    LOGI("Done running unit test.");

    ANativeActivity_finish(nativeActivity);
}
