#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"

class CAnim2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim2D*>  m_mapAnim;  // Animation 목록
    CAnim2D*                m_pCurAnim; // 현재 재생중인 Animation
    bool                    m_bRepeat;  // 반복

    int                     m_iLeft;    // 좌우 방향
    Vec3                    m_ArrangeColor;


public:
    void SetIsLeft(int _iLeft) { m_iLeft = _iLeft; }
    int IsLeft() { return m_iLeft; }

    void SetArrangeColor(Vec3 _Color) { m_ArrangeColor = _Color; }
    Vec3 GetArrangeColor() { return m_ArrangeColor; }

public:
    virtual void finaltick() override;
    void UpdateData();
    void Clear();

public:
    void Play(const wstring& _strName, bool _bRepeat);
    CAnim2D* FindAnim(const wstring& _strName);
    void DeleteAnim(const wstring& _strAnimName);
    void ChangeAnimName(const wstring& _strPrevName, const wstring& _strNewName);

    void CreateAnimation(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    
    void CreateAtlasAnimation(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vBackSize);
    
    // _Index로 어느 위치에 프레임을 삽입할지 정함. -1이면 맨 뒤.
    void AddAtlasFrm(const wstring& _strAnimName, Vec2 _vLeftTop, Vec2 _vSlice, int _Index = -1);
    void SetAnimFPS(const wstring& _strAnimName, int _FPS);
    void SetLeftTop(const wstring& _strAnimName, Vec2 _vLeftTop, int _Index);
    void SetSlice(const wstring& _strAnimName, Vec2 _vSlice, int _Index);
    void SetOffsetPos(const wstring& _strAnimName, Vec2 _OffsetPos, int _Index);
    void DeleteFrm(const wstring& _strAnimName, int _Index);
    void SetBackSize(const wstring& _strAnimName, Vec2 _vBackSize);

    CAnim2D* LoadAnim2D(const wstring& _strRelativePath);

    const map<wstring, CAnim2D*>& GetAnimationMap() { return m_mapAnim; };

    const wstring& GetCurAnimationName();
    

    CLONE(CAnimator2D);
public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _Other);
    ~CAnimator2D();
};

