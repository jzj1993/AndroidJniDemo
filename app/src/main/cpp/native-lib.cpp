#include <jni.h>
#include <string>
#include <android/log.h>

#define  LOG_TAG    "Native"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

void callJavaExceptionMethod(JNIEnv *env, jobject thisObj) {
    jclass cls = env->FindClass("com/demo/jnidemo/MainActivity");
    jmethodID method = env->GetMethodID(cls, "exceptionMethod", "()V");
    env->CallVoidMethod(thisObj, method);
}

void callJavaNormalMethod(JNIEnv *env, jobject thisObj) {
    jclass cls = env->FindClass("com/demo/jnidemo/MainActivity");
    jmethodID method = env->GetMethodID(cls, "normalMethod", "()V");
    env->CallVoidMethod(thisObj, method);
}

void throwJavaException(JNIEnv *env, const char *msg) {
    jclass cls = env->FindClass("java/lang/RuntimeException");
    env->ThrowNew(cls, msg);
}

extern "C" JNIEXPORT void JNICALL
Java_com_demo_jnidemo_MainActivity_nativeMethod(JNIEnv *env, jobject thisObj, jint i) {

    switch ((int) i) {
        case 1: {
            // test jni basic
            LOGD("c++ 1");
            int d = 2 - 1;
            LOGD("val = %d", (4 / d));
            callJavaNormalMethod(env, thisObj);
            LOGD("c++ 2");
            break;
        }
        case 2: {
            // native crash and unhandled
            LOGD("c++ 1");
            int d = 1 - 1;
            // crash here without telling the details, and java can not catch:
            // Fatal signal 8 (SIGFPE), code 1 (FPE_INTDIV), fault addr 0xc3aa9f51 in tid 9992 (om.demo.jnidemo), pid 9992 (om.demo.jnidemo)
            LOGD("val = %d", (4 / d));
            LOGD("c++ 2");
            break;
        }
        case 3: {
            // native call java and exception occurred
            LOGD("c++ 1");
            callJavaExceptionMethod(env, thisObj);
            LOGD("c++ 2, %ld", (long) env->ExceptionOccurred());
            LOGD("c++ 3"); // continue running
            // java catch the error
            break;
        }
        case 4: {
            // native call java and exception occurred, call java again
            LOGD("c++ 1");
            callJavaExceptionMethod(env, thisObj);
            LOGD("c++ 2, %ld", (long) env->ExceptionOccurred());
            LOGD("c++ 3"); // continue running
            // crash here and JVM can tell the Java exception details:
            // JNI DETECTED ERROR IN APPLICATION: JNI FindClass called with pending exception java.lang.RuntimeException: java runtime exception
            // at void com.demo.jnidemo.MainActivity.exceptionMethod() (MainActivity.java:60)
            callJavaNormalMethod(env, thisObj); // crash here:
            LOGD("c++ 4");
            break;
        }
        case 5: {
            // native call java and exception occurred, clear exception, call java again
            LOGD("c++ 1");
            callJavaExceptionMethod(env, thisObj);
            LOGD("c++ 2, %ld", (long) env->ExceptionOccurred());
            env->ExceptionClear();
            LOGD("c++ 3"); // continue running
            callJavaNormalMethod(env, thisObj); // continue running
            LOGD("c++ 4");
            break;
        }
        case 6: {
            // native throw error explicitly
            LOGD("c++ 1");
            throwJavaException(env, "native error 1");
            LOGD("c++ 2, %ld", (long) env->ExceptionOccurred());
            LOGD("c++ 3"); // continue running
            // java catch the error
            break;
        }
        case 7: {
            // native throw error explicitly, and throw again
            LOGD("c++ 1");
            throwJavaException(env, "native error 1");
            LOGD("c++ 2, %ld", (long) env->ExceptionOccurred());
            // crash here and JVM can tell the Java exception details:
            // JNI DETECTED ERROR IN APPLICATION: JNI ThrowNew called with pending exception java.lang.RuntimeException: native error 1
            throwJavaException(env, "native error 2");
            LOGD("c++ 3, %ld", (long) env->ExceptionOccurred());
            break;
        }
        case 8: {
            // native throw error explicitly, clear, and throw again
            LOGD("c++ 1");
            throwJavaException(env, "native error 1");
            LOGD("c++ 2, %ld", (long) env->ExceptionOccurred()); // error occurred is non-null
            env->ExceptionClear(); // native error 1 is cleared
            throwJavaException(env, "native error 2"); // continue running
            LOGD("c++ 3, %ld", (long) env->ExceptionOccurred()); // error occurred is non-null
            LOGD("c++ 4"); // continue running
            // java catch the native error 2
            break;
        }
        case 9: {
            // describe exception
            LOGD("c++ 1");
            throwJavaException(env, "native error 1");
            LOGD("c++ 2");
            env->ExceptionDescribe(); // print to the Java System.err
            LOGD("c++ 3");
            // java catch the native error 1
            break;
        }
        default:
            break;
    }
}