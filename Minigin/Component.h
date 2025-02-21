#pragma once
#include <memory> 

namespace dae
{
    class GameObject;

    class Component
    {
    public:
        explicit Component(GameObject* owner)
            : m_pOwner(owner) {} 

        virtual ~Component() = default;
        virtual void Update() {}
        virtual void FixedUpdate() {};
        virtual void Render() const {}

        GameObject* GetOwner() const { return m_pOwner; }

    protected:
        GameObject* m_pOwner = nullptr;
    };
}





