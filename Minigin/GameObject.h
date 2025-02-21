#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "Component.h"
#include "Transform.h"
#include <glm.hpp>



namespace dae
{
    class GameObject final
    {
    public:
        GameObject() = default;
        ~GameObject();
        
        void Update();
        void FixedUpdate();
        void Render() const;

        Transform* GetTransform();
        glm::vec3 GetPosition() const;
        void SetPosition(float x, float y);

        void SetTexture(const std::string& filename);
        /*void SetParent(GameObject* parent, bool keepWorldPosition);
        void AddChild(GameObject* child);
        void RemoveChild(GameObject* child);

        const glm::vec3& SetLocalPosition(const glm::vec3& pos);
        const void GetWorldPosition();
        void UpdateWorldPosition();*/

        void SetParent(GameObject* parent, bool keepWorldPosition);
        void RemoveChild(GameObject* child);
        bool IsChild(GameObject* parent) const;



        const glm::vec3& GetWorldPosition();
        void UpdateWorldPosition();
        void SetLocalPosition(const glm::vec3& pos);
        void SetPositionDirty();




        template <typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            T* component = new T(this, std::forward<Args>(args)...);
            m_components.push_back(component);
            return component;
        }

        template <typename T>
        T* GetComponent() const
        {
            for (auto& component : m_components)
            {
                if (auto found = dynamic_cast<T*>(component))
                {
                    return found;
                }
            }
            return nullptr;
        }

        template <typename T>
        void RemoveComponent()
        {
            auto it = std::remove_if(m_components.begin(), m_components.end(),
                [](Component* component) {
                    return dynamic_cast<T*>(component) != nullptr;
                });

            for (auto iter = it; iter != m_components.end(); ++iter)
            {
                delete* iter;  
            }

            m_components.erase(it, m_components.end());  
        }

        template <typename T>
        bool HasComponent() const
        {
            return std::any_of(m_components.begin(), m_components.end(),
                [](Component* component) {
                    return dynamic_cast<T*>(component) != nullptr;
                });
        }

    private:
        std::vector<Component*> m_components;

        std::vector<GameObject*> m_children;
        GameObject* m_parent;

        glm::vec3 m_worldPos;
        glm::vec3 m_localPos;
        bool m_isPosDirty;
    };

}




