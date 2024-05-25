#pragma once

#include "UI.h"

class TileMapTool :
    public UI
{
public:
    TileMapTool();
    ~TileMapTool();

private:
    class CGameObject* m_pTarget;
    UINT                m_iCountX;          // 타일 가로
    UINT                m_iCountY;          // 타일 세로
    Vec2                m_vTileSliceSize;   // 타일 하나의 크기(UV 단위)
    vector<tTile>       m_vecTileInfo;

    Ptr<class CTexture> m_TileAtlasTex;
    int                 m_TileIndex[2];     // 선택된 타일 Index

public:
    void SetTileIndex(int _iIndexX, int _iIndexY) { m_TileIndex[0] = _iIndexX; m_TileIndex[1] = _iIndexY; }
    const vector<tTile>& GetTileInfo() { return m_vecTileInfo; }

public:
    virtual void finaltick() override;
    virtual int render_update() override;

public:
    void SetTargetObject(class CGameObject* _Target);

public:
    void Reset();
};

