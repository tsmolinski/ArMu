#include "World.h"
#include "Obstacle.h"
#include "Image2D.h"
#include "ResourceManager.h"
#include "GameLogic.h"

#include <cstdlib>


void CWorld::Initialize(const CResourceManager& resourceManager)
{
    m_Background.SetTexture(resourceManager.GetTexture(CGameLogic::BACKGROUND));

    m_Ceil.SetPosition(0.0f, -100.0f);
    m_Ceil.SetSize(100.0f, 100.0f);

    for (auto& floor : m_Floor)
        floor.SetTexture(resourceManager.GetTexture(CGameLogic::FLOOR));

    for (auto& obstacle : m_Obstacles)
        obstacle.Initialize(resourceManager);

    int floorId = 0;
    for (auto& floor : m_Floor)
    {
        floor.SetPosition(floorId * floor.GetWidth(), FLOOR_Y);
        floorId++;
    }

    int obstacleId = 0;
    for (auto& obstacle : m_Obstacles)
    {
        obstacle.SetPosition(OBSTACLE_START_OFFSET_X + obstacleId * OBSTACLES_DISTANCE, GetObstacleRandomYPosition());
        obstacleId++;
    }
}


void CWorld::ResetWorld()
{
    int floorId = 0;
    for (auto& floor : m_Floor)
    {
        floor.SetPosition(floorId * floor.GetWidth(), FLOOR_Y);
        floorId++;
    }

    int obstacleId = 0;
    for (auto& obstacle : m_Obstacles)
    {
        obstacle.SetPosition(OBSTACLE_START_OFFSET_X + obstacleId * OBSTACLES_DISTANCE, GetObstacleRandomYPosition());
        obstacleId++;
    }

    m_FirstObstacleIndex = 0;
}


bool CWorld::CheckPlayerCollision(const CObject2D& player) const
{
    auto result = false;
    const auto& obstacle = GetFirstObstacle();

    const auto& pipes = obstacle.GetPipes();
    for (const auto& pipe : pipes)
        result |= pipe.CheckCollisionWith(player);

    result |= m_Ceil.CheckCollisionWith(player);

    for (const auto& floor : m_Floor)
        result |= floor.CheckCollisionWith(player);

    return result;
}


const CObstacle& CWorld::GetFirstObstacle() const
{
    return m_Obstacles[m_FirstObstacleIndex];
}

int CWorld::GetFirstObstacleIndex() const
{
    return m_FirstObstacleIndex;
}

const CObstacle& CWorld::GetObstacleAtIndex(int index) const
{
    return m_Obstacles[index];
}

int CWorld::GetObstaclesCount() const
{
    return OBSTACLES_COUNT;
}


float CWorld::GetObstacleRandomYPosition() const
{
    return static_cast<float>(rand() % static_cast<int>(OBSTACLE_MAX_Y - OBSTACLE_MIN_Y)) + OBSTACLE_MIN_Y;
}


void CWorld::Update(float deltaTime)
{
    for (auto& floor : m_Floor)
    {
        floor.SetPosition(floor.GetX() - m_WorldMoveSpeed * deltaTime, FLOOR_Y);
        if (floor.GetX() < -floor.GetWidth())
            floor.SetPosition(floor.GetX() + floor.GetWidth() * FLOOR_COUNT, FLOOR_Y);
    }

    for (auto& obstacle : m_Obstacles)
    {
        obstacle.SetPosition(obstacle.GetX() - m_WorldMoveSpeed * deltaTime, obstacle.GetY());
    }
}


void CWorld::Render(SDL_Renderer* renderer)
{
    m_Background.Render(renderer);

    for (auto& obstacle : m_Obstacles)
        obstacle.Render(renderer);

    for (auto& floor : m_Floor)
        floor.Render(renderer);
}
