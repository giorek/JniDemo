#include <jni.h>
#include <string>

/**
 * {@link http://blog.csdn.net/jiangwei0910410003/article/details/17465457}
 * http://blog.csdn.net/jiangwei0910410003/article/details/17466369
 * *env java环境变量，可以用于对java端进行操作，例如创建java端对象，方法调用，获取属性
 * jobject      若是static 则掉膘当前native方法的类的class对象实例
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
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_qiyi_androidjnidemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject obj/* this */) {
    std::string hello = "Hello from C++";

    //获取类名，属性名和方法名
    jclass java_claz = env->GetObjectClass(obj);
    jfieldID fieldId_property = env->GetFieldID(java_claz,"property","I");
    jmethodID methodId_function = env->GetMethodID(java_claz,"","(ILjava/util/Date;[I)I");




    jstring jStr = env->NewStringUTF("Hello C++");
    return env->NewStringUTF(hello.c_str());
}
