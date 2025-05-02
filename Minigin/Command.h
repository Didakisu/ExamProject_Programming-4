#pragma once
#include "GameObject.h"
#include <vec2.hpp>
//#include "HealthComponent.h"
//#include "ScoreComponent.h"
#include <memory>

namespace dae
{
    class Command {
    public:
        virtual ~Command() = default;
        virtual void Execute(float deltaTime = 0.f) = 0;
    };
}