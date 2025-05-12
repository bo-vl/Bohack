#pragma once
#include "Mapper.h"
#include "Mappings_vanilla_1_21_4.h"
#include "Mappings_fabric_1_21_4.h"

namespace Mappings
{
    enum class LoaderType {
        Vanilla,
        Fabric
    };

    inline void selectMappings(LoaderType loaderType = LoaderType::Vanilla)
    {
        Mapper::Mapper::clean();

        switch (loaderType) {
        case LoaderType::Vanilla:
            Mappings_vanilla_1_21_4::registerMappings();
            break;
        case LoaderType::Fabric:
            Mappings_fabric_1_21_4::registerMappings();
            break;
        default:
            throw std::runtime_error("Unknown loader type");
        }
    }
}