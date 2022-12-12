#include "BaseCharacter.h"
#include "raymath.h"

BaseCharacter::BaseCharacter()
{
    width = texture.width / maxFrame;
    height = texture.height;
}

void BaseCharacter::UndoMovement()
{
    worldPos = worldPosLastFrame;
}

Rectangle BaseCharacter::GetCollisionRec()
{
    return Rectangle{GetScreenPos().x, GetScreenPos().y, width * scale, height * scale};
}

void BaseCharacter::Tick(float deltaTime)
{
    worldPosLastFrame = worldPos;

    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if (frame > maxFrame)
        {
            frame = 0;
        }
    }

    if (Vector2Length(velocity) != 0.0)
    {
        Vector2 velocityNormalized = Vector2Normalize(velocity);
        Vector2 scaledvelocity = Vector2Scale(velocityNormalized, speed);
        worldPos = Vector2Add(worldPos, scaledvelocity);
        rightLeft = velocity.x < 0.0f ? -1.f : 1.f;
        texture = run;
    }
    else
    {
        texture = idle;
    }

    velocity = {};

    Rectangle source{width * frame, 0.0f, rightLeft * width, height};
    Rectangle destination{GetScreenPos().x, GetScreenPos().y, scale * width, scale * height};

    DrawTexturePro(texture, source, destination, Vector2{}, 0, WHITE);
}