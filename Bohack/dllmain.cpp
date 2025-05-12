#include <Windows.h>
#include <thread>
#include <jni.h>
#include "Mapper.h"
#include "Minecraft.h"
#include "MappingsManager.h"

void MainThread(HMODULE instance) {
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
    printf("Injecting Bohack...\n");
    JavaVM* jvm = nullptr;
    JNIEnv* env = nullptr;
    if (JNI_GetCreatedJavaVMs(&jvm, 1, nullptr) != JNI_OK) {
        printf("Failed to get Java VM\n");
        return;
    }
    if (jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr) != JNI_OK) {
        printf("Failed to attach to JVM\n");
        return;
    }

    Mappings::selectMappings(Mappings::LoaderType::Vanilla);

    Mapper::Mapper::init();
    try {
        Minecraft mc = Minecraft::getTheMinecraft(env);
        Minecraft plr = mc.GetPlayer();

		Minecraft sprint = plr.SetSprinting(true);
		printf("Sprinting set to true\n");
    }
    catch (const std::exception& e) {
        printf("Failed to get Minecraft instance: %s\n", e.what());
    }
    printf("Attached to JVM\n");

    jvm->DetachCurrentThread();
    FreeConsole();
}

bool APIENTRY DllMain(HMODULE instance, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);
        std::thread(MainThread, instance).detach();
    }
    return true;
}