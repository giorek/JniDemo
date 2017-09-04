#include <jni.h>
#include <string>

#include <android/log.h>

/**
 * {@link http://blog.csdn.net/jiangwei0910410003/article/details/17465457}
 * http://blog.csdn.net/jiangwei0910410003/article/details/17466369
 * http://blog.csdn.net/virus026/article/details/25213157
 *
 * jni数组相关处理参考资料
 * http://blog.csdn.net/jiangwei0910410003/article/details/17653803
 *
 * *env java环境变量，可以用于对java端进行操作，例如创建java端对象，方法调用，获取属性
 * jobject      若是static 则代表当前native方法的类的class对象实例
 * jobject      若当前方法非static 则代表当前方法的类实例
 * jclass       jni中标识java端class
 * jfieldID     jni中标识java端属性
 * jmethodID    jni中标识java端方法
 *
 * env->GetFieldID      获取java端属性
 * env->GetStaticFieldID        获取java端静态属性
 * env->GetMethodID     获取java端方法
 * env->GetStaticMethodID       获取java端静态方法
 *
 * jmethodID GetMethodID(jclass clazz, const char* name, const char* sig)
 *      clazz    Java端全类名"."使用"/"替代
 *      name    java端方法名
 *      sig     签名即该方法返回的参数例如 空：()V String:()Ljava/lang/String
 *      sig     可以在命令行使用javap -s -p clazz.class 可以得到clazz的签名
 *
 *      java端类型对应的sign
 *      boolean     Z
 *      byte        B
 *      char        C
 *      short       S
 *      int         I
 *      long        L
 *      float       F
 *      double      D
 *      void        V
 *      object      Ljava/lang/String;   L+完整类名 使用"/"作为分隔符
 *      Array       [签名     [Ljava/lang/object;
 *      Method      (参数1签名,.....)返回值类型签名
 *
 *      实例可以参考下面定义
 *
 *
 * 常用命令行
 *  javah -jni clazz    根据java文件生成头文件
 */

#define TAG "JNI_DEMO"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__);
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__);
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG,__VA_ARGS__);
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,TAG,__VA_ARGS__);
#define LOGU(...) __android_log_print(ANDROID_LOG_UNKNOWN,TAG,__VA_ARGS__);

/**
 * 日志打印
 * __android_log_print(ANDROID_LOG_INFO,TAG,"hahahahahahahaha");
 * 调用jni.h中的方法，参数1：日志级别。参数2：相当于android日志打印中的tag值。参数3：需要打印出的信息
 * 也可以使用define进行替代
 * 前提是需要引入<android/log.h>头文件
 */

extern "C"
JNIEXPORT jstring JNICALL
Java_com_qiyi_androidjnidemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject obj/* this */) {
    std::string hello = "Hello from C++";

    //获取类名，属性名和方法名
    jclass java_claz = env->GetObjectClass(obj);
    jfieldID fieldId_property = env->GetFieldID(java_claz, "property", "I");
    jmethodID methodId_function = env->GetMethodID(java_claz, "function", "(ILjava/util/Date;[I)I");
    env->CallIntMethod(obj, methodId_function, 1, NULL, NULL);

    //调用java端参数为char的函数，返回值为void
    jmethodID methodId_arg_char = env->GetMethodID(java_claz, "function", "(C)V");
    const char c = 'K';
    env->CallVoidMethod(obj, methodId_arg_char, c);

    //调用java端参数为String的函数
    jmethodID methodId_arg_str = env->GetMethodID(java_claz, "functionString",
                                                  "(Ljava/lang/String;)V");
    jstring str = env->NewStringUTF("中华人民共和国万岁");
    env->CallVoidMethod(obj, methodId_arg_str, str);

    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_qiyi_androidjnidemo_MainActivity_callJniGetJavaInrAry(JNIEnv *env, jobject instance) {

    jclass claz = env->GetObjectClass(instance);
    jfieldID fileID_java_int_Array = env->GetFieldID(claz, "globalIntAry", "[I");
    jintArray obj_java_int_array = (jintArray) env->GetObjectField(instance, fileID_java_int_Array);
    jint array_len = env->GetArrayLength(obj_java_int_array);
    jint *int_array = env->GetIntArrayElements(obj_java_int_array, nullptr);
    for (int i = 0; i < array_len; i++) {
        LOGI("obj_java_int_array[%d]=%d", i, int_array[i]);
    }

    //创建本地数组，并将java端数组传递到这里
    jintArray local_int_ary = env->NewIntArray(array_len);
    jint *loacal_int = env->GetIntArrayElements(local_int_ary, nullptr);
    for (int j = 0; j < array_len; j++) {
        loacal_int[j] = int_array[j];
        LOGI("local_int_ary[%d]=%d", j, loacal_int[j]);
    }

    env->ReleaseIntArrayElements(obj_java_int_array, int_array, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_qiyi_androidjnidemo_MainActivity_callJniArgIntAry(JNIEnv *env, jobject instance,
                                                           jintArray array_) {
    /**
     *从java端传递的数据转换到c/c++中，参数2表示在做传递时是否进行拷贝，true，将上层数据做一份拷贝传递给本地代码。false:不做拷贝直接将java层数组的指针传递给本地代码
     * 需要和Release配对使用
     */
    jint *array = env->GetIntArrayElements(array_, NULL);
    //获取java层传递的数组长度
    jint ary_length = env->GetArrayLength(array_);
    LOGI("callJniArgIntAry length of array_:%d", ary_length);
    for (int i = 0; i < ary_length; i++) {
        LOGI("callJniArgIntAry array_:[%d]:%d", i, array[i]);
    }

    /**
     * 释放时参数一是java端数组，参数二是本地c/c++端数据，参数三是内存释放模式
     * 0：更新java端数据，并释放本地数组
     * JNI_COMMIT：对Java端数组进行更新但是不释放本地数组内存
     * JNI_ABORT：对java端数组不进行更新但是释放本地数组内存
     */
    env->ReleaseIntArrayElements(array_, array, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_qiyi_androidjnidemo_MainActivity_callJniArgChar(JNIEnv *env, jobject instance, jchar c) {

    LOGI("callJniArgChar c:%c", c);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_qiyi_androidjnidemo_MainActivity_callJniArgInt(JNIEnv *env, jobject instance, jint i) {

    LOGI("callJniArgInt i:%d", i);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_qiyi_androidjnidemo_MainActivity_callJniArgStr(JNIEnv *env, jobject instance,
                                                        jstring str_) {
    const char *str = env->GetStringUTFChars(str_, 0);
    LOGI("JniDemo run here %s:", str);
    env->ReleaseStringUTFChars(str_, str);
}