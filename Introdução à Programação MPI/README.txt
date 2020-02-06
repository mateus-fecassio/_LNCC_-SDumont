----------Objetivo
O objetivo deste curso é ensinar aos programadores que não estão familiarizados com programação paralela as noções básicas para desenvolver e executar programas paralelos de acordo com as normas do padrão MPI. Para isto iremos apresentar durante o curso apenas as rotinas básicas para se trabalhar com o padrão MPI. MPI é um padrão que vem sendo largamente utilizado para o desenvolvimento de programas paralelos em arquitetura de sistemas distribuídos, tais como clusters de PCs, A norma do padrão MPI foi implantada a mais de 25 anos e existe hoje muito material sobre este assunto. Este curso toma como base diversas apostilas que foram desenvolvidas ao longo dos últimos anos em centros de supercomputação, particularmente nos tutoriais do Laurence Livermore Laboratory (httpscomputing.llnl.govtutorialsmpi). As demais referências utilizadas para a elaboração do curso estão relacionadas na seção bibliográfica. O curso inicia fornecendo informações sobre o ambiente de sistemas distribuídos e sobre a programação de troca de mensagens. A seguir apresentamos as rotinas básicas da norma MPI, incluindo as rotinas de gerenciamento de ambiente, de comunicação ponto a ponto e as rotinas de comunicações coletivas. Durante o curso, serão apresentados exemplos em linguagem C e ao final serão passados exercícios. Assumimos que o usuário possui compreensão gerais sobre programação com a linguagem C.

----------Ementa
1- Introdução
Características de MPI
Histórico de MPI
Modelo de Programação.
Plataformas de Hardware
Como funciona MPI

2- O padrão MPI.
Funcionalidades básicas do padrão MPI
Rotinas básicas de Comunicação.
Funcionamento Das Operações Ponto A Ponto.
Mensagens bloqueantes Versus não-bloqueantes
Ordem e Equidade
3- Rotinas de Comunicação Coletivas
Exemplo Rotina de comunicação com MPI_Reduce
Exemplo Rotina de comunicação com SCATTER
Tipos de dados Derivados  Derived Data Types
Exemplo de um programa com Contiguous Derived Data Type (tipo contínuo)
Exemplo de um Tipo de dado Vetorial (Vector Derived Data Type)
Exemplo de um Indexed Derived Data Type ( Tipo Indexado)
Exemplo de um Struct Derived Data Type ( Tipo estruturado)

4- Comparação entre Grupos e Comunicadores
5- Considerações e Restrições sobre Programação com MPI
6- Topologias Virtuais
7- MPI 3

----------Bibliografia
1. Gabriel Silva, Programação Paralela com MPI Um Curso Introdutório, Amazon Serviços de varejo do Brasil, 2018.
2. Peter Pacheco. Parallel Programming with MPI . Morgan Kaufmann Publishers Inc., San Francisco, CA, USA, 2019.
3. Wesley Kendall, 2013, beginning MPI( An Introduction in C) .

----------Participantes
Micaella Coelho
Carla Osthoff Ferreira de Barros
Mateus Mello