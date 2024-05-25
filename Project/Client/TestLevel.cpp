#include "pch.h"
#include "TestLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CEventMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>

#include "CLevelSaveLoad.h"



void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"Light");
	pCurLevel->GetLayer(7)->SetName(L"MainCamera");
	pCurLevel->GetLayer(8)->SetName(L"FloorLevel");
	pCurLevel->GetLayer(9)->SetName(L"Item");			// 문, 아이템
	pCurLevel->GetLayer(10)->SetName(L"Effect");
	pCurLevel->GetLayer(30)->SetName(L"UI");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	///////////////////////////////////////////////////
	
	// Level 불러오기
	CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"Level\\Start.lv");

	tEvent evn = {};
	evn.Type = EVENT_TYPE::LEVEL_CHANGE;
	evn.wParam = (DWORD_PTR)pLoadedLevel;

	CEventMgr::GetInst()->AddEvent(evn);

	return;
}
