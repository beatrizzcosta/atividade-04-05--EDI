# Algoritmos e Estruturas de Dados I - Atividades Práticas com Raylib

Repositório com as soluções das atividades práticas da disciplina de Algoritmos e Estrutura de Dados I (UFERSA)[cite: 1, 2]. As aplicações utilizam a linguagem C e a biblioteca gráfica Raylib para ilustrar o gerenciamento de memória, estruturas de dados heterogêneas e ponteiros[cite: 1, 2].

---

## 📌 Conteúdo

* **`atividade4.c`**: Demonstração de vetor de `struct` alocado em bloco contíguo de memória com `malloc`, passagem e retorno de ponteiros para `struct`, uso de `enum` (`EstadoInimigo`) para controle de ciclo de vida e busca por menor distância (`sqrtf`).
* **`atividade5.c`**: Sistema baseado em vetor de ponteiros para `struct` (`Entidade *vetorEntidades[MAX_ENTIDADES]`), alocação dinâmica individual por entidade, unificação de dados com `union` (`ExtraEntidade`) e `enum` (`TipoEntidade`), remoção $O(1)$ por troca de ponteiros e criação dinâmica de entidades em tempo de execução.

---

## 🛠️ Tecnologias e Bibliotecas

* **Linguagem C** (padrão C99/C11)
* **[Raylib](https://www.raylib.com/)** (Renderização gráfica e tratamento de entrada)
* **Biblioteca Padrão C**: `<stdlib.h>`, `<math.h>`[cite: 1, 2]

---

## ⚙️ Compilação e Execução

### Pré-requisitos
Certifique-se de ter um compilador C (`gcc` ou `clang`) e a biblioteca Raylib instalada no sistema.

### Compilar a Atividade 4
```bash
gcc atividade4.c -o atividade4 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./atividade4
