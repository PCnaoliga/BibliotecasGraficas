#include <raylib.h>

#define telaMAX 660;

int main() {
  // Variaveis dos objetos
  float ball1X, ball1Y; // Bola 1
  float ball2X, ball2Y; // Bola 2
  float ball3X, ball3Y; // Bola 3
  float Bteste, Tx, Ty;
  float ballR;    // Raio das bola
  float moviment; // Variavel de movimentaçao

  // Valores das variaveis
  Tx = Ty = 400;
  ballR = 50.2;

  ball1X = 250;
  ball1Y = 100;

  ball2X = 450;
  ball2Y = 100;

  ball3X = 650;
  ball3Y = 100;

  moviment = 100;

  /*Armazenamento de CORES para os objetos e cenarios*/
  Color grew = {0, 0, 0, 100};

  /* Função para iniciar janela(altura,largura,nome da janela)*/
  InitWindow(1280, 720, "Telinha legal");

  /* Fps da janela*/
  SetTargetFPS(60);

  /* Loop do jogo se divide em 3 partes:
   *  Manipulação de eventos
   *  Atualizar posições
   *  Desenhar
   */
  while (!WindowShouldClose()) {

    // Atualizando posições

    moviment = ball1Y < telaMAX;
    moviment = ball2Y < telaMAX;
    moviment = ball3Y < telaMAX;

    if (moviment) {
      ball1Y += moviment;
      ball2Y += moviment;
      ball3Y += moviment;
    }

    // Desenhando objetos
    BeginDrawing();
    ClearBackground(grew);

    // Desenho das bolas
    DrawCircle(ball1X, ball1Y, ballR, GREEN);
    DrawCircle(ball2X, ball2Y, ballR, RED);
    DrawCircle(ball3X, ball3Y, ballR, YELLOW);

    // Desenho das areas de acerto
    DrawCircle(ball1X, 660, ballR, GREEN);
    DrawCircle(ball2X, 660, ballR, RED);
    DrawCircle(ball3X, 660, ballR, YELLOW);

    // Desenho linha delimitadora
    DrawLine(1280, 600, 0, 600, WHITE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

/*
    if (IsKeyDown(KEY_RIGHT)) {
      Tx += 10;
    } else if (IsKeyDown(KEY_LEFT)) {
      Tx -= 10;
    }
    if (IsKeyDown(KEY_DOWN)) {
      Ty += 10;
    } else if (IsKeyDown(KEY_UP)) {
      Ty -= 10;
    }
 * */
