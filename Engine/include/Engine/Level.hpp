#pragma once

namespace vi
{
	class Level
	{
	private:
		friend class Application;

		virtual void Start() = 0;
		virtual void Clean() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;
	};
}