/*
 * GuiWindow.h
 *
 *  Created on: 06-May-2022
 *      Author: sujak
 */

#ifndef GUI_GUIWINDOW_H_
#define GUI_GUIWINDOW_H_

class GuiWindow {
public:
	GuiWindow();
	virtual ~GuiWindow();


	void onGui();
	void update();
	void render();
};

#endif /* GUI_GUIWINDOW_H_ */
