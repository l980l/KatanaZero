#pragma once
#include "CEntity.h"

#include "ptr.h"
#include "CTexture.h"

class CAnimator2D;

class CAnim2D :
    public CEntity
{
private:
    CAnimator2D*        m_pOwner;
    vector<tAnim2DFrm>  m_vecFrm;
    Vec2                m_vBackSize;
    Ptr<CTexture>       m_AtlasTex;

    int                 m_iCurFrm;
    float               m_fTime;

    int                 m_iFPS;

    bool                m_bFinish;

    //
    bool                m_bStop;
    
public:
    void SetCurIndex(int Index) { m_iCurFrm = Index; }
    void SetStop(bool _bStop) { m_bStop = _bStop; }

public:
    int GetCurFrameIndex() { return m_iCurFrm; }
    int GetEndFrameIndex() { return (int)m_vecFrm.size() - 1; }

public:
    void finaltick();
    void Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _BackSize, int _FrameCount, int _FPS);
    
    void CreateAtlas(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vBackSize);
    void AddAtlasFrm(Vec2 _vLeftTop, Vec2 _vSlice, int _Index = -1);
    void SetFPS(int _FPS);
    void SetLeftTop(Vec2 _vLeftTop, int _Index);
    void SetSlice(Vec2 _vSlice, int _Index);
    void SetOffsetPos(Vec2 _OffsetPos, int _Index);
    void SetAllOffsetPos(Vec2 _OffsetPos);
    void DeleteFrm(int _Index);
    void SetBackSize(Vec2 _vBackSize);
    void SwapFrm(int _Index1, int _Index2);

    const tAnim2DFrm& GetCurFrame() { return m_vecFrm[m_iCurFrm];}
    const Vec2& GetBackSize() { return m_vBackSize; }
    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    int GetFPS() { return m_iFPS; }

    void Save(const wstring& _strRelativePath);
    void Load(const wstring& _strRelativePath);

    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_iCurFrm = 0;
        m_fTime = 0.f;
        m_bFinish = false;
    }

    vector<tAnim2DFrm>& GetAnimFrmVector()
    {
        return m_vecFrm;
    }


    CLONE(CAnim2D);
public:
    CAnim2D();
    CAnim2D(const CAnim2D& _Other);
    ~CAnim2D();

    friend class CAnimator2D;
};

