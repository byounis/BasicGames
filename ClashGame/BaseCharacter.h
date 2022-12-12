#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H
#include "raylib.h"

class BaseCharacter
{
public:
    BaseCharacter();
    Vector2 GetWorldPos() { return worldPos; }
    void UndoMovement();
    Rectangle GetCollisionRec();
    virtual void Tick(float deltaTime);
    virtual Vector2 GetScreenPos() = 0;
    bool GetAlive() { return alive; }
    void SetAlive(bool isAlive) { alive = isAlive; }

protected:
    Texture2D texture{};
    Texture2D idle{};
    Texture2D run{};
    Vector2 worldPos{};
    Vector2 worldPosLastFrame{};
    float rightLeft{1.f};
    int frame{};
    int maxFrame{6};
    float updateTime{1.f / 12.f};
    float runningTime{};
    float speed{7.f};
    float width{};
    float height{};
    float scale{4.f};
    Vector2 velocity{};

private:
    bool alive{true};
};
#endif