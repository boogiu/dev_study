#include "GUIWidget.h"

void GUIWidget::ShowListButton(const vector<string>& vector,function<void(const string&)> callback)
{
	for (const string& btnName : vector)
	{
		if (ImGui::Button(btnName.c_str()))
		{
			callback(btnName);
		}
	}
}

void GUIWidget::ShowListString(const vector<string>& vector, function<void(const string&)> callback)
{
	for (auto it = vector.begin(); it != vector.end(); ++it)
	{
		string ListID = "##" + *it;
		ImGui::PushID(ListID.c_str());
		if (ImGui::Selectable(it->c_str()))
		{
			callback(*it);
		}
		ImGui::PopID();
	}
}

void GUIWidget::ShowListInt(const vector<string>& vector, function<void(_uint)> callback)
{
	for (size_t i = 0; i < vector.size(); ++i) {
		string ListID = "##" + i;
		ImGui::PushID(ListID.c_str());
		if (ImGui::Selectable(vector[i].c_str()))
		{
			callback(i);
		}
		ImGui::PopID();
	}
}

 _vector GUIWidget::Vector4Float(const string& Name, _fvector vector, bool Editable)
{
	ImGui::Text(Name.c_str()); ImGui::SameLine();
	ImGui::DragFloat4(("##" + Name).c_str(), (float*)&vector, 0.1f);
	return vector;
}

 void GUIWidget::ShowCombo(const vector<string>& vector, function<void(_uint)> callback)
 {
	 if (ImGui::BeginCombo("##Layer", currentPreview)) {
		 for (int i = 0; i < layerList.size(); ++i) {
			 bool isSelected = (i == currentLayerIndex);

			 if (ImGui::Selectable(layerList[i].c_str(), isSelected)) {
				 currentLayerIndex = i;
				 CSceneMgr::GetInstance()->Get_NowScene()->SwapLayer(this, LayerName, layerList[i]);
				 LayerName = layerList[i]; // ¹Ý¿µ
			 }

			 if (isSelected)
				 ImGui::SetItemDefaultFocus();
		 }
		 ImGui::EndCombo();
	 }
 }
