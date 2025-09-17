#include "AudioSource.h"
#include "SoundData.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "IAudioService.h"
#include "GameObject.h"
#include "Transform.h"
CAudioSource::CAudioSource()
	:m_pAudioDevice(CGameInstance::GetInstance()->Get_AudioDev())
{
	Safe_AddRef(m_pAudioDevice);
}

CAudioSource::CAudioSource(const CAudioSource& rhs)
	:m_pAudioDevice(CGameInstance::GetInstance()->Get_AudioDev())
	, CComponent(rhs), m_Audios(rhs.m_Audios)
{
	for (auto& sound : m_Audios)
		Safe_AddRef(sound.second.pSound);

	Safe_AddRef(m_pAudioDevice);
}

CAudioSource::~CAudioSource()
{
}

HRESULT CAudioSource::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAudioSource::Initialize(COMPONENT_DESC* pArg)
{
	m_pTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pTransform);
	return S_OK;
}

HRESULT CAudioSource::Add_Slot(const string& levelTag, const string& SoundKey, const string& slotKey, bool isLoop)
{
	IResourceService* pService = CGameInstance::GetInstance()->Get_ResourceMgr();
	AUDIO_SLOT audioSlot = {};
	audioSlot.pSound = pService->Load_Sound(levelTag, SoundKey);
	audioSlot.isLoop = isLoop;

	if (audioSlot.pSound == nullptr)
		return E_FAIL;

	auto iter = m_Audios.emplace(slotKey.empty() ? SoundKey : slotKey, audioSlot);
	if (iter.second == false)
	{
		MSG_BOX("There is Same Key Audio : CAudioSource");
		return E_FAIL;
	}
	Safe_AddRef(audioSlot.pSound);
	return S_OK;
}

void CAudioSource::Play(const string& SoundKey)
{
	auto iter = m_Audios.find(SoundKey);

	if (iter == m_Audios.end())
		return;
	AUDIO_SLOT& slot = iter->second;

	AUDIO_PACKET packet;
	packet.ppChannelToUpdate = &(slot.pChanel);
	packet.pSound = (slot.pSound);
	_vector vec = m_pTransform->Get_Pos();

	XMStoreFloat4(
		(&m_vPos),
		(m_pTransform->Get_Pos())
	);
	packet.vPosition = { m_vPos.x, m_vPos.y, m_vPos.z };
	m_pAudioDevice->Play(packet);
}

void CAudioSource::Render_GUI()
{
	ImGui::SeparatorText("Audio Source");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight * (m_Audios.size() + 3)) + (ImGui::GetStyle().WindowPadding.y * 2);

	ImGui::BeginChild("##AudioSourceChild", ImVec2{ 0, childHeight }, true);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
	for (auto& pair : m_Audios)
	{
		if (ImGui::Button(pair.first.c_str())) {
			Play(pair.first);
		}
	}
	ImGui::PopStyleVar();
	ImGui::EndChild();
}

CAudioSource* CAudioSource::Create()
{
	CAudioSource* instance = new CAudioSource();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
		MSG_BOX("AudioSource Comp Failed To Create : CAudioSource");
	}
	return instance;
}

CComponent* CAudioSource::Clone()
{
	return new CAudioSource(*this);
}

void CAudioSource::Free()
{
	__super::Free();

	Safe_Release(m_pAudioDevice);

	for (auto& sound : m_Audios)
		Safe_Release(sound.second.pSound);

	Safe_Release(m_pTransform);
	m_Audios.clear();
}
