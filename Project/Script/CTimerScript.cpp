#include "pch.h"
#include "CTimerScript.h"
#include "CPlayerScript.h"

void CTimerScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	m_fPos = Transform()->GetRelativePos();
	m_fScale = Transform()->GetRelativeScale();
}

void CTimerScript::tick()
{
	m_fPassedTime += DT;

	// 0 이상인 경우 크기 및 위치 조절. 줄어드는 만큼 왼쪽으로 이동시키면 될듯? 
	if (m_fPassedTime <= m_fTimelimit)
	{
		Vec3 Pos = m_fPos;
		Vec3 Scale = m_fScale;

		Pos.x -= ((m_fPassedTime / m_fTimelimit) * (m_fScale.x)) / 2.f;
		Scale.x *= ((m_fTimelimit - m_fPassedTime )/ m_fTimelimit);

		Transform()->SetRelativeScale(Scale);
		Transform()->SetRelativePos(Pos);
	}

	if (m_fTimelimit - m_fPassedTime <= 3.f && !m_bTimeEnd)
	{
		m_fPurpleShaderTime += DT;

		if (m_fPurpleShaderTime > 0.2f)
		{
			m_fPurpleShaderTime -= m_fPurpleShaderTime;

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Purple.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Vec3(0.f, 0.f, 0.f), L"Effect");
			pCloneEffect->SetLifeSpan(0.1f);

			float Time = GlobalData.tAccTime;
			CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Purple.mtrl")->SetScalarParam(FLOAT_0, &Time);
		}
	}

	// 보라보라 쉐이더 적용. 0초로 갈수록 빠르게 깜빡이다가, 0초가 되면 켜진 상태로 고정.
	else if (m_fTimelimit - m_fPassedTime <= 7.f && !m_bTimeEnd)
	{
		m_fPurpleShaderTime += DT;

		if (m_fPurpleShaderTime > 0.4f)
		{
			m_fPurpleShaderTime -= m_fPurpleShaderTime;

			Ptr<CPrefab> pEffectPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Purple.pref");
			CGameObject* pCloneEffect = pEffectPrefab->Instantiate();
			SpawnGameObject(pCloneEffect, Vec3(0.f, 0.f, 0.f), L"Effect");
			pCloneEffect->SetLifeSpan(0.2f);

			float Time = GlobalData.tAccTime;
			CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Purple.mtrl")->SetScalarParam(FLOAT_0, &Time);
		}
	}

	// 시간제한 초과
	if (m_fPassedTime > m_fTimelimit && !m_bTimeEnd)
	{
		m_bTimeEnd = true;

		/*Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\playerdie.wav");
		pSound->Play(1, 0.5f, false);*/

		// MonsterLayer의 모든 옵젝들
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
					((CPlayerScript*)vecScript[i])->CPlayerScript::SetState(1);
					((CPlayerScript*)vecScript[i])->CPlayerScript::SetAnimNum(8);
				}
			}
		}
	}
}

void CTimerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fTimelimit, sizeof(float), 1, _File);
}

void CTimerScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_fTimelimit, sizeof(float), 1, _FILE);
}

CTimerScript::CTimerScript()
	: CScript((UINT)SCRIPT_TYPE::TIMERSCRIPT)
	, m_fTimelimit(0.f)
	, m_fPassedTime(0.f)
	, m_bTimeEnd(false)
	, m_fPurpleShaderTime(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTimelimit, "Timelimit");
}

CTimerScript::~CTimerScript()
{
}
