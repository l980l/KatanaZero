#include "pch.h"
#include "PrefabUI.h"

#include "ParamUI.h"
#include <Engine\CPrefab.h>
#include "CLevelSaveLoad.h"

PrefabUI::PrefabUI()
    :ResUI(RES_TYPE::PREFAB)
{
    SetName("Prefab");
}

PrefabUI::~PrefabUI()
{
}

int PrefabUI::render_update()
{
    ResUI::render_update();

    // Prefab 이름
    ImGui::Text("Prefab          ");
    ImGui::SameLine();

    Ptr<CPrefab> pPrefab = (CPrefab*)GetTargetRes().Get();
    string strKey = string(pPrefab->GetKey().begin(), pPrefab->GetKey().end());
    ImGui::InputText("##PrefabUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    // Save 버튼
    ImGui::Text("Save            ");
    ImGui::SameLine();
    static char szEmtpy[30] = {};
    ImGui::InputText("##PrefabNewName", szEmtpy, 30);
    ImGui::SameLine();

    if (ImGui::Button("##PrefabSaveButton", ImVec2(18.f, 18.f)))
    {
        string strTemp = szEmtpy;
        wstring wstrTemp = wstring(strTemp.begin(), strTemp.end());
        
        CLevelSaveLoad::SavePrefab(L"prefab\\" + wstrTemp + L".pref", (CPrefab*)GetTargetRes().Get());
    }

    // Spawn
    static int iSpawnLayer = 0;
    ImGui::Text("SpawnLayerIndex ");
    ImGui::SameLine();
    ImGui::SliderInt("##SpawnLayerIndex", &iSpawnLayer, 0, MAX_LAYER);

    static float SpawnPos[3] = { 0.f,0.f,0.f };
    ImGui::Text("SpawnPos        ");
    ImGui::SameLine();
    ImGui::DragFloat3("##SpawnPos", SpawnPos);
    ImGui::SameLine();
    
    if (ImGui::Button("##SpawnGameObjectButton", ImVec2(18.f, 18.f)))
    {
        SpawnGameObject(pPrefab->Instantiate(), Vec3(SpawnPos[0], SpawnPos[1], SpawnPos[2]), iSpawnLayer);
    }

    return 0;
}