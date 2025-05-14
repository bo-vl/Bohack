#include "Minecraft.h"
#include "Mapper.h"
#include <iostream>

Minecraft::Minecraft(_jobject* obj, JNIEnv* env) {
    this->object = obj;
    this->env = env;
}

Minecraft Minecraft::getTheMinecraft(JNIEnv* env)
{
    Mapper::Mapper minecraftMapper("net.minecraft.client.Minecraft");

    std::string obfClass = minecraftMapper.getObfuscatedClassName();

    jclass minecraftClass = env->FindClass(obfClass.c_str());
    Mapper::Mapper::MethodInfo methodInfo = minecraftMapper.getMethodID("getInstance");
    jmethodID getInstanceMethod = env->GetStaticMethodID(minecraftClass, methodInfo.obfuscatedName.c_str(), methodInfo.signature.c_str());
    jobject minecraftObj = env->CallStaticObjectMethod(minecraftClass, getInstanceMethod);

    return Minecraft(minecraftObj, env);
}

Minecraft Minecraft::GetPlayer()
{
    Minecraft mc = getTheMinecraft(env);

    Mapper::Mapper minecraftMapper("net.minecraft.client.Minecraft");
    std::string obfClass = minecraftMapper.getObfuscatedClassName();

    jclass clazz = env->FindClass(obfClass.c_str());
    Mapper::Mapper::FieldInfo fieldInfo = minecraftMapper.getFieldID("thePlayer");
    jfieldID playerField = env->GetFieldID(clazz, fieldInfo.obfuscatedName.c_str(), fieldInfo.signature.c_str());
    jobject thePlayer = env->GetObjectField(mc.object, playerField);
    std::cerr << "Player instance: " << thePlayer << std::endl;

    return Minecraft(thePlayer, env);
}

Minecraft Minecraft::SetSprinting(bool sprinting)
{
    Minecraft player = GetPlayer();

    Mapper::Mapper entityMapper("net.minecraft.world.entity.Entity");
    std::string obfClass = entityMapper.getObfuscatedClassName();

    jclass entityClass = env->FindClass(obfClass.c_str());
    Mapper::Mapper::MethodInfo methodInfo = entityMapper.getMethodID("setSprinting");
    jmethodID setSprintingMethod = env->GetMethodID(entityClass, methodInfo.obfuscatedName.c_str(), methodInfo.signature.c_str());

    env->CallVoidMethod(player.object, setSprintingMethod, sprinting);

    return *this;
}
