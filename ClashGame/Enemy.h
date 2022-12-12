#include "raylib.h"
#include "BaseCharacter.h"
#include "Character.h"

class Enemy : public BaseCharacter
{
public:
    Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture);
    virtual void Tick(float deltaTime) override;
    void SetTarget(Character *character);
    virtual Vector2 GetScreenPos() override;

private:
    Character *target;
    float damagePerSec{10.f};
    float radius{25.f};
};