#include "Enemy.h"
#include "raymath.h"

Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture)
{
    worldPos = pos;
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;
    width = texture.width / maxFrame;
    height = texture.height;
    speed = 3.5f;
}

void Enemy::Tick(float deltaTime)
{
    if (!GetAlive())
    {
        return;
    }

    velocity = Vector2Subtract(target->GetScreenPos(), GetScreenPos());
    if (Vector2Length(velocity) < radius)
    {
        velocity = {};
    }
    BaseCharacter::Tick(deltaTime);

    if (CheckCollisionRecs(GetCollisionRec(), target->GetCollisionRec()))
    {
        target->TakeDamage(damagePerSec * deltaTime);
    }
}

Vector2 Enemy::GetScreenPos()
{
    return Vector2Subtract(worldPos, target->GetWorldPos());
}

void Enemy::SetTarget(Character *characterptr)
{
    target = characterptr;
}