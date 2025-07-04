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
    iniciarPartida();
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
    distribuirCartas();
    definirManilha();
    cartasMesa.clear();
    pontosRodadaDupla1.clear();
    pontosRodadaDupla2.clear();
    indiceJogadorVencedor = 0;
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

void GameEngine::definirManilha() {
    manilha = baralho.comprarCarta();
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
    definirManilha();
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

void GameEngine::gerarCodigoSala() {
    // Gera código da sala aleatório
    const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString codigo = "#";
    for (int i = 0; i < 4; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        codigo += chars.at(index);
    }
    codigoSala.push_back(codigo);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/salas.db");

    if (!db.open()) {
        qDebug() << "Erro ao abrir banco de dados:" << db.lastError().text();
        return;
    }

    // Cria tabela se ainda não existir
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS salas ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "codigo TEXT UNIQUE)");

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO salas (codigo) VALUES (:codigo)");
    insertQuery.bindValue(":codigo", codigo);

    if (!insertQuery.exec()) {
        qDebug() << "Erro ao inserir código:" << insertQuery.lastError().text();
    } else {
        qDebug() << "Código da sala salvo com sucesso!";
        qDebug() << "Banco será salvo em:" << db.databaseName();

        // Agora lê todos os códigos salvos:
        QSqlQuery selectQuery;
        if (!selectQuery.exec("SELECT id, codigo FROM salas")) {
            qDebug() << "Erro ao ler registros:" << selectQuery.lastError().text();
        } else {
            while (selectQuery.next()) {
                int id = selectQuery.value("id").toInt();
                QString codigo = selectQuery.value("codigo").toString();
                qDebug() << "ID:" << id << ", Código:" << codigo;
            }
        }
    }
    db.close();
}

bool GameEngine::verificarCodigoSala(QString codigoEntrada){

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/salas.db");

    if (!db.open()) {
        qDebug() << "Erro ao abrir banco de dados:" << db.lastError().text();
        return false;
    }

    // Agora lê todos os códigos salvos:
    QSqlQuery selectQuery;
    bool aux = false;
    if (!selectQuery.exec("SELECT id, codigo FROM salas")) {
        qDebug() << "Erro ao ler registros:" << selectQuery.lastError().text();
    } else {
        while (selectQuery.next()) {
            int id = selectQuery.value("id").toInt();
            QString codigo = selectQuery.value("codigo").toString();
            qDebug() << "ID:" << id << ", Código:" << codigo;
            if(codigoEntrada == codigo){
                aux = true;
                codigoSala = codigoEntrada;
            }
        }
    }

    db.close();

    return aux;
}
