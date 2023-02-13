#pragma once

#include "Engine/Utils.hpp"

namespace Gaun
{
	class Level
	{
	public:
		

	private:
		friend class Application;
		virtual void Start() = 0;
		virtual void Clean() = 0;
		virtual void Update(f32 dt) = 0;
		virtual void Render() = 0;
	};
}