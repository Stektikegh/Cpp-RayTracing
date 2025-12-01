#include "raylib.h"
#include <iostream>
#include <math.h>

using namespace std;
#define screenWidth 1300
#define screenHeight 900
#define mapHeight 300
#define mapWidth 300

Color transparentWhite = (Color){255, 255, 255, 0};

int main(void) {
  InitWindow(screenWidth, screenHeight, "TOP DOWN SHOOTER");
  SetTargetFPS(60);

  float timer = 0.0f;
  float spawn_interval = 1.0f;

  Image player_image = LoadImage("../assets/Soldier-Idle.png");
  ImageResize(&player_image, 165, 165);
  Texture2D player_texture = LoadTextureFromImage(player_image);
  UnloadImage(player_image);

  float playerX = (float)mapWidth / 2;
  float playerY = (float)mapHeight / 2 + 25;
  float playerWidth = (float)player_texture.width / 8;
  float playerHeight = (float)player_texture.height / 8;
  float playerRotation = 0;
  int acceleration = 70;
  int break_speed = 100;
  int MAX_SPEED = 40;
  double dx = 0, dy = 0;

  float speed = 0;

  while (!WindowShouldClose()) {

    float dt = GetFrameTime();
    timer += dt;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
      playerRotation += 4;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
      playerRotation -= 4;
    }

    float playerRadRotation = DEG2RAD * (float)playerRotation;

    float WALL_WIDTH = 5;
    Rectangle walls[] = {

        {(float)3 * mapWidth / 4 - 10, (float)3 * mapHeight / 4 - 10, 20, 20},
        {0, 0, WALL_WIDTH, mapHeight},
        {0, 0, mapWidth, WALL_WIDTH},
        {mapWidth - WALL_WIDTH, 0, WALL_WIDTH, mapHeight},
        {0, mapHeight - WALL_WIDTH, mapWidth, WALL_WIDTH},
        {(float)mapWidth / 2 - WALL_WIDTH / 2, WALL_WIDTH, WALL_WIDTH, 80},

        {(float)mapWidth / 2 - WALL_WIDTH / 2, mapHeight - WALL_WIDTH - 80,
         WALL_WIDTH, 80},

        {WALL_WIDTH, (float)mapHeight / 2 - WALL_WIDTH, 80, WALL_WIDTH},

        {mapWidth - WALL_WIDTH - 80, (float)mapHeight / 2 - WALL_WIDTH, 80,
         WALL_WIDTH},

        {(float)mapWidth / 4 - 4, (float)mapHeight / 4 - 4, 8, 8},
        {(float)2 * mapWidth / 6 - 4, (float)7 * mapHeight / 8 - 4, 8, 8},

        {(float)1 * mapWidth / 6 - 4, (float)7 * mapHeight / 8 - 4, 8, 8},

        {(float)2 * mapWidth / 6 - 4, (float)5 * mapHeight / 8 - 4, 8, 16},

        {(float)1 * mapWidth / 6 - 4, (float)5 * mapHeight / 8 - 4, 8, 16},

    };

    float x = 0, y = 0;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
      speed += 0.5 * acceleration * dt;

      y = speed * (float)sin(playerRadRotation) * dt;
      x = speed * (float)cos(playerRadRotation) * dt;

      speed += 0.5 * acceleration * dt;

      speed = speed > MAX_SPEED ? MAX_SPEED : speed;
    } else {
      speed -= break_speed * dt;
      if (speed > 0) {
        y = speed * (float)sin(playerRadRotation) * dt;
        x = speed * (float)cos(playerRadRotation) * dt;
      } else {
        speed = 0;
      }
    }

    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
      speed = -20 * MAX_SPEED * dt;

      y = speed * (float)sin(playerRadRotation) * dt;
      x = speed * (float)cos(playerRadRotation) * dt;
    }

    // wall collision
    bool collision_x = false;
    bool collision_y = false;

    for (Rectangle wall : walls) {
      if (playerX + x > wall.x && playerX + x < wall.x + wall.width &&
          playerY > wall.y && playerY < wall.y + wall.height) {
        collision_x = true;
      }

      if (playerX > wall.x && playerX < wall.x + wall.width &&
          playerY + y > wall.y && playerY + y < wall.y + wall.height) {
        collision_y = true;
      }

      if (collision_x && collision_y) {
        break;
      }
    }

    playerX += !collision_x ? x : 0;
    playerY += !collision_y ? y : 0;

    BeginDrawing();

    ClearBackground(BLACK);

    // DrawCircle(playerX, playerY, playerHeight / 2, RED);

    DrawTexturePro(
        player_texture,
        Rectangle{0, 0, (float)player_texture.width,
                  (float)player_texture.height},
        Rectangle{playerX, playerY, (float)playerWidth, (float)playerHeight},
        Vector2((float)playerWidth / 2, (float)playerHeight / 2),
        playerRotation + 90, WHITE);

    for (Rectangle wall : walls) {
      DrawRectangleRec(wall, WHITE);
    }

    int RAYS = 1300;
    int FOV = 20;
    float STEP = (float)1 / FOV;
    float ddx = 0;
    playerRadRotation = playerRadRotation - RAYS * STEP * DEG2RAD * 0.5;
    for (int i = 0; i < RAYS; i++) {
      playerRadRotation += STEP * DEG2RAD;
      dx = 0;
      dy = 0;
      bool running = true;
      while (running) {
        dx += 4 * cos(playerRadRotation);
        dy += 4 * sin(playerRadRotation);
        for (Rectangle wall : walls) {
          if (playerX + dx > wall.x && playerX + dx < wall.x + wall.width &&
              playerY + dy > wall.y && playerY + dy < wall.y + wall.height) {
            running = false;
            break;
          }
        }
      }
      running = true;
      while (running) {
        dx -= 0.5 * cos(playerRadRotation);
        dy -= 0.5 * sin(playerRadRotation);
        for (Rectangle wall : walls) {
          running = false;
          if (playerX + dx > wall.x && playerX + dx < wall.x + wall.width &&
              playerY + dy > wall.y && playerY + dy < wall.y + wall.height) {
            running = true;
            break;
          }
        }
      }

      DrawLine(playerX, playerY, playerX + dx, playerY + dy, RED);
      double distance = sqrt(dx * dx + dy * dy);
      ddx += (float)screenWidth / RAYS;
      DrawLine(ddx, 500 - 10000 / distance, ddx, 500 + 10000 / distance,
               ColorAlpha(BLUE, 100 / distance));
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
