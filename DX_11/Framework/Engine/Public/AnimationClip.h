#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class ENGINE_DLL CAnimationClip :
    public CBase
{
protected:
    CAnimationClip();
    virtual ~CAnimationClip() DEFAULT;

public:
	HRESULT Initialize();
	_float TranslateAnimateMatrix( vector<_float4x4>& transfomationMatrices, _float CurrentTrackPosition, _float dt);
	_float Get_Duration() { return m_fDuration; }
	_bool isLoop() { return m_bLoop; }

	_bool ConvertTo(vector<_float4x4>& transfomationMatrices,
		 CAnimationClip& DestAnimation, 
		_float fConvertDuration, 
		_float PrevTrackPosition, 
		_float ConversionTrackPosition);

public:
	class CChannel* Find_ChannelByBoneName(const string& boneName);

public:
	virtual void Render_GUI();
	const string& Get_Name() { return m_ClipName; }

protected:
	_bool					m_bLoop = { false };
	_float					m_fDuration = {}; //ÃÑ Àç»ý
	_float					m_fTickPerSecond = {}; //¼Óµµ
	_uint					m_iNumChannels = {};
	string				m_ClipName = {};
	vector<class CChannel*> m_Channels;

public:
    static CAnimationClip* Create();
	virtual void Free();
};
NS_END
