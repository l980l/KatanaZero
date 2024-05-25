#pragma once
#include "CSingleton.h"

class CLayer;
class CCollider2D;


union CollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};


class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_matrix[MAX_LAYER];
	map<UINT_PTR, bool>		m_mapColID;

public:
	void LayerCheck(UINT _left, UINT _right);
	void LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer);

	void Clear()
	{
		memset(m_matrix, 0, sizeof(UINT) * MAX_LAYER);
	}

public:
	void tick();

private:
	void CollisionBtwLayer(CLayer* _LeftLayer, CLayer* _RightLayer);
	void CollisionBtwObject(CGameObject* _LeftObject, CGameObject* _RightObject);
	bool CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight);

	bool BoxToBoxCollider(CCollider2D* _pLeft, CCollider2D* _pRight);
	bool CircleToBoxCollider(CCollider2D* _Circle, CCollider2D* _Box);
	bool LineToBoxCollider(CCollider2D* _LineCollider, CCollider2D* _BoxCollider);
	bool LineToCircleCollider(CCollider2D* _LineCollider, CCollider2D* _CircleCollider);
	bool LineToLineCollider(CCollider2D* _LineCollider1, CCollider2D* _LineCollider2);

	bool LineToCircleCollisionCheck(Vec3 _LP1, Vec3 _LP2, Vec3 _CircleCenter, Vec3 _CircleRight);
	bool LineToLineCollisionCheck(Vec3 _L1P1, Vec3 _L1P2, Vec3 _L2P1, Vec3 _L2P2);
};

