/*
 * GameWorldDisplayWindowGui.h
 *
 *  Created on: 10-May-2022
 *      Author: anantha
 */

#ifndef CORE_GUI_WINDOW_GAMEWORLDDISPLAYWINDOWGUI_H_
#define CORE_GUI_WINDOW_GAMEWORLDDISPLAYWINDOWGUI_H_

#include <Core/Graphics/FrameBuffer.h>

#include <Core/GUI/Window/GuiWindow.h>

class GameWorldDisplayWindowGui : public GuiWindow
{
public:
	GameWorldDisplayWindowGui();
	virtual ~GameWorldDisplayWindowGui();

	void OnGui() override;
	void update() override;
	void render() override;

private:
	FrameBuffer m_frameBuffer;
};

#endif /* CORE_GUI_WINDOW_GAMEWORLDDISPLAYWINDOWGUI_H_ */
