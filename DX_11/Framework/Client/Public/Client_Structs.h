#pragma once

typedef struct tagItemDataDesc{
	ITEM_TYPE eType = { ITEM_TYPE::NONE };
	string TypeTag = {"None"};
	string modelName = {};
	string materialName = {};
}ITEM_DATA_DESC;