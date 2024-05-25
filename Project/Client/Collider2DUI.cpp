#include "pch.h"
#include "Collider2DUI.h"

#include <Engine\CCollider2D.h>

Collider2DUI::Collider2DUI()
    : ComponentUI("##Coillider2D", COMPONENT_TYPE::COLLIDER2D)
{
    SetName("Coillider2D");
}

Collider2DUI::~Collider2DUI()
{
}

int Collider2DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    Vec3 vOffsetPos = GetTarget()->Collider2D()->GetOffsetPos();
    Vec3 vOffsetScale = GetTarget()->Collider2D()->GetOffsetScale();
    bool bAbsolute = GetTarget()->Collider2D()->GetAbsolute();
    int ColliderShape = (int)GetTarget()->Collider2D()->GetShape();

    ImGui::Text("OffsetPos  ");
    ImGui::SameLine();
    ImGui::DragFloat3("##Collider2D OffsetPos", vOffsetPos);

    ImGui::Text("OffsetScale");
    ImGui::SameLine();
    ImGui::DragFloat3("##Collider2D OffsetScale", vOffsetScale);

    ImGui::Text("Absolute   ");
    ImGui::SameLine();
    ImGui::Checkbox("##Collider2D Absolute", &bAbsolute);

    ImGui::Text("Shape      ");
    ImGui::SameLine();
    ImGui::Combo("##Collider2D Shape", &ColliderShape, "RECT\0CIRCLE\0LINE\0\0");


    GetTarget()->Collider2D()->SetOffsetPos(vOffsetPos);
    GetTarget()->Collider2D()->SetOffsetScale(vOffsetScale);
    GetTarget()->Collider2D()->SetAbsolute(bAbsolute);
    GetTarget()->Collider2D()->SetCollider2DType((COLLIDER2D_TYPE)ColliderShape);

    return TRUE;
}