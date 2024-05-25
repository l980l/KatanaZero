#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"

CCollisionMgr::CCollisionMgr()
    : m_matrix{}
{

}


CCollisionMgr::~CCollisionMgr()
{

}



void CCollisionMgr::tick()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		for (UINT iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			if (!(m_matrix[iRow] & (1 << iCol)))
				continue;

			// iRow 레이어와 iCol 레이어는 서로 충돌검사를 진행한다.
			CollisionBtwLayer(pLevel->GetLayer(iRow), pLevel->GetLayer(iCol));
		}
	}
}

void CCollisionMgr::CollisionBtwLayer(CLayer* _Left, CLayer* _Right)
{
	const vector<CGameObject*>& vecLeft = _Left->GetObjects();
	const vector<CGameObject*>& vecRight = _Right->GetObjects();

	if (_Left == _Right)
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = i + 1; j < vecRight.size(); ++j)
			{
				CollisionBtwObject(vecLeft[i], vecRight[j]);				
			}
		}
	}
	else
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = 0; j < vecRight.size(); ++j)
			{
				CollisionBtwObject(vecLeft[i], vecRight[j]);				
			}
		}
	}
}

void CCollisionMgr::CollisionBtwObject(CGameObject* _LeftObject, CGameObject* _RightObject)
{
	if (!(_LeftObject->Collider2D() && _RightObject->Collider2D()))
		return;

	// 충돌체 ID 생성
	CollisionID id = {};
	id.LeftID = _LeftObject->Collider2D()->GetID();
	id.RightID = _RightObject->Collider2D()->GetID();
	
	// ID 검색
	map<UINT_PTR, bool>::iterator iter = m_mapColID.find(id.id);
	if (iter == m_mapColID.end())
	{
		m_mapColID.insert(make_pair(id.id, false));
		iter = m_mapColID.find(id.id);
	}

	// 둘 중 하나라도 삭제 예정 상태라면(Dead 상태)
	bool bDead = false;
	if (_LeftObject->IsDead() || _RightObject->IsDead())
	{
		bDead = true;
	}
	
	// 두 충돌체가 지금 충돌 중인지 확인
	if (CollisionBtwCollider(_LeftObject->Collider2D(), _RightObject->Collider2D()))
	{
		// 이전에 충돌한 적이 있고, 둘중 하나 이상이 삭제 예정이라면
		if (bDead && iter->second)
		{
			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
		}
		else if (iter->second)
		{
			// 계속 충돌 중
			_LeftObject->Collider2D()->OnOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->OnOverlap(_LeftObject->Collider2D());
		}
		else
		{
			// 이번 프레임에 충돌
			if (!bDead) // 둘중 하나라도 Dead 상태면 충돌을 무시한다.
			{
				_LeftObject->Collider2D()->BeginOverlap(_RightObject->Collider2D());
				_RightObject->Collider2D()->BeginOverlap(_LeftObject->Collider2D());
				iter->second = true;
			}			
		}
	}

	else
	{
		// 충돌 해제
		if (iter->second)
		{
			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
			iter->second = false;
		}
	}


}

// 두 충돌체의 충돌 검사 진행
bool CCollisionMgr::CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	// 사각형과 사각형 충돌
	if (_pLeft->GetShape() == COLLIDER2D_TYPE::RECT && _pRight->GetShape() == COLLIDER2D_TYPE::RECT)
		return BoxToBoxCollider(_pLeft, _pRight);

	// 선과 사각형 충돌
	else if (_pLeft->GetShape() == COLLIDER2D_TYPE::LINE && _pRight->GetShape() == COLLIDER2D_TYPE::RECT)
		return LineToBoxCollider(_pLeft, _pRight);

	else if (_pLeft->GetShape() == COLLIDER2D_TYPE::RECT && _pRight->GetShape() == COLLIDER2D_TYPE::LINE)
		return LineToBoxCollider(_pRight, _pLeft);

	// 선과 선 충돌
	else if (_pLeft->GetShape() == COLLIDER2D_TYPE::LINE && _pRight->GetShape() == COLLIDER2D_TYPE::LINE)
		return LineToLineCollider(_pLeft, _pRight);

	// 선과 원 충돌
	else if (_pLeft->GetShape() == COLLIDER2D_TYPE::LINE && _pRight->GetShape() == COLLIDER2D_TYPE::CIRCLE)
		return LineToCircleCollider(_pLeft, _pRight);

	else if (_pLeft->GetShape() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetShape() == COLLIDER2D_TYPE::LINE)
		return LineToCircleCollider(_pRight, _pLeft);

	// 원과 사각형 충돌
	else if (_pLeft->GetShape() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetShape() == COLLIDER2D_TYPE::RECT)
		return CircleToBoxCollider(_pLeft, _pRight);

	else if (_pLeft->GetShape() == COLLIDER2D_TYPE::RECT && _pRight->GetShape() == COLLIDER2D_TYPE::CIRCLE)
		return CircleToBoxCollider(_pRight, _pLeft);

	// 안걸리는건 일단은 BTB로.
	return BoxToBoxCollider(_pLeft, _pRight);
}

bool CCollisionMgr::BoxToBoxCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	// 0 -- 1
	// |    |
	// 3 -- 2
	Vec3 arrLocal[4] =
	{
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f),
	};

	// 두 충돌체의 각 표면 벡터 2개씩 구함
	Vec3 arrProj[4] = {};

	arrProj[0] = XMVector3TransformCoord(arrLocal[1], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
	arrProj[1] = XMVector3TransformCoord(arrLocal[3], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());

	arrProj[2] = XMVector3TransformCoord(arrLocal[1], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
	arrProj[3] = XMVector3TransformCoord(arrLocal[3], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());

	// 두 충돌체의 중심점을 구함
	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());


	// 분리축 테스트
	for (int i = 0; i < 4; ++i)
	{
		Vec3 vProj = arrProj[i];
		vProj.Normalize();

		// 4개의 표면백터를 지정된 투영축으로 투영시킨 거리의 총합 / 2
		float fProjDist = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fProjDist += fabsf(arrProj[j].Dot(vProj));
		}
		fProjDist /= 2.f;

		float fCenter = fabsf(vCenter.Dot(vProj));

		if (fProjDist < fCenter)
			return false;
	}

	return true;
}

bool CCollisionMgr::CircleToBoxCollider(CCollider2D* _Circle, CCollider2D* _Box)
{
	// 0 -- 1
	// |    |
	// 3 -- 2
	Vec3 arrLocalBox[4] =
	{
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f),
	};

	Vec3 CircleCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _Circle->GetColliderWorldMat());

	Vec3 CircleRight = XMVector3TransformCoord(Vec3(0.5f, 0.f, 0.f), _Circle->GetColliderWorldMat());

	// Box의 꼭지점 4개
	Vec3 arrBox[4] = {};

	arrBox[0] = XMVector3TransformCoord(arrLocalBox[0], _Box->GetColliderWorldMat());
	arrBox[1] = XMVector3TransformCoord(arrLocalBox[1], _Box->GetColliderWorldMat());
	arrBox[2] = XMVector3TransformCoord(arrLocalBox[2], _Box->GetColliderWorldMat());
	arrBox[3] = XMVector3TransformCoord(arrLocalBox[3], _Box->GetColliderWorldMat());

	if (LineToCircleCollisionCheck(arrBox[0], arrBox[1], CircleCenter, CircleRight))
		return true;
	if (LineToCircleCollisionCheck(arrBox[0], arrBox[3], CircleCenter, CircleRight))
		return true;
	if (LineToCircleCollisionCheck(arrBox[1], arrBox[2], CircleCenter, CircleRight))
		return true;
	if (LineToCircleCollisionCheck(arrBox[2], arrBox[3], CircleCenter, CircleRight))
		return true;

	return false;
}

bool CCollisionMgr::LineToBoxCollider(CCollider2D* _LineCollider, CCollider2D* _BoxCollider)
{
	Vec3 arrLocalLine[2] =
	{
		Vec3(-0.5f, 0.f, 0.f),
		Vec3(0.5f, 0.f, 0.f),
	};

	// 0 -- 1
	// |    |
	// 3 -- 2
	Vec3 arrLocalBox[4] =
	{
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f),
	};

	// 선의 꼭지점 2개
	Vec3 arrLine[2] = {};

	arrLine[0] = XMVector3TransformCoord(arrLocalLine[0], _LineCollider->GetColliderWorldMat());
	arrLine[1] = XMVector3TransformCoord(arrLocalLine[1], _LineCollider->GetColliderWorldMat());

	// Box의 꼭지점 4개
	Vec3 arrBox[4] = {};

	arrBox[0] = XMVector3TransformCoord(arrLocalBox[0], _BoxCollider->GetColliderWorldMat());
	arrBox[1] = XMVector3TransformCoord(arrLocalBox[1], _BoxCollider->GetColliderWorldMat());
	arrBox[2] = XMVector3TransformCoord(arrLocalBox[2], _BoxCollider->GetColliderWorldMat());
	arrBox[3] = XMVector3TransformCoord(arrLocalBox[3], _BoxCollider->GetColliderWorldMat());

	float BoxMinX = min(min(min(arrBox[0].x, arrBox[1].x), arrBox[2].x), arrBox[3].x);
	float BoxMaxX = max(max(max(arrBox[0].x, arrBox[1].x), arrBox[2].x), arrBox[3].x);
	float BoxMinY = min(min(min(arrBox[0].y, arrBox[1].y), arrBox[2].y), arrBox[3].y);
	float BoxMaxY = max(max(max(arrBox[0].y, arrBox[1].y), arrBox[2].y), arrBox[3].y);

	if (arrLine[0].x > BoxMaxX && arrLine[1].x > BoxMaxX) {
		return false;
	}

	if (arrLine[0].x < BoxMinX && arrLine[1].x < BoxMinX) {
		return false;
	}

	if (arrLine[0].y > BoxMaxY && arrLine[1].y > BoxMaxY) {
		return false;
	}

	if (arrLine[0].y < BoxMinY && arrLine[1].y < BoxMinY) {
		return false;
	}

	// Box 꼭지점 4개로 만든 4개의 선과 선이 겹치는지 확인.
	if (LineToLineCollisionCheck(arrLine[0], arrLine[1], arrBox[0], arrBox[1]))
		return true;
	if (LineToLineCollisionCheck(arrLine[0], arrLine[1], arrBox[0], arrBox[3]))
		return true;
	if (LineToLineCollisionCheck(arrLine[0], arrLine[1], arrBox[1], arrBox[2]))
		return true;
	if (LineToLineCollisionCheck(arrLine[0], arrLine[1], arrBox[3], arrBox[2]))
		return true;

	return false;
}

bool CCollisionMgr::LineToCircleCollider(CCollider2D* _LineCollider, CCollider2D* _CircleCollider)
{
	Vec3 arrLocalLine[2] =
	{
		Vec3(-0.5f, 0.f, 0.f),
		Vec3(0.5f, 0.f, 0.f),
	};

	// 선의 꼭지점 2개
	Vec3 arrLine[2] = {};

	arrLine[0] = XMVector3TransformCoord(arrLocalLine[0], _LineCollider->GetColliderWorldMat());
	arrLine[1] = XMVector3TransformCoord(arrLocalLine[1], _LineCollider->GetColliderWorldMat());

	Vec3 CircleCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _CircleCollider->GetColliderWorldMat());
	
	Vec3 CircleRight = XMVector3TransformCoord(arrLocalLine[1], _CircleCollider->GetColliderWorldMat());

	return LineToCircleCollisionCheck(arrLine[0], arrLine[1], CircleCenter, CircleRight);
}

bool CCollisionMgr::LineToLineCollider(CCollider2D* _LineCollider1, CCollider2D* _LineCollider2)
{
	Vec3 arrLocalLine[2] =
	{
		Vec3(-0.5f, 0.f, 0.f),
		Vec3(0.5f, 0.f, 0.f),
	};

	// 선의 꼭지점 2개
	Vec3 arrLeftLine[2] = {};

	arrLeftLine[0] = XMVector3TransformCoord(arrLocalLine[0], _LineCollider1->GetColliderWorldMat());
	arrLeftLine[1] = XMVector3TransformCoord(arrLocalLine[1], _LineCollider1->GetColliderWorldMat());

	// 선의 꼭지점 2개
	Vec3 arrRightLine[2] = {};

	arrRightLine[0] = XMVector3TransformCoord(arrLocalLine[0], _LineCollider2->GetColliderWorldMat());
	arrRightLine[1] = XMVector3TransformCoord(arrLocalLine[1], _LineCollider2->GetColliderWorldMat());

	if (LineToLineCollisionCheck(arrLeftLine[0], arrLeftLine[1], arrRightLine[0], arrRightLine[1]))
		return true;

	return false;
}

bool CCollisionMgr::LineToCircleCollisionCheck(Vec3 _LP1, Vec3 _LP2, Vec3 _CircleCenter, Vec3 _CircleRight)
{
	float fRadius = std::sqrt((_CircleRight.x - _CircleCenter.x) * (_CircleRight.x - _CircleCenter.x) + (_CircleRight.y - _CircleCenter.y) * (_CircleRight.y - _CircleCenter.y));

	// 원 안에 선이 있는지 확인
	float distance1 = std::sqrt((_LP1.x - _CircleCenter.x) * (_LP1.x - _CircleCenter.x) + (_LP1.y - _CircleCenter.y) * (_LP1.y - _CircleCenter.y));
	float distance2 = std::sqrt((_LP2.x - _CircleCenter.x) * (_LP2.x - _CircleCenter.x) + (_LP2.y - _CircleCenter.y) * (_LP2.y - _CircleCenter.y));

	if (distance1 <= fRadius && distance2 <= fRadius)
		return true;

	// 원과 선이 겹치는지 확인
	Vec3 lineVec = _LP2 - _LP1;
	Vec3 circleToLine = _CircleCenter - _LP1;
	float projection = (circleToLine.x * lineVec.x + circleToLine.y * lineVec.y) / (lineVec.x * lineVec.x + lineVec.y * lineVec.y);
	projection = max(0.0f, min(1.0f, projection));
	Vec3 closestPoint = _LP1 + projection * lineVec;
	float distance = std::sqrt((_CircleCenter.x - closestPoint.x) * (_CircleCenter.x - closestPoint.x) + (_CircleCenter.y - closestPoint.y) * (_CircleCenter.y - closestPoint.y));

	return distance <= fRadius;
}

bool CCollisionMgr::LineToLineCollisionCheck(Vec3 _L1P1, Vec3 _L1P2, Vec3 _L2P1, Vec3 _L2P2)
{
	Vec3 L1Left = _L1P2;
	Vec3 L1Right = _L1P1;

	if (_L1P1.x < _L1P2.x)
	{
		L1Left = _L1P1;
		L1Right = _L1P2;
	}

	Vec3 L2Left = _L2P2;
	Vec3 L2Right = _L2P1;

	if (_L2P1.x < _L2P2.x)
	{
		L2Left = _L2P1;
		L2Right = _L2P2;
	}

	if (L1Right.x < L2Left.x)
		return false;

	if (L2Right.x < L1Left.x)
		return false;

	// L1을 기준으로 L2의 두 점의 위치를 확인
	float x1 = L1Right.x - L1Left.x;
	float y1 = L1Right.y - L1Left.y;
	float x2 = L2Right.x - L1Left.x;
	float y2 = L2Right.y - L1Left.y;

	float x3 = L1Right.x - L1Left.x;
	float y3 = L1Right.y - L1Left.y;
	float x4 = L2Left.x - L1Left.x;
	float y4 = L2Left.y - L1Left.y;

	bool bLPisLeft = false;
	bool bRPisLeft = false;

	bLPisLeft = (x1 * y2 - x2 * y1) < 0;
	bRPisLeft = (x3 * y4 - x4 * y3) < 0;

	if (bLPisLeft == bRPisLeft)
		return false;
	
	// L2을 기준으로 L1의 두 점의 위치를 확인
	x1 = L2Right.x - L2Left.x;
	y1 = L2Right.y - L2Left.y;
	x2 = L1Right.x - L2Left.x;
	y2 = L1Right.y - L2Left.y;

	x3 = L2Right.x - L2Left.x;
	y3 = L2Right.y - L2Left.y;
	x4 = L1Left.x - L2Left.x;
	y4 = L1Left.y - L2Left.y;

	bLPisLeft = false;
	bRPisLeft = false;

	bLPisLeft = (x1 * y2 - x2 * y1) < 0;
	bRPisLeft = (x3 * y4 - x4 * y3) < 0;

	if (bLPisLeft == bRPisLeft)
		return false;

	return true;
}



void CCollisionMgr::LayerCheck(UINT _left, UINT _right)
{
	UINT iRow = (UINT)_left;
	UINT iCol = (UINT)_right;

	if (iRow > iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iTemp;
	}

	m_matrix[iRow] |= (1 << iCol);
}

void CCollisionMgr::LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	CLayer* pLeft = pCurLevel->FindLayerByName(_strLeftLayer);
	CLayer* pRight = pCurLevel->FindLayerByName(_strRightLayer);

	LayerCheck(pLeft->GetLayerIndex(), pRight->GetLayerIndex());
}
