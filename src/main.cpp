#include <raylib.h>
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>

struct Cell {
  bool isAlive;
  bool willBeAlive;
  int age;
};

const int SCALE = 4;
const int WIDTH = 1024 / SCALE;
const int HEIGHT = 768 / SCALE;
const int FERTILITY = 25;  // Percentage chance a cell starts alive
const bool SHOW_GRID = false;
const int FPS = 60;
const int AGE_SPEED = 4;

int main() {
  Cell grid[WIDTH][HEIGHT];
  int generations = 0;
  std::vector<std::bitset<WIDTH*HEIGHT>> previousGenerations;
  std::bitset<WIDTH*HEIGHT> thisGeneration;
  bool stable = false;
  int finalGeneration;

  static unsigned int seed = static_cast<unsigned int>(time(nullptr));

  for (int i = 0; i < WIDTH; ++i) {
    for (int j = 0; j < HEIGHT; ++j) {
      bool randomValue = (rand_r(&seed) % 100) >= FERTILITY;
      grid[i][j].isAlive = false;
      grid[i][j].willBeAlive = randomValue;
      grid[i][j].age = 0;
    }
  }

  InitWindow(1024, 768, "Life");
  SetTargetFPS(FPS);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    generations++;
    thisGeneration = 0;
    for (int y = 0; y < HEIGHT; ++y) {
      for (int x = 0; x < WIDTH; ++x) {
        if (grid[x][y].willBeAlive) {
          unsigned char color_value;
          // color_value ranges from 64 to 255 based on age, where older is darker, in steps of 32.
          if (255 - (grid[x][y].age - 1) * AGE_SPEED < 64) {
            color_value = 64;
          } else {
            color_value = 255 - (grid[x][y].age - 1) * AGE_SPEED;
          }
          Color age_color = {color_value, color_value, color_value, 255};
          DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, age_color);
        }
        grid[x][y].isAlive = grid[x][y].willBeAlive;
      }
    }

    // Update grid state
    for (int y = 0; y < HEIGHT; ++y) {
      for (int x = 0; x < WIDTH; ++x) {
        int aliveNeighbors = 0;
        for (int dy = -1; dy <= 1; ++dy) {
          for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            if (grid[(x + dx + WIDTH) % WIDTH][(y + dy + HEIGHT) % HEIGHT].isAlive) {
              aliveNeighbors++;
            }
          }
        }

        if (grid[x][y].isAlive) {
          grid[x][y].willBeAlive = (aliveNeighbors == 2 || aliveNeighbors == 3);
        } else {
          grid[x][y].willBeAlive = (aliveNeighbors == 3);
        }

        if (grid[x][y].willBeAlive) {
          grid[x][y].age++;
          thisGeneration.set(y * WIDTH + x);
        } else {
          grid[x][y].age = 0;
        }
      }
    }
    if (SHOW_GRID) {
      for (int x = 0; x < WIDTH; x++) {
        DrawLine(x * SCALE, 0, x * SCALE, HEIGHT * SCALE, DARKGRAY);
      }
      for (int y = 0; y < HEIGHT; y++) {
        DrawLine(0, y * SCALE, WIDTH * SCALE, y * SCALE, DARKGRAY);
      }
    }
    DrawText(std::to_string(generations).c_str(), 10, 10, 20, GREEN);
    if (stable || std::find(previousGenerations.begin(), previousGenerations.end(), thisGeneration) != previousGenerations.end()) {
      if (!stable)
        finalGeneration = generations;
      stable = true;
      std::string finalText = "Stable state reached at generation " + std::to_string(finalGeneration);
      DrawText(finalText.c_str(), 10, 40, 20, RED);
    } else {
      if (sizeof(previousGenerations) > 100) {
        previousGenerations.erase(previousGenerations.begin());
      }
      previousGenerations.push_back(thisGeneration);
    }
    EndDrawing();
  }
  return 0;
}
