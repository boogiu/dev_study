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
    const string& Get_NextLevel() { return m_NextLevelTag; } //로딩 이후 레벨은 무엇인지
    void Notify_LoadComplete(); //로딩이 다되었다면 호출
#pragma endregion

private:
    string m_LoadingLevelKey = { }; // 로딩 레벨이 있다면 그 로딩 레벨의 태그(키)

    class CLevel* m_pCurrentLevel = { nullptr }; //현재 레벨 포인터
    LEVEL_STATE m_eState = {}; //현재 레벨 전환 과정 상태
    string m_NextLevelTag = { }; //전환될 레벨 태그(키)
    
    unordered_map<string, LEVEL_CREATOR> m_LevelCreators;/*레벨 생성자*/

public:
    static CLevelMgr* Create();
    virtual void Free() override;
};

NS_END