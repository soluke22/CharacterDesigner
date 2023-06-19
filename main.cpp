#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <string>
#include "raylib.h"

Color headColor, bodyColor;
int headShape, bodyShape;
Rectangle headRect, bodyRect;
bool characterGenerated = false;

void GenerateCharacter();
void DrawCharacter();

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int buttonWidth = 250;
    const int buttonHeight = 50;
    const int buttonX = (screenWidth - buttonWidth) / 2;
    const int buttonY = screenHeight - buttonHeight - 50;
    Rectangle button = { buttonX, buttonY, buttonWidth, buttonHeight };
    
    InitWindow(screenWidth, screenHeight, "Character Designer");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    GenerateCharacter();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   


        //Update
        //---------------------------------------------------------------------------------

        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();

            if (CheckCollisionPointRec(mousePos, button)) {
                GenerateCharacter();
            }
        }

        DrawRectangleRec(button, RED);
        DrawText("Generate Character!", buttonX + 20, buttonY + 10, 20, BLACK);
        DrawText("Press the button to randomly create a character!", 225, 100, 20, BLACK);

        if (characterGenerated) {
            DrawCharacter();
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void GenerateCharacter()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> headDist(0, 1);
    std::uniform_int_distribution<> bodyDist(0, 1);

    headShape = headDist(gen);
    headRect = (Rectangle){150, 150, 100, 100};
    
    bodyShape = bodyDist(gen);
    bodyRect = (Rectangle){150, 250, 100, 100};

    int red = GetRandomValue(0, 255);
    int green = GetRandomValue(0, 255);
    int blue = GetRandomValue(0, 255);

    headColor = (Color){(unsigned char)red, (unsigned char)green, (unsigned char)blue, 255};

    red = GetRandomValue(0, 255);
    green = GetRandomValue(0, 255);
    blue = GetRandomValue(0, 255);

    bodyColor = (Color){(unsigned char)red, (unsigned char)green, (unsigned char)blue, 255};

    characterGenerated = true;
}

void DrawCharacter()
{
    // Draw head shape
    if (headShape == 0) {
        DrawRectangleRec(headRect, headColor);
    } else {
        DrawCircle(headRect.x + headRect.width / 2, headRect.y + headRect.height / 2, headRect.width / 2, headColor);
    }

    // Draw body shape
    if (bodyShape == 1) {
        DrawRectangleRec(bodyRect, bodyColor);
    } else {
        DrawCircle(bodyRect.x + bodyRect.width / 2, bodyRect.y + bodyRect.height / 2, bodyRect.width / 2, bodyColor);
    }
}
