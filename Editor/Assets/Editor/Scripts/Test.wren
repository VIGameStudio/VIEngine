import "Core" for EditorWindow, ImGui

class TestWindow is EditorWindow {
	construct new() {
		_value = 5
		_min = 0
		_max = 5

		this.PrintTest()
	}

	Update(dt) {
		// Main menu bar
		ImGui.BeginMainMenuBar()
		
		if (ImGui.BeginMenu("File")) {

			if (ImGui.MenuItem("New")) {
				System.print("File::New")
			}

			if (ImGui.MenuItem("Open")) {
				System.print("File::Open")
			}

			if (ImGui.MenuItem("Save")) {
				System.print("File::Save")
			}

			ImGui.EndMenu()
		}
		
		if (ImGui.BeginMenu("Edit")) {

			if (ImGui.MenuItem("Cut")) {
				System.print("Edit::Cut")
			}

			if (ImGui.MenuItem("Copy")) {
				System.print("Edit::Copy")
			}

			if (ImGui.MenuItem("Paste")) {
				System.print("Edit::Paste")
			}

			ImGui.EndMenu()
		}

		if (ImGui.BeginMenu("Help")) {

			if (ImGui.MenuItem("About")) {
				System.print("Help::About")
			}

			ImGui.EndMenu()
		}

		ImGui.EndMainMenuBar()

		// Tool window
		ImGui.Begin("TestWindow")

		if (ImGui.Button("Button", 0, 0)) {
			System.print("Button clicked!")
		}

		_min = ImGui.DragFloat("Min", _min)
		_max = ImGui.DragFloat("Max", _max)
		_value = ImGui.SliderFloat("Value", _value, _min, _max)

		ImGui.End()
	}
}