#include "pch.h"
#include "Light2DUI.h"

#include <Engine\CLight2D.h>


Light2DUI::Light2DUI()
    : ComponentUI("##Light2D", COMPONENT_TYPE::LIGHT2D)
{
    SetName("Light2D");
}

Light2DUI::~Light2DUI()
{
}


int Light2DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    ImGui::Text("LIGHT TYPE     ");
    ImGui::SameLine();
    const char* LightTypeCombo[] = { "DIRECTIONAL", "POINT", "SPOT"};
    const int numLightTypeCombo = sizeof(LightTypeCombo) / sizeof(LightTypeCombo[0]);
    static int selectedLightTypeCombo = GetTarget()->Light2D()->GetLightType();
    ImGui::Combo("##LightTypeCombo", &selectedLightTypeCombo, LightTypeCombo, numLightTypeCombo);

    GetTarget()->Light2D()->SetLightType((LIGHT_TYPE)selectedLightTypeCombo);

    static float LightDiffuse[3] = { GetTarget()->Light2D()->GetLightDiffuse().x, GetTarget()->Light2D()->GetLightDiffuse().y, GetTarget()->Light2D()->GetLightDiffuse().z };

    ImGui::Text("LightDiffuse   ");
    ImGui::SameLine();
    ImGui::DragFloat3("##LightDiffuse", LightDiffuse);
    ImGui::SameLine();
    if (ImGui::Button("##Apply LightDiffuse", ImVec2(18.f, 18.f)))
    {
        GetTarget()->Light2D()->SetLightDiffuse(Vec3(LightDiffuse[0], LightDiffuse[1], LightDiffuse[2]));
    }

    // Directional Light이면 나올 필요 없음.
    if (selectedLightTypeCombo != 0)
    {
        static float LightAmbient[3] = { GetTarget()->Light2D()->GetLightAmbient().x, GetTarget()->Light2D()->GetLightAmbient().y, GetTarget()->Light2D()->GetLightAmbient().z };

        ImGui::Text("LightAmbient   ");
        ImGui::SameLine();
        ImGui::DragFloat3("##LightAmbient", LightAmbient);
        ImGui::SameLine();
        if (ImGui::Button("##Apply LightAmbient", ImVec2(18.f, 18.f)))
        {
            GetTarget()->Light2D()->SetLightAmbient(Vec3(LightAmbient[0], LightAmbient[1], LightAmbient[2]));
        }
    }

    // Point Light이면 나올 필요 없음.
    if (selectedLightTypeCombo != 1)
    {
        static float LightDirection[3] = { GetTarget()->Light2D()->GetLightDirection().x, GetTarget()->Light2D()->GetLightDirection().y, GetTarget()->Light2D()->GetLightDirection().z };

        ImGui::Text("LightDirection ");
        ImGui::SameLine();
        ImGui::DragFloat3("##LightDirection", LightDirection);
        ImGui::SameLine();
        if (ImGui::Button("##Apply LightDirection", ImVec2(18.f, 18.f)))
        {
            GetTarget()->Light2D()->SetLightDirection(Vec3(LightDirection[0], LightDirection[1], LightDirection[2]));
        }
    }

    // Point Light인 경우에만 나와야 함. 
    if (selectedLightTypeCombo == 1)
    {
        static float Radius = GetTarget()->Light2D()->GetRadius();

        ImGui::Text("Radius         ");
        ImGui::SameLine();
        ImGui::DragFloat("##Radius", &Radius);
        ImGui::SameLine();
        if (ImGui::Button("##Apply Radius", ImVec2(18.f, 18.f)))
        {
            GetTarget()->Light2D()->SetRadius(Radius);
        }
    }

    // Spot Light인 경우에만 나와야 함. 
    if (selectedLightTypeCombo == 2)
    {
        static float Angle = GetTarget()->Light2D()->GetAngle();

        ImGui::Text("Angle          ");
        ImGui::SameLine();
        ImGui::DragFloat("##Angle", &Angle);
        ImGui::SameLine();
        if (ImGui::Button("##Apply Angle", ImVec2(18.f, 18.f)))
        {
            GetTarget()->Light2D()->SetAngle(Angle);
        }
    }


    return TRUE;
}