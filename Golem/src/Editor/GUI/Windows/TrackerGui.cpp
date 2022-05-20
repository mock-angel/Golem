
#include <imgui.h>
#include <iostream>

#include <Core/Tracker/Tracker.h>

#include "TrackerGui.h"

void TrackerGui::OnGui()
{

    ImGui::Text("Tracker");
    std::map<std::string, std::list<TimerResult>> *timers = Tracker::instance.GetTrackerData();
    // std::list<TimerData> timerData = timers->;

    auto node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    std::cout << "index: "
              << "index" << std::endl;

    std::vector<std::string> keys;

    for (std::map<std::string, std::list<TimerResult>>::iterator it = timers->begin(); it != timers->end(); ++it)
    {
        keys.push_back(it->first);
        // value.push_back(it->second);
    }

    if (ImGui::TreeNode("Tracker"))
    {
        for (int index = 0; index < keys.size(); index++)
        {
            // std::cout << "index: " << index << std::endl;
            bool o = ImGui::TreeNode(keys.at(index).c_str());
            ImGui::Text("Called Previous Frame: %d", timers->at(keys.at(index)).size());
            if (o)
                ImGui::TreePop();
        }

        // ImGui::TreePop();
        ImGui::TreePop();
    }
}
