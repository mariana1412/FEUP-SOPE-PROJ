# READ ME 

## Funcionalidades implementadas

O programa desenvolvido contém todas as features pretendidas no enunciado, todas a funcionar bem.

## Detalhes de implementação

O programa utiliza alarmes para determinar o fim do tempo indicado na chamada do programa.

O cliente tenta aceder ao servidor e se este não estiver disponível é lançada a mensagem "Oops server is closed" e o programa termina.

Se durante o correr do programa o servidor fechar (após passar o seu tempo de execução) a mesma mensagem é exibida na consola, terminando a geração de pedidos. Caso o tempo de execução do cliente termine primeiro que o do servidor, o servidor espera o restante tempo e termina após atingir o seu tempo de execução.

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


