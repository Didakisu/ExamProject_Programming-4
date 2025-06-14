#include "PlayerComponent.h"
#include "Data.h"
#include "EnemyComponent.h"
#include "FireBallComponent.h"
#include "VersusEnemyComponent.h"

namespace dae 
{
    void PlayerObserver::OnNotify(const GameObject& gameObject, Event event)
    {
        if (event != EVENT_COLLISION)
            return;

        if (gameObject.HasComponent<EnemyComponent>())
        {
            auto enemyComp = gameObject.GetComponent<EnemyComponent>();
            if (enemyComp->IsInBonusState()) 
            {
                enemyComp->Die();
                EventManager::GetInstance().FireEvent(EVENT_ENEMY_KILLED_BY_PLAYER_IN_BONUS_STATE, nullptr);
            }
            else
            {
                m_pPlayer->SetDead(true);
            }
        }
        else if (gameObject.HasComponent<VersusEnemyComponent>())
        {
            gameObject.GetComponent<VersusEnemyComponent>();
            m_pPlayer->SetDead(true);
        }
    }

    PlayerComponent::PlayerComponent(GameObject* owner, Scene& scene, std::shared_ptr<TileMap> tileMap, int& sharedScore, int& sharedLives)
        : Component(owner), m_Scene(scene), m_pTileMap(tileMap) , m_SharedScore(sharedScore) , m_SharedLives(sharedLives)
    {
   
    }

    PlayerComponent::~PlayerComponent()
    {
        if (m_pPlayerObserver)
        {
            m_pCollisionComponent->RemoveObserver(m_pPlayerObserver);
        }

        EventManager::GetInstance().RemoveObserver(m_pScoreComponent);
    }

    void PlayerComponent::Initialize(const glm::vec3& startPosition)
    {
        m_pTransform = GetOwner()->AddComponent<Transform>();
        m_pDirectionComponent = GetOwner()->AddComponent<DirectionComponent>();
        GetOwner()->AddComponent<RenderComponent>("MainCharacter.png", 32, 32);

        m_pTransform->SetLocalPosition(startPosition.x , startPosition.y , startPosition.z);

        m_pAnimationComponent = GetOwner()->AddComponent<AnimationComponent>();
        m_pAnimationComponent->AddAnimation("Run", "MainCharacter.png", 32, 32, 4, 0.15f);
        m_pAnimationComponent->PlayAnimation("Run");

        m_pCollisionComponent = GetOwner()->AddComponent<CollisionComponent>(32.f, 32.f, &m_Scene);

        m_pScoreComponent = GetOwner()->AddComponent<ScoreComponent>(m_SharedScore);
        m_pHealthComponent = GetOwner()->AddComponent<HealthComponent>(m_SharedLives);

        m_pScoreComponent->SetTotalGemsInLevel(dae::LevelLoader::GetTotalGemCount());

        EventManager::GetInstance().AddObserver(m_pScoreComponent, { EVENT_COLLECTED_GEM ,  EVENT_COLLECTED_GOLD , EVENT_ENEMY_KILLED_BY_FIREBALL , EVENT_ENEMY_KILLED_BY_PLAYER_IN_BONUS_STATE });

        m_pPlayerObserver = std::make_shared<PlayerObserver>(this);
        m_pCollisionComponent->AddObserver(m_pPlayerObserver);

        m_RespawnPosition = startPosition;

        //hole behind character
        auto pHoleBehindCharacter = std::make_shared<dae::GameObject>();
        pHoleBehindCharacter->AddComponent<dae::RenderComponent>("tile.png", 35, 28);
        pHoleBehindCharacter->AddComponent<dae::Transform>();
        pHoleBehindCharacter->GetTransform()->SetLocalPosition(0.f, 0.f, -2.f);//fix z
        pHoleBehindCharacter->SetParent(GetOwner() , false);

        m_Scene.Add(pHoleBehindCharacter); 
    }

    void PlayerComponent::BindInput(const InputProfile& kbProfile, const GamepadProfile& gpProfile)
    {
        m_InputProfile = kbProfile;
        m_GamepadProfile = gpProfile;
        m_InputsBound = true;

        auto& input = InputManager::GetInstance();

        input.BindKeyboardCommand(kbProfile.up, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ 0, -1 }, m_pTileMap, m_Scene));
        input.BindKeyboardCommand(kbProfile.down, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ 0, 1 }, m_pTileMap, m_Scene));
        input.BindKeyboardCommand(kbProfile.left, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ -1, 0 }, m_pTileMap, m_Scene));
        input.BindKeyboardCommand(kbProfile.right, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ 1, 0 }, m_pTileMap, m_Scene));
        input.BindKeyboardCommand(kbProfile.fire, InputState::Pressed,
            std::make_unique<FireCommand>(GetOwner(), m_Scene));

        using GPB = Gamepad::GamePadButton;

        input.BindGamepadCommand(gpProfile.controllerId,gpProfile.up, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ 0, -1 }, m_pTileMap, m_Scene));
        input.BindGamepadCommand(gpProfile.controllerId ,gpProfile.down, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ 0, 1 }, m_pTileMap, m_Scene));
        input.BindGamepadCommand(gpProfile.controllerId ,gpProfile.left, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ -1, 0 }, m_pTileMap, m_Scene));
        input.BindGamepadCommand(gpProfile.controllerId ,gpProfile.right, InputState::Pressed,
            std::make_unique<MoveCommand>(GetOwner(), m_Speed, glm::vec2{ 1, 0 }, m_pTileMap, m_Scene));
        input.BindGamepadCommand(gpProfile.controllerId ,gpProfile.fire, InputState::Down,
            std::make_unique<FireCommand>(GetOwner(), m_Scene));
    }

    void PlayerComponent::UnbindInput()
    {
        if (!m_InputsBound)
            return;

        auto& input = InputManager::GetInstance();

        input.UnbindKeyboardCommand(m_InputProfile.up);
        input.UnbindKeyboardCommand(m_InputProfile.down);
        input.UnbindKeyboardCommand(m_InputProfile.left);
        input.UnbindKeyboardCommand(m_InputProfile.right);
        input.UnbindKeyboardCommand(m_InputProfile.fire);

        using GPB = Gamepad::GamePadButton;
        input.UnbindGamepadCommand(m_GamepadProfile.controllerId ,m_GamepadProfile.up);
        input.UnbindGamepadCommand(m_GamepadProfile.controllerId ,m_GamepadProfile.down);
        input.UnbindGamepadCommand(m_GamepadProfile.controllerId ,m_GamepadProfile.left);
        input.UnbindGamepadCommand(m_GamepadProfile.controllerId ,m_GamepadProfile.right);
        input.UnbindGamepadCommand(m_GamepadProfile.controllerId ,m_GamepadProfile.fire);

        m_InputsBound = false;
    }

    void PlayerComponent::Update(float deltaTime)
    {
        if (m_IsDead)
        {
            BouncingEffect(deltaTime);

            if (m_ShouldRespawn)
            {
                m_RespawnTimer += deltaTime;
                if (m_RespawnTimer >= m_RespawnDelay)
                {
                    RespawnPlayer();
                    m_ShouldRespawn = false;
                }
            }
        }

        m_FireCooldown -= deltaTime;
    }

    void PlayerComponent::SetDead(bool isDead)
    {
        if (m_IsDead == false && isDead)
        {
            m_IsDead = true;
            m_ShouldRespawn = true;
            m_RespawnTimer = 0.f;

            if (m_pHealthComponent)
            {
                m_pHealthComponent->LoseLife();
            }

            m_pAnimationComponent->StopAnimation(); 
            m_pAnimationComponent->AddAnimation("Dead", "test_1.png", 32, 32, 1, 0.f, false);
            m_pAnimationComponent->PlayAnimation("Dead");

            m_BounceTime = 0.f; 

            dae::EventManager::GetInstance().FireEvent(EVENT_PLAYER_LOSING_LIFE, GetOwner(), nullptr);
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

    void PlayerComponent::RespawnPlayer()
    {
        m_IsDead = false;
        m_HasPlayedRipAnimation = false;
        m_BounceTime = 0.f;

        if (m_pTransform)
        {
            m_pTransform->SetLocalPosition(m_RespawnPosition.x , m_RespawnPosition.y , m_RespawnPosition.z);
        }

        if (m_pAnimationComponent)
        {
            m_pAnimationComponent->PlayAnimation("Run");
        }
    }
}