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

private:
    string m_boneName = {};
public:
    static CAIChannel* Create(const aiNodeAnim* pAIChannel,class CModelData* pData);
    virtual void Free() override;
};

NS_END