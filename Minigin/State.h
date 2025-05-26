#pragma once

namespace dae 
{
    class State 
    {
    public:
        virtual ~State() = default;
        virtual void OnEnter() {};
        virtual void OnExit() {}
        virtual void Update(float /*deltaTime*/) {};
    };
}