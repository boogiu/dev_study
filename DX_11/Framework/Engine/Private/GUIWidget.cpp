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

void GUIWidget::ShowList(const vector<string>& vector, function<void(const string&)> callback)
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

ENGINE_DLL _vector GUIWidget::Vector4Float(const string& Name, _fvector vector, bool Editable)
{
	ImGui::Text(Name.c_str()); ImGui::SameLine();
	ImGui::DragFloat4(("##" + Name).c_str(), (float*)&vector, 0.1f);
	return vector;
}
