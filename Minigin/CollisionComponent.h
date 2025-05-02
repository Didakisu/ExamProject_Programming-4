#pragma once
#include "Component.h"
#include "Subject.h"
#include "Scene.h"
#include <glm.hpp>

namespace dae
{

    constexpr dae::Event EVENT_COLLISION{ENGINE_SPACE + 1}; //user space and engine variables(better way)

    class CollisionComponent : public Component, public Subject
    {
    public:
        CollisionComponent(GameObject* owner, float width, float height , Scene* scene);

        void Update(float /*deltaTime*/) override;

        bool IsOverlapping(const CollisionComponent& other) const;
        glm::vec2 GetPosition() const;
        glm::vec2 GetSize() const;

    private:
        float m_Width;
        float m_Height;
        Scene* m_Scene;
    };
}

