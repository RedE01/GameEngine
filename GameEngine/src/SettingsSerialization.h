#pragma once

namespace GameEngine {

    class Application;

    namespace SettingsSerialization {

        void saveSettingsToFile(const char* filepath, Application* application);
        void updateSettingsFromFile(const char* filepath, Application* application);

    }

}
