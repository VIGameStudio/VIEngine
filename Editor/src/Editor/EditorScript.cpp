#include "Editor/EditorScript.hpp"

#include "Editor/ImGui.hpp"

#include "Engine/Resource.hpp"
#include "Engine/Script.hpp"

namespace vi
{
	struct EditorWindow
	{
		ScriptHandle classHandle = nullptr;
		ScriptHandle constructorHandle = nullptr;
		ScriptHandle updateHandle = nullptr;
	};
	static EditorWindow editorWindow;

	struct Test
	{
		ScriptHandle classHandle = nullptr;
		ScriptHandle constructorHandle = nullptr;
		ScriptHandle updateHandle = nullptr;

		ScriptHandle instanceHandle = nullptr;
	};
	static Test test;

	static bool InitializeEditorWindow()
	{
		ScriptManager::BindFunction<decltype(&ImGui::Begin), &ImGui::Begin>(true, "Core::ImGui::Begin(_)_s");
		ScriptManager::BindFunction<decltype(&ImGui::End), &ImGui::End>(true, "Core::ImGui::End()_s");

		ScriptManager::BindFunction<decltype(&ImGui::Button), &ImGui::Button>(true, "Core::ImGui::Button(_,_,_)_s");
		
		ScriptManager::BindFunction<decltype(&ImGui::DragFloat), &ImGui::DragFloat>(true, "Core::ImGui::DragFloat(_,_)_s");
		
		ScriptManager::BindFunction<decltype(&ImGui::SliderFloat), &ImGui::SliderFloat>(true, "Core::ImGui::SliderFloat(_,_,_,_)_s");
		
		ScriptManager::BindFunction<decltype(&ImGui::BeginMenuBar), &ImGui::BeginMenuBar>(true, "Core::ImGui::BeginMenuBar()_s");
		ScriptManager::BindFunction<decltype(&ImGui::EndMenuBar), &ImGui::EndMenuBar>(true, "Core::ImGui::EndMenuBar()_s");
		ScriptManager::BindFunction<decltype(&ImGui::BeginMainMenuBar), &ImGui::BeginMainMenuBar>(true, "Core::ImGui::BeginMainMenuBar()_s");
		ScriptManager::BindFunction<decltype(&ImGui::EndMainMenuBar), &ImGui::EndMainMenuBar>(true, "Core::ImGui::EndMainMenuBar()_s");
		ScriptManager::BindFunction<decltype(&ImGui::BeginMenu), &ImGui::BeginMenu>(true, "Core::ImGui::BeginMenu(_)_s");
		ScriptManager::BindFunction<decltype(&ImGui::EndMenu), &ImGui::EndMenu>(true, "Core::ImGui::EndMenu()_s");
		ScriptManager::BindFunction<decltype(&ImGui::MenuItem), &ImGui::MenuItem>(true, "Core::ImGui::MenuItem(_)_s");

		Resource<Script> res = Resource<Script>("/Editor/Scripts/Core.wren");
		if (!res.Load()) return false;
		ScriptManager::Interpret("Core", res.GetData().source.c_str());

		// Load the class into slot 0.
		ScriptManager::EnsureSlots(1);
		ScriptManager::GetVariable("Core", "EditorWindow", 0);

		ScriptHandle classHandle = ScriptManager::GetSlotHandle(0);
		ScriptManager::SetSlotHandle(0, classHandle);

		ScriptHandle constructorHandle = ScriptManager::MakeCallHandle("new()");
		ScriptHandle methodHandle = ScriptManager::MakeCallHandle("Update(_)");

		editorWindow.classHandle = classHandle;
		editorWindow.constructorHandle = constructorHandle;
		editorWindow.updateHandle = methodHandle;

		return true;
	}

	static bool InitializeTest()
	{
		//ScriptManager::BindFunction<decltype(&ImGui::Begin), &ImGui::Begin>(true, "Core::ImGui::Begin(_)_s");
		//ScriptManager::BindFunction<decltype(&ImGui::End), &ImGui::End>(true, "Core::ImGui::End()_s");

		Resource<Script> res = Resource<Script>("/Editor/Scripts/Test.wren");
		if (!res.Load()) return false;

		auto& data = res.GetData();
		ScriptManager::Interpret("Test", data.source.c_str());

		// Load the class into slot 0.
		ScriptManager::EnsureSlots(1);
		ScriptManager::GetVariable("Test", "TestWindow", 0);

		ScriptHandle classHandle = ScriptManager::GetSlotHandle(0);
		ScriptManager::SetSlotHandle(0, classHandle);

		ScriptHandle constructorHandle = ScriptManager::MakeCallHandle("new()");
		ScriptManager::Call(constructorHandle);

		ScriptHandle instanceHandle = ScriptManager::GetSlotHandle(0);
		ScriptManager::SetSlotHandle(0, instanceHandle);

		ScriptHandle methodHandle = ScriptManager::MakeCallHandle("Update(_)");

		test.classHandle = classHandle;
		test.constructorHandle = constructorHandle;
		test.updateHandle = methodHandle;
		test.instanceHandle = instanceHandle;
	}

	bool EditorScript::Initialize()
	{
		try
		{
			InitializeEditorWindow();
			InitializeTest();
		}
		catch (...) {}

		return true;
	}

	void EditorScript::Shutdown()
	{
	}

	void EditorScript::Update(float dt)
	{
		try
		{
			ScriptManager::SetSlotHandle(0, test.instanceHandle);
			ScriptManager::SetSlotDouble(1, dt);
			ScriptManager::Call(test.updateHandle);
		}
		catch (...) {}
	}
}