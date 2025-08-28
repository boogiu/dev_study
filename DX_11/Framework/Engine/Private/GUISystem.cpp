#include "GUISystem.h"
#include "Helper_Func.h"
#include "GameInstance.h"
#include "ILevelService.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);                // Use ImGui::GetCurrentContext()

CGUISystem::CGUISystem()
{
}


HRESULT CGUISystem::Initialize(const ENGINE_DESC& engine, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImFont* fonts = io.Fonts->AddFontFromFileTTF("../../Resources/font/NanumSquareNeo-cBd.ttf", 14.0f, nullptr,
		io.Fonts->GetGlyphRangesKorean()); //
	io.FontDefault = fonts;

	ImGui::StyleColorsDark();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     /*키보드 컨트롤*/
	io.DisplaySize = ImVec2((float)engine.iWinSizeX, (float)engine.iWinSizeY);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.WindowRounding = 4.0f;
	style.FrameRounding = 1.0f;
	style.ScrollbarRounding = 4.0f;
	style.ChildRounding = 4.0f;
	style.GrabRounding = 6.0f;
	style.FramePadding = ImVec2(5.f, 5.f);

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.3f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

	if (!ImGui_ImplWin32_Init(engine.hWnd))
		return E_FAIL;
	if (!ImGui_ImplDX11_Init(pDevice, pContext))
		return E_FAIL;

	return S_OK;
}

void CGUISystem::Update(_float& dt)
{
	_bool itemActive = ImGui::IsAnyItemActive();
	_bool hoverWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow); //마우스가 ㅇ올라가면

	if (!itemActive && !hoverWindow)
		m_bUsingUI = false;
	else
		m_bUsingUI = true;

	ImGuiStyle& style = ImGui::GetStyle();
	float alpha = style.Alpha;

	if (m_bUsingUI)
		alpha += dt;
	else
		alpha -= dt * 0.4f;

	alpha = clamp(alpha, 0.3f, 1.f);
	style.Alpha = alpha;
}

void CGUISystem::GUI_Begin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CGUISystem::Render_GUI()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_Always);
	ImGui::Begin("FPSWindow", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %d", (int)ImGui::GetIO().Framerate);

	ImGui::End();

	Test();
}

void CGUISystem::GUI_End()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool CGUISystem::Set_ProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
}


void CGUISystem::Test()
{
	if (ImGui::Begin("Level Selector")) // UI를 담을 창을 시작합니다.
	{
		ImGui::Separator();
		const auto& levelList = CGameInstance::GetInstance()->Get_LevelMgr()->Get_LevelList();
		GUIWidget::ShowList(levelList, [&](const string& selectedLevel)->void {
			string debugMessage = "Button for '" + selectedLevel + "' was clicked. \n";
			OutputDebugStringA(debugMessage.c_str());
			ImGuiStyle& style = ImGui::GetStyle();
			});

		static _vector vec = XMVectorSet(1, 1, 1, 1);
		vec= GUIWidget::Vector4Float("TestVector", vec);
	}
	ImGui::End(); // 창을 닫습니다.
}

CGUISystem* CGUISystem::Create(const ENGINE_DESC& engine, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGUISystem* instance = new CGUISystem();
	if (FAILED(instance->Initialize(engine, pDevice, pContext))) {
		Safe_Release(instance);
	}
	return instance;
}

void CGUISystem::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
