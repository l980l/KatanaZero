#include "pch.h"
#include "CBatteryScript.h"
#include "CPlayerScript.h"

#include <Engine\CAnim2D.h>

void CBatteryScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	wstring temp = CurLevel->GetName();

	// Level에 맞게 BGM 설정.
	if (temp == L"Level\\Motel1.lv" || temp == L"Level\\Motel2.lv" || temp == L"Level\\Motel3.lv")
	{
		Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_fullconfession.ogg");
		pSound2->Stop();

		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_sneaky_driver.ogg");
		pSound->Play(0, 0.5f, false);
	}

	else if (temp == L"Level\\Boss1.lv" || temp == L"Level\\Boss2.lv")
	{
		Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_sneaky_driver.ogg");
		pSound2->Stop();

		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_fullconfession.ogg");
		pSound->Play(0, 0.5f, false);
	}

	// 플레이어 스크립트 설정.
	CLevel* Level = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* PlayerLayer = Level->GetLayer(2);
	vector<CGameObject*> vecPlayer = PlayerLayer->GetParentObject();
	int Size = (int)vecPlayer.size();

	for (int i = 0; i < Size; ++i)
	{
		CGameObject* PlayerObj = vecPlayer[i];
		vector<CScript*> vecScript = PlayerObj->GetScripts();

		int vecScriptSize = (int)vecScript.size();

		for (size_t i = 0; i < vecScriptSize; ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);

			if (ScriptName == L"CPlayerScript")
			{
				m_PlayerScript = ((CPlayerScript*)vecScript[i]);
			}
		}
	}
}

void CBatteryScript::tick()
{
	if (!m_PlayerScript)
		return;

	if (m_fBattery >= 0.f)
	{
		if (m_fBattery > 6.f)
			m_fBattery = 6.f;

		int AnimIndex = m_fBattery / 6.f * 9;

		if (AnimIndex < 0)
			AnimIndex = 0;
		if (AnimIndex > 9)
			AnimIndex = 9;
		
		CAnim2D* CurAnim = Animator2D()->FindAnim(Animator2D()->GetCurAnimationName());

		CurAnim->SetCurIndex(AnimIndex);
	}
	
	if (m_PlayerScript->GetState() == 0)
	{
		if (!m_bTimeSlow)
		{
			if (KEY_PRESSED(KEY::LSHIFT) && m_fBattery > 0.f)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\Slowmo_Enter.wav");
				pSound->Play(1, 0.5f, false);

				for (int i = 0; i < 32; ++i)
				{
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_sneaky_driver.ogg");
					pSound2->SetVolume(0.1f, i);

					Ptr<CSound> pSound3 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_fullconfession.ogg");
					pSound3->SetVolume(0.1f, i);
				}

				CTimeMgr::GetInst()->SetTimeScale(0.5f);

				m_bTimeSlow = true;

				Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Dark.pref");
				CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
				SpawnGameObject(pCloneEffect, Vec3(0.f, 0.f, 0.f), L"Effect");
				m_DarkPostprocess = pCloneEffect;

				float Time = GlobalData.tAccTime;
				CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Dark.mtrl")->SetScalarParam(FLOAT_0, &Time);

				m_PlayerScript->GetOwner()->Animator2D()->SetArrangeColor(Vec3(1.f, 300.f, 300.f));
			}

			else
				m_fBattery += DT;
		}

		if (m_bTimeSlow)
		{
			m_fBattery -= DT;

			if (KEY_RELEASE(KEY::LSHIFT) || m_fBattery<=0.f || m_PlayerScript->GetState()!=0)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\Slowmo_Exit.wav");
				pSound->Play(1, 0.5f, false);

				for (int i = 0; i < 32; ++i)
				{
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_sneaky_driver.ogg");
					pSound2->SetVolume(0.5f, i);

					Ptr<CSound> pSound3 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_fullconfession.ogg");
					pSound3->SetVolume(0.5f, i);
				}

				CTimeMgr::GetInst()->SetTimeScale(1.f);

				m_bTimeSlow = false;

				m_DarkPostprocess->SetLifeSpan(0.f);

				m_PlayerScript->GetOwner()->Animator2D()->SetArrangeColor(Vec3(1.f, 1.f, 1.f));
			}
		}
	}
}

CBatteryScript::CBatteryScript()
	: CScript((UINT)SCRIPT_TYPE::BATTERYSCRIPT)
	, m_fBattery(6.f)
	, m_bTimeSlow(false)
	, m_PlayerScript(nullptr)
	, m_DarkPostprocess(nullptr)
{
}

CBatteryScript::~CBatteryScript()
{
}
