#include "SKSE/Interfaces.h"
#include "Logging.h"
#include "Events.h"
#include "FormLookup.h"
#include "Settings.h"

void Listener(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
        Events::OnSpellCastEventHandler::Register();
        FormLookup::LoadSpell();
        Settings::LoadSettings();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    InitializeLogging();
    const auto* plugin = SKSE::PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    logger::info("{} {} is loading...", plugin->GetName(), version);
    Init(skse);

    const auto messaging = SKSE::GetMessagingInterface();
    if (!messaging->RegisterListener(Listener)) return false;

    logger::info("{} has finished loading.", plugin->GetName());

    return true;
}
