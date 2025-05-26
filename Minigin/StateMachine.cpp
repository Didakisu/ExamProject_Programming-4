#include "StateMachine.h"
#include <cassert>
#include <stdexcept>
#include <iostream>

namespace dae {

    void StateMachine::Update(float deltaTime) 
    {
        assert(!CurrentState.empty() && "CurrentState not set!");

        auto it = States.find(CurrentState);
        if (it != States.end())
            it->second->Update(deltaTime);

        const std::string nextState = CheckNextState();
        if (!nextState.empty() && nextState != CurrentState) {
            SwitchState(nextState);
        }
    }

    void StateMachine::AddState(const std::string& name, std::unique_ptr<State> state) 
    {
        States.emplace(name, std::move(state));
    }

    void StateMachine::SetInitialState(const std::string& stateName) 
    {
        assert(States.find(stateName) != States.end() && "Initial state must exist");
        CurrentState = stateName;
        States[CurrentState]->OnEnter();
    }

    void StateMachine::SwitchState(const std::string& newState) 
    {
        if (States.find(CurrentState) != States.end() && States[CurrentState] != nullptr)
            States[CurrentState]->OnExit();

        CurrentState = newState;

        if (States.find(CurrentState) != States.end() && States[CurrentState] != nullptr)
            States[CurrentState]->OnEnter();
        else
            throw std::runtime_error("Trying to switch to a state that doesn't exist: " + CurrentState);
    }

    State* dae::StateMachine::GetCurrentState() 
    {
        auto it = States.find(CurrentState);
        if (it != States.end())
            return it->second.get();
        return nullptr;
    }

    const State* dae::StateMachine::GetCurrentState() const 
    {
        auto it = States.find(CurrentState);
        if (it != States.end())
            return it->second.get();
        return nullptr;
    }

}