/*
 * GuiWindow.h
 *
 *  Created on: 06-May-2022
 *      Author: anantha
 */

#ifndef GUI_GUIWINDOW_H_
#define GUI_GUIWINDOW_H_

#include <string>

class GuiWindow
{
public:
	GuiWindow();
	GuiWindow(std::string title) { m_title = title; }
	virtual ~GuiWindow();

	virtual void updateGui();
	virtual void OnGui();
	virtual void update();
	virtual void render();

	void SetTitle(std::string title) { m_title = title; }

protected:
	bool m_isOpen = true;
	std::string m_title = "test";
};

#endif /* GUI_GUIWINDOW_H_ */
