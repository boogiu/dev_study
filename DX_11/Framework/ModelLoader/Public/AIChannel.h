#pragma once
#include "Channel.h"

NS_BEGIN(Engine)
    class CModelData;
NS_END

NS_BEGIN(Loader)
class CAIChannel :
    public CChannel
{
private:
    CAIChannel();
    virtual ~CAIChannel() DEFAULT;

public:
    HRESULT Initialize(const aiNodeAnim* pAIChannel, class CModelData* pData);
    void Save_Channel(ofstream& ofs);

public:
    virtual void Render_GUI() override;
    virtual HRESULT TranslateAnimateMatrix(vector<_float4x4>& transfomationMatrices, _float CurrentTrackPosition, _bool Loop)override;
    virtual HRESULT ConvertAnimateMatrix(vector<_float4x4>& transfomationMatrices, _float ConvertDuration, _float PrevTrackPosition, _float ConversionTrackPosition, CChannel* NextChannel)override;

public:
    void Remove_MdlTransform() { m_RemoveMdlTrans = !m_RemoveMdlTrans; };

private:
    string m_boneName = {};
    _bool m_isModelRoot = {false};

    _bool m_RemoveMdlTrans = { false };
    KEYFRAME nowFrame = {};

public:
    static CAIChannel* Create(const aiNodeAnim* pAIChannel,class CModelData* pData);
    virtual void Free() override;
};

NS_END