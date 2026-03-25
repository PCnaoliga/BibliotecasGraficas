#include <raylib.h>

int main() {
  const int screenWidth = 800;
  const int screenHeight = 600;

  InitWindow(screenWidth, screenHeight, "Telinha legal");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Opa carai", 190, 200, 20, BLACK);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
