#include <iostream>
#include "snake_mp_util_JavaCallCTools.h"
#include "../include/String_convert.h"
#include "../include/AutoChat.h"

using namespace std;


JNIEXPORT void JNICALL Java_snake_mp_util_JavaCallCTools_init(JNIEnv *env, jobject _this) {
	init();
}

JNIEXPORT jstring JNICALL Java_snake_mp_util_JavaCallCTools_get_1reply(JNIEnv *env, jobject _this, jstring jstr) {
	const char *str = env->GetStringUTFChars(jstr, JNI_FALSE);
	if (str == NULL) return NULL;
	char reply[1024] = {};
	get_reply(str, reply);
	env->ReleaseStringUTFChars(jstr, str);
	return env->NewStringUTF(reply); /*如果JVM分配内存失败，则抛出OutOfMemoryError，并且返回NULL */
}

JNIEXPORT void JNICALL init() {
	cout << "call c init()" << endl;
	AutoChat::init();
}

JNIEXPORT void JNICALL get_reply(const char *str, char *reply) {
	cout << "call c get_reply() and recv " << String_convert::utf8_to_string(str) << endl;

	string s = AutoChat::get_reply(str);
	cout << "ok" << endl;
	strcpy_s(reply, 1024, s.c_str());
	cout << "ok" << endl;

}