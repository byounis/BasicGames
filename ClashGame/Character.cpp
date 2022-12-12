#include "Character.h"
#include "raymath.h"

Character::Character(int winWidth, int winHeight, Texture2D idle_texture, Texture2D run_texture) :
    windowWidth{winWidth}, windowHeight{winHeight}
{
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;
    width = texture.width / maxFrame;
    height = texture.height;
}

Vector2 Character::GetScreenPos(){
    return Vector2{
        static_cast<float>(windowWidth) / 2.0f - scale * 0.5f * width,
        static_cast<float>(windowHeight) / 2.0f - scale * 0.5f * height};
}

void Character::Tick(float deltaTime)
{
    if(!GetAlive()) {
        return;
    }

    if (IsKeyDown(KEY_W))
    {
        velocity.y -= 1.0;
    }
    else if (IsKeyDown(KEY_S))
    {
        velocity.y += 1.0;
    }

    if (IsKeyDown(KEY_A))
    {
        velocity.x -= 1.0;
    }
    else if (IsKeyDown(KEY_D))
    {
        velocity.x += 1.0;
    }

    BaseCharacter::Tick(deltaTime);

    Vector2 origin{};
    Vector2 offset{};
    float rotation{};
    if(rightLeft > 0.f){
        origin = {0.f, weapon.height * scale};
        offset = {35.f, 55.f};
        weaponCollisionRec = {
            GetScreenPos().x + offset.x,
            GetScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale
        };

        rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 35.f : 0.f;
    }
    else{
        origin = {weapon.width * scale, weapon.height * scale};
        offset = {25.f, 55.f};
        weaponCollisionRec = {
            GetScreenPos().x + offset.x - weapon.width * scale,
            GetScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale
        };
        
        rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? -35.f : 0.f;
    }

    //Draw the sword
    Rectangle source{0.f, 0.f, weapon.width * rightLeft, static_cast<float>(weapon.height)};
    Rectangle dest{GetScreenPos().x + offset.x, GetScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale };

    DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);
    DrawRectangleLines(weaponCollisionRec.x, weaponCollisionRec.y, weaponCollisionRec.width, weaponCollisionRec.height, RED);
}

void Character::TakeDamage(float damage){
    health -= damage;
    if(health <= 0){
        SetAlive(false);
    }
}