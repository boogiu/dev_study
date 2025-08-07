#pragma once
#include "ILevelService.h"

NS_BEGIN(Engine)

class CLevelMgr :
    public ILevelService
{
private:
    CLevelMgr();
    virtual ~CLevelMgr();

public:
    HRESULT Request_ChangeLevel(string key);
    void Update(_float dt);
    HRESULT Render();

public :
    void Register_Level(string key, LEVEL_CREATOR creator); //레벨들의 생성을 매니저에게

#pragma region For_LoadingLevel
    void Set_LoadingLevel(const string& LoadingKey); //로딩 역할을 하는 레벨이 있는지.
    const string& Get_NextLevel() { return m_NextLevel; } //로딩 이후 레벨은 무엇인지
    void Notify_LoadComplete(); //로딩이 다되었다면 호출
#pragma endregion

private:
    
    string m_LoadingLevelKey = { };
    string m_NextLevel = { };

    LEVEL_STATE m_eState = {};
    class CLevel* m_pCurrentLevel = { nullptr };
    unordered_map<string, LEVEL_CREATOR> m_LevelCreators;

public:
    static CLevelMgr* Create();
    virtual void Free() override;
};

NS_END