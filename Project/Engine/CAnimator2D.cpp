#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"
#include "CMeshRender.h"

#include "ptr.h"
#include "CMaterial.h"
#include "CResMgr.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
	, m_iLeft(0)
	, m_ArrangeColor{1.f, 1.f, 1.f}
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _Other)
	: CComponent(_Other)
	, m_bRepeat(_Other.m_bRepeat)
	, m_iLeft(0)
	, m_ArrangeColor(_Other.m_ArrangeColor)
{
	auto iter = _Other.m_mapAnim.begin();
	auto iterEnd = _Other.m_mapAnim.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnim2D* AnimClone = iter->second->Clone();
		AnimClone->m_pOwner = this;

		m_mapAnim.insert(make_pair(iter->first, AnimClone));
	}

	auto CurAnimIter = m_mapAnim.find(_Other.m_pCurAnim->GetName());
	m_pCurAnim = CurAnimIter->second;
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->Reset();
		}

		m_pCurAnim->finaltick();
	}	
}

void CAnimator2D::UpdateData()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	const tAnim2DFrm& frm = m_pCurAnim->GetCurFrame();
	Vec2 vBackSize = m_pCurAnim->GetBackSize();

	int iAnimUse = 1;
	pMtrl->SetScalarParam(INT_0, &iAnimUse);
	pMtrl->SetScalarParam(INT_1, &m_iLeft);				// 애니메이션 방향
	pMtrl->SetScalarParam(VEC2_0, &frm.LeftTopUV);
	pMtrl->SetScalarParam(VEC2_1, &frm.SliceUV);
	pMtrl->SetScalarParam(VEC2_2, &frm.Offset);
	pMtrl->SetScalarParam(VEC2_3, &vBackSize);
	pMtrl->SetScalarParam(VEC4_0, &m_ArrangeColor);		// 색상 적용.

	pMtrl->SetTexParam(TEX_0, m_pCurAnim->GetAtlasTex());
}

void CAnimator2D::Clear()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	int iAnimUse = 0;
	pMtrl->SetScalarParam(INT_0, &iAnimUse);

	Ptr<CTexture> pTex = nullptr;
	pMtrl->SetTexParam(TEX_0, pTex);
}

void CAnimator2D::Play(const wstring& _strName, bool _bRepeat)
{
	CAnim2D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_pCurAnim = pAnim;
	m_bRepeat = _bRepeat;
}

CAnim2D* CAnimator2D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator2D::DeleteAnim(const wstring& _strAnimName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strAnimName);
	
	// 없으면 그냥 return;
	if (iter == m_mapAnim.end())
	{
		return;
	}

	CAnim2D* pAnim = iter->second;

	// 지우려는 Anim이 현재 Anim이면 현재 Anim을 m_mapAnim.begin()->second Anim으로 변경시킴. 만약 m_mapAnim.begin()->second이 삭제하려는 Anim이면 2번째 Anim을 현재 Anim으로 지정.
	if (m_pCurAnim == pAnim)
	{
		if (m_mapAnim.begin()->second == pAnim)
			m_pCurAnim = ++m_mapAnim.begin()->second;

		else
			m_pCurAnim = m_mapAnim.begin()->second;
	}

	delete iter->second;
	m_mapAnim.erase(iter);
}

void CAnimator2D::ChangeAnimName(const wstring& _strPrevName, const wstring& _strNewName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strPrevName);

	// 없으면 그냥 return;
	if (iter == m_mapAnim.end())
	{
		return;
	}

	CAnim2D* pAnim = iter->second;
	pAnim->m_pOwner = this;

	m_mapAnim.insert(make_pair(_strNewName, pAnim));
	
	// delete iter->second 를 하면 안됨. CAnim2D* 는 그대로 쓰고 있으니까.
	m_mapAnim.erase(iter);
}

void CAnimator2D::CreateAnimation(const wstring& _strAnimName
	, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize
	, int _FrameCount, int _FPS)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Create(_strAnimName, _AtlasTex, _vLeftTop, _vSlice, _vBackSize, _FrameCount, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}

void CAnimator2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);
	
	size_t AnimCount = m_mapAnim.size();
	fwrite(&AnimCount, sizeof(size_t), 1, _File);

	// Anim은 별개 파일로 저장함.
	for (const auto& pair : m_mapAnim)
	{	
		//wstring wstrPath = L"animation\\" + pair.first + L".anim";
		wstring wstrPath = pair.first;
		pair.second->Save(wstrPath);
		SaveWString(wstrPath, _File);
	}

	wstring strCurAnimName;	
	if(nullptr != m_pCurAnim)	
	{
		strCurAnimName = m_pCurAnim->GetName();
	}
	SaveWString(strCurAnimName, _File);
}

void CAnimator2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_bRepeat, sizeof(bool), 1, _File);

	size_t AnimCount = 0;
	fread(&AnimCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < AnimCount; ++i)
	{
		wstring wstrPath;
		LoadWString(wstrPath, _File);

		CAnim2D* pNewAnim = new CAnim2D;
		pNewAnim->Load(wstrPath);
		
		m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
		pNewAnim->m_pOwner = this;
	}

	wstring strCurAnimName;
	LoadWString(strCurAnimName, _File);

	m_pCurAnim = FindAnim(strCurAnimName);		
}

void CAnimator2D::CreateAtlasAnimation(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vBackSize)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->CreateAtlas(_strAnimName, _AtlasTex, _vBackSize);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}

void CAnimator2D::AddAtlasFrm(const wstring& _strAnimName, Vec2 _vLeftTop, Vec2 _vSlice, int _Index)
{
	CAnim2D* Anim = FindAnim(_strAnimName);

	if (!Anim)
		return;

	Anim->AddAtlasFrm(_vLeftTop, _vSlice, _Index);
}

void CAnimator2D::SetAnimFPS(const wstring& _strAnimName, int _FPS)
{
	CAnim2D* Anim = FindAnim(_strAnimName);

	if (!Anim)
		return;

	Anim->SetFPS(_FPS);
}

void CAnimator2D::SetLeftTop(const wstring& _strAnimName, Vec2 _vLeftTop, int _Index)
{
	CAnim2D* Anim = FindAnim(_strAnimName);

	if (!Anim)
		return;

	Anim->SetLeftTop(_vLeftTop, _Index);
}

void CAnimator2D::SetSlice(const wstring& _strAnimName, Vec2 _vSlice, int _Index)
{
	CAnim2D* Anim = FindAnim(_strAnimName);

	if (!Anim)
		return;

	Anim->SetSlice(_vSlice, _Index);
}

void CAnimator2D::SetOffsetPos(const wstring& _strAnimName, Vec2 _OffsetPos, int _Index)
{
	CAnim2D* Anim = FindAnim(_strAnimName);

	if (!Anim)
		return;

	Anim->SetOffsetPos(_OffsetPos, _Index);
}

void CAnimator2D::DeleteFrm(const wstring& _strAnimName, int _Index)
{
	CAnim2D* Anim = FindAnim(_strAnimName);

	if (!Anim)
		return;

	Anim->DeleteFrm(_Index);
}

void CAnimator2D::SetBackSize(const wstring& _strAnimName, Vec2 _vBackSize)
{
	CAnim2D* Anim = FindAnim(_strAnimName);

	if (!Anim)
		return;

	Anim->SetBackSize(_vBackSize);
}

CAnim2D* CAnimator2D::LoadAnim2D(const wstring& _strRelativePath)
{
	CAnim2D* pAnim = new CAnim2D;

	pAnim->m_pOwner = this;

	pAnim->Load(_strRelativePath);

	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));

	return pAnim;
}

const wstring& CAnimator2D::GetCurAnimationName()
{
	return m_pCurAnim->GetName();
}
