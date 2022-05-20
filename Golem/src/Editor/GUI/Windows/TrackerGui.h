
#ifndef _GOLEM_TRACKER_GUI_H_
#define _GOLEM_TRACKER_GUI_H_

#include <Core/Tracker/Tracker.h>
#include <Core/Gui/Window/GuiWindow.h>
#include <map>
#include <list>

class TrackerGui : public GuiWindow
{
    std::map<std::string, std::list<TimerResult>> m_trackerData;

public:
    TrackerGui() : GuiWindow("Tracker"){};
    ~TrackerGui() {}

    // void OnUpdate() override;
    void OnGui() override;
    // void OnEvent(SDL_Event &e) override;

    // void OnOpen() override;
    // void OnClose() override;

    // void OnInit() override;

    // void OnResize(int width, int height) override;

    // void OnMouseMove(int x, int y) override;
    // void OnMouseDown(int button, int x, int y) override;
    // void OnMouseUp(int button, int x, int y) override;
    // void OnMouseWheel(int x, int y) override;

    // void OnKeyDown(int key, int scancode, int mods) override;
    // void OnKeyUp(int key, int scancode, int mods) override;
    // void OnTextInput(const char *text) override;

    // void OnFileDrop(const char *file) override;

    // void OnCloseRequest() override;

    // void OnFocus() override;
    // void OnFocusLost() override;

    // void OnMinimize() override;
    // void OnMaximize() override;
    // void OnRestore() override;
};

#endif