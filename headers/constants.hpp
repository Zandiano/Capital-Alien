#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "house.hpp"

const int ScreenW = 850;
const int ScreenH = 500;

const House BOARD_DATA[] = {
// Nome - Cor - Type - Aluguel - Comprar - Residencia  - Hipoteca - Dono - Hipotecada? - ResidenciasConstruidas
    {"Início", WHITE, START}, // Casa Inicial - 0
    {"Av. Sumaré", PURPLE, NORMAL, 4, 60, 50, 30}, // Casa Normal - 1
    {"Cofre", WHITE, 0, 0, M0, 0, CHEST, -1}, // Cofre - 2
    {"Praça da Sé", PURPLE, NORMAL, 2, 60, 50, 30}, // Casa Normal - 3
    {"Imposto de Renda", WHITE, TAXES}, // Taxas - 4
    {"Estação de Metrô Maracanã", WHITE, 0, 200, M0, 0, RAILROAD, -1}, // Train Station - 5
    {"Rua 25 de Março", GRAY, 0, 100, M0, 0, NORMAL, -1}, // Casa Normal - 6
    {"Chance", WHITE, 0, 0, M0, 0, QUESTION_MARK, -1}, // Chance - 7
    {"Av. São João", GRAY, 0, 100, M0, 0, NORMAL, -1}, // Casa Normal - 8
    {"Av. Paulista", GRAY, 0, 120, M0, 0, NORMAL, -1}, // Casa Normal - 9
    {"Prisão", WHITE, 0, 0, M0, 0, PRISION, -1}, // Jail - 10
    {"Av. Vieira Souto", PINK, 0, 140, M0, 0, NORMAL, -1}, // Casa Normal - 11
    {"Companhia Elétrica", WHITE, 0, 150, M0, 0, COMPANY, -1}, // Company - 12
    {"Niterói", PINK, 0, 140, M0, 0, NORMAL, -1}, // Casa Normal - 13
    {"Av. Atlântica", PINK, 0, 160, M0, 0, NORMAL, -1}, // Casa Normal - 14
    {"Estação de Metrô Carioca", WHITE, 0, 200, M0, 0, RAILROAD, -1}, // Train Station - 15
    {"Av. Presidente JK", ORANGE, 0, 180, M0, 0, NORMAL, -1}, // Casa Normal - 16
    {"Cofre", WHITE, 0, 0, M0, 0, CHEST, -1}, // Cofre - 17
    {"Av. Engenheiro Luis Carlos Berrini", ORANGE, 0, 180, M0, 0, NORMAL, -1}, // Casa Normal - 18
    {"Av. Brigadeiro Faria Lima", ORANGE, 0, 200, M0, 0, NORMAL, -1}, // Casa Normal - 19
    {"Estacionamento", WHITE, 0, 0, M0, 0, PARK, -1}, // Park - 20
    {"Ipanema", RED, 0, 220, M0, 0, NORMAL, -1}, // Casa Normal - 21
    {"Chance", WHITE, 0, 0, M0, 0, QUESTION_MARK, -1}, // Chance - 22
    {"Leblon", RED, 0, 220, M0, 0, NORMAL, -1}, // Casa Normal - 23
    {"Copacabana", RED, 0, 240, M0, 0, NORMAL, -1}, // Casa Normal - 24
    {"Estação de Metrô Consolação", WHITE, 0, 200, M0, 0, RAILROAD, -1}, // Train Station - 25
    {"Av. Cidade Jardim", YELLOW, 0, 260, M0, 0, NORMAL, -1}, // Casa Normal - 26
    {"Pacaembu", YELLOW, 0, 260, M0, 0, NORMAL, -1}, // Casa Normal - 27
    {"Companhia de Água", WHITE, 0, 150, M0, 0, COMPANY, -1}, // Company - 28
    {"Ibirapuera", YELLOW, 0, 280, M0, 0, NORMAL, -1}, // Casa Normal - 29
    {"Vá para Prisão", WHITE, 0, 0, M0, 0, TELEPORT, -1}, // Go to Jail - 30
    {"Barra da Tijuca", GREEN, 0, 300, M0, 0, NORMAL, -1}, // Casa Normal - 31
    {"Jardim Botânico", GREEN, 0, 300, M0, 0, NORMAL, -1}, // Casa Normal - 32
    {"Cofre", WHITE, 0, 0, M0, 0, CHEST, -1}, // Cofre - 33
    {"Lagoa Rodrigo Freitas", GREEN, 0, 320, M0, 0, NORMAL, -1}, // Casa Normal - 34
    {"Estação de Metrô República", WHITE, 0, 200, M0, 0, RAILROAD, -1}, // Train Station - 35
    {"Chance", WHITE, 0, 0, M0, 0, QUESTION_MARK, -1}, // Chance - 36
    {"Av. Morumbi", BLUE, 0, 350, M0, 0, NORMAL, -1}, // Casa Normal - 37
    {"Taxa de Riqueza", WHITE, 0, 0, M0, 0, TAXES, -1}, // Taxas - 38
    {"Rua Oscar Freire", BLUE, 0, 400, M0, 0, NORMAL, -1}, // Casa Normal - 39
};

const Card CARDS_CHANCE_DATA[] = {
    // STRING - BOOL - ACTION - VALUE - TARGET - FROM
    {"Avançar até o Estacionamento", true, MOVE_TO, 0, 20},
    {"Avançar até Início e receber $200", true, MOVE_AND_RECEIVE, 200, 0},
    {"Avançar até Copacabana; receber $200 se passar pelo Início", true, MOVE_RECEIVE_IF, 200, 24, 0},
    {"Avançar até Av. Presidente JK; receber $200 se passar pelo Início", true, MOVE_RECEIVE_IF, 200, 16, 0},
    {"Avançar até a ferrovia mais próxima; se tiver dono, pagar o dobro do aluguel", false, MOVE_NEAREST_RAILROAD},
    {"Pagar $100 ao banco", false, PAY_MONEY, 100},
    {"Avançar até o serviço público mais próximo; se tiver dono, pagar 10x o resultado dos dados", false, MOVE_NEAREST_COMPANY},
    {"Receber $50 do banco", true, COLLECT_MONEY, 50},
    {"Sair da prisão gratuitamente", true, GET_OUT_OF_JAIL},
    {"Voltar 3 casas", false, MOVE_BACK, 0, 3},
    {"Ir diretamente para a prisão, sem receber $200", false, MOVE_TO, 0, 10},
    {"Pagar ao banco $25 por cada casa e $100 por cada hotel", false, ESPECIAL_PAY},
    {"Pagar multa de $15", false, PAY_MONEY, 15},
    {"Ir até a Estação de Metrô Maracanã; receber $200 se passar pelo Início", true, MOVE_RECEIVE_IF, 200, 5, 0},
    {"Pagar $50 para cada outro jogador", false, PAY_EACH_PLAYER, 50},
    {"Receber $150 do banco", true, COLLECT_MONEY, 150},
}; 

const Card CARDS_CHEST_DATA[] = {
    // STRING - BOOL - ACTION - VALUE - TARGET - FROM
    {"Receber $100 por passar tempo com um vizinho idoso", true, COLLECT_MONEY, 100, -1},
    {"Receber $50 por organizar uma limpeza comunitária", true, COLLECT_MONEY, 50, -1},
    {"Receber $10 por participar de uma doação de sangue", true, COLLECT_MONEY, 10, -1},
    {"Pagar $50 por comprar algo em uma venda escolar", false, PAY_MONEY, 50, -1},
    {"Pagar $60 por fazer uma doação à caridade", false, PAY_MONEY, 60, -1},
    {"Receber $10 de cada outro jogador por organizar uma festa", true, COLLECT_FROM_EACH_PLAYER, 10, -1},
    {"Ir diretamente para a prisão", false, GO_TO_JAIL, 0, 10},
    {"Receber $20 por ajudar um vizinho com compras", true, COLLECT_MONEY, 20, -1},
    {"Receber $100 por ajudar a construir um parquinho escolar", true, COLLECT_MONEY, 100, -1},
    {"Receber $100 por participar de atividades com crianças em um hospital", true, COLLECT_MONEY, 100, -1},
    {"Pagar $100 por participar de uma arrecadação escolar", false, PAY_MONEY, 100, -1},
    {"Avançar até o Início e receber $200", true, MOVE_AND_RECEIVE, 200, 0},
    {"Receber $200 por ajudar vizinhos após uma tempestade", true, COLLECT_MONEY, 200, -1},
    {"Pagar $50 por fazer uma doação a um abrigo de animais", false, PAY_MONEY, 50, -1},
    {"Pagar $40 por cada casa e $115 por cada hotel", false, ESPECIAL_PAY, 0, -1},
    {"Receber $25 por organizar uma venda de bolos escolar", true, COLLECT_MONEY, 25, -1},
}; 

#endif