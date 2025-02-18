#pragma once
#include <vector>
#include <memory>
#include <string>
#include <algorithm> 
#include "Component.h"
#include "Transform.h"


namespace dae
{
    class GameObject final : public std::enable_shared_from_this<GameObject>
    {
    public:
        GameObject() = default;
        ~GameObject() = default;

        void Update();
        void FixedUpdate();
        void Render() const;

        Transform* GetTransform();
        glm::vec3 GetPosition() const;
        void SetPosition(float x, float y);

        void SetTexture(const std::string& filename);

        template <typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args);

        template <typename T>
        T* GetComponent() const;

        template <typename T>
        void RemoveComponent();

        template <typename T>
        bool HasComponent() const;     

    private:
        std::vector<std::shared_ptr<Component>> m_components;
    };


    template <typename T, typename... Args>
    std::shared_ptr<T> GameObject::AddComponent(Args&&... args)
    {
        auto component = std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...);
        m_components.push_back(component);
        return component;
    }


    template <typename T>
    T* GameObject::GetComponent() const
    {
        for (const auto& component : m_components)
        {
            if (auto found = std::dynamic_pointer_cast<T>(component))
            {
                return found.get();
            }
        }
        return nullptr;
    }

    template <typename T>
    void GameObject::RemoveComponent()
    {
        auto it = std::remove_if(m_components.begin(), m_components.end(),
            [](const std::shared_ptr<Component>& component) {
                return std::dynamic_pointer_cast<T>(component) != nullptr;
            });
        if (it != m_components.end()) {
            m_components.erase(it, m_components.end());
        }
    }

    template <typename T>
    bool GameObject::HasComponent() const
    {
        return std::any_of(m_components.begin(), m_components.end(),
            [](const std::shared_ptr<Component>& component) {
                return std::dynamic_pointer_cast<T>(component) != nullptr;
            });
    }

}
