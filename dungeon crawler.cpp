#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define MENU 0
#define VILA 1
#define FASE1 2
#define FASE2 3
#define FASE3 4
#define GAME_OVER 5
#define VITORIA 6

typedef struct {
    int x, y;
    char dir; 
    int vidas;
    int arma; 
    int chaves;
} Jogador;

char mapa[25][25];
int linhas, colunas;
int estado_atual = MENU;
Jogador j;
int turnos_boss = 0;

void limpar_tela() {
    system("clear || cls");
}

void inicializar_jogador() {
    j.vidas = 3;
    j.arma = 0;
    j.chaves = 0;
    j.dir = 'A'; 
}


void carregar_fase(int fase) {
    int i, k;
    j.chaves = 0; 
    
    
    for(i = 0; i < 25; i++) {
        for(k = 0; k < 25; k++) {
            mapa[i][k] = '*';
        }
    }

    if (fase == VILA) {
        linhas = 10; colunas = 10;
        for(i = 1; i < 9; i++) {
            for(k = 1; k < 9; k++) mapa[i][k] = ' ';
        }
        mapa[2][3] = 'N'; 
        mapa[8][7] = 'L';
        j.x = 2; j.y = 5; j.dir = 'A';
    } 
    else if (fase == FASE1) {
        linhas = 10; colunas = 10;
        for(i = 1; i < 9; i++) {
            for(k = 1; k < 9; k++) mapa[i][k] = ' ';
        }
       
        for(k = 2; k <= 9; k++)  mapa[2][k] = '*';
        for(k = 0; k <= 7; k++)  mapa[4][k] = '*';
        for(k = 2; k <= 9; k++)  mapa[6][k] = '*';

        mapa[1][5] = 'k'; 
        mapa[1][8] = '@'; 
        mapa[6][1] = 'D'; 
        mapa[7][1] = 'L'; 
        j.x = 2; j.y = 3; j.dir = 'A';
    }
    else if (fase == FASE2) { 
        linhas = 15; colunas = 15;
        for(i = 1; i < 14; i++) {
            for(k = 1; k < 14; k++) mapa[i][k] = ' ';
        }
       
        for(i = 1; i <= 13; i++) mapa[i][5] = '*';
        mapa[3][5] = 'D'; 
        
        for(i = 1; i <= 13; i++) mapa[i][10] = '*';
        mapa[8][10] = 'D'; 
        
      
        for(k = 6; k <= 9; k++) mapa[7][k] = '#';
        
        mapa[1][1] = '@';   
        mapa[9][9] = '@';   
        mapa[2][8] = 'O';   
        mapa[5][7] = 'X';   
        mapa[12][12] = 'L'; 
        
        j.x = 2; j.y = 2; j.dir = 'A';
    }
    else if (fase == FASE3) { 
        linhas = 25; colunas = 25;
        for(i = 1; i < 24; i++) {
            for(k = 1; k < 24; k++) mapa[i][k] = ' ';
        }
      
        for(k = 1; k < 24; k++) mapa[6][k] = '*';
        mapa[6][5] = 'D'; 
        
        for(k = 1; k < 24; k++) mapa[12][k] = '*';
        mapa[12][12] = 'D'; 
        
        for(k = 1; k < 24; k++) mapa[18][k] = '*';
        mapa[18][18] = 'D'; 

      
        mapa[1][2] = '@'; mapa[8][22] = '@'; mapa[14][2] = '@'; 
        mapa[3][10] = 'k'; mapa[3][11] = 'k';                     
        mapa[8][10] = 'X';                                        
        mapa[9][15] = 'Y';                                       
        mapa[15][12] = 'Z';                                       
        mapa[21][12] = 'L';                                      
        
        j.x = 2; j.y = 2; j.dir = 'A';
    }
}

void imprimir_estado() {
    limpar_tela();
    printf("--- DUNGEON CRAWLER ---\n");
    printf("Vidas: %d | Chaves: %d | Arma: ", j.vidas, j.chaves);
    if(j.arma == 0) printf("Nenhuma (Visita o NPC)\n");
    else if(j.arma == 1) printf("Espada\n");
    else if(j.arma == 2) printf("Arco e Flecha\n");
    else if(j.arma == 3) printf("Cajado\n");

    for (int i = 0; i < linhas; i++) {
        for (int k = 0; k < colunas; k++) {
            if (i == j.y && k == j.x) {
                printf("%c", j.dir); 
            } else {
                printf("%c", mapa[i][k]);
            }
        }
        printf("\n");
    }
    printf("\nComandos: W/A/S/D + ENTER (Mover) | i + ENTER (Interagir) | o + ENTER (Atacar)\n");
}

void atacar() {
    if (j.arma == 0) return;
    int dx = 0, dy = 0;
    if (j.dir == 'A') dy = -1;
    if (j.dir == 'V') dy = 1;
    if (j.dir == '<') dx = -1;
    if (j.dir == '>') dx = 1;

   
    if (j.arma == 1) { 
        int start_f = (j.dir == 'A' || j.dir == '<') ? -2 : 1;
        int end_f = (j.dir == 'A' || j.dir == '<') ? -1 : 2;
        
        for(int f = start_f; f <= end_f; f++) {
            for(int w = -1; w <= 1; w++) {
                int nx = (j.dir == 'A' || j.dir == 'V') ? j.x + w : j.x + f;
                int ny = (j.dir == 'A' || j.dir == 'V') ? j.y + f : j.y + w;
                if (nx >= 0 && nx < colunas && ny >= 0 && ny < linhas) {
                    if (mapa[ny][nx] == 'Z') estado_atual = VITORIA; 
                    if (mapa[ny][nx] == 'X' || mapa[ny][nx] == 'Y' || mapa[ny][nx] == 'Z' || mapa[ny][nx] == 'k') mapa[ny][nx] = ' ';
                }
            }
        }
    }
    else if (j.arma == 2) { 
        for (int i = 1; i <= 4; i++) {
            int nx = j.x + dx * i; int ny = j.y + dy * i;
            if (nx >= 0 && nx < colunas && ny >= 0 && ny < linhas) {
                if (mapa[ny][nx] == '*') break; 
                if (mapa[ny][nx] == 'Z') estado_atual = VITORIA;
                if (mapa[ny][nx] == 'X' || mapa[ny][nx] == 'Y' || mapa[ny][nx] == 'Z' || mapa[ny][nx] == 'k') mapa[ny][nx] = ' ';
            }
        }
    }
    else if (j.arma == 3) { 
        for(int i = -1; i <= 1; i++) {
            for(int k = -1; k <= 1; k++) {
                if(i == 0 && k == 0) continue;
                int nx = j.x + k; int ny = j.y + i;
                if (nx >= 0 && nx < colunas && ny >= 0 && ny < linhas) {
                    if (mapa[ny][nx] == 'Z') estado_atual = VITORIA;
                    if (mapa[ny][nx] == 'X' || mapa[ny][nx] == 'Y' || mapa[ny][nx] == 'Z' || mapa[ny][nx] == 'k') mapa[ny][nx] = ' ';
                }
            }
        }
    }
}

void interagir() {
    int dx = 0, dy = 0;
    if (j.dir == 'A') dy = -1;
    if (j.dir == 'V') dy = 1;
    if (j.dir == '<') dx = -1;
    if (j.dir == '>') dx = 1;

    int nx = j.x + dx; int ny = j.y + dy;

    if (nx >= 0 && nx < colunas && ny >= 0 && ny < linhas) {
        char alvo = mapa[ny][nx];
        if (alvo == 'N') { 
            printf("\nNPC: Escolha sua arma!\n1-Espada | 2-Arco e Flecha | 3-Cajado\nOpcao: ");
            int esc;
            if (scanf("%d", &esc) == 1 && esc >= 1 && esc <= 3) j.arma = esc;
        } else if (alvo == '@') { 
            j.chaves++;
            mapa[ny][nx] = ' ';
        } else if (alvo == 'D') { 
            if (j.chaves > 0) {
                j.chaves--;
                mapa[ny][nx] = '='; 
            }
        } else if (alvo == 'O') { 
            mapa[ny][nx] = ' '; 
            for(int r = 0; r < linhas; r++) {
                for(int c = 0; c < colunas; c++) {
                    if(mapa[r][c] == '#') mapa[r][c] = ' ';
                }
            }
        }
    }
}

void atualizar_monstros() {
    int i, k;
    char mapa_temp[25][25];
    for(i=0; i<linhas; i++) for(k=0; k<colunas; k++) mapa_temp[i][k] = mapa[i][k];

    for (i = 0; i < linhas; i++) {
        for (k = 0; k < colunas; k++) {
            if (mapa_temp[i][k] == 'X') {
                int r = rand() % 4;
                int nx = k, ny = i;
                if(r==0) ny--; else if(r==1) ny++; else if(r==2) nx--; else nx++;
                if (nx >= 0 && nx < colunas && ny >= 0 && ny < linhas && mapa[ny][nx] == ' ') {
                    mapa[i][k] = ' '; mapa[ny][nx] = 'X';
                }
            }
            else if (mapa_temp[i][k] == 'Y') {
                int nx = k, ny = i;
                if (j.x > k) nx++; else if (j.x < k) nx--;
                else if (j.y > i) ny++; else if (j.y < i) ny--;
                if (nx >= 0 && nx < colunas && ny >= 0 && ny < linhas && (mapa[ny][nx] == ' ' || (nx == j.x && ny == j.y))) {
                    mapa[i][k] = ' ';
                    if(mapa[ny][nx] == ' ') mapa[ny][nx] = 'Y';
                }
            }
            else if (mapa_temp[i][k] == 'Z') { 
                turnos_boss++;
                if (turnos_boss % 2 == 0) { 
                    int nx = k, ny = i;
                    if (j.x > k) nx++; else if (j.x < k) nx--;
                    else if (j.y > i) ny++; else if (j.y < i) ny--;
                    if (nx >= 0 && nx < colunas && ny >= 0 && ny < linhas && (mapa[ny][nx] == ' ' || (nx == j.x && ny == j.y))) {
                        mapa[i][k] = '#'; 
                        if(mapa[ny][nx] == ' ') mapa[ny][nx] = 'Z';
                    }
                }
            }
        }
    }
}

void verificar_colisoes() {
   
    if (mapa[j.y][j.x] == '#' || mapa[j.y][j.x] == 'X' || mapa[j.y][j.x] == 'Y' || mapa[j.y][j.x] == 'Z') {
        j.vidas--;
        if (j.vidas <= 0) estado_atual = GAME_OVER;
        else carregar_fase(estado_atual); 
    }
    
    if (mapa[j.y][j.x] == 'L') {
        if(estado_atual != FASE3) {
            estado_atual++;
            carregar_fase(estado_atual);
        }
    }
}

void loop_jogo() {
    char comando;
    while (estado_atual >= VILA && estado_atual <= FASE3) {
        imprimir_estado();
        if (scanf(" %c", &comando) != 1) break;

        int nx = j.x, ny = j.y;
        int acao_valida = 0;

       
        if (comando == 'w' || comando == 'W') { ny--; j.dir = 'A'; acao_valida = 1; }
        else if (comando == 's' || comando == 'S') { ny++; j.dir = 'V'; acao_valida = 1; }
        else if (comando == 'a' || comando == 'A') { nx--; j.dir = '<'; acao_valida = 1; }
        else if (comando == 'd' || comando == 'D') { nx++; j.dir = '>'; acao_valida = 1; }
        else if (comando == 'i' || comando == 'I') { interagir(); acao_valida = 1; }
        else if (comando == 'o' || comando == 'O') { atacar(); acao_valida = 1; }

        if (acao_valida) {
           
            if (comando == 'w' || comando == 'a' || comando == 's' || comando == 'd' ||
                comando == 'W' || comando == 'A' || comando == 'S' || comando == 'D') {
                if (nx >= 0 && nx < colunas && ny >= 0 && ny < linhas) {
                    char celula = mapa[ny][nx];
                    if (celula == ' ' || celula == '=' || celula == 'L' || celula == '#' || celula == 'X' || celula == 'Y' || celula == 'Z') {
                        j.x = nx; j.y = ny;
                    }
                }
            }
            
            verificar_colisoes(); 
           
            if (estado_atual >= VILA && estado_atual <= FASE3) {
                atualizar_monstros();
                verificar_colisoes(); 
            }
        }
    }
}

int main() {
    srand(time(NULL));
    int opcao;

    while (1) {
        limpar_tela();
        if (estado_atual == MENU) { 
            printf("=== DUNGEON CRAWLER ===\n");
            printf("1. Jogar\n2. Tutorial\n3. Sair\nEscolha: ");
            if (scanf("%d", &opcao) != 1) {
                while(getchar() != '\n'); 
                continue;
            }

            if (opcao == 1) {
                inicializar_jogador();
                estado_atual = VILA;
                carregar_fase(VILA);
                loop_jogo();
            } else if (opcao == 2) {
                limpar_tela();
                printf("=== HISTORIA E TUTORIAL ===\n");
                printf("voce entrou na Masmorra Proibida em busca do amuleto perdido.\n\n");
                printf("Sinalizacao do Jogo:\n");
                printf(" A, V, <, > : O teu Personagem\n");
                printf(" * : Parede                 |  # : Espinho Mortal\n");
                printf(" k : Caixa Destruivel       |  O : Botao Secreto\n");
                printf(" D : Porta Trancada         |  = : Porta Aberta\n");
                printf(" @ : Chave de Bronze        |  L : Escada de Acesso\n\n");
                printf("Controles: Digite o comando e pressione ENTER.\n");
                printf(" W/A/S/D - Movimentacao e Direcao\n");
                printf(" i - Interagir com objetos\n");
                printf(" o - atacar\n\n");
                printf("Digita 0 para voltar ao menu: ");
                scanf("%d", &opcao);
            } else if (opcao == 3) {
                limpar_tela();
                printf("=== CREDITOS DO JOGO ===\n");
                printf("Desenvolvido por: Abner Italo, Jean Yves e Bernardo Emanuel\n");
                printf("Obrigado por jogar!\n");
                break;
            }
        } 
        else if (estado_atual == GAME_OVER) { 
            printf("\n--- GAME OVER ---\nAlcancaste o teu fim. As tuas vidas esgotaram-se.\nDigita 0 para voltar ao menu: ");
            scanf("%d", &opcao);
            estado_atual = MENU;
        } 
        else if (estado_atual == VITORIA) { 
            limpar_tela();
            printf("=== TELA DE VITORIA ===\n");
            printf("Parabens! voce ganhou\n");
            printf("Com o amuleto em maos, voce retorna a Vila como um verdadeiro heroi.\n");
            printf("A paz foi restaurada no reino subterraneo.\n\n");
            printf("Digita 0 para voltar ao menu principal: ");
            scanf("%d", &opcao);
            estado_atual = MENU;
        }
    }
    return 0;
}
