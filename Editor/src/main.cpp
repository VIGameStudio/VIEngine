#include "Engine/Application.hpp"
#include "Engine/Audio.hpp"

using namespace Gaun;

class EditorLevel : public Level
{
private:
	void Start() override {}
	void Clean() override {}
	void Update(f32 dt) override {}
	void Render() override {}
};

int main(int argc, char** argv)
{
	Gaun::Application::SetLevel(new EditorLevel());
	return Gaun::Application::Run(argc, argv);
}