#pragma once
#include <jni.h>

class Minecraft {
public:
    Minecraft(_jobject* obj, JNIEnv_* env);
    static Minecraft getTheMinecraft(JNIEnv_* env);
    Minecraft GetPlayer();
	Minecraft SetSprinting(bool sprinting);

private:
    _jobject* object;
    JNIEnv_* env;
};