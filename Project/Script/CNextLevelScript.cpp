#include "pch.h"
#include "CNextLevelScript.h"
#include "CPlayerScript.h"
#include "CLevelSaveLoad2.h"
#include "CMonsterGruntScript.h"
#include "CMonsterPompScript.h"
#include "CMonsterCopScript.h"
#include "CHeadhunterScript.h"

#include <Engine\CEventMgr.h>

void CNextLevelScript::NextLevel()
{
	// 레벨 이동
	if (m_iLevel == 0)
	{
		CLevel* pLoadedLevel = CLevelSaveLoad2::LoadLevel(L"Level\\Motel2.lv");
		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)pLoadedLevel;
		CEventMgr::GetInst()->AddEvent(evn);
	}

	if (m_iLevel == 1)
	{
		CLevel* pLoadedLevel = CLevelSaveLoad2::LoadLevel(L"Level\\Motel3.lv");
		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)pLoadedLevel;
		CEventMgr::GetInst()->AddEvent(evn);
	}

	if (m_iLevel == 2)
	{
		CLevel* pLoadedLevel = CLevelSaveLoad2::LoadLevel(L"Level\\Boss1.lv");
		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)pLoadedLevel;
		CEventMgr::GetInst()->AddEvent(evn);
	}

	if (m_iLevel == 3)
	{
		CLevel* pLoadedLevel = CLevelSaveLoad2::LoadLevel(L"Level\\Boss2.lv");
		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)pLoadedLevel;
		CEventMgr::GetInst()->AddEvent(evn);
	}

	if (m_iLevel == 4)
	{
		CLevel* pLoadedLevel = CLevelSaveLoad2::LoadLevel(L"Level\\Start.lv");
		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)pLoadedLevel;
		CEventMgr::GetInst()->AddEvent(evn);
	}
}

bool CNextLevelScript::CheckMonsterAllDead()
{
	// Monster 스크립트
	CLevel* Level = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* MonsterLayer = Level->GetLayer(3);
	vector<CGameObject*> vecMonster = MonsterLayer->GetParentObject();
	int Size = (int)vecMonster.size();

	for (int i = 0; i < Size; ++i)
	{
		CGameObject* MonsterObj = vecMonster[i];
		vector<CScript*> vecScript = MonsterObj->GetScripts();

		int vecScriptSize = (int)vecScript.size();

		for (size_t i = 0; i < vecScriptSize; ++i)
		{
			wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);

			if (ScriptName == L"CHeadhunterScript")
			{
				if (((CHeadhunterScript*)vecScript[i])->CHeadhunterScript::GetState() == 0)
					return false;
			}

			if (ScriptName == L"CMonsterPompScript")
			{
				if (((CMonsterPompScript*)vecScript[i])->CMonsterPompScript::GetState() == 0)
					return false;
			}

			if (ScriptName == L"CMonsterCopScript")
			{
				if (((CMonsterCopScript*)vecScript[i])->CMonsterCopScript::GetState() == 0)
					return false;
			}

			if (ScriptName == L"CMonsterGruntScript")
			{
				if (((CMonsterGruntScript*)vecScript[i])->CMonsterGruntScript::GetState() == 0)
					return false;
			}
		}
	}

	return true;
}

void CNextLevelScript::begin()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	wstring temp = CurLevel->GetName();

	// Level에 맞게 BGM 설정.
	if (temp == L"Level\\Motel1.lv")
		m_iLevel = 0;

	if(temp == L"Level\\Motel2.lv")
		m_iLevel = 1;

	if (temp == L"Level\\Motel3.lv")
		m_iLevel = 2;

	if (temp == L"Level\\Boss1.lv")
		m_iLevel = 3;

	if (temp == L"Level\\Boss2.lv")
		m_iLevel = 4;
}

void CNextLevelScript::tick()
{
	// Video 재생 중인 경우에는 다음 레벨로 가야되는지 확인.
	if (m_bVideoPlay)
	{
		m_fGridPostprocess += DT;

		if (m_fGridPostprocess > 1.f)
		{
			// shader가 끝나면
			NextLevel();
		}

		//// 재생이 끝나면 
		//if (m_PlayerScript->GetVideoPlayEnd())
		//{
		//	NextLevel();
		//}
	}
}

void CNextLevelScript::BeginOverlap(CCollider2D* _Other)
{
	// 몬스터들이 다 죽어있다면.
	if (_Other->GetOwner()->GetLayerIndex() == 2 && !m_bVideoPlay)
	{
		if (CheckMonsterAllDead())
		{
			m_PlayerScript = _Other->GetOwner()->GetScript<CPlayerScript>();

			// 우선 플레이 영상 재생.
			m_PlayerScript->Rewind(1.f, false);

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\GridPostprocess.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Vec3(0.f, 0.f, 0.f), L"Effect");

			float Time = GlobalData.tAccTime;
			CResMgr::GetInst()->FindRes<CMaterial>(L"material\\GridPostprocess.mtrl")->SetScalarParam(FLOAT_0, &Time);

			m_bVideoPlay = true;
		}
	}
}

void CNextLevelScript::OnOverlap(CCollider2D* _Other)
{
	// 몬스터들이 다 죽어있다면.
	if (_Other->GetOwner()->GetLayerIndex() == 2 && !m_bVideoPlay)
	{
		if (CheckMonsterAllDead())
		{
			m_PlayerScript = _Other->GetOwner()->GetScript<CPlayerScript>();

			// 우선 플레이 영상 재생.
			m_PlayerScript->Rewind(1.f, false);

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\GridPostprocess.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Vec3(0.f, 0.f, 0.f), L"Effect");

			float Time = GlobalData.tAccTime;
			CResMgr::GetInst()->FindRes<CMaterial>(L"material\\GridPostprocess.mtrl")->SetScalarParam(FLOAT_0, &Time);

			m_bVideoPlay = true;
		}
	}
}

CNextLevelScript::CNextLevelScript()
	: CScript((UINT)SCRIPT_TYPE::NEXTLEVELSCRIPT)
	, m_iLevel(0)
	, m_bVideoPlay(false)
	, m_fGridPostprocess(0.f)
{
}

CNextLevelScript::~CNextLevelScript()
{
}
