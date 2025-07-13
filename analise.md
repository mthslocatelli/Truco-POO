# Análise orientada a objeto
> [!NOTE]
> <p>A <strong>análise</strong> orientada a objeto consiste na descrição do 
> problema a ser tratado, duas primeiras etapas da tabela abaixo, a definição de 
> casos de uso e a definição do domínio do problema.</p>

## Descrição Geral do domínio do problema

Descrever de forma geral o domínio do problema.

Possíveis requisitos funcionais e não-funcionais podem ser elencados aqui.

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
 
## Diagrama de Classe
O diagrama de classes detalha a estrutura interna do sistema, com as classes que compõem os pacotes Cliente e Servidor.

- **Pacote Cliente:**
  - **Classes Modelo:** `Carta`, `Mao`, `Baralho`, `Mesa`, `Jogador` (abstrato), com subclasses `JogadorHumano` e `JogadorBot`.
  - **GameEngine:** responsável pela lógica do jogo no cliente, gerenciando jogadores, cartas e mesa.
  - **ClienteTruco:** gerencia a comunicação via socket com o servidor.
  - **Interface Gráfica:** classes como `MainWindow`, `TelaMenu`, `TelaLobby`, `TelaProcurando` e `TelaPartida` que compõem as telas e controlam a interação do usuário.

- **Pacote Servidor:**
  - Espelhamento das classes modelo do cliente (`Carta_Serv`, `Mao_Serv`, `Baralho_Serv`, `Mesa_Serv`, `Jogador_Serv`) adaptadas para controle centralizado.
  - **ServidorTruco:** classe principal que gerencia conexões TCP, jogadores conectados, e coordena o fluxo das partidas.

- **Relações:**
  - Composições indicam que `GameEngine` possui múltiplos jogadores e um baralho.
  - Heranças indicam a especialização de jogadores humanos e bots.
  - Associação entre `MainWindow` e `ClienteTruco` para comunicação.
  - No servidor, o `ServidorTruco` agrega jogadores e mantém o estado do jogo centralizado.

Este diagrama representa a organização das classes, seus atributos e métodos essenciais, permitindo a implementação do sistema com separação clara entre cliente e servidor.

![Descrição da Imagem](img/diagrama%20de%20classes%20BR.png) 

## Diagrama de Sequência
O diagrama de sequência descreve a interação entre os componentes cliente e servidor durante uma partida de Truco Paulista Online.

- **Fluxo típico:**
  1. O **Jogador** interage com a interface (`MainWindow`), solicitando criar ou entrar em uma sala.
  2. O **ClienteTruco** conecta-se ao servidor e envia comandos JSON representando ações do jogador.
  3. O **ServidorTruco** recebe os comandos, atualiza o estado da partida (ex.: distribuir cartas, controlar jogadas).
  4. O servidor envia atualizações para todos os clientes conectados na sala, sincronizando o estado do jogo.
  5. Quando um jogador joga uma carta, essa ação é enviada ao servidor, que processa e atualiza a mesa e o estado da rodada.
  6. Bots agem automaticamente via servidor quando necessário para completar a partida.
  7. O cliente atualiza a interface (`TelaPartida`) para refletir as ações e mudanças do jogo.

Essa sequência garante que a lógica do jogo fique no servidor, mantendo consistência e evitando trapaças, enquanto os clientes apenas enviam comandos e exibem o estado atualizado.

![Descrição da Imagem](img/diagrama%20de%20sequencia.png)

<div align="center">

[Retroceder](README.md) | [Avançar](projeto.md)

</div>
