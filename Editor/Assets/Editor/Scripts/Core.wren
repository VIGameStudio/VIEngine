class EditorWindow {
    construct new() {
        System.print("EditorWindow::new")
    }

    Update(dt) {
        System.print("EditorWindow::Update: %(dt)")
    }

	// TODO: make this foreign (impl in c++)
	PrintTest() {
		System.print("PrintTest")
	}
}

class ImGui {
	foreign static Begin(name)
    foreign static End()

	foreign static Button(label, w, h)

    foreign static DragFloat(label, value)

	foreign static SliderFloat(label, value, min, max)

	foreign static BeginMenuBar()
	foreign static EndMenuBar()
	foreign static BeginMainMenuBar()
	foreign static EndMainMenuBar()
	foreign static BeginMenu(label)
	foreign static EndMenu()
	foreign static MenuItem(label)

    //foreign static SetWindowSize(size)
}