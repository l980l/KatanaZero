#pragma once
#include "UI.h"

class TreeUI;

class ContentUI :
    public UI
{
private:
    TreeUI*     m_Tree;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual int render_update() override;

public:
    void ResetContent();
    void SetTargetToInspector(DWORD_PTR _SelectedNode);

public:
    ContentUI();
    ~ContentUI();
};

