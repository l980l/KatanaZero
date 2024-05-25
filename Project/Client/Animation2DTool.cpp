#include "pch.h"
#include "Animation2DTool.h"

#include "ParamUI.h"

#include <Engine\CAnimator2D.h>
#include <Engine\CAnim2D.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CResMgr.h>

#include "ListUI.h"

Animation2DTool::Animation2DTool()
	: UI("##Animation2DTool"),
	m_pTarget(nullptr),
	m_SelectedAnim2D(nullptr),
	m_SelectedIndex(-1),
	m_FrameSlice{124, 100},
	m_TempAnimID(1),
	m_bAnimChange(true)
{
	SetName("Animation2DTool");
}

Animation2DTool::~Animation2DTool()
{
}

void Animation2DTool::finaltick()
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

int Animation2DTool::render_update()
{
	if (!m_SelectedAnim2D || !m_pTarget)
		return TRUE;
	
	static char szBuff[100] = {};

	if (m_bAnimChange)
	{
		memset(szBuff, 0, sizeof(char) * 100);

		wstring wName = m_SelectedAnim2D->GetName();
		string bName = string(wName.begin(), wName.end());

		memcpy(szBuff, bName.data(), sizeof(char) * bName.length());

		m_bAnimChange = false;
	}
	
	ImGui::Text("SelectedAnim2D");
	ImGui::SameLine();
	ImGui::PushItemWidth(200.f);	// InputText 가로길이 설정.
	ImGui::InputText("##SelectedAnim2D", szBuff, 100);

	// Anim2D 선택 버튼
	ImGui::SameLine();
	if (ImGui::Button("##SelectAnimButtton", ImVec2(18, 18)))
	{
		const map<wstring, CAnim2D*>& mapAnimation = m_pTarget->Animator2D()->GetAnimationMap();

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Animation2D", ImVec2(300.f, 500.f));
		for (const auto& pair : mapAnimation)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animation2DTool::SelectAnimation);
	}

	ImGui::SameLine();

	// Anim2D 이름 변경
	if (ImGui::Button("Rename", ImVec2(50, 18)))
	{
		// 기존의 anim 파일이 있는지 확인하고 있으면 삭제하고 새로 생성한다. 없다면 그냥 아무것도 안하고 Save를 누르면 그때 anim 파일을 생성하면 된다.
		//wstring Path = (L"animation\\" + m_SelectedAnim2D->GetName() + L".anim");
		wstring Path = (m_SelectedAnim2D->GetName());

		wstring wstrFilepath = CPathMgr::GetInst()->GetContentPath();
		wstrFilepath += Path;

		string strFilePath = string(wstrFilepath.begin(), wstrFilepath.end());

		// 기존의 anim 파일이 있는지 확인하고 있으면 삭제하고 새로 생성한다. 없다면 anim 파일을 생성하고 해당 anim으로 세팅한다.
		if (remove(strFilePath.c_str()) == 0)
		{
			// 삭제할 기존 Anim의 이름을 저장해둬야 한다.
			wstring DeleteName = m_SelectedAnim2D->GetName();

			// 기존 Anim의 이름을 변경하고 파일로 저장.
			string strRename = string(szBuff);
			wstring Rename = wstring(strRename.begin(), strRename.end());
			m_bAnimChange = true;

			Path = (L"animation\\" + Rename + L".anim");
			m_SelectedAnim2D->SetName(Path);

			wstrFilepath = CPathMgr::GetInst()->GetContentPath();
			wstrFilepath += Path;

			string strFilePath = string(wstrFilepath.begin(), wstrFilepath.end());

			m_SelectedAnim2D->Save(Path);

			// 기존 Anim를 지우고 새로 만든 Anim으로 교체한다.
			m_pTarget->Animator2D()->DeleteAnim(DeleteName);

			m_SelectedAnim2D = m_pTarget->Animator2D()->LoadAnim2D(Path);
		}

		else
		{
			// 기존 anim 파일의 이름을 저장해둬야 한다.
			wstring PrevName = m_SelectedAnim2D->GetName();

			string strNewName = string(szBuff);
			wstring NewName = wstring(strNewName.begin(), strNewName.end());
			
			// Anim2D의 이름과 Animator2D의 mapAnim에 저장된 키 값을 모두 변경함.
			m_SelectedAnim2D->SetName(NewName);
			m_pTarget->Animator2D()->ChangeAnimName(PrevName, NewName);
			m_bAnimChange = true;

			// 애니메이션 작업 할 때 편하려고 넣음.
			m_pTarget->Animator2D()->Play(NewName, true);
		}
	}

	// 프레임 별로 이미지 버튼을 만들어서 클릭하면 해당 프레임 편집 가능하게 만듦.
	vector<tAnim2DFrm>& vecAnimFrm = m_SelectedAnim2D->GetAnimFrmVector();

	ImTextureID& my_tex_id = (ImTextureID&)m_SelectedAnim2D->GetAtlasTex()->GetSRV();

	int EndFrm = int(vecAnimFrm.size());

	if (EndFrm >= 1)
	{
		for (int i = 0; i < EndFrm; ++i)
		{
			if (i % 5 != 0)
				ImGui::SameLine();

			ImVec2 LT = ImVec2(vecAnimFrm[i].LeftTopUV.x - vecAnimFrm[i].Offset.x, vecAnimFrm[i].LeftTopUV.y - vecAnimFrm[i].Offset.y);
			ImVec2 RB = ImVec2(vecAnimFrm[i].LeftTopUV.x + vecAnimFrm[i].SliceUV.x - vecAnimFrm[i].Offset.x, vecAnimFrm[i].LeftTopUV.y + vecAnimFrm[i].SliceUV.y - vecAnimFrm[i].Offset.y);
			
			if (ImGui::ImageButton(std::to_string(i).c_str(), my_tex_id, ImVec2(70.f, 70.f), LT, RB))
			{
				m_SelectedIndex = i;
			}

			// Drag & Drop으로 프레임 변경할 수 있게 함.
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("DND_DEMO_CELL", &i, sizeof(int)); 

				ImGui::Text("Swap Frame %d", i);
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
				{
					IM_ASSERT(payload->DataSize == sizeof(int));
					int payload_n = *(const int*)payload->Data;
					{
						m_SelectedAnim2D->SwapFrm(i, payload_n);
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
	}

	// 프레임이 선택되지 않았으면 그만.
	if (-1 == m_SelectedIndex)
		return TRUE;
	
	// Group으로 묶기
	ImGui::BeginGroup();

	ImGui::Text("FrameIndex    ");
	ImGui::SameLine();
	ImGui::SliderInt("##FrameIndex", &m_SelectedIndex, 0, EndFrm - 1);

	float TexWidth = m_SelectedAnim2D->GetAtlasTex()->Width();
	float TexHeight = m_SelectedAnim2D->GetAtlasTex()->Height();

	ImGui::Text("LeftTop       ");
	ImGui::SameLine();
	// UV 값으로 저장된 LeftTop을 픽셀 단위로 만들어야 함.
	int PixelLT[2] = {};
	PixelLT[0] = int(TexWidth * vecAnimFrm[m_SelectedIndex].LeftTopUV.x);
	PixelLT[1] = int(TexHeight * vecAnimFrm[m_SelectedIndex].LeftTopUV.y);
	ImGui::InputInt2("##LeftTop", PixelLT);

	ImGui::Text("Slice         ");
	ImGui::SameLine();
	// UV 값으로 저장된 Slice을 픽셀 단위로 만들어야 함.
	int PixelSlice[2] = {};
	PixelSlice[0] = int(TexWidth * vecAnimFrm[m_SelectedIndex].SliceUV.x);
	PixelSlice[1] = int(TexHeight * vecAnimFrm[m_SelectedIndex].SliceUV.y);
	ImGui::InputInt2("##Slice", PixelSlice);

	ImGui::Text("Offset        ");
	ImGui::SameLine();
	// UV 값으로 저장된 Offset을 픽셀 단위로 만들어야 함.
	int PixelOffset[2] = {};
	PixelOffset[0] = int(TexWidth * vecAnimFrm[m_SelectedIndex].Offset.x);
	PixelOffset[1] = int(TexHeight * vecAnimFrm[m_SelectedIndex].Offset.y);
	ImGui::SetNextItemWidth(173.7f);
	ImGui::InputInt2("##Offset", PixelOffset);
	ImGui::SameLine();
	// Anim의 모든 프레임에 Offset 적용하는 버튼
	if (ImGui::Button("##AdjustOffsetToAnim", ImVec2(18.f, 18.f)))
	{
		m_SelectedAnim2D->SetAllOffsetPos(Vec2(PixelOffset[0], PixelOffset[1]));
	}

	ImGui::Text("BackSize      ");
	ImGui::SameLine();
	// UV 값으로 저장된 BackSize을 픽셀 단위로 만들어야 함.
	int PixelBackSize[2] = {};
	PixelBackSize[0] = int(TexWidth * m_SelectedAnim2D->GetBackSize().x);
	PixelBackSize[1] = int(TexHeight * m_SelectedAnim2D->GetBackSize().y);
	ImGui::InputInt2("##BackSize", PixelBackSize);

	ImGui::Text("FPS           ");
	ImGui::SameLine();
	int FPS = m_SelectedAnim2D->GetFPS();
	ImGui::InputInt("##FPS", &FPS);

	// 변경사항 적용
	m_SelectedAnim2D->SetLeftTop(Vec2(PixelLT[0], PixelLT[1]), m_SelectedIndex);
	m_SelectedAnim2D->SetSlice(Vec2(PixelSlice[0], PixelSlice[1]), m_SelectedIndex);
	m_SelectedAnim2D->SetOffsetPos(Vec2(PixelOffset[0], PixelOffset[1]), m_SelectedIndex);
	m_SelectedAnim2D->SetBackSize(Vec2(PixelBackSize[0], PixelBackSize[1]));
	m_SelectedAnim2D->SetFPS(FPS);

	ImGui::Text("Frame         ");
	ImGui::SameLine();
	if (ImGui::Button("Delete", ImVec2(61.f, 18.f)))
	{
		m_SelectedAnim2D->DeleteFrm(m_SelectedIndex);

		if (m_SelectedIndex >= EndFrm - 1)
			m_SelectedIndex -= 1;
	}
	// 선택한 프레임 앞에 추가
	ImGui::SameLine();
	if (ImGui::Button("PrevAdd", ImVec2(61.f, 18.f)))
	{
		m_SelectedAnim2D->AddAtlasFrm(m_FrameLT, Vec2((float)m_FrameSlice[0], (float)m_FrameSlice[1]), m_SelectedIndex);
	}

	// 선택한 프레임 뒤에 추가
	ImGui::SameLine();
	if (ImGui::Button("NextAdd", ImVec2(61.f, 18.f)))
	{
		m_SelectedAnim2D->AddAtlasFrm(m_FrameLT, Vec2((float)m_FrameSlice[0], (float)m_FrameSlice[1]), ++m_SelectedIndex);
	}

	ImGui::Text("FrameSlice    ");
	ImGui::SameLine();
	ImGui::InputInt2("##FrameSlice", m_FrameSlice);

	ImGui::Text("Frame Zoom    ");
	ImGui::SameLine();
	static float FrameZoom = 1.f;
	ImGui::InputFloat("##FrameZoom", &FrameZoom);

	ImGui::EndGroup();

	ImGui::SameLine();


	ImGui::BeginGroup();

	// 추가할 프레임 출력
	ImGui::Image(my_tex_id, ImVec2(100.f, 100.f), m_FrameLTUV, m_FrameRBUV);

	if (ImGui::Button("Save", ImVec2(96.f, 18.f)))
	{
		//wstring Path = (L"animation\\" + m_SelectedAnim2D->GetName() + L".anim");
		wstring Path = (m_SelectedAnim2D->GetName());

		m_SelectedAnim2D->Save(Path);
	}

	// 프레임 선택 방식
	static int CaptureType = 0;

	ImGui::SetNextItemWidth(96.f);
	ImGui::Combo("##FrameExtractType", &CaptureType, "FixedSlice\0Drag\0Grid\0");

	ImGui::EndGroup();

	// Anim2D 생성 및 Atlas Texture 선택
	Ptr<CTexture> AtlasTex = m_SelectedAnim2D->GetAtlasTex();
	ParamUI::Param_Tex("CreateAnim2D", AtlasTex, this, (UI_DELEGATE_1)&Animation2DTool::CreateAnim2D);

	// 아틀라스 이미지 
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	
	// Animation2DTool의 가로 길이에 맞게 아틀라스 이미지 확대 및 축소.
	float AtlasZoom = ImGui::GetContentRegionAvail().x / TexWidth;

	ImGui::Image(my_tex_id, ImVec2(TexWidth * AtlasZoom, TexHeight * AtlasZoom));

	// 고정된 슬라이스로 선택하는 모드
	if (CaptureType == 0)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();

			float region_x = io.MousePos.x - pos.x - m_FrameSlice[0] * 0.5f * AtlasZoom;
			float region_y = io.MousePos.y - pos.y - m_FrameSlice[1] * 0.5f * AtlasZoom;

			if (region_x < 0.0f)
			{
				region_x = 0.0f;
			}

			else if (region_x > TexWidth - m_FrameSlice[0] * AtlasZoom)
			{
				region_x = TexWidth - m_FrameSlice[0] * AtlasZoom;
			}

			if (region_y < 0.0f)
			{
				region_y = 0.0f;
			}

			else if (region_y > TexHeight - m_FrameSlice[1] * AtlasZoom)
			{
				region_y = TexHeight - m_FrameSlice[1] * AtlasZoom;
			}

			// Min, Max는 출력되는 아틀라스 이미지 크기에 무관하게 실물에 대한 위치 값으로 나오게 함.
			ImGui::Text("Min: (%.2f, %.2f)", region_x / AtlasZoom, region_y / AtlasZoom);
			ImGui::Text("Max: (%.2f, %.2f)", region_x / AtlasZoom + m_FrameSlice[0], region_y / AtlasZoom + m_FrameSlice[1]);
			ImVec2 uv0 = ImVec2((region_x) / (TexWidth * AtlasZoom), (region_y) / (TexHeight * AtlasZoom));
			ImVec2 uv1 = ImVec2((region_x + m_FrameSlice[0] * AtlasZoom) / (TexWidth * AtlasZoom), (region_y + m_FrameSlice[1] * AtlasZoom) / (TexHeight * AtlasZoom));
			ImGui::Image(my_tex_id, ImVec2(m_FrameSlice[0] * FrameZoom, m_FrameSlice[1] * FrameZoom), uv0, uv1);

			// 마우스 클릭시 추가할 Frame의 LT, LTUV, RBUV 설정.
			if (ImGui::IsMouseClicked(0))
			{
				m_FrameLT = Vec2(region_x / AtlasZoom, region_y / AtlasZoom);
				m_FrameLTUV = uv0;
				m_FrameRBUV = uv1;
			}

			ImGui::EndTooltip();
		}
	}

	// Drag 모드
	else if (CaptureType == 1)
	{
		// 드래그로 프레임 크기 정할 수 있게 해보자.
		if (ImGui::IsItemHovered())
		{
			static bool bMouseClick = false;

			// 마우스 클릭시 추가할 Frame의 LT, LTUV, RBUV 설정.
			if (ImGui::IsMouseDown(0))
			{
				// 영역을 보여주기 위한 사각형
				static ImVec2 RecMin, RecMax;
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				draw_list->AddRect(RecMin, RecMax, IM_COL32(255, 255, 0, 255));

				if (!bMouseClick)
				{
					m_FrameLT = Vec2((io.MousePos.x - pos.x) / AtlasZoom, (io.MousePos.y - pos.y) / AtlasZoom);
					m_FrameLTUV = ImVec2((m_FrameLT.x) / (TexWidth), (m_FrameLT.y) / (TexHeight));
					bMouseClick = true;
					RecMin = io.MousePos;	// 사각형 LT 
				}

				else
				{
					m_FrameRBUV = ImVec2(((io.MousePos.x - pos.x) / AtlasZoom) / (TexWidth), ((io.MousePos.y - pos.y) / AtlasZoom) / (TexHeight));
					m_FrameSlice[0] = int(((io.MousePos.x - pos.x) / AtlasZoom) - m_FrameLT.x);
					m_FrameSlice[1] = int(((io.MousePos.y - pos.y) / AtlasZoom) - m_FrameLT.y);
					RecMax = io.MousePos;	// 사각형 RB
				}
			}

			else
				bMouseClick = false;
		}
	}

	// Grid로 잘라서 선택하는 모드
	else if (CaptureType == 2)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();

			float region_x = io.MousePos.x - pos.x - m_FrameSlice[0] * 0.5f * AtlasZoom;
			float region_y = io.MousePos.y - pos.y - m_FrameSlice[1] * 0.5f * AtlasZoom;

			if (region_x < 0.0f)
			{
				region_x = 0.0f;
			}

			else if (region_x > TexWidth - m_FrameSlice[0] * AtlasZoom)
			{
				region_x = TexWidth - m_FrameSlice[0] * AtlasZoom;
			}

			if (region_y < 0.0f)
			{
				region_y = 0.0f;
			}

			else if (region_y > TexHeight - m_FrameSlice[1] * AtlasZoom)
			{
				region_y = TexHeight - m_FrameSlice[1] * AtlasZoom;
			}

			// 배율이 적용된 Grid 프레임 하나의 크기
			float FrameWidth = AtlasZoom * m_FrameSlice[0];
			float FrameHeigth = AtlasZoom * m_FrameSlice[1];

			// 마우스 클릭시 추가할 Frame의 LT, LTUV, RBUV 설정.
			if (ImGui::IsMouseClicked(0))
			{
				float region_x = io.MousePos.x - pos.x;
				float region_y = io.MousePos.y - pos.y;

				int IndexX = int(region_x / FrameWidth);
				int IndexY = int(region_y / FrameHeigth);

				int RowNum = (int)TexWidth / m_FrameSlice[0];
				int ColNum = (int)TexHeight / m_FrameSlice[1];

				m_FrameLTUV = ImVec2(((float)IndexX / (float)RowNum), ((float)IndexY / (float)ColNum));
				m_FrameRBUV = ImVec2(((float)++IndexX / RowNum), ((float)++IndexY / (float)ColNum));
				m_FrameLT = Vec2(m_FrameLTUV.x * TexWidth, m_FrameLTUV.y * TexHeight);

				// LCTRL 눌려있는 상태면 프레임 바로 추가.
				if (KEY_PRESSED(KEY::LCTRL))
				{
					m_SelectedAnim2D->AddAtlasFrm(m_FrameLT, Vec2((float)m_FrameSlice[0], (float)m_FrameSlice[1]), ++m_SelectedIndex);
				}
			}

			ImGui::EndTooltip();
		}
	}

	return TRUE;
}

void Animation2DTool::Reset()
{
	m_pTarget = nullptr;
	m_SelectedAnim2D = nullptr;
	m_SelectedIndex = -1;
	m_bAnimChange = true;

	SetActive(true);
}

void Animation2DTool::Clear()
{
	m_SelectedIndex = -1;
	m_bAnimChange = true;
}

void Animation2DTool::SelectAnimation(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;

	wstring wstrSelectedAnim2DName = wstring(strKey.begin(), strKey.end());

	Clear();

	m_SelectedAnim2D = m_pTarget->Animator2D()->FindAnim(wstrSelectedAnim2DName);
}

void Animation2DTool::CreateAnim2D(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));

	char TempAnimName[50] = "TempAnim";
	char* dest = TempAnimName;
	_itoa_s(m_TempAnimID++, dest + 8, 50 - 8, 10);

	string sTempAnimName = string(TempAnimName);

	wstring wTempAnimName = wstring(sTempAnimName.begin(), sTempAnimName.end());

	m_pTarget->Animator2D()->CreateAnimation(wTempAnimName, pTex, Vec2(0.f, 0.f), Vec2(30.f, 30.f), Vec2(300.f, 300.f), 1, 1);

	Clear();

	m_SelectedAnim2D = m_pTarget->Animator2D()->FindAnim(wTempAnimName);
}
