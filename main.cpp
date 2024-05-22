#include <raylib.h>
#include "includes/bola.h"
#include "includes/raquete.h"
#include "includes/placar.h"

#define ALTURA_T 600
#define LARGURA_T 800
#define PLACAR_MAX 11
#define TAM_FONTE 60


void sacaBola(Bola &bola, int quemSaca);
void verificaColisoes(Bola &bola, Raquete &p1, Raquete &p2);

Placar placarDoJogo = {
    .j1=0,
    .j2=0
};

int main()
{
    InitWindow(LARGURA_T, ALTURA_T, "Ping Pong!");
    SetTargetFPS(60);

    Bola bola;
    sacaBola(bola, 1);

    Raquete p1 = {
        .posicaoRaquete={
            .x=LARGURA_R,
            .y=ALTURA_T / 2 - ALTURA_R / 2
        },
        .velocidadeY=8.0f
    };
    Raquete p2 = {
        .posicaoRaquete={
            .x=LARGURA_T - (LARGURA_R * 2),
            .y=ALTURA_T / 2 - ALTURA_R / 2
        },
        .velocidadeY=8.0f
    };

    // Loop para gerar a animacao
    while(!WindowShouldClose())
    {
        // Movimenta a bola
        atualizaPosicaoBola(bola);
        verificaColisoes(bola, p1, p2);

        // Verifica se um dos jogadores ganhou
        if (placarDoJogo.j1 == PLACAR_MAX || placarDoJogo.j2 == PLACAR_MAX)
        {
            placarDoJogo = { 0, 0 };
        }

        // Inputs do usuario
        if (IsKeyDown(KEY_W)) movimentaRaquete(p1, Direcao::SUBIR);
        if (IsKeyDown(KEY_S)) movimentaRaquete(p1, Direcao::DESCER);
        if (IsKeyDown(KEY_UP)) movimentaRaquete(p2, Direcao::SUBIR);
        if (IsKeyDown(KEY_DOWN)) movimentaRaquete(p2, Direcao::DESCER);

        // Desenhando na tela
        BeginDrawing();
        {
            ClearBackground(PINK);
            DrawText(TextFormat("%02hu", placarDoJogo.j1), (LARGURA_T / 2) - 50 - 60, 50, TAM_FONTE, GRAY);
            DrawText(TextFormat("%02hu", placarDoJogo.j2), (LARGURA_T / 2) + 50, 50, TAM_FONTE, GRAY);
            DrawRectangle(p1.posicaoRaquete.x, p1.posicaoRaquete.y, LARGURA_R, ALTURA_R, WHITE);
            DrawRectangle(p2.posicaoRaquete.x, p2.posicaoRaquete.y, LARGURA_R, ALTURA_R, WHITE);
            DrawCircleV(bola.posicaoBola, bola.raio, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void sacaBola(Bola &bola, int quemSaca)
{
    bola = {
        .posicaoBola={
            .x=(float) LARGURA_T / 2,
            .y=(float) ALTURA_T / 2
        },
        .raio=10,
        .velocidadeX=5.0f * quemSaca,
        .velocidadeY=5.0f * quemSaca
    };
}

void verificaColisoes(Bola &bola, Raquete &p1, Raquete &p2)
{
    Vector2 &posicaoBola = bola.posicaoBola;
    float &velocidadeX = bola.velocidadeX;
    float &velocidadeY = bola.velocidadeY;
    int &raio = bola.raio;

    // Verifica se um dos jogadores pontuou
    if (posicaoBola.x + raio >= LARGURA_T)
    {
        placarDoJogo.j1++;
        sacaBola(bola, 1);
    }
    else if (posicaoBola.x - raio <= 0)
    {
        placarDoJogo.j2++;
        sacaBola(bola, -1);
    }

    // Rebate a bola se ela toca as bordas horizontais
    if (posicaoBola.y + raio >= ALTURA_T || posicaoBola.y - raio <= 0)
    {
        velocidadeY = velocidadeY * -1;
    }

    if (CheckCollisionCircleRec(posicaoBola, raio, converteParaRectangle(p1)))
    {
        velocidadeX = GetRandomValue(5, 10);
        velocidadeY = GetRandomValue(5, 10);
    }
    else if (CheckCollisionCircleRec(posicaoBola, raio, converteParaRectangle(p2)))
    {
        velocidadeX = GetRandomValue(5, 10) * -1;
        velocidadeY = GetRandomValue(5, 10) * -1;
    }
}