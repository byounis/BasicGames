#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Enemy.h"
#include "Prop.h"
#include <string>
#define WINDOW_NAME "Clash Game"

int main(int argc, char const *argv[])
{
    const int TARGET_FPS{60};

    const int windowDimensions[2]{384, 384};

    InitWindow(windowDimensions[0], windowDimensions[1], WINDOW_NAME);
    SetTargetFPS(TARGET_FPS);

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPosition{};
    const float mapScale{4.f};

    Texture2D knightIdle =
        LoadTexture("characters/knight_idle_spritesheet.png");
    Texture2D knightRun =
        LoadTexture("characters/knight_run_spritesheet.png");

    Character knight{
        windowDimensions[0],
        windowDimensions[1],
        knightIdle,
        knightRun};

    Texture2D rockTexture = LoadTexture("nature_tileset/Rock.png");
    Texture2D logTexture = LoadTexture("nature_tileset/Log.png");

    Prop props[2]{
        Prop{Vector2{600, 300}, rockTexture},
        Prop{Vector2{400, 500}, logTexture}};

    Texture2D goblinIdle =
        LoadTexture("characters/goblin_idle_spritesheet.png");
    Texture2D goblinRun =
        LoadTexture("characters/goblin_run_spritesheet.png");

    Enemy goblin{
        Vector2{800.f, 300.f},
        goblinIdle,
        goblinRun};

    Texture2D slimeIdle =
        LoadTexture("characters/slime_idle_spritesheet.png");
    Texture2D slimeRun =
        LoadTexture("characters/slime_run_spritesheet.png");

    Enemy slime{
        Vector2{500.f, 200.f},
        slimeIdle,
        slimeRun};

    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto &&enemy : enemies)
    {
        enemy->SetTarget(&knight);
    }

    while (!WindowShouldClose())
    {
        const float dT{GetFrameTime()};

        BeginDrawing();
        ClearBackground(WHITE);

        mapPosition = Vector2Scale(knight.GetWorldPos(), -1);

        DrawTextureEx(map, mapPosition, 0.0, mapScale, WHITE);

        for (auto prop : props)
        {
            prop.Render(knight.GetWorldPos());
        }

        if (!knight.GetAlive())
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            std::string knightHealth{"Health: "};
            knightHealth.append(std::to_string(knight.GetHealth()), 0, 5);
            DrawText(knightHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        knight.Tick(dT);

        for (auto &&enemy : enemies)
        {
            enemy->Tick(dT);
        }

        // Check map bounds
        if (
            knight.GetWorldPos().x < 0.f ||
            knight.GetWorldPos().y < 0.f ||
            knight.GetWorldPos().x + windowDimensions[0] > map.width * mapScale ||
            knight.GetWorldPos().y + windowDimensions[1] > map.height * mapScale)
        {
            knight.UndoMovement();
        }

        for (auto prop : props)
        {
            if (CheckCollisionRecs(knight.GetCollisionRec(), prop.GetCollisionRec(knight.GetWorldPos())))
            {
                knight.UndoMovement();
            }

            for (auto &&enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->GetCollisionRec(), prop.GetCollisionRec(enemy->GetWorldPos())))
                {
                    enemy->UndoMovement();
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto &&enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->GetCollisionRec(), knight.GetWeaponCollisionRec()))
                {
                    enemy->SetAlive(false);
                }
            }
        }

        EndDrawing();
    }

    UnloadTexture(map);
    UnloadTexture(logTexture);
    UnloadTexture(rockTexture);
    UnloadTexture(goblinIdle);
    UnloadTexture(goblinRun);
    UnloadTexture(slimeIdle);
    UnloadTexture(slimeRun);

    CloseWindow();
}
