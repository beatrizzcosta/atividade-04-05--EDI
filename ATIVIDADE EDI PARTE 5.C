#include "raylib.h"
#include <stdlib.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define MAX_ENTIDADES 50
#define RAIO_JOGADOR 15.0f
#define DANO_TIRO 20

typedef enum {
    ENTIDADE_JOGADOR,
    ENTIDADE_INIMIGO,
    ENTIDADE_ITEM
} TipoEntidade;

typedef union {
    int dano;
    int valor;
} ExtraEntidade;

typedef struct {
    TipoEntidade tipo;
    Vector2 pos;
    float raio;
    int vida;
    Color cor;
    ExtraEntidade extra;
} Entidade;

Entidade *vetorEntidades[MAX_ENTIDADES];
int totalEntidades = 0;

Entidade *criarEntidade(TipoEntidade tipo, Vector2 pos) {
    Entidade *e = (Entidade *)malloc(sizeof(Entidade));
    if (e == NULL) return NULL;

    e->tipo = tipo;
    e->pos = pos;

    if (tipo == ENTIDADE_JOGADOR) {
        e->raio = RAIO_JOGADOR;
    } else if (tipo == ENTIDADE_INIMIGO) {
        e->raio = 15.0f;
    } else {
        e->raio = 8.0f;
    }

    switch (tipo) {
        case ENTIDADE_JOGADOR:
            e->vida = 100;
            e->cor = BLUE;
            break;
        case ENTIDADE_INIMIGO:
            e->vida = 40;
            e->cor = MAROON;
            e->extra.dano = GetRandomValue(5, 15);
            break;
        case ENTIDADE_ITEM:
            e->vida = 1;
            e->cor = GOLD;
            e->extra.valor = GetRandomValue(5, 20);
            break;
    }

    return e;
}

void adicionarEntidade(Entidade *e) {
    if (e == NULL) return;
    if (totalEntidades < MAX_ENTIDADES) {
        vetorEntidades[totalEntidades] = e;
        totalEntidades++;
    } else {
        free(e);
    }
}

void removerEntidade(int indice) {
    if (indice < 0 || indice >= totalEntidades) return;

    free(vetorEntidades[indice]);
    vetorEntidades[indice] = vetorEntidades[totalEntidades - 1];
    totalEntidades--;
}

void ordenarPorDistancia(void) {
    if (totalEntidades <= 2) return;

    Entidade *jogador = vetorEntidades[0];
    int indiceMaisProximo = 1;

    float dx0 = vetorEntidades[1]->pos.x - jogador->pos.x;
    float dy0 = vetorEntidades[1]->pos.y - jogador->pos.y;
    float menorDistancia = dx0 * dx0 + dy0 * dy0;

    for (int i = 2; i < totalEntidades; i++) {
        float dx = vetorEntidades[i]->pos.x - jogador->pos.x;
        float dy = vetorEntidades[i]->pos.y - jogador->pos.y;
        float dist = dx * dx + dy * dy;

        if (dist < menorDistancia) {
            menorDistancia = dist;
            indiceMaisProximo = i;
        }
    }

    if (indiceMaisProximo != 1) {
        Entidade *tmp = vetorEntidades[1];
        vetorEntidades[1] = vetorEntidades[indiceMaisProximo];
        vetorEntidades[indiceMaisProximo] = tmp;
    }
}

void desenharEntidade(Entidade *e) {
    if (e == NULL) return;
    DrawCircleV(e->pos, e->raio, e->cor);
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 5");
    SetTargetFPS(60);

    adicionarEntidade(criarEntidade(ENTIDADE_JOGADOR, (Vector2){ LARGURA_JANELA / 2.0f, ALTURA_JANELA / 2.0f }));

    for (int i = 0; i < 4; i++) {
        Vector2 pos = { (float)GetRandomValue(60, LARGURA_JANELA - 60), (float)GetRandomValue(60, ALTURA_JANELA - 180) };
        adicionarEntidade(criarEntidade(ENTIDADE_INIMIGO, pos));
    }

    for (int i = 0; i < 5; i++) {
        Vector2 pos = { (float)GetRandomValue(40, LARGURA_JANELA - 40), (float)GetRandomValue(40, ALTURA_JANELA - 150) };
        adicionarEntidade(criarEntidade(ENTIDADE_ITEM, pos));
    }

    while (!WindowShouldClose()) {
        Entidade *jogador = vetorEntidades[0];

        if (IsKeyDown(KEY_RIGHT)) jogador->pos.x += 4.0f;
        if (IsKeyDown(KEY_LEFT))  jogador->pos.x -= 4.0f;
        if (IsKeyDown(KEY_UP))    jogador->pos.y -= 4.0f;
        if (IsKeyDown(KEY_DOWN))  jogador->pos.y += 4.0f;

        if (IsKeyPressed(KEY_N)) {
            if (totalEntidades < MAX_ENTIDADES) {
                Vector2 pos = { (float)GetRandomValue(40, LARGURA_JANELA - 40), (float)GetRandomValue(40, ALTURA_JANELA - 150) };
                adicionarEntidade(criarEntidade(ENTIDADE_ITEM, pos));
            }
        }

        for (int i = 1; i < totalEntidades; i++) {
            if (vetorEntidades[i]->tipo == ENTIDADE_ITEM) {
                float dx = vetorEntidades[i]->pos.x - jogador->pos.x;
                float dy = vetorEntidades[i]->pos.y - jogador->pos.y;
                float distSqr = dx * dx + dy * dy;
                float somaRaios = (jogador->raio + vetorEntidades[i]->raio) * (jogador->raio + vetorEntidades[i]->raio);

                if (distSqr <= somaRaios) {
                    jogador->vida += vetorEntidades[i]->extra.valor;
                    removerEntidade(i);
                    i--;
                }
            }
        }

        if (IsKeyPressed(KEY_SPACE) && totalEntidades > 1) {
            Entidade *alvo = vetorEntidades[1];
            if (alvo->tipo == ENTIDADE_INIMIGO) {
                alvo->vida -= DANO_TIRO;
                if (alvo->vida <= 0) {
                    removerEntidade(1);
                }
            }
        }

        ordenarPorDistancia();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < totalEntidades; i++) {
            desenharEntidade(vetorEntidades[i]);
        }

        DrawText(TextFormat("Vida: %d", jogador->vida), 20, 20, 20, BLACK);

        EndDrawing();
    }

    for (int i = 0; i < totalEntidades; i++) {
        free(vetorEntidades[i]);
    }

    CloseWindow();
    return 0;
}