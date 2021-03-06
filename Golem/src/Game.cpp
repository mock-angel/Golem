/*
 * Game.cpp
 *
 *  Created on: 04-Sep-2020
 *      Author: anantha
 */

#include "Game.h"

#include "Debug.h"
#include "Window.h"

#include "Core/GUI/ImGuiLayer.h"
#include "Core/ProjectManager/ProjectManager.h"
#include "Components/ComponentTypeHolder.h"
#include "Res/Scripts/ComponentsAssembly.h"
#include "Editor/EditorLayer.h"
#include "Scene/SceneManager.h"
#include "Graphics/Camera/CameraManager.h"

/*
#include "NodeController.h"
#include "Components/ComponentController.h"
#include "Graphics/Shader/ShaderLoader.h"
#include "Core/Event/EventBus.h"
#include "Core/Input/InputSystem.h"
#include "Core/Layer/LayerManager.h"
#include "Core/ScriptingEngine/ScriptingEngine.h"
*/

namespace Golem {

std::weak_ptr<Window> Game::m_window;
std::weak_ptr<NodeController> Game::m_nodeControllerStatic;
std::weak_ptr<ComponentController> Game::m_componentControllerStatic;
std::weak_ptr<ShaderLoader> Game::m_shaderControllerStatic;
std::weak_ptr<EventBus> Game::m_eventBusStatic;
std::weak_ptr<InputSystem> Game::m_inputSystemStatic;

Game::Game() {
    Node::print("Game:: Constructing");

    /* Create Node Controller. */
    m_nodeManager = std::make_shared<NodeController>();
    m_nodeControllerStatic = m_nodeManager;

    /* Create Component Controller. */
    m_componentController = std::make_shared<ComponentController>();
    m_componentControllerStatic = m_componentController;

    m_shaderLoader = std::make_shared<ShaderLoader>();
    m_shaderControllerStatic = m_shaderLoader;
    m_shaderLoader->loadShaders();

    m_eventBus = std::make_shared<EventBus>();
    m_eventBusStatic = m_eventBus;

    //FIXME:: Camera is forced before LayerManager. Create and call a start() method.

    m_layerManager =  std::make_shared<LayerManager>();
    m_projectManager = std::make_shared<ProjectManager>();
    m_resourceManager = std::make_shared<ResourceManager>();
    m_componentTypeHolder = std::make_shared<ComponentTypeHolder>();
    m_sceneManager =  std::make_shared<SceneManager>();

    camera =  std::make_shared<Camera>();
    ComponentsAssembly::assemble();
}

Game::~Game() {
    // TODO Auto-generated destructor stub
}

void Game::update() {
    //Debug::log("Game::update() called");

   /* Update nodes. */
   m_nodeManager->updateNodes();

   /* Update components. */
   m_componentController->updateComponents();

   m_layerManager->update();
}

void Game::start() {

    //FIXME:: CameraManagerLayer must be created before ImGuiLayer.
    std::shared_ptr<CameraManager> camera_manager = std::make_shared<CameraManager>();
    m_layerManager->attachLayer(camera_manager);

    std::shared_ptr<ImGuiLayer> imgui_layer = std::make_shared<ImGuiLayer>();
    m_layerManager->attachLayer(imgui_layer);


}

void Game::render() {
    //Debug::log("Game::render() called");

    /* Render Components. */
    m_componentController->renderComponents();
    m_layerManager->render();

}

void Game::processEvents(){

    //m_eventBus->
}

void onEvent(Event& t_event){

}

} /* namespace Golem */
