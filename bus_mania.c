#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifdef _WIN32
    #include<windows.h>
#endif
typedef struct{
    int pontos;
    char nome[20];
    char mododejogo[20];
} usuario;
typedef struct{
    int qnt_bus;
    int le_lin;
    int max_col;
    char lixo[10];
    char mapa[10][10];
    char tipo_bus[10];
    char final;
} info;
usuario jogador;
int menu;
void limp_tela(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void logo2(){
    printf("——————————————————\n");
    printf("| Jogo Bus Mania |\n");
    printf("——————————————————\n");
}
//função faz a fila ser testada toda vez que o onibus muda.
void verifica_fila(info *fase, char fila[5], char asentos[3],int *cont_fila, int *cont_asento, int *cont, usuario *jogador){
    int mudou = 1;
    while (mudou && *cont_fila > 0){
        mudou = 0;
        for (int i = 0; i < *cont_fila; i++){
            if (fila[i] == fase->tipo_bus[*cont]){
                asentos[*cont_asento] = fila[i];
                (*cont_asento)++;
                for (int j = i; j < *cont_fila - 1; j++)
                    fila[j] = fila[j + 1];
                (*cont_fila)--;
                fila[*cont_fila] = '-';
                if (*cont_asento == 3){
                    jogador->pontos += 15;
                    (*cont)++;
                    asentos[0] = asentos[1] = asentos[2] = ' ';
                    *cont_asento = 0;
                }
                mudou = 1;
            }
        }
    }
}
//função que carrega carrega o layout da fase, logica dos movimentos, telas de vitoria, derrota e de confirmação para a proxima fase e gravação dos dados do jogador
void jogo(){
    limp_tela();
    logo2();
    jogador.pontos = 0;
    info fase;
    FILE*entrada = fopen(jogador.mododejogo, "r");
    if (entrada == NULL){
        printf("coloque o arquivo '%s' na mesma pasta do jogo!\nPrecione ENTER para voltar...", jogador.mododejogo);
        getchar(); getchar();
        return;
    }
    int cont_fase = 1;
    while (!feof(entrada)){
        fscanf(entrada, "%d", &fase.qnt_bus);
        fscanf(entrada, "%d", &fase.le_lin);
        fscanf(entrada, "%d", &fase.max_col);
        fscanf(entrada, "%s", fase.lixo);
        for (int i = 0; i < fase.le_lin; i++) {
            for (int j = 0; j < 10; j++) {
                fscanf(entrada, " %c", &fase.mapa[i][j]);
            }
        }
        for (int i = 0; i < fase.qnt_bus; i++){
            fscanf(entrada, " %c", &fase.tipo_bus[i]);
        }
        fase.tipo_bus[fase.qnt_bus] = '\0';
        fscanf(entrada, " %c", &fase.final);
        char fila[5] = {'-', '-', '-', '-', '-'};
        char asentos[3] = {' ', ' ', ' '};
        int cont = 0 ,cont_fila = 0, cont_asento = 0;
        int ativo = 1;
        while(ativo == 1){
            limp_tela();
            logo2();
            int x, y;
            printf("               FASE:%d\n", cont_fase);
            printf("                                Pontuação: %d\n", jogador.pontos);
            if (cont == fase.qnt_bus - 1){
                printf("Prox\nBus\n");
                printf("        +————O————————O——+\n");
                printf("        |  —    —    —   |—+\n");
                printf("        | |%c   |%c   |%c   |%c|\n", asentos[0], asentos[1], asentos[2], fase.tipo_bus[cont]);
                printf("        |  —    —    —   |—+\n");
                printf("        +————O———==———O——+\n\n");
            }
            else{
                printf("Prox\nBus\n");
                printf("        +————O————————O——+\n");
                printf("|—+     |  —    —    —   |—+\n");
                printf("|%c|     | |%c   |%c   |%c   |%c|\n",fase.tipo_bus[cont + 1], asentos[0], asentos[1], asentos[2], fase.tipo_bus[cont]);
                printf("|—+     |  —    —    —   |—+\n");
                printf("        +————O———==———O——+\n\n");
            }
            printf("              ");
            for (int i = 0; i < 5; i++){
                printf("%c ", fila[i]);
                if (fila[i] ==  5){
                    printf("%c\n", fila[i]);
                }
            }
            printf("\n\n");
            for (int i = 0; i < fase.le_lin; i++){
                printf("      %d ", i + 1);
                for (int j = 0; j < 10; j++){
                    printf("|%c", fase.mapa[i][j]);
                }
                printf("|\n");
            }
            printf("         ");
            for (int j = 0; j < 10; j++) {
                printf("%d ", j + 1);
            }
            printf("\n\n");
            printf("Digite as coordenadas:");
            scanf("%d %d", &x, &y);
            if (fase.mapa[x - 1][y - 1] == '_'|| x > fase.le_lin || x < 1 || y > 10 || y < 1 ){
                printf("Movimento Invalido Tente Novamente\nTecle ENTER\n");
                getchar(); getchar();
                continue;
            }
            if (fase.mapa[x - 1][y - 1] == ' '){
                printf("Você ja fez esse movimento\nTecle ENTER\n");
                getchar(); getchar();
                continue;
            }
            if(x > 1){
                if(fase.mapa[x - 2][y - 1] != ' ' && (y == 1 || fase.mapa[x - 1][y - 2] != ' ') && (y == 10 || fase.mapa[x - 1][y] != ' ')){
                printf("Movimento Bloqueado\nTecle ENTER\n");
                getchar(); getchar();
                continue;
                }
            }
            if (fase.mapa[x - 1][y - 1] == fase.tipo_bus[cont] ){
                asentos[cont_asento] = fase.mapa[x - 1][y - 1];
                fase.mapa[x - 1][y - 1] = ' ';
                cont_asento++;
                if (cont_asento == 3){
                    jogador.pontos+=15;
                    cont++;
                    asentos[0] = ' ', asentos[1] = ' ', asentos[2] = ' ';
                    cont_asento = 0;
                }
            }
            else{
                fila[cont_fila] = fase.mapa[x - 1][y - 1];
                fase.mapa[x - 1][y - 1] = ' ';
                cont_fila++;
            }
            if (cont_fila == 5){
                printf("————————————————————————————————————————————————\n");
                printf("|Com esse movimento você lotou a fila de espera|\n");
                printf("|                você perdeu😔😭               |\n");
                printf("|                                              |\n");
                printf("|                 Aperte ENTER                 |\n");
                printf("————————————————————————————————————————————————\n");
                FILE * fd = fopen("ranking.bin", "ab");
                if (fd == NULL){
                    printf("Erro ao cirar arquivo\n");
                    getchar(); getchar();
                    return;
                }
                fwrite(&jogador, sizeof(usuario), 1, fd);
                fclose(fd);
                getchar(); getchar();
                return;
            }
            verifica_fila(&fase, fila, asentos, &cont_fila, &cont_asento, &cont, &jogador);
            if (cont == fase.qnt_bus){
                cont_fase++;
                jogador.pontos+=100;
                limp_tela();
                logo2();
                printf("                                Pontuação: %d\n\n", jogador.pontos);
                if (fase.final == 'F' || fase.final == 'f'){
                    char prox;
                    printf("————————————————————————————————————————————————\n");
                    printf("|      Parabens!!! Você completou a fase       |\n");
                    printf("|                                              |\n");
                    printf("|       Continuar para a proxima? (S/N)        |\n");
                    printf("————————————————————————————————————————————————\n");
                    scanf(" %c", &prox);
                    if (prox == 'N' || prox == 'n'){
                        fclose(entrada);
                        FILE * fd = fopen("ranking.bin", "ab");
                        if (fd == NULL){
                            printf("Erro ao cirar arquivo\n");
                            getchar(); getchar();
                            return;
                        }
                        fwrite(&jogador, sizeof(usuario), 1, fd);
                        fclose(fd);
                        ativo = 0;
                        return;
                    }
                    ativo = 0;
                }
                else if (fase.final == 'U' || fase.final == 'u'){
                    limp_tela();
                    logo2();
                    printf("————————————————————————————————————————————————\n");
                    printf("|     Parabens!!! Você completou o jogo!!      |\n");
                    printf("|              Sua pontuação: %d              |\n", jogador.pontos);
                    printf("|                                              |\n");
                    printf("|                 Aperte ENTER                 |\n");
                    printf("————————————————————————————————————————————————\n");
                    getchar(); getchar();
                    fclose(entrada);
                    FILE * fd = fopen("ranking.bin", "a+b");
                    if (fd == NULL){
                        printf("Erro ao cirar arquivo ou Abrir aqruivo\n");
                        getchar(); getchar();
                        return;
                    }
                    fwrite(&jogador, sizeof(usuario), 1, fd);
                    fclose(fd);
                    return;
                }
            }
        }
    }
    fclose(entrada);
}
//menu de edição de fase
void editor(){
    limp_tela();
    info edit[20];
    int cont = 0;
    char opcao;
    int maisfase = 1;
    char nome_arq[20];
    printf("———————————————————\n");
    printf("| Editor de fases |\n");
    printf("———————————————————\n");
    printf("\nDe nome para o seu arquivo de entrada: ");
    scanf("%s", nome_arq);
    while (maisfase && cont<20){ 
        printf("\nQuantidade de ônibus: ");
        scanf("%d", &edit[cont].qnt_bus);
        printf("\nTipo de Ônibus: ");
        for (int i = 0; i < edit[cont].qnt_bus; i++){
            scanf(" %c", &edit[cont].tipo_bus[i]);
        }
        printf("\nQuantidade de linhas na matriz: ");
        scanf("%d", &edit[cont].le_lin);
        printf("\nNumero de colunas ocupadas: ");
        scanf("%d", &edit[cont].max_col);
        printf("\nDigite as posições do grid");
        printf("\n1234567890\n");
        for (int i = 0; i < edit[cont].le_lin; i++){
            for (int j = 0; j < 10; j++){
                scanf(" %c", &edit[cont].mapa[i][j]);
            }
        }
        printf("\nDeseja cirar mais uma fase? (S/N)");
        scanf(" %c", &opcao);
        if (opcao == 'S' || opcao == 's'){
            edit[cont].final = 'F';
            cont++;
        }
        else{
            edit[cont].final = 'U';
            maisfase = 0;
        }
    }
    FILE* edicao = fopen(nome_arq, "w");
    if (edicao == NULL){
        fopen(nome_arq, "w");
    }
    for (int i = 0; i <= cont; i++){
        fprintf(edicao, "%d\n", edit[i].qnt_bus);
        fprintf(edicao, "%d\n", edit[i].le_lin);
        fprintf(edicao, "%d\n", edit[i].max_col);
        fprintf(edicao, "1234567890\n");
        for (int l = 0; l < edit[i].le_lin; l++){
            for (int j = 0; j < 10; j++){
                fprintf(edicao, "%c", edit[i].mapa[l][j]);
            }
            fprintf(edicao, "\n");
        }
        for (int j = 0; j < edit[i].qnt_bus; j++){
            fprintf(edicao, "%c", edit[i].tipo_bus[j]);
        }
        fprintf(edicao, "\n");
        fprintf(edicao, "%c\n", edit[i].final);
    }
        fclose(edicao);
    printf("\nFase Criada com Sucesso!\nAperte ENTER para voltar...");
    getchar();getchar();
    return;
}
//menu de dificulade de 1 a 3 são arquivos pre montados a 4 e para arquivos feitos na função editor
void dificulade(){
    int modo_ativo = 1, sele;
    while (modo_ativo){
    limp_tela();
    printf("———————————————————————————————————\n");
    printf("|  Dificuldade / Arquivo pessoal  |\n");
    printf("———————————————————————————————————\n");
    printf("1 - modo Facil\n");
    printf("2 - Modo Normal\n");
    printf("3 - Modo Difícil\n");
    printf("4 - Fases feitas por você\n");
    printf("5 - Voltar para as configurações\n");
    printf("\nDigite a opcão desejada:");
    scanf("%d", &sele);
        switch (sele){
        case 1:{
            FILE* modo = fopen("facil.txt", "r");
            if (modo == NULL){
                printf("\nColoque o arquivo 'facil.txt', na pasta do jogo");
                getchar(); getchar();
                break;
            }
            fclose(modo);
            strcpy(jogador.mododejogo, "facil.txt");
            printf("\nModo Facil seliconado com secesso!!\nAperte ENTER para voltar...");
            getchar(); getchar();
            modo_ativo = 0;
            break;
        }
        case 2:{
            FILE* modo = fopen("entrada.txt", "r");
            if (modo == NULL){
                printf("\nColoque o arquivo 'entrada.txt', na pasta do jogo");
                getchar();
                break;
            }
            fclose(modo);
            strcpy(jogador.mododejogo, "entrada.txt");
            printf("\nModo Normal seliconado com secesso!!\nAperte ENTER para voltar...");
            getchar(); getchar();
            modo_ativo = 0;
            break;
        }
        case 3:{
            FILE* modo = fopen("dificil.txt", "r");
            if (modo == NULL){
                printf("\nColoque o arquivo 'dificil.txt', na pasta do jogo");
                getchar(); getchar();
                break;
            }
            fclose(modo);
            strcpy(jogador.mododejogo, "dificil.txt");
            printf("\nModo Difícil seliconado com secesso!!\nAperte ENTER para voltar...");
            getchar(); getchar();
            modo_ativo = 0;
            break;
        }
        case 4:{
            printf("\nDigite o arquivo que você quer carregar:");
            scanf("%s", jogador.mododejogo);
            FILE* modo = fopen(jogador.mododejogo, "r");
            if (modo == NULL){
                printf("Fase não encontrada\nAperte ENTER para voltar...");
                getchar(); getchar();
                break;
            }
            fclose(modo);
            printf("\nFase(s) carregada(S) com sucesso!!\nAperte ENTER para voltar...");
            getchar(); getchar();
            modo_ativo = 0;
            break;
        }
        case 5:
            modo_ativo = 0;
            getchar();
            return;
        default:
            modo_ativo = 0;
            getchar();
            return;
        }   
    }
}
//tela geral de configurações com a junção das duas funcoes acima junto com o reset do ranking
void config(){
    int ativo = 1;
    while (ativo){
        limp_tela();
        printf("———————————————————\n");
        printf("|  Configurações  |\n");
        printf("———————————————————\n");
        printf("1 - Zerar Ranking\n");
        printf("2 - Modo Dificulade\n");
        printf("3 - Editor de Fases\n");
        printf("4 - Voltar ao Menu Principal\n");
        printf("\nDigite a opção desejada: ");
        int sele;
        scanf("%d", &sele);
        switch (sele){
            case 1:
                limp_tela();
                logo2();
                printf("\n\nDeseja Limpar o Ranking? (S/N) ");
                char confirma;
                scanf(" %c", &confirma);
                if (confirma == 's' || confirma == 'S'){
                    FILE* confere = fopen("ranking.bin", "rb");
                    if (confere == NULL){
                        printf("Não ha registro de ranking ainda!\nAperte ENTER para voltar...");
                        getchar();getchar();
                        return;
                    }
                    fclose(confere);
                    FILE* deleta = fopen("ranking.bin", "wb");
                    fclose(deleta);
                    printf("\n\nRanking Zerado com Sucesso!\nPrecione ENTER para voltar...");
                    getchar(); getchar();
                    return;
                }
                getchar(); getchar();
                break;
            case 2:
                dificulade();
                break;
            case 3:
                editor();
                break;
            case 4:
                ativo = 0;
                getchar();
                return;
                break;
            default:
                ativo = 0;
                getchar();
                return;
        }
    }
}
//o cunjunto principal onde tudo começou e a porta de entrada para todas as outras funcionalidades do codigo
int main(){
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #endif
    limp_tela();
    printf("——————————————————————————————————————\n");
    printf("| Bem vindo(a)! ao Jogo Bus Mania!!! |\n");
    printf("——————————————————————————————————————\n");
    printf("\n\n\nDigite seu nickname: ");
    scanf("%19[^\n]", jogador.nome);
    strcpy(jogador.mododejogo, "entrada.txt");
    int rodando = 1;
    while (rodando){
        limp_tela();
        logo2();
        printf("1 - Jogar\n");
        printf("2 - Configurações\n");
        printf("3 - instruções\n");
        printf("4 - Ranking\n");
        printf("5 - sair\n\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &menu);
        switch (menu){
        case 1:
            jogo();
            break;
        case 2:
            config();
            break;
        case 3:
            limp_tela();
            printf("—————————————————————————————————————————————————————————————————\n");
            printf("|                 INSTRUÇÕES DE JOGO: BUS MANIA                 |\n");
            printf("—————————————————————————————————————————————————————————————————\n");
            printf("| 1. OBJETIVO: Mover os passageiros do grid para os onibus.     |\n");
            printf("| 2. COMANDOS: Digite a Linha e a Coluna (ex: '1 5').           |\n");
            printf("| 3. BLOQUEIO: Se você selecionar uma posição e ele não for     |\n");
            printf("|    o primeiro ele não pode se mover                           |\n");
            printf("| 4. FILA DE ESPERA: Tem 5 vagas. Se lotar, é FIM DE JOGO!      |\n");
            printf("—————————————————————————————————————————————————————————————————\n\n");
            printf("———————————————————————————————————————————————————————————————————\n");
            printf("|                   COMO USAR O EDITOR DE FASES                   |\n");
            printf("———————————————————————————————————————————————————————————————————\n");
            printf("| A. NOME DO ARQUIVO: Escolha um nome com '.txt' (ex: mapa1.txt). |\n");
            printf("| B. CONFIGURAÇÃO: Defina a quantidade de onibus, os tipos        |\n");
            printf("|    de passageiros (ex: A, *, @) e o tamanho da matriz.          |\n");
            printf("| C. O GRID (10 Colunas): Digite os caracteres linha por linha.   |\n");
            printf("|    Use '_' (underline) para preencher todo o grid               |\n");
            printf("|    e letras ou caracteres para os passageiros. Preencha         |\n");
            printf("|    exatamente as 10 colunas por linha.                          |\n");
            printf("| D. COMO JOGAR SUA FASE: Vá em Configurações -> Dificuldade ->   |\n");
            printf("|    Opção 4, digite o nome do arquivo salvo e divirta-se!        |\n");
            printf("———————————————————————————————————————————————————————————————————\n");
            printf("\nAperte ENTER para voltar...");
            getchar();getchar();
            break;
        case 4:
            limp_tela();
            logo2();
            usuario aux[100];
            int qnt = 0;
            printf("\n           RANKING\n\n");
            FILE* le = fopen("ranking.bin", "rb");
            if (le == NULL){
                printf("Não ha registro de ranking ainda!\nAperte ENTER para voltar...");
                getchar();getchar();
                continue;
            }
            while(fread(&aux[qnt], sizeof(usuario), 1, le) == 1){
                qnt++;
            }
            fclose(le);
            for (int i = 0; i < qnt; i++){
                for (int j = 0; j < qnt - i - 1; j++){
                    if (aux[j].pontos < aux[j + 1].pontos){
                        usuario temp = aux[j];
                        aux[j] = aux[j + 1];
                        aux[j + 1] = temp;
                    }
                }
            }
            printf("————————————————————————————————\n");
            printf("|  Pontuação  |    Nickname    |\n");
            printf("————————————————————————————————\n");
            for (int i = 0; i < qnt; i++){
                printf("|     %-7d |     %-6s     |\n", aux[i].pontos, aux[i].nome);
            }
            printf("————————————————————————————————\n");
            printf("\n Precione ENTER para voltar...");
            getchar(); getchar();
            break;
        case 5:
            limp_tela();
            printf("Obrigado por jogar!\nAte logo!\n");
            rodando = 0;
            return 0;
            break;
        default:
            limp_tela();
            printf("Obrigado por jogar!\nAte logo!\n");
            rodando = 0;
            return 0;
            break;
        }
    }
    return 0;
}