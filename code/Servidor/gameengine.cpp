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
    //iniciarPartida();
}

GameEngine* GameEngine::instance = nullptr;

GameEngine* GameEngine::getInstance() {
    if (!instance) {
        instance = new GameEngine();
    }
    return instance;
}

void GameEngine::iniciarPartida() {
    pontosDupla1 = 0;
    pontosDupla2 = 0;
    valorRodada = 1;
    rodadaAtual = 0;
    trucoPedido = false;
    turno = 0;
    turnoAnterior = 0;
    //distribuirCartas();
    //definirManilha();
    cartasMesa.clear();
    pontosRodadaDupla1.clear();
    pontosRodadaDupla2.clear();
    indiceJogadorVencedor = 0;
}

void GameEngine::distribuirCartas() {
    baralho.embaralhar();

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
        turnoAnterior = turno;
        turno++;
        proxIdxJogador = (jogadorIdx + 1) % 4;
        qDebug() <<  jogadores[jogadorIdx]->getNome() << "jogou" << jogadores[jogadorIdx]->getMao()[cartaIdx].toString().first << "de" <<jogadores[jogadorIdx]->getMao()[cartaIdx].toString().second;
        cartasMesaAnimacao = cartasMesa;
    }else{
        auto c = jogadores[jogadorIdx]->jogarCarta(false);
        cartaBotJogada = c;
        cartasMesa.push_back(c);
        turnoAnterior = turno;
        turno++;
        proxIdxJogador = (jogadorIdx + 1) % 4;
        cartasMesaAnimacao = cartasMesa;
        qDebug() <<  jogadores[jogadorIdx]->getNome() << "jogou" << jogadores[jogadorIdx]->getMao()[cartaIdx].toString().first << "de" <<jogadores[jogadorIdx]->getMao()[cartaIdx].toString().second;
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
    int qtdPontosDupla2 = 0;

    for(int i = 0; i < int(pontosRodadaDupla1.size()); i++){
        qtdRodadas += pontosRodadaDupla1[i] + pontosRodadaDupla2[i];
        qtdPontosDupla1 += pontosRodadaDupla1[i];
        qtdPontosDupla2 += pontosRodadaDupla2[i];
    }

    qDebug() << "Rodada:" << qtdRodadas;


    if (qtdPontosDupla1 == 2) {
        pontosDupla1 += valorRodada;
        //cartasMesa.clear();
        //proximaRodada();
    } else if (qtdPontosDupla2 == 2){
        pontosDupla2 += valorRodada;
        //cartasMesa.clear();
        //proximaRodada();
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

void GameEngine::gerarCodigoSala(QString userID) {
    // Gera código da sala aleatório e garante que ele é único
    const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString codigo;
    do {
        codigo = "#";
        for (int i = 0; i < 6; ++i) {
            int index = QRandomGenerator::global()->bounded(chars.length());
            codigo += chars.at(index);
        }
    } while (verificarCodigoSala(codigo));

    codigoSala = codigo;

    // Gerencia conexão com o banco de forma segura
    const QString connectionName = "sala_connection";
    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(QCoreApplication::applicationDirPath() + "/salas.db");
    }

    if (!db.open()) {
        qDebug() << "Erro ao abrir banco de dados:" << db.lastError().text();
        return;
    }

    // Cria tabela se não existir
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS salas ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "codigo TEXT UNIQUE, "
               "userID0 TEXT, "
               "status0 TEXT, "
               "userID1 TEXT, "
               "status1 TEXT, "
               "userID2 TEXT, "
               "status2 TEXT, "
               "userID3 TEXT, "
               "status3 TEXT)");

    // Insere código e userID
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO salas (codigo, userID0, status0) VALUES (:codigo, :userID0, :status0)");
    insertQuery.bindValue(":codigo", codigo);
    insertQuery.bindValue(":userID0", userID);
    insertQuery.bindValue(":status0", "conectado");

    if (!insertQuery.exec()) {
        qDebug() << "Erro ao inserir código:" << insertQuery.lastError().text();
    } else {
        qDebug() << "Código da sala salvo com sucesso!";
        jogadores.clear();
        jogadores.push_back(new JogadorHumano(userID));
        qDebug() << "Banco será salvo em:" << db.databaseName();

        // Lê todos os registros
        QSqlQuery selectQuery(db);
        if (!selectQuery.exec("SELECT id, userID0, codigo FROM salas")) {
            qDebug() << "Erro ao ler registros:" << selectQuery.lastError().text();
        } else {
            while (selectQuery.next()) {
                int id = selectQuery.value("id").toInt();
                QString codigo = selectQuery.value("codigo").toString();
                QString uID0 = selectQuery.value("userID0").toString();
                qDebug() << "ID:" << id << ", Código:" << codigo << ", userID0:" << uID0;
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


QString GameEngine::entrarSala(QString codigo, QString userID) {
    const QString connectionName = "sala_connection";
    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(QCoreApplication::applicationDirPath() + "/salas.db");
    }

    if (!db.open()) {
        qDebug() << "Erro ao abrir banco de dados:" << db.lastError().text();
        return "Erro ao abrir banco de dados:" + db.lastError().text();
    }

    QSqlQuery selectQuery(db);
    selectQuery.prepare("SELECT * FROM salas WHERE codigo = :codigo");
    selectQuery.bindValue(":codigo", codigo);

    if (!selectQuery.exec() || !selectQuery.next()) {
        qDebug() << "Sala com código" << codigo << "não encontrada.";
        db.close();
        return "Sala com código " + codigo + " não encontrada.";
    }

    QStringList userFields = {"userID1", "userID2", "userID3"};
    QStringList statusFields = {"status1", "status2", "status3"};
    bool inserted = false;

    for (int i = 0; i < userFields.size(); ++i) {
        QString currentUser = selectQuery.value(userFields[i]).toString();
        if (currentUser.isEmpty()) {
            QSqlQuery updateQuery(db);
            QString sql = QString("UPDATE salas SET %1 = :userID, %2 = :status WHERE codigo = :codigo")
                              .arg(userFields[i])
                              .arg(statusFields[i]);

            updateQuery.prepare(sql);
            updateQuery.bindValue(":userID", userID);
            updateQuery.bindValue(":status", "conectado");
            updateQuery.bindValue(":codigo", codigo);

            if (updateQuery.exec()) {
                qDebug() << "Usuário" << userID << "entrou na sala" << codigo;
                inserted = true;
                jogadores.push_back(new JogadorHumano(userID));
                return "Usuário " + userID + " entrou na sala" + codigo;
            } else {
                qDebug() << "Erro ao adicionar usuário:" << updateQuery.lastError().text();
                return "Erro ao adicionar usuário: " + updateQuery.lastError().text();
            }
            break;
        }
    }

    if (!inserted) {
        qDebug() << "Sala cheia. Não foi possível entrar.";
        return "Sala cheia. Não foi possível entrar.";
    }

    db.close();
}

void GameEngine::sairSala(QString userID) {
    const QString connectionName = "sala_connection";
    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(QCoreApplication::applicationDirPath() + "/salas.db");
    }

    if (!db.open()) {
        qDebug() << "Erro ao abrir banco de dados:" << db.lastError().text();
        return;
    }

    QSqlQuery selectQuery(db);
    selectQuery.prepare("SELECT * FROM salas");

    if (!selectQuery.exec()) {
        qDebug() << "Erro ao buscar registros:" << selectQuery.lastError().text();
        db.close();
        return;
    }

    bool usuarioEncontrado = false;

    while (selectQuery.next()) {
        QString codigoSala = selectQuery.value("codigo").toString();
        QStringList userFields = {"userID0", "userID1", "userID2", "userID3"};
        QStringList statusFields = {"status0", "status1", "status2", "status3"};

        for (int i = 0; i < userFields.size(); ++i) {
            if (selectQuery.value(userFields[i]).toString() == userID) {
                usuarioEncontrado = true;

                // Limpar userID e status
                QString updateSQL = QString("UPDATE salas SET %1 = NULL, %2 = NULL WHERE codigo = :codigo")
                                        .arg(userFields[i])
                                        .arg(statusFields[i]);

                QSqlQuery updateQuery(db);
                updateQuery.prepare(updateSQL);
                updateQuery.bindValue(":codigo", codigoSala);

                if (!updateQuery.exec()) {
                    qDebug() << "Erro ao remover usuário da sala:" << updateQuery.lastError().text();
                    db.close();
                    return;
                }

                qDebug() << "Usuário" << userID << "removido da sala" << codigoSala;

                // Verificar se a sala está vazia
                QSqlQuery verificarQuery(db);
                verificarQuery.prepare("SELECT userID0, userID1, userID2, userID3 FROM salas WHERE codigo = :codigo");
                verificarQuery.bindValue(":codigo", codigoSala);

                if (verificarQuery.exec() && verificarQuery.next()) {
                    bool salaVazia = true;
                    for (int j = 0; j < 4; ++j) {
                        if (!verificarQuery.value(j).toString().trimmed().isEmpty()) {
                            salaVazia = false;
                            break;
                        }
                    }

                    if (salaVazia) {
                        QSqlQuery deleteQuery(db);
                        deleteQuery.prepare("DELETE FROM salas WHERE codigo = :codigo");
                        deleteQuery.bindValue(":codigo", codigoSala);

                        if (deleteQuery.exec()) {
                            qDebug() << "Sala" << codigoSala << "estava vazia e foi excluída.";
                        } else {
                            qDebug() << "Erro ao excluir sala:" << deleteQuery.lastError().text();
                        }
                    }
                }

                db.close();
                return;  // Como o user só está em uma sala, podemos sair da função
            }
        }
    }

    if (!usuarioEncontrado) {
        qDebug() << "Usuário" << userID << "não está em nenhuma sala.";
    }

    db.close();
}

void GameEngine::addBot(int idx){
    if(jogadores.size() < 4)
        jogadores.push_back(new JogadorBot("Bot " + QString::number(idx)));
}
