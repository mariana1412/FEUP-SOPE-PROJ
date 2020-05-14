
# READ ME

## Acesso informático aos Quartos de Banho

### Objetivos
 * criar programas multithread;
 * promover a intercomunicação entre processos através de canais com nome (named pipes ou FIFOs);
 * evitar conflitos entre entidades concorrentes, por via de mecanismos de sincronização.

### Funcionalidades implementadas
 O programa desenvolvido contém todas as features pretendidas no enunciado, todas a funcionar da forma correta.


### Compilação
 Para efetuar a compilação do programa, basta executar o comando > make no diretório  principal do projeto. Este comando irá gerar os executáveis necessários, nomeadamente Q2 (servidor) e U2(cliente). 


### Execução
 A execução do programa deve ser feita com a invocação dos executáveis, em terminais separados, com os argumentos devidos:

| Flag          | Descrição           | 
| ------------ |:-----------------------------------------------------------------------------: |
| -t nsecs     | nº (aproximado) de segundos que o programa deve funcionar                     |                            |                                                                                              |
| -l nplaces   | lotação do Quarto de Banho                                                    |                                     |                                                                                              |
| -n nthreads  | nº (máximo) de threads a atender pedidos                                      |                          |                                                                                              |
| fifoname     | nome do canal público (FIFO) a criar pelo servidor para atendimento de pedidos|
                                                                                             
 * Un <-t nsecs> fifoname
 * Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname

## Detalhes de implementação

* O programa utiliza alarmes para determinar o fim do tempo indicado na chamada do programa.

* Neste momento, não são utilizados mecanismos de sincronização.

* O pedido de um cliente é atendido, independentemente do seu tempo de duração ultrapassar ou não o que resta de execução. Se este pedido foi recebido antes da casa de banho fechar ele é atendido.
O programa do servidor só termina após responder a todos os pedidos recebidos até fechar.


### Programa do Cliente (U2)
* O programa do cliente lança pedidos com um curto intervalo entre pedidos (15ms),por forma a exacerbar condições de competição. Termina após passar o tempo de execução fornecido pelo utilizador ou até o servidor fechar.

* O cliente tenta aceder ao servidor e se este não estiver disponível, é lançada a mensagem "Oops server is closed" e o programa termina.

* Se durante o correr do programa o servidor fechar (após passar o seu tempo de execução), a mesma mensagem é exibida na consola, terminando a geração de pedidos. 

* A partir do momento que a casa de banho fecha, o cliente já não espera pela resposta dos pedidos pendentes.

* É criada uma thread por pedido que o envia por um fifo publico, fornecido pelo utilizador, para o servidor e lê a resposta do servidor de um fifo privado criado por esta mesma thread. No final destas ações, esta thread destrói o fifo privado.

### Programa do Servidor (Q2)
* O programa do servidor lança uma thread por cada pedido do cliente de modo a atênde-lo.

* A thread criada lê o pedido do cliente e elabora uma resposta para cada pedido recebido.

* Caso o tempo de execução do cliente termine primeiro que o do servidor, o servidor espera o restante tempo e temina após atingir o seu tempo de execução.

### Mensagens na consola

Ao longo do programa são exibidas mensagens de tracking no formato “inst;i ; pid ; tid ; dur ; pl ; oper”, em que :
* inst - valor retornado pela chamada ao sistema time(), na altura da produção da linha
* i - o número sequencial do pedido (gerado por Un)
* pid - identificador de sistema do processo (cliente, no caso do pedido; servidor, no caso da resposta)
* tid - identificador no sistema do  thread cliente (cliente, no caso do pedido; servidor, no caso daresposta)
* dur - duração, em milissegundos, de utilização (de um lugar) do Quarto de Banho (valor atribuídono pedido e repetido na resposta, se se der a ocupação; se não se der, por motivo de o Quarto deBanho estar em vias de encerrar, o servidor responde aqui com o valor -1
* pl – nº de lugar que eventualmente lhe será atribuído no Quarto de Banho (no pedido, este campo épreenchido com o valor -1 e na resposta terá o valor do lugar efetivamente ocupado ou também -1,na sequência de insucesso de ocupação, por motivo de encerramento)
* oper – siglas de 5 letras ajustadas às fases da operação que cada processo/thread acabou de executar eque variam conforme se trate do cliente ou do servidor: 

 	■ IWANT- cliente faz pedido inicial

	■ RECVD- servidor acusa receção de pedido

	■ ENTER- servidor diz que aceitou pedido

	■ IAMIN- cliente acusa a utilização do Quarto de Banho

	■ TIMUP- servidor diz que terminou o tempo de utilização

	■ 2LATE- servidor rejeita pedido por Quarto de Banho já ter encerrado

	■ CLOSD- cliente acusa informação de que o Quarto de Banho está fechado

	■ FAILD- cliente já não consegue receber resposta do servidor

	■ GAVUP- servidor já não consegue responder a pedido porque FIFO privado do cliente fechou








