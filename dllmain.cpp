#include <Windows.h>
#include "Hooks/Hooks.h"
#include <iostream>

void MainThread(HMODULE hModule) {
    // Abrimos una consola de depuración en segundo plano (muy útil para testear)
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "[+] Tripu1404 Client Iniciando..." << std::endl;

    // Inicializamos el sistema de Hooks
    InitializeHooks();

    std::cout << "[+] Hooks aplicados. Esperando en el juego..." << std::endl;

    // Bucle principal (Mantiene el DLL vivo hasta que presiones END)
    while (!GetAsyncKeyState(VK_END)) {
        Sleep(50);
    }

    // Proceso de apagado (Eject)
    std::cout << "[-] Desactivando cliente..." << std::endl;
    // MH_DisableHook(MH_ALL_HOOKS); (Asumiendo que hiciste el include de MinHook)
    // MH_Uninitialize();
    
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule); // Optimización estándar
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
    }
    return TRUE;
}
