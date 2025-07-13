# Análise orientada a objeto
> [!NOTE]
> <p>A <strong>análise</strong> orientada a objeto consiste na descrição do 
> problema a ser tratado, duas primeiras etapas da tabela abaixo, a definição de 
> casos de uso e a definição do domínio do problema.</p>

## Descrição Geral do domínio do problema

O projeto Truco Paulista Online foi desenvolvido com o objetivo de permitir que jogadores humanos participem de partidas de Truco pela internet, interagindo entre si em tempo real com suporte à substituição de jogadores ausentes por bots automatizados. O sistema utiliza uma arquitetura distribuída baseada no modelo cliente-servidor, desenvolvida com a biblioteca Qt em C++, usando `QTcpSocket` para comunicação entre os clientes e o servidor.

### Escopo do Problema

O domínio central envolve o gerenciamento de partidas de Truco, que por sua vez depende da criação de salas, controle de jogadores, administração das jogadas (jogar carta, pedir truco, aceitar/recusar) e manutenção do estado do jogo. É fundamental garantir:

- Sincronização entre múltiplos jogadores em tempo real.
- Controle consistente do estado das partidas mesmo com desconexões e reconexões.
- Substituição de jogadores ausentes por bots.
- Persistência de dados como códigos de sala, UUIDs e status dos jogadores.

### Tecnologias e Estratégias

- Comunicação entre cliente e servidor utilizando `QTcpSocket` e mensagens em formato **JSON**.
- Identificação persistente dos jogadores por meio de arquivos locais `id_jogador.txt` com UUIDs gerados via `QUuid`.
- Armazenamento e gerenciamento de salas e status dos jogadores em banco de dados **SQLite**, através do arquivo `salas.db`.
- Separação clara entre cliente (interface e ações do usuário) e servidor (lógica do jogo e orquestração de estados).

### Requisitos Funcionais

- Criar sala de jogo com código único.
- Entrar em sala existente informando o código.
- Procurar partida com jogadores disponíveis.
- Jogar carta da mão atual.
- Pedir truco e responder ao pedido.
- Sair da sala a qualquer momento.
- Jogar contra bots automaticamente.
- Sincronizar estado da partida entre todos os jogadores conectados.
- Registrar e controlar as ações dos jogadores.

### Requisitos Não-Funcionais

- A comunicação entre cliente e servidor deve ser segura, validada e tolerante a falhas.
- O sistema deve ser modular, extensível e reutilizável (princípios de POO).
- As mensagens JSON devem seguir um protocolo bem definido e documentado.
- O sistema deve funcionar com latência mínima para manter a responsividade do jogo.
- O servidor deve lidar com múltiplas conexões simultâneas sem interferência entre as salas.
- O banco de dados deve ser leve, confiável e local (SQLite).

---

## Diagrama de Casos de Uso
O diagrama de casos de uso representa as funcionalidades principais do sistema Truco Paulista Online e os atores envolvidos.

- **Atores:**
  - **Jogador Humano:** usuário que interage com o sistema para criar ou entrar em partidas, jogar cartas, pedir truco e sair de salas.
  - **Bot:** jogador automatizado que participa das partidas quando não há jogadores humanos suficientes.
  - **Servidor:** responsável por gerenciar as salas, partidas e a comunicação entre clientes.

- **Casos de Uso principais:**
  - **Criar Sala:** permite ao jogador humano criar uma nova sala para iniciar uma partida.
  - **Entrar em Sala via Código:** possibilita ao jogador acessar uma sala existente informando seu código.
  - **Procurar Partida:** o jogador pode buscar uma partida disponível para participar.
  - **Jogar Carta:** ação do jogador ao selecionar uma carta para jogar na mesa.
  - **Pedir Truco:** o jogador pode solicitar aumento do valor da rodada.
  - **Responder Pedido de Truco:** aceitar ou recusar um pedido de truco feito por outro jogador.
  - **Sair da Sala:** ação para deixar a sala em que está participando.
  - **Jogar Automaticamente:** ação do bot para jogar sua carta automaticamente durante a partida.

O sistema garante a comunicação e sincronização das ações entre clientes e servidor, permitindo partidas online consistentes.

![Descrição da Imagem](img/Diagrama%20de%20caso%20de%20uso.png)
 
## Diagrama de Domínio do problema
O diagrama conceitual de domínio modela as entidades fundamentais do sistema Truco Paulista Online e suas relações.

- **Jogador:** representa os participantes da partida, podendo ser humanos ou bots, cada um com uma mão de cartas.
- **Sala:** espaço virtual que contém até quatro jogadores e onde ocorre uma partida.
- **Partida:** instância do jogo que gerencia o estado atual, pontuação, valor do truco e as rodadas em andamento.
- **Carta:** elemento básico do jogo, definido por valor e naipe.
- **Mão:** conjunto de cartas que um jogador possui em uma partida.
- **Mesa:** local onde as cartas jogadas são colocadas durante as rodadas.
- **Rodada:** representa uma sequência de jogadas feitas pelos jogadores durante a partida.
- **Truco:** entidade que controla o valor atual da aposta e o status do pedido de truco.

As relações entre essas entidades expressam que uma sala hospeda jogadores e uma partida, que por sua vez controla várias rodadas e a mesa. Cada jogador possui uma mão com cartas e participa das rodadas jogando suas cartas na mesa.

![Descrição da Imagem](img/diagrama%20de%20dominio.png)

<div align="center">

[Retroceder](README.md) | [Avançar](projeto.md)

</div>
