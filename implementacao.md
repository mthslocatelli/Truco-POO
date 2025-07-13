# Implementação

> [!NOTE]  
> Relatar o processo de implementação do problema, incluindo as ferramentas e bibliotecas utilizadas.

## Dificuldades de Implementação do Projeto de Truco Online

O desenvolvimento do meu projeto final da disciplina de **Programação Orientada a Objetos (POO)** — um jogo de Truco online — envolveu diversos desafios técnicos, arquiteturais e práticos. Este projeto serviu como uma excelente oportunidade para aplicar conceitos de orientação a objetos, comunicação em rede, controle de estado distribuído e integração com banco de dados. Abaixo, descrevo com riqueza de detalhes os principais obstáculos enfrentados durante a implementação.

---

### 1. Evolução da Arquitetura: Do Sistema Único para Cliente-Servidor

O projeto começou como uma aplicação local, com toda a lógica de jogo, UI e inteligência dos bots em um único processo. Inicialmente, ele consistia em um jogador humano enfrentando três bots (modo 1 jogador VS 3 bots). Essa abordagem facilitava o teste e a prototipação, já que não exigia comunicação em rede nem sincronização entre diferentes instâncias.

Entretanto, para torná-lo um verdadeiro jogo online, foi necessária uma **mudança radical de arquitetura**:

- **Separação da lógica de jogo:** A lógica e estado da partida migraram para o servidor. O cliente passou a ser responsável apenas por enviar comandos e renderizar a UI.
- **Criação de uma infraestrutura cliente-servidor:** Com uso de `QTcpSocket`, cada cliente passou a se conectar ao servidor e enviar mensagens JSON estruturadas.
- **Controle de múltiplas conexões simultâneas:** O servidor passou a manter o controle de cada jogador conectado, suas salas, status e jogadas.

Essa reformulação afetou profundamente o design das classes e exigiu uma reestruturação completa do projeto, respeitando os princípios de coesão e responsabilidade única da POO.

---

### 2. Comunicação via `QTcpSocket` e JSON: Simples na Teoria, Complexa na Prática

A comunicação entre cliente e servidor foi construída com base na classe `QTcpSocket`, usando sockets TCP para garantir comunicação confiável e ordenada. As mensagens trocadas são objetos `QJsonObject`, convertidos para strings JSON.

Apesar de parecer direto, isso trouxe **vários desafios práticos**:

- **Criação de um protocolo robusto:** Foi necessário estabelecer um padrão de mensagens que inclua `type` e `payload`, com campos bem definidos.
- **Validação rigorosa:** Mensagens incompletas ou com campos errados causavam falhas. Isso exigiu verificações constantes antes do processamento.
- **Serialização e desserialização dos dados:** Cada carta, jogador e estado de sala precisou ser convertido adequadamente entre objetos e strings JSON.
- **Concorrência e ordem de eventos:** Várias mensagens podem ser recebidas em sequência. Foi necessário garantir a consistência na ordem das ações e sincronização do estado da partida.
- **Depuração das mensagens JSON:** Grande parte do tempo foi dedicada à leitura de logs detalhados para entender por que certas mensagens causavam erros.

---

### 3. Integração com Banco de Dados SQLite (`salas.db`)

Para armazenar as informações persistentes das salas e jogadores, o projeto utiliza um banco de dados SQLite. As tabelas armazenam:

- O **código da sala** (gerado automaticamente)
- O **UUID de cada jogador**
- O **status do jogador** (ativo, desconectado, aguardando)

Essa integração exigiu:

- **Execução de comandos SQL no servidor** para consultar, inserir e atualizar registros a cada nova conexão, entrada ou saída de sala.
- **Controle de integridade das informações**, especialmente em situações de desconexão ou falha de sincronismo.
- **Evitar concorrência de escrita** ao banco, garantindo que atualizações simultâneas não corrompessem os dados.

---

### 4. Identificação única de jogadores com `QUuid` e `id_jogador.txt`

Cada cliente precisa ser identificado de forma única e persistente, mesmo que o aplicativo seja fechado e reaberto. Para isso:

- Foi implementada a criação de um **UUID com `QUuid::createUuid()`** na primeira execução.
- Esse UUID é salvo em um arquivo chamado `id_jogador.txt`, lido nas execuções seguintes.
- O UUID é utilizado em todas as comunicações com o servidor para garantir que cada jogador seja reconhecido corretamente, mesmo que reconecte depois.

Esse mecanismo simples foi crucial para manter a consistência dos dados e a identidade dos jogadores durante o uso prolongado.

---

### 5. Garantia da Estabilidade e Consistência dos Dados Transmitidos

Como o estado do jogo depende inteiramente das mensagens trocadas entre cliente e servidor, a integridade dessas mensagens é crítica.

Foram implementadas estratégias para:

- **Detectar mensagens malformadas e rejeitá-las** antes do processamento.
- **Verificar se os dados estão coerentes com o estado atual** do jogo (ex: jogada feita fora do turno).
- **Reforçar a robustez do sistema contra falhas de conexão**, utilizando respostas do servidor para confirmar ações antes de executá-las no cliente.
- **Registrar logs detalhados** de todos os eventos e mensagens para facilitar a depuração.

---

### 6. Gerenciamento da Sala, Jogadores e Fluxo do Jogo

A lógica de gerenciamento de salas foi um dos pontos mais complexos do projeto:

- **Limite de 4 jogadores por sala**: o servidor rejeita conexões extras ou preenche com bots automaticamente.
- **Controle de turnos e jogadas válidas**: o servidor valida cada jogada recebida, garantindo que respeite a ordem e as regras.
- **Sincronização entre todos os clientes**: todos os jogadores devem receber atualizações corretas e ao mesmo tempo.
- **Tratamento de desconexões**: o servidor substitui jogadores desconectados por bots ou finaliza a sala se necessário.

---

### 7. Desafios com Bots vs Jogadores Humanos

Para testes com número reduzido de humanos, foi fundamental implementar bots que agissem como jogadores reais, mas com lógica automatizada. Os desafios foram:

- **Tratar bots como jogadores reais**, participando da comunicação JSON.
- **Tomada de decisões automatizada** dentro das regras do jogo.
- **Manter sincronismo e fluidez no turno dos bots**, simulando o comportamento de um jogador humano.

Essa estrutura permitiu que o jogo fosse testado mesmo em ambientes com apenas um ou dois usuários reais.

---

### 8. Limitações de Infraestrutura

Durante o desenvolvimento, tive acesso apenas a **dois computadores físicos** (o meu e o do meu sobrinho). Isso trouxe limitações importantes:

- **Impossibilidade de testar com 4 jogadores reais simultâneos**.
- **Dependência do Radmin VPN** para simular conexões externas — útil, mas não totalmente fiel ao comportamento real da internet.
- **Necessidade de simular jogadores restantes com bots** para validar o jogo em salas completas.
- **Dificuldade de reproduzir cenários de alta concorrência**, como várias jogadas sendo feitas ao mesmo tempo.

---

### Considerações Finais

Esse projeto foi **desafiador e transformador**. A migração de uma estrutura local simples para um sistema distribuído com comunicação via rede, gerenciamento de estado, uso de banco de dados e testes em tempo real exigiu:

- Aplicação profunda de **conceitos de POO**, como encapsulamento, herança e coesão.
- Domínio de **comunicação em rede** com `QTcpSocket`.
- Uso eficiente de **JSON e SQLite**.
- Habilidades de **debugging e resolução de problemas reais**, especialmente em ambiente com poucos recursos.

A experiência foi extremamente enriquecedora, e os obstáculos enfrentados contribuíram diretamente para minha evolução como desenvolvedor. O código resultante é modular, extensível e estável — pronto para ser expandido com novos recursos como chat, partidas ranqueadas ou IA mais elaborada para os bots.

---

<div align="center">

[Retroceder](projeto.md) | [Início](analise.md)

</div>
