#pragma once
#include "Base.h"

class CEventListner :
    public CBase
{
    //게임 오브젝트로 추가 되지만, 그려지지 않고 별다른 것을 하지 않음.

    //대신, 오브젝트들간의 이벤트를 받고, 돌려주는 역할을 함.
    //프리오리티에서 각 

    HRESULT Add_EventListner(CGameObject* pObject);

};

