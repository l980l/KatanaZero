#include "pch.h"
#include "TileMapUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTileMap.h>
#include <Engine\CResMgr.h>

#include "TileMapTool.h"
#include "ListUI.h"

TileMapUI::TileMapUI()
    : ComponentUI("##TileMap", COMPONENT_TYPE::TILEMAP),
    m_pTileMapTool(nullptr)
{
    SetName("TileMap");
}

TileMapUI::~TileMapUI()
{
}

int TileMapUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    // Material 변경 버튼
    // Anim2D 생성 및 Atlas Texture 선택
    ImGui::Text("Material     ");
    ImGui::SameLine();
    string strKey = string(GetTarget()->TileMap()->GetMaterial()->GetKey().begin(), GetTarget()->TileMap()->GetMaterial()->GetKey().end());
    ImGui::InputText("##TileMapMaterial", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    if (ImGui::Button("##TileMapMaterialChangeButton", ImVec2(18, 18)))
    {
        const map<wstring, Ptr<CRes>>& mapMaterial = CResMgr::GetInst()->GetResources(RES_TYPE::MATERIAL);

        ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
        pListUI->Reset("Material List", ImVec2(300.f, 500.f));
        for (const auto& pair : mapMaterial)
        {
            pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
        }

        // 항목 선택시 호출받을 델리게이트 등록
        pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&TileMapUI::SelectMaterial);
    }

    //
    m_TileAtlasTex = GetTarget()->TileMap()->GetMaterial()->GetTexParam(TEX_0);

    float TexWidth = m_TileAtlasTex->Width();
    float TexHeight = m_TileAtlasTex->Height();

    ImGui::Text("TileCountXY  ");
    ImGui::SameLine();
    static int TileCount[2] = { (int)GetTarget()->TileMap()->GetTileCountX(), (int)GetTarget()->TileMap()->GetTileCountY() };
    ImGui::InputInt2("##TileCount", TileCount);
    ImGui::SameLine();
    if (ImGui::Button("##Apply TileCount", ImVec2(18.f, 18.f)))
    {
        GetTarget()->TileMap()->SetTileCount((UINT)TileCount[0], (UINT)TileCount[1]);
    }

    // TileSlice 크기를 정하기 위한 배열.
    static float SliceSizeUV[2] = { (float)GetTarget()->TileMap()->GetSliceSize().x, (float)GetTarget()->TileMap()->GetSliceSize().y };
    static int SliceSizePx[2] = { (float)GetTarget()->TileMap()->GetSliceSize().x * TexWidth, (float)GetTarget()->TileMap()->GetSliceSize().y * TexHeight };

    ImGui::Text("SliceSizeUV  ");
    ImGui::SameLine();
    ImGui::InputFloat2("##SliceSizeUV", SliceSizeUV);
    ImGui::SameLine();
    if (ImGui::Button("##Apply SliceSizeUV", ImVec2(18.f, 18.f)))
    {
        GetTarget()->TileMap()->SetSliceSize(Vec2(SliceSizeUV[0], SliceSizeUV[1]));
        GetTarget()->TileMap()->SetTileCount((UINT)TileCount[0], (UINT)TileCount[1]);
        SliceSizePx[0] = (float)GetTarget()->TileMap()->GetSliceSize().x * TexWidth;
        SliceSizePx[1] = (float)GetTarget()->TileMap()->GetSliceSize().y * TexHeight;
    }

    ImGui::Text("SliceSizePx  "); 
    ImGui::SameLine();
    
    ImGui::InputInt2("##SliceSizePx", SliceSizePx);
    ImGui::SameLine();
    if (ImGui::Button("##Apply SliceSizePx", ImVec2(18.f, 18.f)))
    {
        GetTarget()->TileMap()->SetSliceSize(Vec2(SliceSizePx[0] / (float)TexWidth, SliceSizePx[1] / (float)TexHeight));
        GetTarget()->TileMap()->SetTileCount((UINT)TileCount[0], (UINT)TileCount[1]);
        SliceSizeUV[0] = (float)GetTarget()->TileMap()->GetSliceSize().x;
        SliceSizeUV[1] = (float)GetTarget()->TileMap()->GetSliceSize().y;
    }

    if (ImGui::Button("TileMapTool", ImVec2(95.f, 18.f)))
    {
        m_pTileMapTool = (TileMapTool*)ImGuiMgr::GetInst()->FindUI("##TileMapTool");

        m_pTileMapTool->Reset();
        m_pTileMapTool->SetTargetObject(GetTarget());
    }

    if (m_pTileMapTool && m_pTileMapTool->IsActive())
    {
        ImGui::SameLine();

        if (ImGui::Button("Tile Apply", ImVec2(95.f, 18.f)))
        {
            GetTarget()->TileMap()->SetTile(m_pTileMapTool->GetTileInfo());
        }

        // TileMapUI의 가로 길이에 맞게 아틀라스 이미지 확대 및 축소.
        float AtlasZoom = ImGui::GetContentRegionAvail().x / TexWidth;

        // 타일 이미지
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        
        ImGui::Image((ImTextureID&)m_TileAtlasTex->GetSRV(), ImVec2(TexWidth * AtlasZoom, TexHeight * AtlasZoom));

        float TileWidth = TexWidth * AtlasZoom * SliceSizeUV[0];
        float TileHeigth = TexHeight * AtlasZoom * SliceSizeUV[1];

        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseClicked(0))
            {
                float region_x = io.MousePos.x - pos.x;
                float region_y = io.MousePos.y - pos.y;

                int IndexX = int(region_x / TileWidth);
                int IndexY = int(region_y / TileHeigth);

                m_pTileMapTool->SetTileIndex(IndexX, IndexY);
            }
        }
    }


    return TRUE;
}

void TileMapUI::SelectMaterial(DWORD_PTR _Key)
{
    string strKey = (char*)_Key;
    Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(wstring(strKey.begin(), strKey.end()));
    GetTarget()->TileMap()->SetMaterial(pMtrl);;
}
