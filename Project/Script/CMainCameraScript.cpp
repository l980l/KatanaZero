#include "pch.h"
#include "CMainCameraScript.h"
#include <Engine\CRenderMgr.h>

void CMainCameraScript::begin()
{
	// 3이 지금은 마지막 카메라 인덱스라서 3이면 충돌체 카메라인 거임.
	if (Camera()->GetCameraIndex() == 3)
		m_bColliderCamera = true;
}

void CMainCameraScript::tick()
{
	if (KEY_TAP(KEY::TAB))
	{
		if (m_bColliderOn)
			m_bColliderOn = false;
		else
			m_bColliderOn = true;
	}

	if (GetOwner()->GetParent())
	{
		if (m_bColliderCamera && !m_bColliderOn)
		{
			Vec3 Pos = { 50000.f,50000.f,1.f };
			Transform()->SetRelativePos(Pos);
		}

		else
		{
			Vec3 PlayerPos = GetOwner()->GetParent()->Transform()->GetRelativePos();

			// 카메라 클리핑
			Vec3 MainCameraPos = CRenderMgr::GetInst()->GetMainCam()->GetOwner()->Transform()->GetRelativePos(); // 이전 프레임의 메인 카메라 위치.

			// 현재 프레임의 플레이어 위치를 중심으로 LT와 RB를 만들때의 위치.
			Vec3 MainCameraSize = Vec3(1280.f, 768.f, 0.f) * CRenderMgr::GetInst()->GetMainCam()->GetScale();
			Vec3 TempClientLTPos = PlayerPos + Vec3(-MainCameraSize.x / 2.f, MainCameraSize.y / 2.f, 0.f);
			Vec3 TempClientRBPos = PlayerPos + Vec3(MainCameraSize.x / 2.f, -MainCameraSize.y / 2.f, 0.f);

			// 마우스 위치 따라가기 적용.
			Vec3 Diff = TempClientLTPos + Vec3(CKeyMgr::GetInst()->GetMousePos().x, -CKeyMgr::GetInst()->GetMousePos().y, 0.f) - PlayerPos;

			PlayerPos += Diff / 20.f;
			TempClientLTPos += Diff / 20.f;
			TempClientRBPos += Diff / 20.f;

			if (!m_bStart)
			{
				CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
				wstring temp = CurLevel->GetName();

				// Level에 맞게 BGM 설정.
				if (temp == L"Level\\Motel1.lv")
				{
					m_MapSize = Vec3(2497.f, 953.f, 0.f);
					m_MapOffset = Vec3(0.f, -50.f, 0.f);
				}

				else if (temp == L"Level\\Motel2.lv")
				{
					m_MapSize = Vec3(2475.f, 1219.f, 0.f);
					m_MapOffset = Vec3(0.f, -50.f, 0.f);
				}

				else if (temp == L"Level\\Motel3.lv")
				{
					//m_MapSize = Vec3(2468.f, 727.f, 0.f);
					m_MapSize = Vec3(2468.f, 768.f, 0.f);
					m_MapOffset = Vec3(0.f, -50.f, 0.f);
				}

				else if (temp == L"Level\\Boss1.lv")
				{
					//m_MapSize = Vec3(1341.f, 577.f, 0.f);
					m_MapSize = Vec3(1278.f, 768.f, 0.f);
					m_MapOffset = Vec3(0.f, -50.f, 0.f);
				}

				else if (temp == L"Level\\Boss2.lv")
				{
					//m_MapSize = Vec3(1278.f, 720.f, 0.f);
					m_MapSize = Vec3(1278.f, 768.f, 0.f);
					m_MapOffset = Vec3(0.f, -50.f, 0.f);
				}

				m_MapLT = Vec3(0.f, 0.f, 0.f) + Vec3(-m_MapSize.x / 2.f, m_MapSize.y / 2.f, 0.f) + m_MapOffset;
				m_MapRB = Vec3(0.f, 0.f, 0.f) + Vec3(m_MapSize.x / 2.f, -m_MapSize.y / 2.f, 0.f) + m_MapOffset;

				if (TempClientLTPos.x < m_MapLT.x)
				{
					MainCameraPos.x = PlayerPos.x + (m_MapLT.x - TempClientLTPos.x);
				}

				if (TempClientRBPos.x > m_MapRB.x)
				{
					MainCameraPos.x = PlayerPos.x - (TempClientRBPos.x - m_MapLT.x);
				}

				if (TempClientRBPos.y < m_MapRB.y)
				{
					MainCameraPos.y = PlayerPos.y + (m_MapRB.y - TempClientRBPos.y);
				}

				if (TempClientLTPos.y > m_MapLT.y)
				{
					MainCameraPos.y = PlayerPos.y - (TempClientLTPos.y - m_MapLT.y);
				}

				m_bStart = true;
			}

			else
			{
				// 카메라 범위가 맵 크기 안에 들어가는 경우에만 동작.
				if (TempClientLTPos.x > m_MapLT.x && TempClientRBPos.x < m_MapRB.x)
				{
					MainCameraPos.x = PlayerPos.x;
				}

				if (TempClientRBPos.y > m_MapRB.y && TempClientLTPos.y < m_MapLT.y)
				{
					MainCameraPos.y = PlayerPos.y;
				}
			}

			Transform()->SetRelativePos(MainCameraPos);
		}
	}
}

CMainCameraScript::CMainCameraScript()
	: CScript((UINT)SCRIPT_TYPE::MAINCAMERASCRIPT)
	, m_bStart(false)
	, m_bColliderCamera(false)
	, m_bColliderOn(false)
{
}

CMainCameraScript::~CMainCameraScript()
{
}
