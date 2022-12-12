#include "raylib.h"
#define WINDOW_NAME "Dasher Game"

struct AnimationData
{
    Rectangle rect;
    Vector2 position;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimationData data, int windowHeight)
{
    return data.position.y >= windowHeight - data.rect.height;
}

AnimationData updateAnimationData(AnimationData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime){
        data.runningTime = 0.0;

        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if(data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }

    return data;
}

int main()
{
    const int TARGET_FPS{60};
    const int GRAVITY{1200};

    int windowDimensions[2]{512, 380};

    InitWindow(windowDimensions[0], windowDimensions[1], WINDOW_NAME);
    SetTargetFPS(TARGET_FPS);

    int velocity{0};
    int jump_speed{-600};

    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimationData scarfyData;
    scarfyData.rect.width = scarfy.width / 6;
    scarfyData.rect.height = scarfy.height;
    scarfyData.rect.x = 0;
    scarfyData.rect.y = 0;
    scarfyData.position.x = windowDimensions[0] / 2 - scarfyData.rect.width / 2;
    scarfyData.position.y = windowDimensions[1] - scarfyData.rect.height;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.frame = 0;
    scarfyData.runningTime = 0.0;

    Texture2D nebula = LoadTexture("textures/nebula.png");
    const int sizeOfNebulae{6};
    AnimationData nebulae[sizeOfNebulae]{};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float backgroundX{};

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float midgroundX{};
    
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float foregroundX{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rect.x = 0;
        nebulae[i].rect.y = 0;
        nebulae[i].rect.width = nebula.width / 8;
        nebulae[i].rect.height = nebula.height / 8;
        nebulae[i].position.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;

        nebulae[i].position.x = windowDimensions[0] + 250  + i * 300 + i * i * 10;
    }

    float finishLine{nebulae[sizeOfNebulae - 1].position.x};

    int nebulaVelocity{-300};
    bool collision{};

    while (!WindowShouldClose())
    {
        const float dT{GetFrameTime()};

        BeginDrawing();
        ClearBackground(WHITE);

        backgroundX -= 20 * dT;
        foregroundX -= 80 * dT;
        midgroundX -= 40 * dT;

        if(backgroundX <= -background.width *2)
        {
            backgroundX = 0;
        }

        if(foregroundX <= -foreground.width *2)
        {
            foregroundX = 0;
        }

        if(midgroundX <= -midground.width *2)
        {
            midgroundX = 0;
        }

        Vector2 bg1Position{backgroundX, 0.0};
        DrawTextureEx(background, bg1Position, 0.0, 2.0, WHITE);
        Vector2 bg2Position{backgroundX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Position, 0.0, 2.0, WHITE);

        Vector2 midground1Position{midgroundX, 0.0};
        DrawTextureEx(midground, midground1Position, 0, 2, WHITE);
        Vector2 midground2Position{midgroundX + midground.width * 2, 0.0};
        DrawTextureEx(midground, midground2Position, 0, 2, WHITE);

        Vector2 foreground1Position{foregroundX, 0.0};
        DrawTextureEx(foreground, foreground1Position, 0, 2, WHITE);
        Vector2 foreground2Position{foregroundX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, foreground2Position, 0, 2, WHITE);
        
        if(isOnGround(scarfyData, windowDimensions[1]))
        {
            scarfyData = updateAnimationData(scarfyData, dT, 5);
            velocity = 0;
            if(IsKeyPressed(KEY_SPACE))
            {
                velocity += jump_speed;
            }
        }
        else
        {
            velocity += GRAVITY * dT;
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].position.x += nebulaVelocity * dT;
            nebulae[i] = updateAnimationData(nebulae[i], dT, 8);
        }

        scarfyData.position.y += velocity * dT;
        finishLine += nebulaVelocity * dT;

        for (AnimationData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebulaRect{nebula.position.x + pad, nebula.position.y + pad, nebula.rect.width - 2 * pad, nebula.rect.height - 2 * pad};
            Rectangle scarfyRect{scarfyData.position.x, scarfyData.position.y, scarfyData.rect.width, scarfyData.rect.height};

            if(CheckCollisionRecs(nebulaRect, scarfyRect))
            {
                collision = true;
            }
        }
        
        if(collision)
        {
            //lose game
            DrawText("Game Over!", windowDimensions[0]/4, windowDimensions[1]/2, 40, RED);
        }
        else if (scarfyData.position.x >= finishLine + 50)
        {
            DrawText("You win!", windowDimensions[0]/4, windowDimensions[1]/2, 40, WHITE);
        }
        else
        {
            DrawTextureRec(scarfy, scarfyData.rect, scarfyData.position, WHITE);

            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rect, nebulae[i].position, WHITE);
            }
        }

        EndDrawing();
    }
    
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}