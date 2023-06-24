#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include "raylib.h"

struct Character {
    int head;
    int body;

    Color headColor;
    Color bodyColor;
    Rectangle headRect, bodyRect;

    int speed;

    void GenerateCharacter();
    void DrawCharacter();
};

const int screenWidth = 800;
const int screenHeight = 450;
const int buttonWidth = 250;
const int buttonHeight = 50;
const int buttonSpacing = 20;

const int generateButtonX = (screenWidth - buttonWidth) / 2;
const int generateButtonY = screenHeight - buttonHeight - 50;
const int saveButtonX = screenWidth - buttonWidth - buttonSpacing;
const int saveButtonY = buttonSpacing;
const int loadButtonX = saveButtonX;
const int loadButtonY = saveButtonY + buttonHeight + buttonSpacing;

Rectangle generateButton = { generateButtonX, generateButtonY, buttonWidth, buttonHeight };
Rectangle saveButton = { saveButtonX, saveButtonY, buttonWidth, buttonHeight };
Rectangle loadButton = { loadButtonX, loadButtonY, buttonWidth, buttonHeight };

Color headColor, bodyColor;
int headShape, bodyShape;
Rectangle headRect, bodyRect;
bool characterGenerated = false;

void GenerateCharacter(Character& randomCharacter);
void DrawCharacter(const Character& character);
int GenerateShapeValues();

void SaveCharacter(const std::string& filename, const Character& character);
void LoadCharacter(const std::string& filename, Character& character);

int main()
{
    InitWindow(screenWidth, screenHeight, "Character Designer");
    SetTargetFPS(60);

    Character currentCharacter;
    currentCharacter.speed = 100;
    currentCharacter.headRect.x = 150;
    currentCharacter.headRect.y = 150;
    currentCharacter.bodyRect.x = 150;
    currentCharacter.bodyRect.y = 250;

    currentCharacter.GenerateCharacter();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float deltaTime = GetFrameTime();

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        {
            currentCharacter.headRect.y -= currentCharacter.speed * deltaTime;
            currentCharacter.bodyRect.y -= currentCharacter.speed * deltaTime;
        }
        else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        {
            currentCharacter.headRect.y += currentCharacter.speed * deltaTime;
            currentCharacter.bodyRect.y += currentCharacter.speed * deltaTime;
        }

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        {
            currentCharacter.headRect.x -= currentCharacter.speed * deltaTime;
            currentCharacter.bodyRect.x -= currentCharacter.speed * deltaTime;
        }
        else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        {
            currentCharacter.headRect.x += currentCharacter.speed * deltaTime;
            currentCharacter.bodyRect.x += currentCharacter.speed * deltaTime;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();

            if (CheckCollisionPointRec(mousePos, generateButton)) {
                currentCharacter.GenerateCharacter();
                characterGenerated = true;
            }
            else if (CheckCollisionPointRec(mousePos, saveButton)) {
                SaveCharacter("currentCharacter.bin", currentCharacter);
            }
            else if (CheckCollisionPointRec(mousePos, loadButton)) {
                LoadCharacter("currentCharacter.bin", currentCharacter);
            }

        }

        DrawRectangleRec(generateButton, RED);
        DrawRectangleRec(saveButton, GREEN);
        DrawRectangleRec(loadButton, GRAY);
        DrawText("Generate Character!", generateButtonX + 20, generateButtonY + 10, 20, BLACK);
        DrawText("Save", saveButtonX + 20, saveButtonY + 20, 20, BLACK);
        DrawText("Load", loadButtonX + 20, loadButtonY + 20, 20, BLACK);

        if (characterGenerated) {
            currentCharacter.DrawCharacter();
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}

void Character::GenerateCharacter()
{
    head = GenerateShapeValues();
    body = GenerateShapeValues();

    headRect.height = 100.0;
    headRect.width = 100.0;
    bodyRect.height = 100.0;
    bodyRect.width = 100.0;

    int red = static_cast<int>(GetRandomValue(0, 255));
    int green = static_cast<int>(GetRandomValue(0, 255));
    int blue = static_cast<int>(GetRandomValue(0, 255));

    headColor = { static_cast<unsigned char>(red), static_cast<unsigned char>(green), static_cast<unsigned char>(blue), 255 };

    red = static_cast<int>(GetRandomValue(0, 255));
    green = static_cast<int>(GetRandomValue(0, 255));
    blue = static_cast<int>(GetRandomValue(0, 255));

    bodyColor = { static_cast<unsigned char>(red), static_cast<unsigned char>(green), static_cast<unsigned char>(blue), 255 };
}

void Character::DrawCharacter()
{
    // Draw head shape
    if (head == 0) {
        DrawRectangleRec(headRect, headColor);
    }
    else {
        DrawCircle(headRect.x + headRect.width / 2, headRect.y + headRect.height / 2, headRect.width / 2, headColor);
    }

    // Draw body shape
    if (body == 1) {
        DrawRectangleRec(bodyRect, bodyColor);
    }
    else {
        DrawCircle(bodyRect.x + bodyRect.width / 2, bodyRect.y + bodyRect.height / 2, bodyRect.width / 2, bodyColor);
    }
}

int GenerateShapeValues()
{
    // Create random seed for head/body to choose between rectangle and circle
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> valueDist(0, 1);

    int shapeValue = static_cast<int>(valueDist(gen));

    return shapeValue;
}

void SaveCharacter(const std::string& filename, const Character& character)
{
    std::ofstream file(filename, std::ios::binary);
    if (file)
    {
        file.write(reinterpret_cast<const char*>(&character), sizeof(Character));
        file.close();
    }
}

void LoadCharacter(const std::string& filename, Character& character)
{
    // Store the position of the previous character
    float previousHeadPosX = character.headRect.x;
    float previousHeadPosY = character.headRect.y;
    float previousBodyPosX = character.bodyRect.x;
    float previousBodyPosY = character.bodyRect.y;

    std::ifstream file(filename, std::ios::binary);
    if (file)
    {
        // Load character data
        file.read(reinterpret_cast<char*>(&character), sizeof(Character));
        file.close();

        // Assign the previous position to the loaded character
        character.headRect.x = previousHeadPosX;
        character.headRect.y = previousHeadPosY;
        character.bodyRect.x = previousBodyPosX;
        character.bodyRect.y = previousBodyPosY;
    }
}
