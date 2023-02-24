#include "Engine/Application.hpp"
#include "Engine/Level.hpp"

using namespace vi;

class EditorLevel : public Level
{
private:
	void Start() override {}
	void Clean() override {}
	void Update(float dt) override {}
	void Render() override {}
};

int main(int argc, char** argv)
{
	Application::SetLevel(new EditorLevel());
	return Application::Run(argc, argv);
}