
#include <Window/Window.h>

namespace Golem
{
    class EditorWindow : public Window
    {
        void onOpen() override;

        // void onGui() override;
        void onUpdate() override;
        void onGui() override;
        void onEvent(SDL_Event &e) override;

        GuiManager m_guiManager;
    };
} // namespace Golem
