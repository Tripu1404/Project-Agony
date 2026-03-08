#include "Hooks.h"
#include "../SDK/ClientInstance.h"

// Variable para que el mensaje solo salga una vez
bool welcomeMessageSent = false;

// Usaremos el hook de tick que ya tienes en Hooks.h
void Hooks::Actor_baseTick(Entity* _this) {
    // Obtenemos el puntero original para que el juego siga funcionando
    auto* oTick = g_Hooks.Actor_baseTickHook->GetFastcall<void, Entity*>();
    
    // Si la entidad es el jugador local y no hemos enviado el mensaje
    if (_this->isLocalPlayer() && !welcomeMessageSent) {
        ClientInstance* ci = g_Data.getClientInstance(); // g_Data suele ser donde se guarda el CI
        if (ci != nullptr) {
            GuiData* gui = ci->getGuiData();
            if (gui != nullptr) {
                std::string msg = "§a[tripu1404] §fCliente inyectado correctamente.";
                gui->displayClientMessage(&msg);
                welcomeMessageSent = true;
            }
        }
    }

    oTick(_this);
}
