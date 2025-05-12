#pragma once
#include <unordered_map>
#include <string>
#include <json.hpp>
#include <jni.h>

namespace Mapper {
    class Mapper {
    public:
        struct FieldInfo {
            jfieldID fieldID = nullptr;
            std::string obfuscatedName;
            std::string signature;
        };

        struct MethodInfo {
            jmethodID methodID = nullptr;
            std::string obfuscatedName;
            std::string signature;
        };

        static bool init();
        static void clean();
        static void registerMappings(const std::string& jsonMappings);
        static jclass findClass(const std::string& class_path, JNIEnv* env);

        Mapper(const std::string& class_path);
        void reload();
        jclass get_jclass(JNIEnv* env);

        FieldInfo getFieldID(const std::string& name) const;
        MethodInfo getMethodID(const std::string& name) const;

        std::string getObfuscatedClassName() const;
        std::string getObfuscatedFieldName(const std::string& name) const;
        std::string getObfuscatedMethodName(const std::string& name) const;
        std::string getObfuscatedFieldSig(const std::string& name) const;
        std::string getObfuscatedMethodSig(const std::string& name) const;

    private:
        struct JavaClassData {
            std::unordered_map<std::string, FieldInfo> fields{};
            std::unordered_map<std::string, MethodInfo> methods{};
        };

        static nlohmann::json mappings;
        static std::unordered_map<std::string, JavaClassData> data;

        std::string class_path;
    };
}