/*
 * GuiWindow.cpp
 *
 *  Created on: 06-May-2022
 *      Author: anantha
 */

#include "GuiWindow.h"
#include "SOIL/SOIL.h"
#include <imgui.h>
#include <iostream>

GuiWindow::GuiWindow()
{
	m_isOpen = false;
	// TODO Auto-generated constructor stub
}

GuiWindow::~GuiWindow()
{
	// TODO Auto-generated destructor stub
}

void GuiWindow::updateGui()
{
	if (ImGui::Begin(m_title.c_str(), &m_isOpen))
	{
		onGui();
	}
	else
		m_isOpen = false;
	ImGui::End();
}
void GuiWindow::onGui() {}
void GuiWindow::update() {}
void GuiWindow::render() {}
