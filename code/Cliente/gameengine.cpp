#include "gameengine.h"
#include "jogadorhumano.h"
#include "jogadorbot.h"
#include <QRandomGenerator>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>

#include <QDebug>

GameEngine::GameEngine() {
    jogadores.push_back(new JogadorHumano("Você"));
    jogadores.push_back(new JogadorBot("Bot 1"));
    jogadores.push_back(new JogadorBot("Bot 2 (Dupla)"));
    jogadores.push_back(new JogadorBot("Bot 3"));
    //iniciarPartida();
}

GameEngine* GameEngine::instance = nullptr;

GameEngine* GameEngine::getInstance() {
    if (!instance) {
        instance = new GameEngine();
    }
    return instance;
}

void GameEngine::removerPrimeirosJogadores(int quantidade) {
    jogadores.erase(jogadores.begin(), jogadores.begin() + quantidade);
}

void GameEngine::iniciarPartida() {

    pontosDupla1 = 0;
    pontosDupla2 = 0;
    valorRodada = 1;
    rodadaAtual = 0;
    trucoPedido = false;
    turno = 0;
    turnoAnterior = 0;
    baralho.embaralhar();
    //distribuirCartas();
    //definirManilha();
    cartasMesa.clear();
    pontosRodadaDupla1.clear();
    pontosRodadaDupla2.clear();
    indiceJogadorVencedor = 0;
    idxAtual = 0;
}

void GameEngine::distribuirCartas() {
    for (auto& j : jogadores) {
        j->limparMao();
    }

    for (int i = 0; i < 3; ++i) {
        for (auto& j : jogadores) {
            j->receberCarta(baralho.comprarCarta());
        }
    }
}

void GameEngine::setManilha(QString valor, QString naipe) {
    manilha = baralho.setCarta(valor, naipe);
    Carta::setManilha(manilha);
}

void GameEngine::jogarCarta(int jogadorIdx, int cartaIdx) {

    if(cartaIdx != -1){
        auto c = jogadores[jogadorIdx]->jogarCarta(cartaIdx);
        cartasMesa.push_back(c);
        turno++;
        proxIdxJogador = (jogadorIdx + 1) % 4;
        qDebug() <<  jogadores[jogadorIdx]->getNome() << "jogou" << cartasMesa.back().getValor();
        cartasMesaAnimacao = cartasMesa;
    }else{
        auto c = jogadores[jogadorIdx]->jogarCarta(false);
        cartaBotJogada = c;
        cartasMesa.push_back(c);
        turno++;
        proxIdxJogador = (jogadorIdx + 1) % 4;
        cartasMesaAnimacao = cartasMesa;
        qDebug() <<  jogadores[jogadorIdx]->getNome() << "jogou" << cartasMesa.back().getValor();
    }

    // Verificar se é fim de rodada
    if (cartasMesa.size() == 4) {
        avaliarRodada();
        cartasMesa.clear();
    }

}

Carta GameEngine::botJogarCarta(int jogadorIdx){
    auto c = jogadores[jogadorIdx]->jogarCarta(true);
    return c;
}

int GameEngine::getTurnoAnterior(){
    return turnoAnterior;
}

int GameEngine::getTurno(){
    return turno;
}

bool GameEngine::getNovoJogo(){

    return novoJogo;
}

bool GameEngine::getResetButton(){

    return resetButton;
}

void GameEngine::setResetButton(bool state){
    resetButton = state;
}

int GameEngine::getIdxJogadorVencedor(){
    return indiceJogadorVencedor;
}

void GameEngine::avaliarRodada() {
    qDebug("Rodada sendo avaliada...");

    std::vector<Carta> aux = cartasMesa;
    for(int i = 0; i < int(cartasMesa.size()); ++i){
        cartasMesa[(indiceJogadorVencedor + i) % cartasMesa.size()] = aux[i];
    }

    int vencedor = 0;
    Carta maior = cartasMesa[0];

    for (int i = 0; i < int(cartasMesa.size()); ++i) {
        if (cartasMesa[i] > maior) {

            maior = cartasMesa[i];
            vencedor = i;

        }
    }

    // Salva o índice do jogador vencedor para iniciar a próxima rodada
    indiceJogadorVencedor = (turnoAnterior + vencedor) % jogadores.size();
    qDebug() << "Jogador Vencedor: " << jogadores[indiceJogadorVencedor]->getNome();

    if (vencedor % 2 == 0){
        pontosRodadaDupla1.push_back(1); // ponto para nós
        pontosRodadaDupla2.push_back(0);
    }
    else{
        pontosRodadaDupla2.push_back(1); // ponto para eles
        pontosRodadaDupla1.push_back(0);
    }

    int qtdRodadas = 0;
    int qtdPontosDupla1 = 0;

    for(int i = 0; i < int(pontosRodadaDupla1.size()); i++){
        qtdRodadas += pontosRodadaDupla1[i] + pontosRodadaDupla2[i];
        qtdPontosDupla1 += pontosRodadaDupla1[i];
    }

    qDebug() << "Rodada:" << qtdRodadas;


    if (qtdRodadas == 3){
        if (qtdPontosDupla1 == 2) pontosDupla1 += valorRodada; else pontosDupla2 += valorRodada;
        //cartasMesa.clear();
        proximaRodada();
    }
}

void GameEngine::proximaRodada() {
    if (pontosDupla1 >= 12 || pontosDupla2 >= 12) {
        // fim do jogo
        qDebug("Fim de jogo");
        iniciarPartida();
        novoJogo = true;
        return;
    }
    qDebug("Próxima rodada");
    novoJogo = false;
    pontosRodadaDupla1.clear();
    pontosRodadaDupla2.clear();
    rodadaAtual++;
    baralho.embaralhar();
    distribuirCartas();
    trucoPedido = false;
    valorRodada = 1;
    resetButton = true;
    turnoAnterior = turno;
    turno = 0;
}

void GameEngine::pedirTruco() {
    if (!trucoPedido) {
        trucoPedido = true;
        valorRodada = 3;
    } else if (valorRodada == 3) {
        valorRodada = 6;
    } else if (valorRodada == 6) {
        valorRodada = 9;
    } else {
        valorRodada = 12;
    }
}

void GameEngine::addBot(int idx){
    jogadores.push_back(new JogadorBot("Bot " + QString::number(idx)));
}

void GameEngine::setMao(int idxJogador, QString valor, QString naipe){
    jogadores[idxJogador]->receberCarta(baralho.setCarta(valor, naipe));
}

void GameEngine::addJogador(QString userID_){
    jogadores.push_back(new JogadorHumano(userID_));
}
