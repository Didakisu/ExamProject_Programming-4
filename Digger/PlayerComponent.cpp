#include "PlayerComponent.h"
#include "Data.h"

namespace dae 
{

    PlayerComponent::PlayerComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap)
        : Component(owner), m_Scene(scene), m_pTileMap(tileMap)
    {
    }

    void PlayerComponent::Initialize(const glm::vec3& startPosition)
    {
        m_pDirectionComponent = GetOwner()->AddComponent<DirectionComponent>();
        GetOwner()->AddComponent<RenderComponent>("MainCharacter.png", 32, 32);

        m_pTransform = GetOwner()->AddComponent<Transform>();
        m_pTransform->SetLocalPosition(startPosition.x , startPosition.y , startPosition.z);

        m_pAnimationComponent = GetOwner()->AddComponent<AnimationComponent>();
        m_pAnimationComponent->AddAnimation("Run", "MainCharacter.png", 32, 32, 4, 0.15f);
        m_pAnimationComponent->PlayAnimation("Run");

        m_pCollisionComponent = GetOwner()->AddComponent<CollisionComponent>(32.f, 32.f, &m_Scene);

        m_pScoreComponent = GetOwner()->AddComponent<ScoreComponent>(0);

        EventManager::GetInstance().AddObserver(m_pScoreComponent, { EVENT_PLAYER_COLLECT_ITEM });

        //hole behind character
        auto pHoleBehindCharacter = std::make_shared<dae::GameObject>();
        pHoleBehindCharacter->AddComponent<dae::RenderComponent>("tile.png", 35, 28);
        pHoleBehindCharacter->AddComponent<dae::Transform>();
        pHoleBehindCharacter->GetTransform()->SetLocalPosition(0.f, 0.f, -2.f);//fix z
        pHoleBehindCharacter->SetParent(GetOwner() , false);

        m_Scene.Add(pHoleBehindCharacter); 
    }

    void PlayerComponent::BindInput()
    {
        InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), 45.0f, glm::vec2{ 0, -1 }, m_pTileMap, m_Scene));

        InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), 45.0f, glm::vec2{ 0, 1 }, m_pTileMap, m_Scene));

        InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), 45.0f, glm::vec2{ -1, 0 }, m_pTileMap, m_Scene));

        InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), 45.0f, glm::vec2{ 1, 0 }, m_pTileMap, m_Scene));

        InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_X, InputState::Down,
            std::make_unique<KillPlayerCommand>(GetOwner()));
    }

    void PlayerComponent::Update(float /*deltaTime*/)
    {

    }

}