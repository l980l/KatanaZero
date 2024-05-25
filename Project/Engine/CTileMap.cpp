#include "pch.h"
#include "CTileMap.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_iTileCountX(1)
	, m_iTileCountY(1)
{	
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\tilemap_bg_motel.mtrl"));

	m_Buffer = new CStructuredBuffer;
	m_Buffer->Create(sizeof(tTile), m_iTileCountX * m_iTileCountY, SB_TYPE::READ_ONLY, true);
}

CTileMap::~CTileMap()
{
	if (nullptr != m_Buffer)
		delete m_Buffer;
}

void CTileMap::finaltick()
{
}

void CTileMap::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	// Transform 에 UpdateData 요청
	Transform()->UpdateData();

	// 재질 업데이트
	GetMaterial()->SetScalarParam(INT_0, &m_iTileCountX);
	GetMaterial()->SetScalarParam(INT_1, &m_iTileCountY);
	GetMaterial()->UpdateData();

	// 구조화버퍼 업데이트
	UpdateData();

	// 렌더
	GetMesh()->render();
}

void CTileMap::UpdateData()
{
	m_Buffer->SetData(m_vecTile.data(), sizeof(tTile) * m_vecTile.size());
	m_Buffer->UpdateData(20, PIPELINE_STAGE::PS_PIXEL);
}

void CTileMap::SetTileCount(UINT _iXCount, UINT _iYCount)
{
	m_iTileCountX = _iXCount;
	m_iTileCountY = _iYCount;

	m_vecTile.clear();
	m_vecTile.resize(m_iTileCountX * m_iTileCountY);

	if (m_Buffer->GetElementCount() < m_vecTile.size())
	{
		m_Buffer->Create(sizeof(tTile), (UINT)m_vecTile.size(), SB_TYPE::READ_ONLY, true);
	}

	// 타일 세팅 테스트
	for (size_t i = 0; i < m_iTileCountY; ++i)
	{
		for (size_t j = 0; j < m_iTileCountX; ++j)
		{			
			m_vecTile[i * m_iTileCountX + j].vLeftTop.x = m_vSliceSize.x * j;
			m_vecTile[i * m_iTileCountX + j].vLeftTop.y = 0.f;
			m_vecTile[i * m_iTileCountX + j].vSlice = m_vSliceSize;
		}
	}

	//m_vecTile[0].vLeftTop = Vec2(m_vSliceSize.x * 7.f, m_vSliceSize.y * 5.f);
	Ptr<class CTexture> TileAtlasTex = GetMaterial()->GetTexParam(TEX_0);

	float fWidth = (float)m_iTileCountX * m_vSliceSize.x * TileAtlasTex->Width();
	float fHeight = (float)m_iTileCountY * m_vSliceSize.y * TileAtlasTex->Height();

	if (GetOwner())
	{
		GetOwner()->Transform()->SetRelativeScale(fWidth, fHeight, 1.f);
	}
}

void CTileMap::SaveToLevelFile(FILE* _File)
{
	SaveWString(GetOwner()->GetName(), _File);

	wstring strFilepath = CPathMgr::GetInst()->GetContentPath();
	strFilepath += GetOwner()->GetName();

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"wb");

	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"TileMap Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return;
	}

	CRenderComponent::SaveToLevelFile(pFile);

	fwrite(&m_iTileCountX, sizeof(UINT), 1, pFile);
	fwrite(&m_iTileCountY, sizeof(UINT), 1, pFile);
	fwrite(&m_vSliceSize, sizeof(Vec2), 1, pFile);
	fwrite(m_vecTile.data(), sizeof(tTile), m_vecTile.size(), pFile);

	fclose(pFile);

	/*CRenderComponent::SaveToLevelFile(_File);

	fwrite(&m_iTileCountX, sizeof(UINT), 1, _File);
	fwrite(&m_iTileCountY, sizeof(UINT), 1, _File);
	fwrite(&m_vSliceSize, sizeof(Vec2), 1, _File);
	fwrite(m_vecTile.data(), sizeof(tTile), m_vecTile.size(), _File);*/
}

void CTileMap::LoadFromLevelFile(FILE* _File)
{
	wstring OwnerName;
	LoadWString(OwnerName, _File);
	
	wstring strFilepath = CPathMgr::GetInst()->GetContentPath();
	strFilepath += OwnerName;

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"rb");

	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"TileMap Load 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return;
	}

	CRenderComponent::LoadFromLevelFile(pFile);

	fread(&m_iTileCountX, sizeof(UINT), 1, pFile);
	fread(&m_iTileCountY, sizeof(UINT), 1, pFile);
	fread(&m_vSliceSize, sizeof(Vec2), 1, pFile);

	SetTileCount(m_iTileCountX, m_iTileCountY);

	fread(m_vecTile.data(), sizeof(tTile), m_iTileCountX * m_iTileCountY, pFile);

	fclose(pFile);

	/*CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_iTileCountX, sizeof(UINT), 1, _File);
	fread(&m_iTileCountY, sizeof(UINT), 1, _File);
	fread(&m_vSliceSize, sizeof(Vec2), 1, _File);

	SetTileCount(m_iTileCountX, m_iTileCountY);

	fread(m_vecTile.data(), sizeof(tTile), m_iTileCountX * m_iTileCountY, _File);*/
}

void CTileMap::SetTile(const vector<tTile>& _vTile)
{
	// 타일 세팅
	for (size_t i = 0; i < m_iTileCountY; ++i)
	{
		for (size_t j = 0; j < m_iTileCountX; ++j)
		{
			m_vecTile[i * m_iTileCountX + j].vLeftTop.x = _vTile[i * m_iTileCountX + j].vLeftTop.x;
			m_vecTile[i * m_iTileCountX + j].vLeftTop.y = _vTile[i * m_iTileCountX + j].vLeftTop.y;
			m_vecTile[i * m_iTileCountX + j].vSlice = _vTile[i * m_iTileCountX + j].vSlice;
		}
	}
}
