#include "pch.h"
#include "CAnim2D.h"

#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CResMgr.h"

CAnim2D::CAnim2D()
	: m_pOwner(nullptr)
	, m_iCurFrm(0)
	, m_bFinish(false)
	, m_fTime(0.f)
	, m_iFPS(0)
	, m_bStop(false)
{
}

CAnim2D::CAnim2D(const CAnim2D& _Other)
	: CEntity(_Other)
	, m_iCurFrm(0)
	, m_bFinish(false)
	, m_fTime(0.f)
	, m_iFPS(_Other.m_iFPS)
	, m_vecFrm(_Other.m_vecFrm)
	, m_vBackSize(_Other.m_vBackSize)
{
	m_AtlasTex = _Other.m_AtlasTex;
}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::finaltick()
{	
	if (m_bFinish)
		return;
	
	if (m_bStop)
		return;

	m_fTime += DT;

	//// 에디터로 프레임이 삭제되는 경우를 대비.
	//if (m_vecFrm.size() <= m_iCurFrm)
	//{
	//	m_iCurFrm = 0;
	//}

	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;
		++m_iCurFrm;

		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}
}

void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS)
{
	assert(_AtlasTex.Get());

	SetName(_strAnimName);

	m_AtlasTex = _AtlasTex;

	m_iFPS = _FPS;

	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	m_vBackSize = _vBackSize / vResolution;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / vResolution;
		frm.SliceUV = _vSlice / vResolution;

		m_vecFrm.push_back(frm);
	}
}

void CAnim2D::CreateAtlas(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vBackSize)
{
	assert(_AtlasTex.Get());

	SetName(_strAnimName);

	m_AtlasTex = _AtlasTex;

	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	m_vBackSize = _vBackSize / vResolution;
}

void CAnim2D::AddAtlasFrm(Vec2 _vLeftTop, Vec2 _vSlice, int _Index)
{
	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());
	
	int Index = _Index;

	tAnim2DFrm frm = {};

	frm.fDuration = 1.f / (float)m_iFPS;
	frm.LeftTopUV = _vLeftTop / vResolution;
	frm.SliceUV = _vSlice / vResolution;

	// 맨 뒤에 추가
	if (-1 == Index)
	{
		m_vecFrm.push_back(frm);
	}

	// 중간 삽입
	else
	{
		auto iter = m_vecFrm.begin();

		for (int i = 0; i < Index; ++i)
		{
			iter++;
		}

		m_vecFrm.insert(iter, frm);
	}
}

void CAnim2D::SetFPS(int _FPS)
{
	m_iFPS = _FPS;

	auto iter = m_vecFrm.begin();
	auto iterEnd = m_vecFrm.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->fDuration = 1.f / (float)m_iFPS;
	}
}

void CAnim2D::SetLeftTop(Vec2 _vLeftTop, int _Index)
{
	if (_Index >= m_vecFrm.size() || _Index < 0)
		return;

	auto iter = m_vecFrm.begin();

	for (int i = 0; i < _Index; ++i)
	{
		iter++;
	}

	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	iter->LeftTopUV = _vLeftTop / vResolution;
}

void CAnim2D::SetSlice(Vec2 _vSlice, int _Index)
{
	if (_Index >= m_vecFrm.size() || _Index < 0)
		return;

	auto iter = m_vecFrm.begin();

	for (int i = 0; i < _Index; ++i)
	{
		iter++;
	}

	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	iter->SliceUV = _vSlice / vResolution;
}

void CAnim2D::SetOffsetPos(Vec2 _OffsetPos, int _Index)
{
	if (_Index >= m_vecFrm.size() || _Index < 0)
		return;

	auto iter = m_vecFrm.begin();

	for (int i = 0; i < _Index; ++i)
	{
		iter++;
	}

	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	iter->Offset = _OffsetPos / vResolution;
}

void CAnim2D::SetAllOffsetPos(Vec2 _OffsetPos)
{
	auto iter = m_vecFrm.begin();
	auto iterEnd = m_vecFrm.end();

	for (; iter != iterEnd; ++iter)
	{
		Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

		iter->Offset = _OffsetPos / vResolution;
	}
}

void CAnim2D::DeleteFrm(int _Index)
{
	if (m_vecFrm.size() == 1)
		return;

	if (_Index >= m_vecFrm.size())
		return;

	auto iter = m_vecFrm.begin();

	m_vecFrm.erase(iter + _Index);
}

void CAnim2D::SetBackSize(Vec2 _vBackSize)
{
	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	m_vBackSize = _vBackSize / vResolution;
}

void CAnim2D::SwapFrm(int _Index1, int _Index2)
{
	auto iter = m_vecFrm.begin();

	tAnim2DFrm Temp = {};
	Temp = *(iter + _Index1);

	*(iter + _Index1) = *(iter + _Index2);

	*(iter + _Index2) = Temp;
}

void CAnim2D::Save(const wstring& _strRelativePath)
{
	wstring strFilepath = CPathMgr::GetInst()->GetContentPath();
	strFilepath += _strRelativePath;

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"wb");

	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return;
	}

	// 프레임 정보 저장
	size_t iFrmCount = m_vecFrm.size();
	fwrite(&iFrmCount, sizeof(size_t), 1, pFile);
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), m_vecFrm.size(), pFile);

	// BackSize 저장
	fwrite(&m_vBackSize, sizeof(Vec2), 1, pFile);

	// 상대경로 저장
	SaveWString(m_AtlasTex->GetRelativePath(), pFile);

	fclose(pFile);
}

void CAnim2D::Load(const wstring& _strRelativePath)
{
	wstring strFilepath = CPathMgr::GetInst()->GetContentPath();
	strFilepath += _strRelativePath;

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"rb");

	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Load 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 로딩 실패", MB_OK);
		return;
	}

	SetName(_strRelativePath);

	// 프레임 정보 로드
	size_t iFrmCount = 0;
	fread(&iFrmCount, sizeof(size_t), 1, pFile);
	
	for (size_t i = 0; i < iFrmCount; ++i)
	{
		tAnim2DFrm frm = {};
		fread(&frm, sizeof(tAnim2DFrm), 1, pFile);
		m_vecFrm.push_back(frm);
	}

	// FPS 설정
	m_iFPS = 1 / m_vecFrm.begin()->fDuration;

	// BackSize 로드
	fread(&m_vBackSize, sizeof(Vec2), 1, pFile);

	// 상대경로 로드
	wstring strRelativePath;
	LoadWString(strRelativePath, pFile);

	m_AtlasTex = CResMgr::GetInst()->Load<CTexture>(strRelativePath, strRelativePath);
	
	fclose(pFile);
}

