#include "pch.h"
#include "CStartMenuScript.h"
#include "CLevelSaveLoad2.h"

#include <Engine\CEventMgr.h>
#include <Engine\CAnim2D.h>
#include <Engine\CEngine.h>

void CStartMenuScript::begin()
{
	Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_fullconfession.ogg");
	pSound2->Stop();

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_rainonbrick.ogg");
	pSound->Play(0, 0.5f, false);

	// 마우스 안보이게 하기
	//ShowCursor(false);
}

void CStartMenuScript::tick()
{
	if (m_iSelectedMenu == 0)
	{
		CAnim2D* Anim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
		Anim->SetCurIndex(0);
	}

	else if(m_iSelectedMenu == 1)
	{
		CAnim2D* Anim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
		Anim->SetCurIndex(1);
	}

	else if (m_iSelectedMenu == 2)
	{
		CAnim2D* Anim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());
		Anim->SetCurIndex(2);
	}

	if (KEY_TAP(KEY::W))
	{
		if (m_iSelectedMenu > 0)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_menu_beep_1.wav");
			pSound->Play(1, 0.5f, true);

			--m_iSelectedMenu;
		}
	}

	if (KEY_TAP(KEY::S))
	{
		if (m_iSelectedMenu < 2)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_menu_beep_1.wav");
			pSound->Play(1, 0.5f, true);

			++m_iSelectedMenu;
		}
	}

	// Space 키가 눌리면 선택.
	if (KEY_TAP(KEY::SPACE))
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_menu_beep_2.wav");
		pSound->Play(1, 0.5f, false);

		if (m_iSelectedMenu == 0)
		{
			// Player Scene으로 이동해야 함. 소리를 우선 끄고, 쉐이더 효과 사용하고 이동.
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_rainonbrick.ogg");
			pSound->Stop();

			// 쉐이더 사용
			

			// 레벨 이동
			CLevel* pLoadedLevel = CLevelSaveLoad2::LoadLevel(L"Level\\Motel1.lv");

			tEvent evn = {};
			evn.Type = EVENT_TYPE::LEVEL_CHANGE;
			evn.wParam = (DWORD_PTR)pLoadedLevel;

			CEventMgr::GetInst()->AddEvent(evn);
		}

		else if (m_iSelectedMenu == 1)
		{
			// 옵션 레벨로 이동해야 함.

		}

		else if (m_iSelectedMenu == 2)
		{
			// 게임이 꺼져야 함.
			DestroyWindow(CEngine::GetInst()->GetMainWnd());
		}
	}
}

CStartMenuScript::CStartMenuScript()
	: CScript((UINT)SCRIPT_TYPE::STARTMENUSCRIPT)
	, m_iSelectedMenu(0)
{
}

CStartMenuScript::~CStartMenuScript()
{
}
