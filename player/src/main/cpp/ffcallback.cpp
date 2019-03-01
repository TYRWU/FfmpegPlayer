//
// Created by Administrator on 2019/2/18.
//

#include "ffcallback.h"

FFCallBack::FFCallBack(JavaVM *javaVM, JNIEnv *jniEnv, jobject jobj) {
    this->javaVM = javaVM;
    this->jniEnv = jniEnv;
    this->jobj = jobj;

    //获取java方法methodId
    jclass jcls = jniEnv->GetObjectClass(jobj);
    if(jcls==NULL&&LOGDEBUG){
        LOGE("get class error")
        return;
    }
    this->jmethod_prepare = jniEnv->GetMethodID(jcls,"onPrepareCallBack","()V");
    this->jmethod_load = jniEnv->GetMethodID(jcls,"onLoadCallBack","(Z)V");
    this->jmethod_progress = jniEnv->GetMethodID(jcls,"onProgressCallBack","(II)V");
    this->jmethod_pause = jniEnv->GetMethodID(jcls,"onPauseCallBack","(Z)V");
}
FFCallBack::~FFCallBack() {

}

void FFCallBack::onPrepareCallBack(int type) {
    if(type==CALL_MAIN){
        this->jniEnv->CallVoidMethod(this->jobj,this->jmethod_prepare);
    } else if(type==CALL_CHILD){
        JNIEnv* jniEnv;
        if(this->javaVM->AttachCurrentThread(&jniEnv,0)!=JNI_OK){
            if(LOGDEBUG){
                LOGW("attach jniEnv failed!");
            }
            return;
        }
        jniEnv->CallVoidMethod(this->jobj,this->jmethod_prepare);
        this->javaVM->DetachCurrentThread();
    }

}

void FFCallBack::onLoadCallBack(int type,bool load) {
    if(type==CALL_MAIN){
        this->jniEnv->CallVoidMethod(this->jobj,this->jmethod_load,load);
    } else if(type==CALL_CHILD){
        JNIEnv* env;
        if(this->javaVM->AttachCurrentThread(&env,0)!=JNI_OK){
            if(LOGDEBUG){
                LOGW("attach jniEnv failed!");
                return;
            }
        }
        env->CallVoidMethod(this->jobj,this->jmethod_load,load);
        this->javaVM->DetachCurrentThread();
    }
}
void FFCallBack::onPauseCallBack(int type, bool status) {
    if(type==CALL_MAIN){
//        this->jniEnv->CallVoidMethod(this->jobj,this->jmethod_pause,status);
    } else if(type==CALL_CHILD){
        JNIEnv* env;
        if(this->javaVM->AttachCurrentThread(&env,0)!=JNI_OK){
            if(LOGDEBUG){
                LOGW("attach jniEnv failed!");
            }
            return;
        }
        env->CallVoidMethod(this->jobj,this->jmethod_pause,status);
    }
}

void FFCallBack::onProgressCallBack(int type, int currentTime, int totalTime) {
    if(type==CALL_MAIN){
        this->jniEnv->CallVoidMethod(this->jobj,this->jmethod_progress,currentTime,totalTime);
    } else if(type==CALL_CHILD){
        JNIEnv* env;
        if(this->javaVM->AttachCurrentThread(&env,0)!=JNI_OK){
            if(LOGDEBUG){
                LOGE("attach jniEnv failed!");
            }
            return;
        }
        env->CallVoidMethod(this->jobj,this->jmethod_progress,currentTime,totalTime);
        this->javaVM->DetachCurrentThread();
    }

}




