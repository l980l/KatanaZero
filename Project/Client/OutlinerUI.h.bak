#pragma once
#include "UI.h"

class TreeUI;

class OutlinerUI :
    public UI
{
private:
    TreeUI* m_Tree;

public:
    virtual void tick() override;
    virtual int render_update() override;

public:
    void ResetOutliner();
    void SetTargetToInspector(DWORD_PTR _SelectedNode);


public:
    OutlinerUI();
    ~OutlinerUI();
};

