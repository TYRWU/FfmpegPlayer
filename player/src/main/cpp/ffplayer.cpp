#include <string>
#include "Ffmpeg.h"

using namespace std;

JavaVM *javaVM;
FfCallBack *ffCallBack;
Ffmpeg *ffmpeg;

/**
 * 获取JavaVM
 * 必须返回版本号
 */
extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    JNIEnv *jniEnv;
    if (vm->GetEnv((void **) (&jniEnv), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hechuangwu_player_ffplayer_FfPlayer__1prepare(JNIEnv *env, jobject instance,
                                                       jstring filePath_) {
    const char *filePath = env->GetStringUTFChars(filePath_, 0);
    if (ffmpeg == NULL) {
        if (ffCallBack == NULL) {
            //对象不能跨线程使用，需生成全局变量
            ffCallBack = new FfCallBack(javaVM, env, env->NewGlobalRef(instance));
        }

        ffmpeg = new Ffmpeg(ffCallBack, filePath);
        ffmpeg->prepare();
    }


    env->ReleaseStringUTFChars(filePath_, filePath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hechuangwu_player_ffplayer_FfPlayer__1start(JNIEnv *env, jobject instance) {
    if(ffmpeg!=NULL){
        ffmpeg->start();
    }

}