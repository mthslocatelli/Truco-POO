<p align="center">
  <img src="img/ifsc-logo.png" width="200" style="padding: 10px"><br>
</p>

<h1 align="center">TRUCO ONLINE</h1>

<p align="center">
  <i>Projeto final da disciplina PRG22107 - Programação Orientada a Objetos</i><br>
  <b>Aluno:</b> Matheus de Lima Locatelli <br>
  <b>Professor:</b> Hugo Marcondes <br>
</p>

---

## 🎯 Objetivo do Projeto

Este projeto tem como finalidade desenvolver um **jogo de truco online em tempo real**, com foco em **salas privadas**, onde amigos possam se reunir virtualmente para partidas organizadas.

## ⚙️ Funcionalidades

- ✅ Criação e gerenciamento de salas privadas com autenticação via código ou convite;
- ✅ Comunicação em rede entre jogadores com sincronização de estado;
- ✅ Interface gráfica amigável e responsiva, desenvolvida com **Qt**;
- ✅ Implementação das regras do truco em **C++**, com suporte a variantes regionais (como o truco paulista);
- ✅ Sistema de chat básico e interações sociais típicas de jogos informais.

---

## 🌐 Conectividade e Rede do Projeto

O **Truco Online** foi implementado no modelo cliente-servidor, onde o servidor centraliza toda a lógica do jogo e mantém o estado das partidas, enquanto os clientes se conectam para enviar comandos e receber atualizações.

### Desafio do IP Público Fixo

Como não dispomos de um endereço IP público fixo para hospedar o servidor, utilizamos o **Radmin VPN** para criar uma rede virtual privada que simula uma rede local segura entre os participantes.

### Uso do Radmin VPN

O computador que executa o servidor atua como um nó dentro da VPN criada pelo Radmin, e os clientes conectam-se ao servidor utilizando o endereço IP virtual fornecido pela VPN.

Dessa forma, mesmo sem IP público fixo, a comunicação via sockets TCP encapsulados em JSON ocorre normalmente, permitindo partidas em tempo real entre jogadores geograficamente dispersos.

### Fluxo de Conexão

1. O servidor é iniciado em um computador na rede (meu), aguardando conexões na rede virtual.
2. Os clientes conectam-se ao IP virtual do servidor dentro da VPN através do sistema cliente.
3. Os comandos (ex.: jogar carta, pedir truco, sair da sala) são enviados do cliente para o servidor, que processa e atualiza o estado da partida.
4. O servidor envia atualizações a todos os clientes conectados na sala, garantindo sincronização do jogo.
5. A interface gráfica desenvolvida em Qt reflete as mudanças da partida conforme as mensagens recebidas.

### Benefícios da VPN no Projeto

- Elimina a necessidade de configuração complexa de roteadores ou firewalls;
- Proporciona experiência próxima a uma rede local (LAN), com baixa latência;
- Permite jogos online entre amigos em diferentes locais com segurança;
- Facilita o desenvolvimento e testes sem depender de infraestrutura externa.

---

## 🧠 Estrutura do Projeto

| Seção | Descrição |
|-------|----------|
| [1. Análise Orientada a Objeto](./analise.md) | Abordagem inicial do problema utilizando princípios de OOP |
| [2. Projeto Orientado a Objeto](./projeto.md) | Modelagem das classes, relacionamentos e estruturas principais (incluindo diagramas PlantUML do cliente e servidor) |
| [3. Implementação (C++)](./implementacao.md) | Código-fonte com a lógica do jogo, interface Qt e comunicação em rede via sockets TCP JSON |
| [4. Testes](./testes.md) | Estratégias e resultados dos testes aplicados ao jogo |

---

## 📸 Capturas de Tela

(Adicione aqui prints do jogo em execução, se desejar)

---

## 🚀 Como Executar

1. Instale o [Qt Creator](https://www.qt.io/download) com suporte ao Qt 5 ou superior.
2. Clone este repositório:
   ```bash
   git clone https://github.com/seu-usuario/truco_online.git
