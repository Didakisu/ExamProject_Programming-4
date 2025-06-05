#include "PlayerComponent.h"
#include "Data.h"
#include "EnemyComponent.h"
#include "FireBallComponent.h"

namespace dae 
{
    void PlayerObserver::OnNotify(const GameObject& gameObject, Event event)
    {
        if (event != EVENT_COLLISION)
            return;

        if (gameObject.HasComponent<EnemyComponent>())
        {
            m_pPlayer->SetDead(true);
        }
    }


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
        m_pCollisionComponent->AddObserver(std::make_shared<PlayerObserver>(this));

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

        InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_SPACE, InputState::Pressed,
            std::make_unique<FireCommand>(GetOwner() , m_Scene));
    }

    void PlayerComponent::Update(float deltaTime)
    {
        if (m_IsDead)
        {
            BouncingEffect(deltaTime);
        }

        m_FireCooldown -= deltaTime;
    }

    void PlayerComponent::SetDead(bool isDead)
    {
        if (m_IsDead == false && isDead)
        {
            std::cout << "[DEBUG] Player just died\n";
            m_IsDead = true;

            m_pAnimationComponent->StopAnimation(); 
            m_pAnimationComponent->AddAnimation("Dead", "test_1.png", 32, 32, 1, 0.f, false);
            m_pAnimationComponent->PlayAnimation("Dead");

            m_BounceTime = 0.f; 
        }
    }

    void PlayerComponent::BouncingEffect(float deltaTime)
    {
        if (m_BounceTime == 0.f)
        {
            m_DeadOriginalPos = m_pTransform->GetLocalPosition();//save curr pos
        }

        m_BounceTime += deltaTime;

        float bounceProgress = m_BounceTime / m_TotalBounceDuration;
        if (bounceProgress > 1.f)
        {
            bounceProgress = 1.f;

            if (!m_HasPlayedRipAnimation)
            {
                m_HasPlayedRipAnimation = true;
                m_pAnimationComponent->AddAnimation("Rip", "GraveSprites.png", 75/5, 18, 4, 0.35f, false , false);
                m_pAnimationComponent->PlayAnimation("Rip");
            }
        }

        float offsetY = 4 * m_BounceHeight * bounceProgress * (1 - bounceProgress);

        m_pTransform->SetLocalPosition(m_DeadOriginalPos.x,m_DeadOriginalPos.y - offsetY,m_DeadOriginalPos.z);
    }

    void PlayerComponent::TryFire()
    {
        if (m_IsDead)
            return;

        if (m_FireCooldown > 0.f)
            return;

        glm::vec2 dir{ 0.f, 0.f };
        if (m_pDirectionComponent)
        {
            switch (m_pDirectionComponent->GetDirection())
            {
            case dae::Direction::Left:  dir = { -1.f, 0.f }; break;
            case dae::Direction::Right: dir = { 1.f, 0.f }; break;
            case dae::Direction::Up:    dir = { 0.f, -1.f }; break;
            case dae::Direction::Down:  dir = { 0.f, 1.f }; break;
            default: break;
            }
        }

        if (dir == glm::vec2{ 0.f, 0.f })
        {
            return;
        }

        auto fireball = std::make_shared<dae::GameObject>();
        fireball->AddComponent<FireBallComponent>( m_Scene, dir , m_pTileMap.get());

        glm::vec3 playerPos = m_pTransform->GetWorldPosition();
        const float offset = 20.f;
        glm::vec3 spawnPos = playerPos + glm::vec3{ dir.x * offset, dir.y * offset, 0.f };
        spawnPos.y += offset/2;
        spawnPos.x += offset/2;
        fireball->GetTransform()->SetLocalPosition(spawnPos.x, spawnPos.y, spawnPos.z);

        m_Scene.Add(fireball);
        m_pFireball = fireball.get();

        m_FireCooldown = m_FireCooldownDuration;
    }
}