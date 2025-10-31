#pragma once
#include "GameObject.h"
NS_BEGIN(MapEditor)
class CTileObject :
    public CGameObject
{

public:

    struct AutoTileEditor {
        string name;
        NEIGHBOR_INDEX Connectable = {};
        NEIGHBOR_INDEX NeverConnectable = { NEIGHBOR_INDEX::UP | 
            NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::RIGHT |
            NEIGHBOR_INDEX::UPLEFT | NEIGHBOR_INDEX::DOWNLEFT | NEIGHBOR_INDEX::DOWNRIGHT | NEIGHBOR_INDEX::UPRIGHT };

        _bool Partitial = { true };

        AutoTileEditor() = default; 

        AutoTileEditor(string n, NEIGHBOR_INDEX c, NEIGHBOR_INDEX nc, bool p = false)
            : name(n), Connectable(c), NeverConnectable(nc), Partitial(p)
        { }

        _float Check_Connect(NEIGHBOR_INDEX state)
        {
            //현재 이웃들이 들어옴
            //데이터가 자신이 연결 가능한 방향을 알고 있음.

            NEIGHBOR_INDEX rotated = Connectable;
            NEIGHBOR_INDEX rotatedNever = NeverConnectable;

            for (int i = 1; i < 5; ++i)
            {
                bool notFit = { false };
                if ((static_cast<_uint>(state) & static_cast<_uint>(rotatedNever)) != 0)
                    notFit = true;

                if(!notFit){
                if (Partitial) {
                    if ((static_cast<_uint>(state) & static_cast<_uint>(rotated)) == static_cast<_uint>(rotated))
                        return(i - 1) * 90.f;
                }
                else {
                    if (static_cast<_uint>(rotated) == static_cast<_uint>(state))
                        return  (i - 1) * 90.f;
                }
                }
                //rotated = Rotate90(rotated); // 누적 회전
                //rotatedNever = Rotate90(rotatedNever);
                rotated = Rotate90(Connectable,i); // 누적 회전
                rotatedNever = Rotate90(NeverConnectable, i);
            }

            return 999.f; // 일치 없음
        }

        static NEIGHBOR_INDEX Rotate90(NEIGHBOR_INDEX mask, int count)
        {
            NEIGHBOR_INDEX rotated = mask;
            for (int i = 0; i < count; ++i)
                rotated = Rotate90(rotated);
            return rotated;
        }

        static NEIGHBOR_INDEX Rotate90(NEIGHBOR_INDEX mask)
        {
            _uint newMask = 0;
            _uint m = static_cast<_uint>(mask);

            // 기본 4방향 회전
            if (m & static_cast<_uint>(NEIGHBOR_INDEX::UP))
                newMask |= static_cast<_uint>(NEIGHBOR_INDEX::RIGHT);
            if (m & static_cast<_uint>(NEIGHBOR_INDEX::RIGHT))
                newMask |= static_cast<_uint>(NEIGHBOR_INDEX::DOWN);
            if (m & static_cast<_uint>(NEIGHBOR_INDEX::DOWN))
                newMask |= static_cast<_uint>(NEIGHBOR_INDEX::LEFT);
            if (m & static_cast<_uint>(NEIGHBOR_INDEX::LEFT))
                newMask |= static_cast<_uint>(NEIGHBOR_INDEX::UP);

            // 대각선 회전
            if (m & static_cast<_uint>(NEIGHBOR_INDEX::UPLEFT))
                newMask |= static_cast<_uint>(NEIGHBOR_INDEX::UPRIGHT);
            if (m & static_cast<_uint>(NEIGHBOR_INDEX::UPRIGHT))
                newMask |= static_cast<_uint>(NEIGHBOR_INDEX::DOWNRIGHT);
            if (m & static_cast<_uint>(NEIGHBOR_INDEX::DOWNRIGHT))
                newMask |= static_cast<_uint>(NEIGHBOR_INDEX::DOWNLEFT);
            if (m & static_cast<_uint>(NEIGHBOR_INDEX::DOWNLEFT))
                newMask |= static_cast<_uint>(NEIGHBOR_INDEX::UPLEFT);

            return static_cast<NEIGHBOR_INDEX>(newMask);
        }
    };

private:
    CTileObject();
    CTileObject(const CTileObject& rhs);
    virtual ~CTileObject() DEFAULT;

public:
    typedef struct TILE_TYPE_DESC : public GAMEOBJECT_DESC {
        string TypeName = {};
        TILE_INDEX index = {};
    };

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(INIT_DESC* pArg) override;
    void Priority_Update(_float dt) override;
    void Update(_float dt) override;
    void Late_Update(_float dt) override;

public:
    HRESULT Link_Data(const string& folderName,_bool Base=false);

public:
    HRESULT Save_MapData(ofstream& ofs, _bool Base = false);
    static HRESULT Save_RuleFile();

public:
    void Render_GUI() override;

private:
    void Update_State(_uint N_State);
public:
    static HRESULT PrepareForTile(const string& baseFolderPath);
    HRESULT Save_TileMap();

private:
    string ModelName = {};
    string MaterialName = {};
    string m_BaseTypeName = {};
    _float m_fRotation = {};
    _uint m_CurState = {};
    _bool Is_Base = {false};

public:
    static CTileObject* Create();
    CGameObject* Clone(INIT_DESC* pArg) override;
    virtual void Free();


    static unordered_map<string, vector<string>>TileMapTable;
};

NS_END

