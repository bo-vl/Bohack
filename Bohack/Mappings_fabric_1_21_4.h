#pragma once
#include "Mapper.h"
namespace Mappings_fabric_1_21_4
{
    constexpr static char mappings_fabric_1_21_4[] = R"(
    {
        "net.minecraft.client.Minecraft": {
            "obfuscated": "net/minecraft/class_310",
            "fields": {
                "thePlayer": {
                    "obfuscated": "field_1724",
                    "signature": "Lnet/minecraft/client/player/LocalPlayer;"
                }
            },
            "methods": {
                "getInstance": {
                    "obfuscated": "method_1551",
                    "signature": "()Lnet/minecraft/class_310;"
                }
            }
        }
    }
    )";

    inline void registerMappings()
    {
        Mapper::Mapper::registerMappings(mappings_fabric_1_21_4);
    }
}