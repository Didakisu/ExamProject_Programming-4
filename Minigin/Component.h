#pragma once
#include <memory> 

namespace dae
{
    class GameObject;

    class Component
    {
    public:
        //Component() = default; 
        Component(std::shared_ptr<GameObject> owner)
            : m_pOwner(owner) {}

        virtual ~Component() = default;
        virtual void Update() {}
        virtual void FixedUpdate() {};
        virtual void Render() const {}

        std::shared_ptr<GameObject> GetOwner() const { return m_pOwner; }

    protected:
        std::shared_ptr<GameObject> m_pOwner = nullptr; 
    };
}








