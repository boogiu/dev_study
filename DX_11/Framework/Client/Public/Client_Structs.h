#pragma once

typedef struct tagToolItemDataDesc{
	TOOL_TYPE eType = { TOOL_TYPE::NONE };
	string TypeTag = {"None"};
	string modelName = {};
	string materialName = {};
}TOOL_DATA_DESC;

typedef struct tagDropItemDataDesc {
	string TypeTag = { "None" };
	string modelName = {};
	string materialName = {};
}ITEM_DATA_DESC;