#pragma once
#include "Mapper.h"

namespace Mappings_vanilla_1_21_4
{
    constexpr static char mappings_vanilla_1_21_4[] = R"(
    {
        "net.minecraft.client.Minecraft": {
            "obfuscated": "flk",
            "fields": {
                "thePlayer": {
                    "obfuscated": "t",
                    "signature": "Lgkx;"
                }
            },
            "methods": {
                "getInstance": {
                    "obfuscated": "Q",
                    "signature": "()Lflk;"
                }
            },
        "net.minecraft.world.entity.Entity": {
            "obfuscated": "bum",
            "methods": {
                "setSprinting": {
                    "obfuscated": "h",
                    "signature": "(Z)V"
                }
            }
        }
    }
    )";
    inline void registerMappings()
    {
        Mapper::Mapper::registerMappings(mappings_vanilla_1_21_4);
    }
}