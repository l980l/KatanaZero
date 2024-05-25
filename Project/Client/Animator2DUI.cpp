#include "pch.h"
#include "Animator2DUI.h"

#include <Engine\CAnimator2D.h>
#include <Engine\CAnim2D.h>
#include <Engine\CTexture.h>
#include <Engine\CTimeMgr.h>

#include "ListUI.h"
#include "Animation2DTool.h"

Animator2DUI::Animator2DUI()
    : ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR2D),
	m_iCurFrm(0)
{
    SetName("Animator2D");    
}

Animator2DUI::~Animator2DUI()
{
}


int Animator2DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    map<wstring, CAnim2D*> AnimMap = GetTarget()->Animator2D()->GetAnimationMap();
    
	if (m_SelectedAnim2DName.empty() || m_SelectedAnim2DName != string(GetTarget()->Animator2D()->GetCurAnimationName().begin(), GetTarget()->Animator2D()->GetCurAnimationName().end()) || !m_SelectedAnim2D->GetAnimFrmVector().size())
	{
		// wstring이라 변환. 초기 선택 애니메이션은 CurAnim으로.
		m_SelectedAnim2DName = string(GetTarget()->Animator2D()->GetCurAnimationName().begin(), GetTarget()->Animator2D()->GetCurAnimationName().end());

		wstring wstrSelectedAnim2DName = wstring(m_SelectedAnim2DName.begin(), m_SelectedAnim2DName.end());

		// 애니메이션 설정
		m_SelectedAnim2D = GetTarget()->Animator2D()->FindAnim(wstrSelectedAnim2DName);

		// 때려쳐
		if (!m_SelectedAnim2D)
			return FALSE;

		// 텍스쳐 설정
		m_AtlasTex = m_SelectedAnim2D->GetAtlasTex();
	}
		

    ImGui::Text("SelectedAnim");
    ImGui::SameLine();
	ImGui::PushItemWidth(220.f);	// InputText 가로길이 설정.
	ImGui::InputText("##SelectedAnim", const_cast<char*>(m_SelectedAnim2DName.c_str()), sizeof(m_SelectedAnim2DName), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##SelectAnimButtton", ImVec2(18, 18)))
	{
		const map<wstring, CAnim2D*>& mapAnimation = GetTarget()->Animator2D()->GetAnimationMap();

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Animation2D", ImVec2(300.f, 500.f));
		for (const auto& pair : mapAnimation)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animator2DUI::SelectAnimation);
	}

	static float ArrangeColor[3] = { GetTarget()->Animator2D()->GetArrangeColor().x,GetTarget()->Animator2D()->GetArrangeColor().y,GetTarget()->Animator2D()->GetArrangeColor().z };
	ImGui::Text("ArrangeColor");
	ImGui::SameLine();
	ImGui::DragFloat3("##ArrangeColor", ArrangeColor, 0.1f, 0.f, 1.f);
	ImGui::SameLine();
	if (ImGui::Button("##ArrangeColorButton", ImVec2(18, 18)))
	{
		GetTarget()->Animator2D()->SetArrangeColor(Vec3(ArrangeColor[0], ArrangeColor[1], ArrangeColor[2]));
	}

	//// 선택된 애니메이션2D 미리보기
	//const vector<tAnim2DFrm>& vecFrm = m_SelectedAnim2D->GetAnimFrmVector();

	//if (vecFrm.size() <= 0)
	//	return FALSE;

	//ImTextureID& my_tex_id = (ImTextureID&)m_AtlasTex->GetSRV();

	//m_fTime += DT;

	//if (vecFrm[m_iCurFrm].fDuration <= m_fTime)
	//{
	//	m_fTime = m_fTime - vecFrm[m_iCurFrm].fDuration;
	//	++m_iCurFrm;

	//	if (vecFrm.size() <= m_iCurFrm)
	//	{
	//		m_iCurFrm = 0;
	//	}
	//}

	//ImVec2 LT = ImVec2(vecFrm[m_iCurFrm].LeftTopUV.x, vecFrm[m_iCurFrm].LeftTopUV.y);
	//ImVec2 RB = ImVec2(vecFrm[m_iCurFrm].LeftTopUV.x + vecFrm[m_iCurFrm].SliceUV.x, vecFrm[m_iCurFrm].LeftTopUV.y + vecFrm[m_iCurFrm].SliceUV.y);

	//ImGui::Image(my_tex_id, ImVec2(85.f, 85.f), LT, RB);

	//ImGui::SameLine();

	if (ImGui::Button("Animation2DTool", ImVec2(130.f, 18.f)))
	{
		Animation2DTool* pAnimation2DTool = (Animation2DTool*)ImGuiMgr::GetInst()->FindUI("##Animation2DTool");

		pAnimation2DTool->Reset();
		pAnimation2DTool->SetTargetObject(GetTarget());
		pAnimation2DTool->SetAnim2D(m_SelectedAnim2D);
	}

    return TRUE;
}

void Animator2DUI::SelectAnimation(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;

	m_SelectedAnim2DName = strKey;

	wstring wstrSelectedAnim2DName = wstring(m_SelectedAnim2DName.begin(), m_SelectedAnim2DName.end());

	// 애니메이션 설정
	m_SelectedAnim2D = GetTarget()->Animator2D()->FindAnim(wstrSelectedAnim2DName);

	// 텍스쳐 설정
	m_AtlasTex = m_SelectedAnim2D->GetAtlasTex();

	wstring CurAnimName = wstring(m_SelectedAnim2DName.begin(), m_SelectedAnim2DName.end());

	GetTarget()->Animator2D()->Play(CurAnimName, true);
}
