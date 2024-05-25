#pragma once
#include "ComponentUI.h"


class TileMapUI :
    public ComponentUI
{
private:
    class TileMapTool*        m_pTileMapTool;
    Ptr<class CTexture>       m_TileAtlasTex;


public:
    virtual int render_update() override;

public:
    void SelectMaterial(DWORD_PTR _Key);

public:
    TileMapUI();
    ~TileMapUI();
};

