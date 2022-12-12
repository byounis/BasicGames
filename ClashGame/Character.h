#ifndef CHARACTER_H
#define CHARACTER_H
#include "raylib.h"
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Character(int winWidth, int winHeight, Texture2D idle_texture, Texture2D run_texture);
    virtual void Tick(float deltaTime) override;
    virtual Vector2 GetScreenPos() override;
    Rectangle GetWeaponCollisionRec() const { return weaponCollisionRec; }
    float GetHealth() const { return health; }
    void TakeDamage(float damage);

private:
    int windowWidth{};
    int windowHeight{};
    Texture2D weapon{LoadTexture("characters/weapon_sword.png")};
    Rectangle weaponCollisionRec{};
    float health{100.f};
};
#endif