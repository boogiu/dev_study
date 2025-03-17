#include "CBroker.h"
#include "pch.h"

CBroker::CBroker()
	: player(nullptr)
{}

CBroker::~CBroker()
{
}

void CBroker::SetPlayer(CPlayer* _player)
{
	if (player != nullptr) return;
	player = _player;
}

void CBroker::DealPC(int _class)
{
	if (player == nullptr) return;

	player->SetClass(_class);
}

char *  CBroker::DealPN()
{
	if (player->GetName() == nullptr) {
		player->SetName();
		return nullptr;
	}
	return player->GetName();
}

void CBroker::DealPG()
{
	cout << "===============" << endl;
	cout << "¼ÒÁö±Ý : " << player->GetGold() << endl;
	cout << "===============" << endl;

}
