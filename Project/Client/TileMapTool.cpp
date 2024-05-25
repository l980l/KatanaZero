#include "pch.h"
#include "TileMapTool.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTileMap.h>

TileMapTool::TileMapTool()
	: UI("##TileMapTool"),
	m_pTarget(nullptr)
{
	SetName("TileMapTool");
}

TileMapTool::~TileMapTool()
{
}


void TileMapTool::finaltick()
{
	UI::finaltick();

	// Esc 눌리면 비활성화
	if (KEY_TAP(KEY::ESC))
	{
		SetActive(false);

		// 모든 UI 포커스 해제
		ImGui::SetWindowFocus(nullptr);
	}
}

int TileMapTool::render_update()
{
	if (!m_pTarget)
		return TRUE;
	
	m_TileAtlasTex = m_pTarget->TileMap()->GetMaterial()->GetTexParam(TEX_0);

	ImTextureID& my_tex_id = (ImTextureID&)m_TileAtlasTex->GetSRV();

	float TileWidth = m_TileAtlasTex->Width() * m_vTileSliceSize.x;
	float TileHeight = m_TileAtlasTex->Height() * m_vTileSliceSize.y;

	// TileMapUI의 가로 길이에 맞게 아틀라스 이미지 확대 및 축소.
	float AtlasZoom = ImGui::GetContentRegionAvail().x / (TileWidth * (float)m_iCountX);

	ImGuiStyle& style = ImGui::GetStyle();

	ImVec2 Temp = style.ItemSpacing;
	style.ItemSpacing = ImVec2(0.f, 0.f);		// 이걸 해야 타일 이미지 사이의 간격이 없음.

	auto iter = m_vecTileInfo.begin();

	

	for (UINT i = 0; i < m_iCountY; ++i)
	{
		for (UINT j = 0; j < m_iCountX; ++j)
		{
			if (j > 0)
				ImGui::SameLine();

			ImVec2 LT = ImVec2(iter->vLeftTop.x, iter->vLeftTop.y);
			ImVec2 RB = ImVec2(iter->vLeftTop.x + iter->vSlice.x, iter->vLeftTop.y + iter->vSlice.y);
			ImGui::Image(my_tex_id, ImVec2(TileWidth * AtlasZoom, TileHeight * AtlasZoom), LT, RB);
			
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDown(0))
					iter->vLeftTop = Vec2((float)m_TileIndex[0] * m_vTileSliceSize.x, (float)m_TileIndex[1] * m_vTileSliceSize.y);
			}

			++iter;
		}
	}

	style.ItemSpacing = Temp;				// 타일 이미지 출력 끝나면 원래대로 돌려놓음.

	return TRUE;
}

void TileMapTool::SetTargetObject(CGameObject* _Target)
{
	m_pTarget = _Target;

	if (!m_pTarget->TileMap())
		return;

	m_iCountX = m_pTarget->TileMap()->GetTileCountX();
	m_iCountY = m_pTarget->TileMap()->GetTileCountY();
	m_vTileSliceSize = m_pTarget->TileMap()->GetSliceSize();

	m_vecTileInfo = m_pTarget->TileMap()->GetTileVector();
}

void TileMapTool::Reset()
{
	m_pTarget = nullptr;

	SetActive(true);
}
