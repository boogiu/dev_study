#pragma once
#include "Base.h"
NS_BEGIN(Engine)
class ENGINE_DLL CChannel :
    public CBase
{
protected:
    CChannel();
    virtual ~CChannel() DEFAULT;
public:
    HRESULT Initialize();
    virtual HRESULT TranslateAnimateMatrix( vector<_float4x4>& transfomationMatrices, _float CurrentTrackPosition , _bool Loop);

    virtual HRESULT ConvertAnimateMatrix(vector<_float4x4>& transfomationMatrices, _float ConvertDuration, _float PrevTrackPosition,_float ConversionTrackPosition, CChannel*  NextChannel);

    const string& Get_Name() { return m_ChannelName; }

public:
    virtual void Render_GUI();
protected:
    string				m_ChannelName;       //본과 일치할 수 있도록.
    _uint				m_iBoneIndex = {};
    _uint				m_iNumKeyFrames = {};
    vector<KEYFRAME>	m_KeyFrames;

public:
    static CChannel* Create();
    virtual void Free() override;
};

NS_END