#include "telamenu.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

TelaMenu::TelaMenu(QWidget *parent) : QWidget(parent) {

    this->parentWidget()->showFullScreen();
    btnIniciar = new QPushButton("Iniciar Jogo", this);
    btnSair = new QPushButton("Sair", this);
    btnCriarSala = new QPushButton("Criar sala", this);
    btnEntrarSala = new QPushButton("Entrar sala", this);
    btnJogarBot = new QPushButton("Jogar bot", this);
    layout = new QVBoxLayout(this);
    txtEdit = new QLineEdit(this);


    // Expressão regular: "#" seguido de 6 caracteres alfanuméricos (letras e números, maiúsculos ou minúsculos)
    QRegularExpression regex("^#[a-zA-Z0-9]{6}$");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);

    txtEdit->setText("#");
    txtEdit->setValidator(validator);
    txtEdit->setObjectName("codigo");
    txtEdit->setFixedSize(100, 30);
    txtEdit->setAlignment(Qt::AlignCenter);
    btnIniciar->setFixedSize(100,30);
    btnSair->setFixedSize(100,30);
    btnEntrarSala->setFixedSize(100,30);
    btnCriarSala->setFixedSize(100,30);
    btnJogarBot->setFixedSize(100,30);

    QStringList nomesBotoes = {btnIniciar->text(), btnCriarSala->text(), btnEntrarSala->text(), btnJogarBot->text(), btnSair->text()};
    QPushButton* botoes[5] = {btnIniciar, btnCriarSala, btnEntrarSala, btnJogarBot, btnSair};

    for (int i = 0; i < 5; ++i) {
        QWidget* wrapper = new QWidget(this);
        QHBoxLayout* hbox = new QHBoxLayout(wrapper);
        hbox->setContentsMargins(0, 0, 0, 0);  // opcional, para ficar compacto
        hbox->setSpacing(0);                   // opcional
        wrapper->setObjectName(nomesBotoes[i]);

        hbox->addStretch();
        hbox->addWidget(botoes[i]);
        hbox->addStretch();

        wrapper->setLayout(hbox);
        layout->addWidget(wrapper);
    }

    connect(btnIniciar, &QPushButton::clicked, this, &TelaMenu::procurarJogo);
    connect(btnJogarBot, &QPushButton::clicked, this, &TelaMenu::jogarBot);
    connect(txtEdit, &QLineEdit::returnPressed, [this](){
        btnEntrarSala->click();
    });
    connect(txtEdit, &QLineEdit::textChanged, this, [this]() {
        QString text = txtEdit->text();
        if (!text.startsWith("#")) {
            txtEdit->setText("#" + text.remove("#"));
            txtEdit->setCursorPosition(txtEdit->text().length()); // mantém cursor no final
        }
    });
    connect(btnEntrarSala, &QPushButton::clicked, [this]() {
        QLayoutItem* item = layout->itemAt(2);
        if (!item) return;
        QWidget* widget = item->widget();
        if (!widget) return;

        if (widget->objectName() != "entradaCodigo") {

            // Muda botão para modo "confirmar código"
            txtEdit->setText("#");
            btnEntrarSala->setText(">");
            btnEntrarSala->setFixedSize(40, 30);

            // Cria wrapper com campo texto + botão
            QWidget* wrapper = new QWidget(this);
            wrapper->setObjectName("entradaCodigo");
            QHBoxLayout* hbox = new QHBoxLayout(wrapper);
            hbox->setContentsMargins(0, 0, 0, 0);

            hbox->addStretch();
            hbox->addWidget(txtEdit);
            hbox->addWidget(btnEntrarSala);
            hbox->addStretch();

            wrapper->setLayout(hbox);
            wrapper->setFixedHeight(txtEdit->height());

            // Substitui widget no layout
            layout->insertWidget(2, wrapper);
            layout->removeWidget(widget);
            widget->hide();

        } else {
            emit entrarSala();
            // Volta para botão original "Entrar Sala"
            btnEntrarSala->setText("Entrar Sala");
            btnEntrarSala->setFixedSize(100, 30);

            QWidget* wrapper = new QWidget(this);
            wrapper->setObjectName("Entrar Sala");
            QHBoxLayout* hbox = new QHBoxLayout(wrapper);
            hbox->setContentsMargins(0, 0, 0, 0);

            hbox->addStretch();
            hbox->addWidget(btnEntrarSala);
            hbox->addStretch();

            wrapper->setLayout(hbox);

            layout->insertWidget(2, wrapper);
            layout->removeWidget(widget);
            widget->hide();
        }
    });


    connect(btnCriarSala, &QPushButton::clicked, this, &TelaMenu::iniciarLobby);
    connect(btnSair, &QPushButton::clicked, this, &TelaMenu::sair);
}
