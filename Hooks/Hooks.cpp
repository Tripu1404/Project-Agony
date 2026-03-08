#include "Hooks.h"
#include "../SDK/ClientInstance.h"
#include "../Utils/Memory.h"
#include <MinHook.h>

DisplayClientMessage oDisplayClientMessage = nullptr;
bool messageSent = false;

// Dummy tick hook (ejemplo de función que corre a 60fps/20tps)
typedef void(__thiscall* ClientInstance_update)(void* _this);
ClientInstance_update oClientInstanceUpdate = nullptr;

void hClientInstanceUpdate(void* _this) {
    // Si no hemos enviado el mensaje de bienvenida, lo enviamos en el primer frame disponible
    if (!messageSent) {
        ChatUtil::sendMessage("¡Cliente by tripu1404 inyectado correctamente!");
        messageSent = true;
    }

    // Devolvemos la ejecución al juego normal
    return oClientInstanceUpdate(_this);
}

void InitializeHooks() {
    MH_Initialize();

    // 1. Buscamos la función del chat usando un patrón de bytes (Signature)
    // NOTA: Este patrón es un ejemplo. Debes buscar el real en IDA Pro para la v26.1
    uintptr_t chatFuncAddr = Memory::FindPattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC");
    oDisplayClientMessage = reinterpret_cast<DisplayClientMessage>(chatFuncAddr);

    // 2. Buscamos la función Update del juego para hookearla
    uintptr_t updateFuncAddr = Memory::FindPattern("48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8");
    
    // 3. Creamos el Hook
    MH_CreateHook((LPVOID)updateFuncAddr, &hClientInstanceUpdate, reinterpret_cast<LPVOID*>(&oClientInstanceUpdate));
    
    MH_EnableHook(MH_ALL_HOOKS);
}
