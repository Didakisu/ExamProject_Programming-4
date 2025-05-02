#pragma once
#include "GameObject.h"

namespace dae
{
	typedef int Event;
	constexpr int ENGINE_SPACE = 0;
	constexpr int USER_SPACE = 10000;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const GameObject& gameObject, Event event) = 0;
	};
}