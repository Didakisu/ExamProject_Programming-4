#pragma once
#include "Component.h"
#include "imgui.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <future>

namespace dae
{
    struct TransformExercise
    {
        float matrix[16] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    };

    class GameObject3D
    {
    public:
        TransformExercise transform;
        int ID;
    };

    class GameObject3DAlt
    {
    public:
        int ID;
    };

    struct GameObject3DAltContainer
    {
        std::vector<int> IDs;
        std::vector<TransformExercise> transforms;
    };

    class ThrashTheCacheComponent : public Component
    {
    public:
        explicit ThrashTheCacheComponent(GameObject* owner);
        ~ThrashTheCacheComponent() override = default;

        void Render() const override;
        void Update() override {};
        void TrashTheCache() const;
        void TrashTheCacheWithGameObject3D() const;
        void TrashTheCacheWithGameObject3DAlt() const;

    private:
        mutable int m_numberSamplesOne;
        mutable int m_numberSamplesTwo;

        mutable std::vector<float> m_stepSizes;

        mutable std::vector<float> m_timingsExercise1; 
        mutable std::vector<float> m_timingsExercise2; 
        mutable std::vector<float> m_timingsExercise2Alt;

        mutable std::future<void> m_futureExercise1;
        mutable std::future<void> m_futureExercise2;
        mutable bool m_isRunningExercise1 = false;
        mutable bool m_isRunningExercise2 = false;
    };
}