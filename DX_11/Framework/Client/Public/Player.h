#pragma once
#include "GameObject.h"
NS_BEGIN(Client)
class CPlayer :
    public CGameObject
{
public:
    typedef struct tagPlayerPartsDesc : GAMEOBJECT_DESC {
        CGameObject* pPlayer = { nullptr };
    }PLAYER_PARTS_DESC;

    struct MovementPacket {
        _bool bFliping = { false };
        _float fCurrentDegree= {};
        _float fTargetDegree = {};
        _float fPlayerHeight = {};
        _float  fMoveSpeed = { 25.f };
        _float2 vInputAxis = {};
    };

    struct TileInfoPacket{
        TILE_INDEX nowIndex = {};
        TILE_INFO Range_FowardInfo = {};
        _uint neighboValidFlag = {};
        vector<TILE_INFO> infos;
        TILE_FLAG markFlag = TILE_FLAG::ONPLAYER;
    };

    struct ItemPacket {
        TOOL_DATA_DESC  CurItem = {};
        TOOL_DATA_DESC  DstItem = {};
        //ITEM_DATA_DESC   DstItem = {};
    };

    struct ControlPacket {
        _bool MsgMove = false; //방향키
        _bool MsgAction = false; //도구 사용키 스페이스
        _bool MsgPickup = false;
        _bool MsgBag = false;
        _bool MsgAdd = false; //시프트 키
        void Reset() {
            MsgMove = MsgAdd = MsgAction = MsgPickup = MsgBag = false;
        }
    };

    struct StateInfoHubPacket {
          CGameObject* m_pObjectOnLeftHand = { nullptr };
          CGameObject* m_pLeftHand = { nullptr };
          CGameObject* m_pRightHand = { nullptr };
          CGameObject* m_pTalker= { nullptr };

    };



private:
    CPlayer();
    CPlayer(const CPlayer& rhs);
    virtual ~CPlayer() override;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Awake() override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;
    virtual void Render_GUI();

private:        
    void Update_Input(_float dt);
    void Update_Movement(_float dt);
    void Update_TileInfo(_float dt);
    void Mark_TileFlag();
    void BroadCast_Event();

public:
    void Adjust_To(_fvector pos);
    void Adjust_To_Foward();
    void Adjust_To_WorldFoward();
    void Camera_Zoom_In(CGameObject* subject = {nullptr});
    void Camera_Zoom_Out(CGameObject* subject = { nullptr });

public:
    MovementPacket& Get_MovementPacket() { return m_MovementPack; }
    TileInfoPacket& Get_TileInfoPacket() { return m_TileInfoPack; }
    ItemPacket& Get_ItemPacket() { return m_ItemPack; }
    ControlPacket& Get_ControlPack() { return m_ControlPack; }
    StateInfoHubPacket& Get_InfoPack() { return m_InfoPack; }

    _bool Can_Walk(_float2& moveAxis);
    TILE_INDEX Get_FowardIndex();

public:
    void OnCollisionEnter(COLLISION_CONTEXT context) override;
    void OnCollisionStay(COLLISION_CONTEXT context) override;
    void OnCollisionExit(COLLISION_CONTEXT context) override;

public:
    void Open_Inventory();
    void Close_Inventory();
    void Change_Item(TOOL_DATA_DESC desc);
    void Set_CurItemData(TOOL_DATA_DESC desc);
    HRESULT Add_ITEM(ITEM_DATA_DESC item);
    HRESULT Set_InvenEvent(ITEM_DATA_DESC item, _int Slot, wstring SelectedEvent);

public:
    void Open_EventMsg(EventMsgDesc* evtMsg);

public:
    void ActiveCollider_Tool(_bool active, string Event = {});
    void ActiveCollider_LeftHand(_bool active, string Event = {});
    void ActiveCollider_RightHand(_bool active, string Event = {});
 public:
    class CLevel* Get_NowLevel();
    
private:
    void Add_AnimationClips();
    void Add_MaterialAnim();
    void Add_PartObjects();
    void Add_Inventory();
    void Set_TargetCamera();
    void Adjust_Cloth_Material(CGameObject* pObject, string TextureKey, string subsetKey);


private:
    class CPlayerStateMachine* m_pStateMachine= { nullptr };
 
    _float m_fRange = {8};

    MovementPacket m_MovementPack = {};
    TileInfoPacket m_TileInfoPack = {};
    ItemPacket m_ItemPack = {};
    ControlPacket m_ControlPack = {};
    StateInfoHubPacket m_InfoPack = {};

    _float4 m_vPrevPos = {  };

    class CTarget_Camera* m_pCamera = { nullptr };
    class CPlayer_Inventory* m_pInventory = { nullptr };

public:
    static CPlayer* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    void Free() override;
};
NS_END


/*눈 4번 , 머리 27번 ,입 3번 */