#pragma once
#include "State.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace dae {
    class StateMachine {
    public:
        virtual ~StateMachine() = default;

        void Update(float deltaTime); 
        void AddState(const std::string& name, std::unique_ptr<State> state);
        void SetInitialState(const std::string& stateName);

        State* GetCurrentState();
        const State* GetCurrentState() const;

        void SwitchState(const std::string& newState);
        virtual std::string CheckNextState() { return ""; }
    private:
        std::unordered_map<std::string, std::unique_ptr<State>> States;
        std::string CurrentState;
    };
}
