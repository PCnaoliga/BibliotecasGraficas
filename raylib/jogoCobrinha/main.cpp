#include <cinttypes>
#include <cstdlib>
#include <deque> // Biblioteca para implementar uma deque
#include <filesystem>
#include <iostream>
#include <raylib.h>  // Biblioteca grafica usada
#include <raymath.h> // Biblioteca do ray lib para calculo da area
#include <stdexcept>
#include <type_traits>

using namespace std;
// Cores que vão ser usadas
Color green = {173, 204, 96, 255};
Color darkgreen = {43, 51, 24, 255};
Color arcadegreen = {140, 156, 72, 255};
Color arcadedarkgreen = {92, 108, 56, 255};

// Sistema de "Celulas", espaçamentos na tela que vão ajudar
int cellSize = 30;
int cellCount = 25;
int offset = 75;

// Tempo de update do jogo
double lastUpdateTime = 0;

// Função responsavel por aumentar a cobra(la ele)
bool ElementInDque(Vector2 element, deque<Vector2> deque) {
  for (unsigned int i = 0; i < deque.size(); i++) {
    if (Vector2Equals(deque[i], element)) {
      return true;
    }
  }
  return false;
}

// Função por fazer a ativaçao do jogo, responsavel por fazer o jogo rodar de
// forma mais concisa
bool eventTriger(double interval) {
  double currentTime = GetTime();
  if (currentTime - lastUpdateTime >= interval) {
    lastUpdateTime = currentTime;
    return true;
  }
  return false;
}

class Button {
public:
  int buttonWidth = cellSize * 8;
  int buttonHeight = cellSize * 3;
  int spacing = cellSize * 3;
  int buttonCount = 4;

  void Draw() {
    int areaSize = cellCount * cellSize;
    int totalMenuHeight =
        (buttonCount * buttonHeight) + ((buttonCount - 1) * spacing);

    int startX = offset + (areaSize - buttonWidth) / 2;
    int startY = offset + (areaSize - totalMenuHeight) / 2;

    for (int i = 0; i < buttonCount; i++) {
      int buttonY = startY + i * (buttonHeight + spacing);

      Rectangle buttonRect = {(float)startX, (float)buttonY, (float)buttonWidth,
                              (float)buttonHeight};

      DrawRectangleRec(buttonRect, arcadedarkgreen);
      DrawRectangleLinesEx(buttonRect, 2, darkgreen);
    }
  }
};

// Classe para o objeto Cobra(La ele novamente?)
class Snake {
public:
  // O tamanho inicial da cobra(nao da para ler isso serio kkkkkkk)
  deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
  Vector2 direction = {1, 0};
  // Variavel de adiconar seguimento como falso
  bool addSegment = false;

  // Função de desenho da cobra
  void Draw() {
    // Loop que vai desenhar o tamanho da cobra com base no tamanho do corpo
    for (unsigned int i = 0; i < body.size(); i++) {
      float x = body[i].x;
      float y = body[i].y;
      Rectangle segment =
          Rectangle{offset + x * cellSize, offset + y * cellSize,
                    (float)cellSize, (float)cellSize};
      DrawRectangleRounded(segment, 0.5, 6, darkgreen);
    }
  }

  // Função responsavel pela movimentação, tira o ultimo vertor e adiciona na
  // frente de todos
  void Update() {
    body.push_front(Vector2Add(body[0], direction));
    if (addSegment == true) {
      addSegment = false;
    } else {
      body.pop_back();
    }
  }

  // Função para o "Reset" da cobra, voltando para o estado inicial
  void Reset() {
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direction = {1, 0};
  }
};

// Classe para o objeto COMIDA
class Food {

public:
  Vector2 position;
  Texture2D texture;

  // Função de desenho e adição da textura do objeto
  Food(deque<Vector2> snakeBody) {
    Image image = LoadImage("./graphics/food.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position =
        GenerateRandomPos(snakeBody); // Position = Valor aleatorio da Função
  }
  // ~Food() { UnloadImage(texture); }

  void Draw() {
    DrawTexture(texture, offset + position.x * cellSize,
                offset + position.y * cellSize, WHITE);
  }
  // Função para gerar de forma "aleatoria" a nova posição da comida
  Vector2 GenereteRandomCell() {
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    Vector2 position = {x, y};
    return Vector2{x, y};
  }

  // Função para retornar valor aleatorio da comida
  Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
    Vector2 position = GenereteRandomCell();
    while (ElementInDque(position, snakeBody)) {
      position = GenereteRandomCell();
    }
    return position;
  }
};

// Classe responsavel pelo comportamento do jogo
class Game {
public:
  // Cenario do começo do jogo
  Snake snake = Snake();
  Food food = Food(snake.body);
  bool running = false;
  int score = 0;
  Sound eatSound;
  Sound wallSound;

  // Definição dos audios
  Game() {
    InitAudioDevice();
    eatSound = LoadSound("./sounds/eat.mp3");
    wallSound = LoadSound("./sounds/wall.mp3");
  }

  ~Game() {
    UnloadSound(eatSound);
    UnloadSound(wallSound);
    CloseAudioDevice();
  }

  // Função do desenho dos objetos
  void Draw() {
    food.Draw();
    snake.Draw();
  }

  // Checagem da coliçao da comida e ativaçao da pontuação e da Função de nova
  // posicao
  void CheckCollisionWithFood() {
    if (Vector2Equals(snake.body[0], food.position)) {
      food.position = food.GenerateRandomPos(snake.body);
      snake.addSegment = true;
      score++;
      PlaySound(eatSound);
    }
  }

  // Checagem da coliçao com as bordas
  void CheckCollisionWithEdges() {
    if (snake.body[0].x == cellCount || snake.body[0].x == -1) {
      GameOver();
    }
    if (snake.body[0].y == cellCount || snake.body[0].y == -1) {
      GameOver();
    }
  }
  // Update geral das colições
  void Update() {
    if (running) {
      snake.Update();
      CheckCollisionWithFood();
      CheckCollisionWithEdges();
      CheckCollitionWithTail();
    }
  }

  // Função de gameover quando for condição para perder
  void GameOver() {
    snake.Reset();
    food.position = food.GenerateRandomPos(snake.body);
    running = false;
    score = 0;
    PlaySound(wallSound);
  }

  // Função de checagem se a cobra se bateu
  void CheckCollitionWithTail() {
    deque<Vector2> headlessBody = snake.body;
    headlessBody.pop_front();
    if (ElementInDque(snake.body[0], headlessBody)) {
      GameOver();
    }
  }
};

int main() {
  // Comando para abrir a janela
  // Obs: o tamanho e a mutiplicação das celulas para ajudar no controle de
  // objetos na tela
  InitWindow(2 * offset + cellSize * cellCount,
             2 * offset + cellSize * cellCount, "Telinha jogo");
  SetTargetFPS(60);

  Game game = Game();
  Button button = Button();

  // Loop do jogo
  while (!WindowShouldClose()) {
    BeginDrawing();

    // Quando o eventTriger for ativado a cada 0.2 Segundos vai ativar o
    // update
    if (eventTriger(0.2)) {
      game.Update();
    }

    // Quando o jogo estiver rodando
    if (game.running) {
      // Ifs de verificação para a movimentação da cobra
      if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
        game.snake.direction = {0, -1};
        // Essa Função e importante para quando o usuario perder a cobra
        // volta(UI) quando e apertado os direcionais
        game.running = true;
      }
      if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
        game.snake.direction = {0, 1};
        game.running = true;
      }
      if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
        game.snake.direction = {-1, 0};
        game.running = true;
      }
      if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
        game.snake.direction = {1, 0};
        game.running = true;
      }

      // Parte direcionada apenas para o visual
      ClearBackground(green);
      game.Draw();
      DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5,
                                     (float)cellSize * cellCount + 10,
                                     (float)cellSize * cellCount + 10},
                           5, darkgreen);
      DrawText("Retro Snake", offset - 5, 20, 40, darkgreen);
      DrawText(TextFormat("%i", game.score), offset - 5,
               offset + cellSize * cellCount + 10, 40, darkgreen);
    }

    // Quando o jogo não estiver rodando/Tela de titulo
    if (!game.running) {
      BeginDrawing();
      ClearBackground(green);
      DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5,
                                     (float)cellSize * cellCount + 10,
                                     (float)cellSize * cellCount + 10},
                           5, darkgreen);
      DrawText("Retro Snake", offset - 5 + 170, 20, 60, darkgreen);
      button.Draw();
    }

    // Quando for pressionado a tecla ENTER starta o jogo
    if (IsKeyPressed(KEY_ENTER)) {
      game.running = true;
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
