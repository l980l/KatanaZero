#include "pch.h"
#include "CLevelSaveLoad2.h"
#include "CScriptMgr.h"

#include <Engine\CPathMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CScript.h>
#include <Engine\CPrefab.h>
#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>



int CLevelSaveLoad2::SaveLevel(const wstring& _LevelPath, CLevel* _Level)
{
	if (_Level->GetState() != LEVEL_STATE::STOP)
		return E_FAIL;

	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	if (nullptr == pFile)	
		return E_FAIL;

	// 레벨 이름 저장
	SaveWString(_Level->GetName(), pFile);


	// 레벨의 레이어들을 저장
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);

		// 레이어 이름 저장
		SaveWString(pLayer->GetName(), pFile);

		// 레이어의 게임오브젝트들 저장
		const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

		// 오브젝트 개수 저장
		size_t objCount = vecParent.size();
		fwrite(&objCount, sizeof(size_t), 1, pFile);
		
		// 각 게임오브젝트
		for (size_t i = 0; i < objCount; ++i)
		{
			SaveGameObject(vecParent[i], pFile);
		}
	}

	fclose(pFile);


	return S_OK;
}

int CLevelSaveLoad2::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// 이름
	SaveWString(_Object->GetName(), _File);
	
	// 컴포넌트
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	{		
		if (i == (UINT)COMPONENT_TYPE::END)
		{
			// 컴포넌트 타입 저장
			fwrite(&i, sizeof(UINT), 1, _File);
			break;
		}

		CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// 컴포넌트 타입 저장
		fwrite(&i, sizeof(UINT), 1, _File);

		// 컴포넌트 정보 저장
		Com->SaveToLevelFile(_File);
	}

	// 스크립트	
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t ScriptCount = vecScript.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		SaveWString(ScriptName, _File);
		vecScript[i]->SaveToLevelFile(_File);
	}


	// 자식 오브젝트
	const vector<CGameObject*>& vecChild = _Object->GetChild();
	size_t ChildCount = vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _File);		
	}

	return 0;
}

int CLevelSaveLoad2::SavePrefab(const wstring& _strRelativePath, CPrefab* _Prefab)
{
	if (_Prefab->IsEngineRes())
		return E_FAIL;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	SaveGameObject(_Prefab->GetProtoObj(), pFile);

	fclose(pFile);

	return S_OK;
}

CLevel* CLevelSaveLoad2::LoadLevel(const wstring& _LevelPath)
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (nullptr == pFile)
		return nullptr;

	CLevel* NewLevel = new CLevel;

	// 레벨 이름
	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	NewLevel->SetName(_LevelPath);


	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = NewLevel->GetLayer(i);

		// 레이어 이름
		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		// 게임 오브젝트 개수
		size_t objCount = 0;
		fread(&objCount, sizeof(size_t), 1, pFile);

		// 각 게임오브젝트
		for (size_t j = 0; j < objCount; ++j)
		{
			CGameObject* pNewObj = LoadGameObject(pFile);
			NewLevel->AddGameObject(pNewObj, i, false);
		}
	}

	fclose(pFile);

	NewLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	NewLevel->GetLayer(0)->SetName(L"Default");
	NewLevel->GetLayer(1)->SetName(L"Tile");
	NewLevel->GetLayer(2)->SetName(L"Player");
	NewLevel->GetLayer(3)->SetName(L"Monster");
	NewLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	NewLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	NewLevel->GetLayer(6)->SetName(L"Light");
	NewLevel->GetLayer(7)->SetName(L"MainCamera");
	NewLevel->GetLayer(8)->SetName(L"FloorLevel");
	NewLevel->GetLayer(9)->SetName(L"Item");			// 문, 아이템
	NewLevel->GetLayer(10)->SetName(L"Effect");
	NewLevel->GetLayer(30)->SetName(L"UI");
	NewLevel->GetLayer(31)->SetName(L"ViewPort UI");

	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Tile");
	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Tile");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"MonsterProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"PlayerProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"Tile", L"PlayerProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"Tile", L"MonsterProjectile");

	return NewLevel;
}

CGameObject* CLevelSaveLoad2::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// 이름
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// 컴포넌트
	while (true)
	{
		UINT ComponentType = 0;
		fread(&ComponentType, sizeof(UINT), 1, _File);

		// 컴포넌트 정보의 끝을 확인
		if ((UINT)COMPONENT_TYPE::END == ComponentType)
			break;

		CComponent* Component = nullptr;

		switch ((COMPONENT_TYPE)ComponentType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			Component = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			//Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			Component = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			break;
		case COMPONENT_TYPE::LIGHT2D:
			Component = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			break;
		case COMPONENT_TYPE::CAMERA:
			Component = new CCamera;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			Component = new CMeshRender;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			Component = new CParticleSystem;
			break;
		case COMPONENT_TYPE::TILEMAP:
			Component = new CTileMap;
			break;
		case COMPONENT_TYPE::LANDSCAPE:			
			break;
		case COMPONENT_TYPE::DECAL:			
			break;
		}

		Component->LoadFromLevelFile(_File);
		pObject->AddComponent(Component);
	}


	// 스크립트	
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName;
		LoadWString(ScriptName, _File);
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromLevelFile(_File);
	}

	// 자식 오브젝트		
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* ChildObject = LoadGameObject(_File);
		pObject->AddChild(ChildObject);
	}

	return pObject;
}

int CLevelSaveLoad2::LoadPrefab(const wstring& _strRelativePath)
{
	Ptr<CPrefab> pPrefab = new CPrefab;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	CGameObject* pNewObj = LoadGameObject(pFile);

	pPrefab->RegisterProtoObject(pNewObj);

	CResMgr::GetInst()->AddRes<CPrefab>(_strRelativePath, pPrefab);

	fclose(pFile);

	return S_OK;
}
