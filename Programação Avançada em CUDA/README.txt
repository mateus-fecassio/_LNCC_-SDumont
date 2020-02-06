----------Resumo
Curso introdutório em linguagem CUDA, uma extensão da linguagem C utilizada para a programação de aplicações de propósito geral, sendo executadas em arquitetura massivamente paralela de placas gráficas, denominadas GPU.

----------Ementa
A arquitetura das GPUs (Graphics Processing Unit) foi projetada para efetuar os cálculos em ponto flutuante mais frequentemente realizados em aplicações gráficas. Esta é uma arquitetura bem mais especializada que a presente em CPUs (Central processing unit) , o que faz com que estes programas rodem mais rapidamente nestas plataformas. Além disso, as GPUs são altamente paralelizadas, já havendo atualmente modelos com milhares de núcleos computacionais (cores). Embora originalmente desenvolvida para executar aplicações gráficas com maior eficiência, também é cada vez maior o uso de GPUs em aplicações não-gráficas. São aplicações que envolvem a resolução de métodos de álgebra numérica computacional, frequentemente também utilizados nas aplicações gráficas, obtendo-se um significativo ganho de desempenho. Neste curso será apresentada a arquitetura de placas gráficas NVIDIA e a conexão destas placas com o computador hospedeiro. Em seguida será apresentado a plataforma de programação CUDA, que é uma extensão do C/C++ processada tanto no computador hospedeiro quanto nas placas gráficas conectadas a este. Serão apresentados exemplos de códigos de cada um dos tópicos abordados, os quais os alunos poderão executá-los nas máquinas disponíveis em sala de aula.

----------Tópicos abordados
1. Introdução (Arquitetura)
2. Modelo de Paralelismo em CUDA
2.1 Organização e Identificação
2.2 Atribuição
2.3 Escalonamento
3. Hieraquia de Memoria
6. Métricas desempenho e otimização
7. Estudo de caso: multiplicação de matrizes

----------Bibliografia:
1) CUDA by Example: An Introduction to General Purpose GPU Programming, by David Weller,
2) Programming Massively Parallel Processors: A Hands-on Approach (Applications of GPU Computing Series), by David B. Kirk and Wen-mmei W. Hwu- Second Edition
3) http://docs.nvidia.com/cuda/cuda-cprogramming-guide/index.htm
4) http://devblogs.nvidia.com/parallelforall

----------Participantes:
Roberto Souto