#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define TOTAL_INIMIGOS 6
#define DANO_TIRO 20

typedef enum {
    INIMIGO_VIVO,
    INIMIGO_MORTO
} EstadoInimigo;

typedef struct {
    Vector2 pos;
    float raio;
    int vida;
    EstadoInimigo estado;
} Inimigo;

void inicializarInimigos(Inimigo *vetor, int n) {
    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i);
        ini->pos = (Vector2){
            (float)GetRandomValue(50, LARGURA_JANELA - 50),
            (float)GetRandomValue(50, ALTURA_JANELA - 50)
        };
        ini->raio = 15.0f;
        ini->vida = 60;
        ini->estado = INIMIGO_VIVO;
    }
}

void atingirInimigo(Inimigo *inimigo, int dano) {
    if (inimigo == NULL || inimigo->estado == INIMIGO_MORTO) return;

    inimigo->vida -= dano;
    if (inimigo->vida <= 0) {
        inimigo->vida = 0;
        inimigo->estado = INIMIGO_MORTO;
    }
}

Inimigo *encontrarInimigoMaisProximo(Inimigo *vetor, int n, Vector2 posJogador) {
    Inimigo *maisProximo = NULL;
    float menorDistancia = 0.0f;

    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i);
        if (ini->estado == INIMIGO_MORTO) continue;

        float dx = ini->pos.x - posJogador.x;
        float dy = ini->pos.y - posJogador.y;
        float distancia = sqrtf(dx * dx + dy * dy);

        if (maisProximo == NULL || distancia < menorDistancia) {
            maisProximo = ini;
            menorDistancia = distancia;
        }
    }
    return maisProximo;
}

void curarTodos(Inimigo *vetor, int n, int cura) {
    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i);
        if (ini->estado == INIMIGO_MORTO) continue;

        ini->vida += cura;
        if (ini->vida > 60) {
            ini->vida = 60;
        }
    }
}

Inimigo *encontrarInimigoMaisFraco(Inimigo *vetor, int n) {
    Inimigo *maisFraco = NULL;

    for (int i = 0; i < n; i++) {
        Inimigo *ini = (vetor + i);
        if (ini->estado == INIMIGO_MORTO) continue;

        if (maisFraco == NULL || ini->vida < maisFraco->vida) {
            maisFraco = ini;
        }
    }
    return maisFraco;
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 4");
    SetTargetFPS(60);

    Vector2 jogador = (Vector2){ LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f };
    float raioJogador = 15.0f;

    Inimigo *inimigos = (Inimigo *)malloc(TOTAL_INIMIGOS * sizeof(Inimigo));
    inicializarInimigos(inimigos, TOTAL_INIMIGOS);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) jogador.x += 4.0f;
        if (IsKeyDown(KEY_LEFT))  jogador.x -= 4.0f;
        if (IsKeyDown(KEY_UP))    jogador.y -= 4.0f;
        if (IsKeyDown(KEY_DOWN))  jogador.y += 4.0f;

        if (IsKeyPressed(KEY_SPACE)) {
            Inimigo *alvo = encontrarInimigoMaisProximo(inimigos, TOTAL_INIMIGOS, jogador);
            atingirInimigo(alvo, DANO_TIRO);
        }

        if (IsKeyPressed(KEY_F)) {
            Inimigo *alvo = encontrarInimigoMaisFraco(inimigos, TOTAL_INIMIGOS);
            atingirInimigo(alvo, DANO_TIRO);
        }

        if (IsKeyPressed(KEY_C)) {
            curarTodos(inimigos, TOTAL_INIMIGOS, 10);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < TOTAL_INIMIGOS; i++) {
            Inimigo *ini = (inimigos + i);
            if (ini->estado == INIMIGO_VIVO) {
                DrawCircleV(ini->pos, ini->raio, RED);
            }
        }

        DrawCircleV(jogador, raioJogador, BLUE);

        EndDrawing();
    }

    free(inimigos);
    CloseWindow();

    return 0;
}