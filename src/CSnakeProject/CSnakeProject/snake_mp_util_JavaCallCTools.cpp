#include <iostream>
#include "snake_mp_util_JavaCallCTools.h"

using namespace std;

JNIEXPORT void JNICALL Java_snake_mp_util_JavaCallCTools_init(JNIEnv *env, jobject _this) {
	cout << "call c init()" << endl;
}

JNIEXPORT jstring JNICALL Java_snake_mp_util_JavaCallCTools_get_1reply(JNIEnv *env, jobject _this, jstring jstr) {
	const char *str = env->GetStringUTFChars(jstr, JNI_FALSE);
	if (str == NULL) return NULL;
	std::cout << "get_reply recv " << str;
	env->ReleaseStringUTFChars(jstr, str);
	return env->NewStringUTF(u8"c return str"); /*如果JVM分配内存失败，则抛出OutOfMemoryError，并且返回NULL */
}
