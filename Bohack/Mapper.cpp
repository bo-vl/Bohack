#include <iostream>
#include <fstream>
#include "Mapper.h"

namespace Mapper
{
    nlohmann::json Mapper::mappings;
    std::unordered_map<std::string, Mapper::JavaClassData> Mapper::data;

    bool Mapper::init()
    {
        try
        {
            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error initializing Mapper: " << e.what() << std::endl;
            return false;
        }
    }

    void Mapper::clean()
    {
        data.clear();
        mappings.clear();
    }

    void Mapper::registerMappings(const std::string& jsonMappings)
    {
        try
        {
            auto newMappings = nlohmann::json::parse(jsonMappings);

            mappings.merge_patch(newMappings);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error registering mappings: " << e.what() << std::endl;
        }
    }

    jclass Mapper::findClass(const std::string& class_path, JNIEnv* env)
    {
        try {
            std::string obfName;

            if (mappings.contains(class_path) && mappings[class_path].contains("obfuscated"))
            {
                obfName = mappings[class_path]["obfuscated"].get<std::string>();
            }
            else
            {
                obfName = class_path;
            }

            jclass clazz = env->FindClass(obfName.c_str());
            if (!clazz) {
                std::cerr << "Failed to find class with obfuscated name: " << obfName << std::endl;
                clazz = env->FindClass(class_path.c_str());
            }
            return clazz;
        }
        catch (...) {
            std::cerr << "Error retrieving class: " << class_path << std::endl;
            return env->FindClass(class_path.c_str());
        }
    }

    Mapper::Mapper(const std::string& class_path) : class_path(class_path) {}

    void Mapper::reload()
    {
        data.erase(class_path);
    }

    jclass Mapper::get_jclass(JNIEnv* env)
    {
        return findClass(class_path, env);
    }

    Mapper::FieldInfo Mapper::getFieldID(const std::string& name) const
    {
        auto it = data.find(class_path);
        if (it != data.end() && it->second.fields.count(name)) {
            return it->second.fields.at(name);
        }

        FieldInfo info;
        try {
            if (mappings.contains(class_path) &&
                mappings[class_path].contains("fields") &&
                mappings[class_path]["fields"].contains(name))
            {
                info.obfuscatedName = getObfuscatedFieldName(name);
                info.signature = getObfuscatedFieldSig(name);
                info.fieldID = nullptr;
            }
            else {
                throw std::runtime_error("Field not found in mappings: " + name);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error getting field info for " << name << ": " << e.what() << std::endl;
            throw std::runtime_error("Field info not found for: " + name);
        }

        return info;
    }

    Mapper::MethodInfo Mapper::getMethodID(const std::string& name) const
    {
        auto it = data.find(class_path);
        if (it != data.end() && it->second.methods.count(name)) {
            return it->second.methods.at(name);
        }

        MethodInfo info;
        try {
            if (mappings.contains(class_path) &&
                mappings[class_path].contains("methods") &&
                mappings[class_path]["methods"].contains(name))
            {
                info.obfuscatedName = getObfuscatedMethodName(name);
                info.signature = getObfuscatedMethodSig(name);

                JNIEnv* env = nullptr;
                JavaVM* jvm;
                jint result = JNI_GetCreatedJavaVMs(&jvm, 1, nullptr);
                if (result != JNI_OK || jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
                    throw std::runtime_error("Failed to get JNIEnv.");
                }

                jclass clazz = findClass(class_path, env);
                if (!clazz) throw std::runtime_error("Failed to find class: " + class_path);

                info.methodID = env->GetStaticMethodID(clazz, info.obfuscatedName.c_str(), info.signature.c_str());
                if (!info.methodID) {
                    info.methodID = env->GetMethodID(clazz, info.obfuscatedName.c_str(), info.signature.c_str());
                    if (!info.methodID) {
                        throw std::runtime_error("Method not found: " + info.obfuscatedName);
                    }
                }

                data[class_path].methods[name] = info;
            }
            else {
                throw std::runtime_error("Method not found in mappings: " + name);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error getting method info for " << name << ": " << e.what() << std::endl;
            throw;
        }

        return info;
    }

    std::string Mapper::getObfuscatedClassName() const
    {
        return mappings.at(class_path)["obfuscated"].get<std::string>();
    }

    std::string Mapper::getObfuscatedFieldName(const std::string& name) const
    {
        return mappings.at(class_path)["fields"][name]["obfuscated"].get<std::string>();
    }

    std::string Mapper::getObfuscatedMethodName(const std::string& name) const
    {
        return mappings.at(class_path)["methods"][name]["obfuscated"].get<std::string>();
    }

    std::string Mapper::getObfuscatedFieldSig(const std::string& name) const
    {
        return mappings.at(class_path)["fields"][name]["signature"].get<std::string>();
    }

    std::string Mapper::getObfuscatedMethodSig(const std::string& name) const
    {
        return mappings.at(class_path)["methods"][name]["signature"].get<std::string>();
    }
}