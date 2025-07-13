# Testes

>[!NOTE]
> Relatar o processo de testes do sistema, especificando quais testes foram 
> realizados e os resultados obtidos.

O processo de testes do sistema de Truco Online envolveu diferentes estratégias adaptadas às limitações de infraestrutura e à complexidade da arquitetura distribuída cliente-servidor.

Dado que eu tinha apenas **dois computadores disponíveis** (o meu e o do meu sobrinho), precisei adotar soluções alternativas para validar o funcionamento do sistema em cenários com múltiplos jogadores. A seguir, descrevo os tipos de testes realizados e os principais resultados obtidos.

---

### 1. Testes Locais (modo 1 jogador + bots)

A primeira fase dos testes foi feita inteiramente local, com o objetivo de validar:

- O funcionamento da lógica de jogo (distribuição de cartas, manilhas, turnos, rodadas e pontuação)
- A criação e execução de partidas contra bots
- A estabilidade da interface gráfica Qt (`MainWindow`, `TelaMenu`, `TelaLobby`, etc.)
- A persistência de dados locais via `id_jogador.txt` com uso de `QUuid`

**Resultados:**
- Todas as interações com os bots foram realizadas com sucesso
- Bugs identificados na manipulação de cartas e turnos foram rapidamente isolados
- O uso do UUID local persistente se mostrou eficaz para manter a identidade do jogador

---

### 2. Testes em Rede Local (Cliente-Servidor)

Com a estrutura cliente-servidor implementada, realizei testes com os dois computadores conectados através da rede local (ou Radmin VPN, simulando uma rede pública).

**Cenários testados:**
- Criação de sala e entrada de outro jogador
- Comunicação via `QTcpSocket` com mensagens JSON
- Validação do UUID de cada jogador
- Transição entre telas e atualização da UI via sinais (`signal/slot`) do Qt
- Sincronização da distribuição de cartas entre cliente e servidor
- Fluxo de jogadas sincronizado entre dois jogadores reais + bots restantes
- Persistência e atualização do banco de dados `salas.db`

**Resultados:**
- A comunicação entre cliente e servidor funcionou corretamente em 100% dos testes com dois jogadores
- A sincronização de jogadas e turnos funcionou como esperado
- O servidor armazenou corretamente os dados de cada jogador no SQLite
- Foi detectado e corrigido um bug de crash causado por mensagens JSON com campos incompletos (resolvido com validações adicionais)

---

### 3. Testes com Múltiplos Bots (Simulando 4 jogadores)

Dado o limite de dois dispositivos físicos, a alternativa para testar o comportamento completo da sala (4 jogadores) foi utilizar bots para ocupar as vagas restantes. Esses bots são gerenciados diretamente pelo servidor, mas utilizam a mesma estrutura de comunicação JSON, simulando jogadores reais.

**Objetivos desses testes:**
- Validar a lógica de controle de turno em presença de bots e humanos mistos
- Testar se os bots respeitam regras de jogo e tempo de jogada
- Garantir que a comunicação e a UI se mantêm estáveis com múltiplos agentes ativos
- Observar o comportamento da aplicação em partidas completas

**Resultados:**
- Bots integraram-se perfeitamente ao sistema, agindo como jogadores normais
- A fluidez das jogadas foi mantida
- A UI refletiu corretamente as ações dos bots
- Foi necessário implementar delays mínimos para evitar jogadas simultâneas (condição de corrida)

---

### 4. Testes de Persistência e Integridade de Dados

Com o uso do arquivo `id_jogador.txt` e o banco de dados `salas.db`, realizei testes voltados à persistência de dados:

- Verificação de manutenção do mesmo UUID entre sessões
- Confirmação de persistência do estado da sala no banco
- Inserção, leitura e exclusão de registros no SQLite

**Resultados:**
- O UUID foi corretamente carregado a partir do arquivo e mantido entre execuções
- O servidor conseguiu persistir e consultar o estado da sala com sucesso
- Ocorreram pequenos problemas de sincronismo que foram resolvidos com tratamento de concorrência no acesso ao banco

---

### 5. Testes de Robustez e Tolerância a Falhas

Outros testes foram realizados para validar a estabilidade geral do sistema:

- Envio de mensagens JSON com dados faltantes ou inválidos
- Fechamento abrupto do cliente durante a partida
- Conexões simultâneas tentando entrar em uma sala cheia
- Reinício do servidor durante partidas

**Resultados:**
- O sistema passou a rejeitar corretamente mensagens inválidas após implementação de validações
- Jogadores desconectados são removidos da sala, e bots assumem a posição
- Limite de jogadores por sala é respeitado
- Reiniciar o servidor zera o estado de forma segura, mas não salva partidas em andamento (funcionalidade futura)

---

### Considerações Finais dos Testes

Apesar das limitações de infraestrutura, os testes cobriram os principais fluxos do sistema e simularam com sucesso a maior parte dos cenários reais de jogo. O uso de bots como substitutos foi essencial para validar a lógica em partidas completas. A integração com banco de dados e UUID também se mostrou estável.

Os testes demonstraram que:

 - A arquitetura cliente-servidor está bem estabelecida  
 - A lógica de jogo é consistente e íntegra  
 - O sistema é robusto contra falhas e mensagens malformadas  
 - O projeto é extensível e preparado para melhorias futuras  

---

<div align="center">

[Retroceder](projeto.md) | [Início](analise.md)

</div>
