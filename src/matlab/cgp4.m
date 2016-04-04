%%
%PROGRAMAÇÃO GENÉTICA CARTESIANA PARA O PROJETO DE CLC
% ANALISE DA TOPOLOGIA
% Autor: FRANCISCO MANFRINI
% PPSN
%///////////////////////////////////////////////////////////////////////
% Os exemplos foram extraídos de BRIAN_2015, tendo como referência para
%comparação do algoritmo denominado "Single".
% " BRIAN_2015: Algorithm Single
%..."Instead of mutating each gene at a set probability single mutates gene
% at random until exactly one active gene is mutated ..."
%**********************************************************************
clc
clear all
global nvar mintermo ns tabela portas  nof pnof cs s c na
%**********************************************************************
% EXEMPLO 1: DETECTOR PARIDADE DE 3 BITS (artigo BRIAN_2015)
%%%%DADOS ENTRADA EXEMPLO 1
nvar = 3;
mintermo = {[1 2 4 7]};
limite = 5000 ; %--- Numero maior do que dobro do limite superior IC brian(1487)
c=500;
lin=1; col=500; ret=500;
pc=0;
r_in = 1; % rodada inicial (as 51 rodadas serão particionadas em 3 grupos)
r_fim = 51; % rodada final
limite = 3000 ; %--- Numero maior do que dobro do limite superior IC brian(1487)
vies=0;
ns = size(mintermo,2); % numero de saídas do circuito
tabela = cria_tab3;% cria a tabela verdade

% COMENTARIOS EXEMPLO 1-------------------------------------------------
% MES = 1214  885  < ic < 1487 Single (Resultado BRIAN_2015)
% MES encontrada Single = 1383... OK!
% MES = 945 para ret=200;
% MES =971 para ret=150
%************************************************************************
% % EXEMPLO 2 CODIFICADOR 16 PARA 4
% % DADOS ENTRADA EXEMPLO 2
% portas=9;
% ns = 4;
% r_in = 1;     % 1   11   21   31   41
% r_fim = 51;   % 10  20   30   40   51
% lin = 1; col =  2000; ret = 1000; %
% limite = 150000;% single
% vies=1;
%  lin = 1; col =  2000; ret = 2000; %   % SINGLE c = 2000;  % ic limite superior => 42278
%  %tabela = cria_tab3;% cria a tabela verdade
% nvar = 16;
% tabela = [ ...
% 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0;  ...
% 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1 0 0 0 1;  ...
% 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 2 0 0 1 0;  ...
% 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 3 0 0 1 1;  ...
% 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 4 0 1 0 0;  ...
% 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 5 0 1 0 1;  ...
% 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 6 0 1 1 0;  ...
% 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 7 0 1 1 1;  ...
% 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 8 1 0 0 0;  ...
% 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 9 1 0 0 1;  ...
% 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 10 1 0 1 0;  ...
% 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 11 1 0 1 1;  ...
% 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 12 1 1 0 0;  ...
% 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 13 1 1 0 1;  ...
% 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 14 1 1 1 0;  ...
% 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 15 1 1 1 1];  ...

%**************************************************************************
% EXEMPLO 3 DECODIFICADOR 4 PARA 16
%dados de entrada exemplo3:------------
% nvar = 4;
% mintermo = {[0], [1], [2], [3], [4], [5], [6], [7], [8], [9], [10], [11],...
%     [12], [13], [14], [15]};
% c = 1000; % Single => c=1000;
% lin = 1; col =  1000; ret = 1000;
% limite = 500000;% single
% vies=1;
% ns = size(mintermo,2); % numero de saídas do circuito
% tabela = cria_tab3;% cria a tabela verdade
% r_in = 1;     % 1   11   21   31   41
% r_fim = 10;   % 10  20   30   40   51
% pc=0;
%COMENTÁRIOS EXEMPLO 3: DECODIFICADOR 4 PARA 16----------------------------
%%% MES = 68,231; 63,266 < ic < 74,463 % Single(BRIAN_2015)
%%% MES encontrada Single 72,017 ... OK! (limite = 90000)
%testes
%niv=3; ae=126; runs =34; MES=52747;

%**************************************************************************
%EXEMPLO 4:MULTIPLICADOR DE 3 BITS => 6 ENTRADAS / 6 SAIDAS

% ---------Dados de entrada exemplo4:------------
% r_in = 1;     % 1   11   21   31   41
% r_fim = 51;   % 10  20   30   40   51
%
% nvar = 6;
% mintermo = {[54 55 61 62 63], ...% SAIDA 1
%     [30 31 36 37 38 39 44 45 46 51 52 53 59 60 63], ...% SAIDA 2
%     [20 21 22 23 27 28 29 34 35 38 39 42 43 45 46 50 52 53 55 58 60 62], ...% SAIDA 3
%     [12 13 14 15 18 19 22 23 26 28 29 31 33 35 37 39 41 43 44 46 49 50 53 54 57 58 59 60], ...% SAIDA 4
%     [10 11 14 15 17 19 21 23 25 26 29 30 42 43 46 47 49 51 53 55 57 58 61 62], ... % SAIDA 5
%     [9 11 13 15 25 27 29 31 41 43 45 47 55 59 61 63]}; % SAIDA 6
% c = 5000; % Single => c=5000;
%  lin = 1; col =  5000; ret = 5000; %   S1 (referencia  pc = 0)
% limite = 1000000;% single
% vies=1;
% pc = 0; % probabilidade de prioridade crescimento( na verdade prob de não decrescimo)
% ns = size(mintermo,2); % numero de saídas do circuito
% tabela = cria_tab3;% cria a tabela verdade

% COMENTÁROS EXEMPLO 4---------------------------------------------------
% MES = 399,911; 280,070 < ic < 516,034 single-Resultado BRIAN_2015
% MES encontrada ~449,093... OK!
%testes:
% 21 runs MES = 352753; => semeadura implemtação2 ret=500;
% 19 runs (ARVORE niv =6;ae=73; 4runs> limite de 700000) MES=280.771

% VARIÁVEIS DO PROGRAMA ***************************************************
portas=9;
% % ns = size(mintermo,2); % numero de saídas do circuito
% % %**************************************************************************
% % tabela = cria_tab3;% cria a tabela verdade
% % %**************************************************************************
tac=3*c + ns +1;
nof = c+nvar;
pnof=3*c;
tap=1;% tamanho da população
pop = zeros(tap,tac);
ym = [nvar+1:nof+ns]; % vetor mutação => contem as possibilidades p mutação
continf=0; % contagem de rodadas infactíveis
% INICIO RODADAS
runs = 500;
passo = (2^32-1)/(runs);
seed = 1:passo:2^32-1;
contnr = 1; % contador do numero de rodadas, pois nr não começa em 1
y_rodada={};
evolucao2 = []; % matriz evolucao - gurada nr, pont filho, pont pai, size filho, size pai
le =1;

%PPSN VIÉS PROBABILIDADE***************************************************
% PROBABILIDADE TROCA PORTAS**********************************************
% matriz transição probabilidade total_ portas (retirada da analise das mutaçoes beneficas)
% SOMA TODAS
tpit_p =   [0 13 10 3 13 10 36 5 10;
    11 0 6 8 3 53 13 5 2;
    10 6 0 37 27 5 3 6 5;
    1 8 22 0 10 46 0 7 6;
    11 5 41 16 0 16 0 8 3;
    0 23 16 27 7 0 9 11 7;
    29 31 3 5 5 12 0 7 8;
    13 19 11 19 10 10 14 0 4;
    19 10 13 6 20 12 12 7 0];
% FIM SOMA TODA
% criação da roleta onde a quantidade de cada elemento (1 2 3 4 5´=> 100,200,...500)
% são proporcionais aos valores de probabilidade da matriz transição
pp=[];
k=1;
for i=1:9
    %cont=1;
    k=1;
    for j=1:9
        if(tpit_p(i,j))>0
            cont=1;
            while cont<= tpit_p(i,j)
                pp(i,k)= j;
                cont=cont+1;
                k=k+1;
            end
        end
    end
end
pp1 = pp(1,1:99); % o arredondamento de round faz alguns vetores prob com 99 100, ou 101 termos
pp2 = pp(2,1:99); % para padronizar utilizamos todos com 99 termos
pp3 = pp(3,1:99);
pp4 = pp(4,1:99);
pp5 = pp(5,1:99);
pp6 = pp(6,1:99);
pp7 = pp(7,1:99);
pp8 = pp(8,1:99);
pp9 = pp(9,1:99);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% vies probabilidade factível
tpft_p =    [0 0 0 0 0 0 89 1 9;
    0 0 0 0 0 0 86 12 2;
    0 0 0 0 0 0 3 41 56;
    0 0 38 0 0 0 3 49 11;
    0 0 21 0 0 0 14 12 52;
    0 0 5 0 0 0 73 7 15;
    0 0 0 0 0 0 0 0 100;
    0 0 0 0 0 0 0 0 0;
    0 0 0 0 0 0 0 0 0];
ppf = zeros(9,9);
%ppf=[];
k=1;
for i=1:9
    %cont=1;
    k=1;
    for j=1:9
        if(tpft_p(i,j))>0
            cont=1;
            while cont<= tpft_p(i,j)
                ppf(i,k)= j;
                cont=cont+1;
                k=k+1;
            end
        end
    end
end
pp1f = ppf(1,1:99); % o arredondamento de round faz alguns vetores prob com 99 100, ou 101 termos
pp2f = ppf(2,1:99); % para padronizar utilizamos todos com 99 termos
pp3f = ppf(3,1:99);
pp4f = ppf(4,1:99);
pp5f = ppf(5,1:99);
pp6f = ppf(6,1:99);
pp7f = ppf(7,1:99);
pp8f = ppf(8,1:99);
pp9f = ppf(9,1:99);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% FIM VIÉS PROBABILIDADE***************************************************

for nr=r_in:r_fim
    
    rng(seed(nr)); %define a semente
    
    % INÍCIO DO AG ************************************************************
    
    % POPULAÇÃO INICIAL MATRICIAL*********************************************
    % pop = gera_ind(c,tac, lin, col, ret);% gera a indivíduo inicial => tap =1;
    qno = ret*lin;% quantidade nós retorno
    lim_ret = nvar+qno; % limite de nós retorno
    % se o nó é menor ou igual que "lim_ret" a ligação obedece a "randi(no-1)"
    
    pop = zeros(1,tac);  % tac = 3*c + ns + 1
    for j=3:3:3*c % varredura para inicialiar as portas e entradas portas
        pop(j) = 100*randi(portas);
        % pop(j) = 300;
    end
    
    for contc=1:ret
        no_i = (contc-1)*lin + nvar + 1;% nó inicio da "coluna"
        no_f = (contc*lin)+nvar; % nó final da "coluna" a ser preenchida
        pno_i =((contc-1)*lin)*3 + 3; % posicao nó inicial
        pno_f =((contc)*lin)*3;       % posicao nó final
        % no_is = no_i-qno; % nó inicio sorteio
        % no_fs = no_i-1; % nó fim sorteio
        %%%%no_fs = no_f-qno; % nó fim sorteio
        
        for k=pno_i:3:pno_f
            % % % %                       pop(k-1)=    no_i -qno + randi(qno);
            % % % %                       pop(k-2)=    no_i -qno + randi(qno);
            pop(k-1)= randi(no_i-1); % MANTEM RESTRIÇAO DE NAO LIGAR EM NOS DA MESMA LINHA
            pop(k-2)= randi(no_i-1);
        end
    end
    % % ABAIXO RETIRA RESTRIÇÃO "NAO LIGAR NA MESMA LINHA"
    %             for j=3:3:qno*3 % varredura para inicialiar as portas e entradas portas
    %                 no = j/3 + nvar;
    %                     pop(j-1)= randi(no-1);
    %                     pop(j-2)= randi(no-1);
    %         %             while ( pop(j-1)== pop(j-2))
    %         %             pop(j-2)= randi(nvar);
    %         %             end
    %                 %end
    %             end
    
    % for j=(qno*3)+3:(3*lin):(3*(lin-1))
    for contc=(ret+1):col
        no_i = (contc-1)*lin + nvar + 1;% nó inicio da "coluna"
        no_f = (contc*lin)+nvar; % nó final da "coluna" a ser preenchida
        pno_i =((contc-1)*lin)*3 + 3; % posicao nó inicial
        pno_f =((contc)*lin)*3;       % posicao nó final
        no_is = no_i-qno; % nó inicio sorteio
        no_fs = no_i-1; % nó fim sorteio
        %%%%no_fs = no_f-qno; % nó fim sorteio
        nos_ret = [no_is:no_fs];
        for k=pno_i:3:pno_f
            % % % %                       pop(k-1)=    no_i -qno + randi(qno);
            % % % %                       pop(k-2)=    no_i -qno + randi(qno);
            pop(k-1)=    nos_ret(randi(size(nos_ret,2)));
            pop(k-2)=    nos_ret(randi(size(nos_ret,2)));
        end
    end
    
    for i=1:ns % Determinação nó saída levando consideração retorno
        pop(pnof+i)= nof-(qno) + randi(qno);
    end
    % FIM POP INICIAL MATRICIAL************************************************
    
    % VARIAVEIS AUXILIARES PARA PREENCHIENTO DA MATRIZ S***********************
    % ' nof  sda des 1,...des ns' sd1...'sd ns' acerts'av'
    tac = 3*c + ns +1; % número de nós + ligação de cada saida + pont total
    nof = nvar +c;     % número (rótulo) do ultimo nó
    pnof = 3*c;        % posição do ultimo nó (posição na matriz pop)
    cs = nof + 2*ns +2; % numero de colunas matriz s
    s = zeros(2^nvar, cs);
    % Preenchimento da matriz s com as colunas comuns a todos os ind
    s(:,1:nvar) = tabela(:,1:nvar);
    for i=1:ns
        s(:,nof+i) = tabela(:,nvar+1+i);
    end
    % Preenchimento da matriz s com as colunas comuns a todos os ind
    s(:,1:nvar) = tabela(:,1:nvar);
    for i=1:ns
        s(:,nof+i) = tabela(:,nvar+1+i);
    end
    % Avaliação da população inicial*******************************************
    y = det_lig4(pop); % determinação dos nós ligantes
    pop = sim4(pop, tac, y);% avalia pop(ind,:)
    best = pop;
    na = 1;
    % FIM DA POP INICIAL E AVALIAÇÃO POP***************************************
    %INICIO DA MUTAÇÃO (Infactível)
    mfilho=zeros(4,tac);
    mutlig = zeros(4,8); % mutacao ligante --- [ nom gm gene_antigo  gene_novo]
    fac = 0;
    %while pop(1,tac)<(ns*2^nvar) && (na < limite);  %critério de parada; eqto infactivel e na < limite
    while (na < limite);
        % y = det_lig4(pop); %%%
        % VERIFICA SE É FACTÍVEL
        if pop(tac)>= ns*2^nvar
            fac=1;
        else
            fac=0;
        end
        for f=1:4
            mfilho(f,:) = pop;
            lig=0; % no mutado não é ligante => lig=0;
            while(lig==0) % lig=> flag para determinar qdo nó é ligante
                gm=randi(3);
                idmut = randi(size(ym,2));
                nom = ym(idmut);
                % TESTA SE NOM PERTENCE AOS LIGANTES***************************
                if any(nom==y)
                    lig=1;
                end
                
                while (gm==3 && lig ==1 && fac ==1 && nom <= nof && ...   
                                                      ( pop((nom - nvar)*3) == 700 || ...
                                                       pop((nom - nvar)*3) == 800 || ...
                                                       pop((nom - nvar)*3) == 900 ))
                    gm=randi(3);
                    idmut = randi(size(ym,2));
                    nom = ym(idmut);
                end
                % TESTA SE NOM PERTENCE AOS LIGANTES***************************
                if any(nom==y)
                    lig=1;
                end
                % % %             for k=1:size(y,2)
                % % %                 if y(k)== nom;
                % % %                     lig = 1;
                % % %                     break
                % % %                 end
                % % %             end
                % ANALISE DAS POSSIBILIDADES PARA A  MUTAÇÃO******************************
                if vies==1
                    
                    % 1 POSSIBILIDADE: nó mutado é ns (independente de ser FAC ou INFAC)
                    if (fac==0) && (nom > nof)
                        aa1 = nom;   aa2 = -2; aa3 = -2;  % alelos antigos 1,2,3
                        lig=1;
                        an1 = randi(nof);    % alelo novo1
                        an2 = -2;  an3 = -2; % alelo novo 2, 3
                        mfilho(f,pnof + nom-nof)= an1; %%%%????????pnof ou nof % MUTAÇÃO
                        
                        % 2 POSSIBILIDADE: Infactível && porta && não é saida
                        %elseif (fac==0) && (gm ==3) && (nom <= nof) %%%&&( tmut==0)  % infac && porta && não é saida &&  nao é no acoplamto
                        % Tem que colocar nom <= nof, pois não existe o termo filho(nom
                        %-nvar3)*3 quando nom>nof
                        % % %     aa1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo antigo 1
                        % % %     aa2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo antigo 2
                        % % %     aa3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo antigo 3
                        % % %     gm
                        % porta_ret =  mfilho(f,((nom - nvar)*3));
                    elseif (fac==0) && (gm == 3) && (nom <= nof) && (lig ==1) %%%&&( tmut==0)  % infac && porta && não é saida &&  nao é no acoplamto
                        switch pop((nom - nvar)*3)
                            case 100
                                mfilho(f,((nom - nvar)*3)) = 100*pp1(1,randi(99));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 200
                                mfilho(f,((nom - nvar)*3)) = 100*pp2(1,randi(99));
                                %filho((nom - nvar2)*3) = 100*pp2(1,randi(size(pp2,2)));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 300
                                mfilho(f,((nom - nvar)*3)) = 100*pp3(1,randi(99));
                                % filho((nom - nvar2)*3) = 100*pp3(1,randi(size(pp3,2)));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 400
                                mfilho(f,((nom - nvar)*3)) = 100*pp4(1,randi(99));
                                %filho((nom - nvar2)*3) = 100*pp4(1,randi(size(pp4,2)));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 500
                                mfilho(f,((nom - nvar)*3)) = 100*pp5(1,randi(99));
                                %                     start =  1;
                                %                     filho((nom - nvar2)*3) = 100*pp5(1,randi(size(pp5,2)));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 600
                                mfilho(f,((nom - nvar)*3)) = 100*pp6(1,randi(99));
                            case 700
                                mfilho(f,((nom - nvar)*3)) = 100*pp7(1,randi(99));
                            case 800
                                mfilho(f,((nom - nvar)*3)) = 100*pp8(1,randi(99));
                            case 900
                                mfilho(f,((nom - nvar)*3)) = 100*pp9(1,randi(99));
                            otherwise
                        end %switch
                        %    mfilho(f,((nom - nvar)*3)) = 100*randi(portas);
                        
                        % % % % % %     porta_entra = 100*randi(portas);
                        % % % % % %
                        % % % % % %     while (porta_entra == porta_ret)
                        % % % % % %         porta_entra = 100*randi(portas);
                        % % % % % %     end
                        % % % % % %     mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        
                        an1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo novo 1
                        an2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo novo 2
                        an3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo novo 3
                        
                        % 3 POSSIBILIDADE: Infactível && conexão && não é saida
                        %%% elseif (fac==0) && (gm<3) && (nom <= nof)% infactível && conexão && não é saida
                    elseif (fac==0) && (gm ==3) && (nom <= nof) && (lig ==0)
                        porta_ret =  mfilho(f,((nom - nvar)*3));
                        porta_entra = 100*randi(portas);
                        
                        while (porta_entra == porta_ret)
                            porta_entra = 100*randi(portas);
                        end
                        mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        
                    elseif (fac==0) && (gm<3) && (nom < nof)
                        aa1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo antigo 1
                        aa2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo antigo 2
                        aa3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo antigo 3
                        
                        mfilho(f,((nom-1-nvar)*3+gm))=randi(nom-1); % MUTAÇÃO
                        
                        an1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo novo 1
                        an2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo novo 2
                        an3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo novo 3
                        %mfilho(f,((nom-1-nvar)*3+gm))= 2/3*round(nom/3)-1 + randi(nom -(round(nom/3)));
                        
                        % FACTÍVEL
                        % 1 POSSIBILIDADE: nó mutado é ns (independente de ser FAC ou INFAC)
                    elseif (fac==1) && (nom > nof)
                        lig=1;
                        an1 = randi(nof);    % alelo novo1
                        mfilho(f,pnof + nom-nof)= an1; %%%%????????pnof ou nof % MUTAÇÃO
                        
                        % 2 POSSIBILIDADE: factível && porta && não é saida
                        %elseif (fac==0) && (gm ==3) && (nom <= nof) %%%&&( tmut==0)  % infac && porta && não é saida &&  nao é no acoplamto
                    elseif (fac==1) && (gm ==3) && (nom <= nof) && (lig ==1) %%%&&( tmut==0)  % infac && porta && não é saida &&  nao é no acoplamto
                        switch pop((nom - nvar)*3)
                            case 100
                                mfilho(f,((nom - nvar)*3)) = 100*pp1f(1,randi(99));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 200
                                mfilho(f,((nom - nvar)*3)) = 100*pp2f(1,randi(99));
                                %filho((nom - nvar2)*3) = 100*pp2(1,randi(size(pp2,2)));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 300
                                mfilho(f,((nom - nvar)*3)) = 100*pp3f(1,randi(99));
                                % filho((nom - nvar2)*3) = 100*pp3(1,randi(size(pp3,2)));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 400
                                mfilho(f,((nom - nvar)*3)) = 100*pp4f(1,randi(99));
                                %filho((nom - nvar2)*3) = 100*pp4(1,randi(size(pp4,2)));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 500
                                mfilho(f,((nom - nvar)*3)) = 100*pp5f(1,randi(99));
                                %                     start =  1;
                                %                     filho((nom - nvar2)*3) = 100*pp5(1,randi(size(pp5,2)));
                                %filho((nom - nvar2)*3) = randi(8);
                            case 600
                                mfilho(f,((nom - nvar)*3)) = 100*pp6f(1,randi(99));
                            case 700
                                % mfilho(f,((nom - nvar)*3)) = 100*pp7f(1,randi(99));
                                aaa=1;
                            case 800
                                % mfilho(f,((nom - nvar)*3)) = 100*pp8f(1,randi(99));
                                aaa=1;
                            case 900
                                % mfilho(f,((nom - nvar)*3)) = 100*pp9f(1,randi(99));
                                aaa=1;
                            otherwise
                        end %switch
                        %    mfilho(f,((nom - nvar)*3)) = 100*randi(portas);
                        
                        % % % % % %     porta_entra = 100*randi(portas);
                        % % % % % %
                        % % % % % %     while (porta_entra == porta_ret)
                        % % % % % %         porta_entra = 100*randi(portas);
                        % % % % % %     end
                        % % % % % %     mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        
                        an1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo novo 1
                        an2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo novo 2
                        an3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo novo 3
                        
                        % 3 POSSIBILIDADE: factível && conexão && não é saida
                        %%% elseif (fac==0) && (gm<3) && (nom <= nof)% factível && conexão && não é saida
                    elseif (fac==1) && (gm ==3) && (nom <= nof) && (lig ==0)
                        porta_ret =  mfilho(f,((nom - nvar)*3));
                        porta_entra = 100*randi(portas);
                        
                        while (porta_entra == porta_ret)
                            porta_entra = 100*randi(portas);
                        end
                        mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        
                    elseif (fac==1) && (gm<3) && (nom < nof)
                        aa1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo antigo 1
                        aa2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo antigo 2
                        aa3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo antigo 3
                        
                        mfilho(f,((nom-1-nvar)*3+gm))=randi(nom-1); % MUTAÇÃO
                        
                        an1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo novo 1
                        an2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo novo 2
                        an3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo novo 3
                        %mfilho(f,((nom-1-nvar)*3+gm))= 2/3*round(nom/3)-1 + randi(nom -(round(nom/3))
                        % EN POSSIBILIDADES FACTÍVEL
                        
                    else
                        % Não foi considerada a opção: FACTÍVEL && nom> nof
                        %afac=fac; agm = gm; anom = nom;
                    end%end  if (fac==0) && (gm==3)
                    
                else % desligndo vies************************************************
                    
                    % 1 POSSIBILIDADE: nó mutado é ns (independente de ser FAC ou INFAC)
                    if (fac==0) && (nom > nof)
                        aa1 = nom;   aa2 = -2; aa3 = -2;  % alelos antigos 1,2,3
                        lig=1;
                        an1 = randi(nof);    % alelo novo1
                        an2 = -2;  an3 = -2; % alelo novo 2, 3
                        mfilho(f,pnof + nom-nof)= an1; %%%%????????pnof ou nof % MUTAÇÃO
                        
                        % 2 POSSIBILIDADE: Infactível && porta && não é saida
                        %elseif (fac==0) && (gm ==3) && (nom <= nof) %%%&&( tmut==0)  % infac && porta && não é saida &&  nao é no acoplamto
                        % Tem que colocar nom <= nof, pois não existe o termo filho(nom
                        %-nvar3)*3 quando nom>nof
                        % % %     aa1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo antigo 1
                        % % %     aa2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo antigo 2
                        % % %     aa3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo antigo 3
                        % % %     gm
                        % porta_ret =  mfilho(f,((nom - nvar)*3));
                    elseif (fac==0) && (gm ==3) && (nom <= nof) && (lig ==1) %%%&&( tmut==0)  % infac && porta && não é saida &&  nao é no acoplamto
                        %  filho((nom - nvar2)*3) = 100* randi(portas);
                        porta_ret =  mfilho(f,((nom - nvar)*3));
                        porta_entra = 100*randi(portas);
                        
                        while (porta_entra == porta_ret)
                            porta_entra = 100*randi(portas);
                        end
                        mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        %    mfilho(f,((nom - nvar)*3)) = 100*randi(portas);
                        
                        % % % % % %     porta_entra = 100*randi(portas);
                        % % % % % %
                        % % % % % %     while (porta_entra == porta_ret)
                        % % % % % %         porta_entra = 100*randi(portas);
                        % % % % % %     end
                        % % % % % %     mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        
                        an1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo novo 1
                        an2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo novo 2
                        an3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo novo 3
                        
                        % 3 POSSIBILIDADE: Infactível && conexão && não é saida
                        %%% elseif (fac==0) && (gm<3) && (nom <= nof)% infactível && conexão && não é saida
                    elseif (fac==0) && (gm ==3) && (nom <= nof) && (lig ==0)
                        porta_ret =  mfilho(f,((nom - nvar)*3));
                        porta_entra = 100*randi(portas);
                        
                        while (porta_entra == porta_ret)
                            porta_entra = 100*randi(portas);
                        end
                        mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        
                    elseif (fac==0) && (gm<3) && (nom < nof)
                        aa1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo antigo 1
                        aa2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo antigo 2
                        aa3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo antigo 3
                        
                        mfilho(f,((nom-1-nvar)*3+gm))=randi(nom-1); % MUTAÇÃO
                        
                        an1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo novo 1
                        an2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo novo 2
                        an3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo novo 3
                        %mfilho(f,((nom-1-nvar)*3+gm))= 2/3*round(nom/3)-1 + randi(nom -(round(nom/3)));
                        
                        % FACTÍVEL
                        % 1 POSSIBILIDADE: nó mutado é ns (independente de ser FAC ou INFAC)
                    elseif (fac==1) && (nom > nof)
                        lig=1;
                        an1 = randi(nof);    % alelo novo1
                        mfilho(f,pnof + nom-nof)= an1; %%%%????????pnof ou nof % MUTAÇÃO
                        
                        % 2 POSSIBILIDADE: factível && porta && não é saida
                        %elseif (fac==0) && (gm ==3) && (nom <= nof) %%%&&( tmut==0)  % infac && porta && não é saida &&  nao é no acoplamto
                    elseif (fac==1) && (gm ==3) && (nom <= nof) && (lig ==1) %%%&&( tmut==0)  % infac && porta && não é saida &&  nao é no acoplamto
                        porta_ret =  mfilho(f,((nom - nvar)*3));
                        porta_entra = 100*randi(portas);
                        
                        while (porta_entra == porta_ret)
                            porta_entra = 100*randi(portas);
                        end
                        mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        %    mfilho(f,((nom - nvar)*3)) = 100*randi(portas);
                        
                        % % % % % %     porta_entra = 100*randi(portas);
                        % % % % % %
                        % % % % % %     while (porta_entra == porta_ret)
                        % % % % % %         porta_entra = 100*randi(portas);
                        % % % % % %     end
                        % % % % % %     mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        
                        an1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo novo 1
                        an2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo novo 2
                        an3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo novo 3
                        
                        % 3 POSSIBILIDADE: factível && conexão && não é saida
                        %%% elseif (fac==0) && (gm<3) && (nom <= nof)% factível && conexão && não é saida
                    elseif (fac==1) && (gm ==3) && (nom <= nof) && (lig ==0)
                        porta_ret =  mfilho(f,((nom - nvar)*3));
                        porta_entra = 100*randi(portas);
                        
                        while (porta_entra == porta_ret)
                            porta_entra = 100*randi(portas);
                        end
                        mfilho(f,((nom - nvar)*3)) = porta_entra; % MUTAÇÃO
                        
                    elseif (fac==1) && (gm<3) && (nom < nof)
                        aa1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo antigo 1
                        aa2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo antigo 2
                        aa3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo antigo 3
                        
                        mfilho(f,((nom-1-nvar)*3+gm))=randi(nom-1); % MUTAÇÃO
                        
                        an1 = mfilho(f,((nom-1-nvar)*3+1)); % alelo novo 1
                        an2 = mfilho(f,((nom-1-nvar)*3+2)); % alelo novo 2
                        an3 = mfilho(f,((nom-1-nvar)*3+3)); % alelo novo 3
                        %mfilho(f,((nom-1-nvar)*3+gm))= 2/3*round(nom/3)-1 + randi(nom -(round(nom/3))
                        % EN POSSIBILIDADES FACTÍVEL
                        
                    else
                        % Não foi considerada a opção: FACTÍVEL && nom> nof
                        %afac=fac; agm = gm; anom = nom;
                    end%end  if (fac==0) && (gm==3)
                end
                % fim vies desligado********************************************
                
                
            end %while(lig ==0)
            
            %%%end %nom=nvar+1:(nof+ns)
            if lig==1
                %[ nom gm gene_antigo  gene_novo]
                mutlig(f,1) = nom;
                mutlig(f,2) = gm;
                mutlig(f,3) = aa1; % aantigo1;
                mutlig(f,4) = aa2; % aantigo2;
                mutlig(f,5) = aa3; % aantigo3;
                mutlig(f,6) = an1; %  anovo1;
                mutlig(f,7) = an2; %  anovo1;
                mutlig(f,8) = an3; %  anovo1;
                
                yf = det_lig4(mfilho(f,:));
                mfilho(f,:) = sim4(mfilho(f,:), tac, yf);%
                %%% aa(f) = min(yf);%%%%
                na = na+1;
            end
        end% for f=1:4
        %%%  pos = find(aa==min(aa));
        [pm, posicao] = max(mfilho(:,tac)); % pontuação maxima e posicao do best
        bestfilho = mfilho(posicao,:);      % Define o melhor filho
        
        if  (bestfilho(tac)>= pop(tac))
            ybest = det_lig4(bestfilho);
            tpai = size(y,2);
            tbest = size(ybest,2);
        end
        
        if (bestfilho(tac)> pop(tac))
            evolucao2(le,1) = nr;
            evolucao2(le,2) = pop(tac);
            evolucao2(le,3) = bestfilho(tac);
            
            % calculo numero portas do pai********************************
            nportp = 0;
            for i=1:size(y,2)
                if (((pop(1,(y(i)-nvar)*3)) == 700) || ...; % quando r>2 analisar possivel modificação
                        ((pop(1,(y(i)-nvar)*3)) == 800) || ...
                        ((pop(1,(y(i)-nvar)*3)) == 900))
                    nportp = nportp;
                else
                    nportp=nportp+1;
                end
            end
            evolucao2(le,4) =  nportp; % tamanho do pai
            % fim calculo nportas pai******************************
            % calculo numero portas do bestfilho********************************
            nportf = 0;
            for i=1:size(ybest,2)
                if (((bestfilho(1,(ybest(i)-nvar)*3)) == 700) || ...; % quando r>2 analisar possivel modificação
                        ((bestfilho(1,(ybest(i)-nvar)*3)) == 800) || ...
                        ((bestfilho(1,(ybest(i)-nvar)*3)) == 900))
                    nportf = nportf;
                else
                    nportf=nportf+1;
                end
            end
            evolucao2(le,5) =  nportf; % numero portas bestfilho....
            % fim calculo nportas best filho******************************
            
            if pop(tac)>= ns*2^nvar
                evolucao2(le,6) = 1; % pai factível
            else
                evolucao2(le,6) = 0;
            end
            
            if bestfilho(tac)>= ns*2^nvar
                evolucao2(le,7) = 1; % filho factível
            else
                evolucao2(le,7) = 0;
            end
            evolucao2(le,8) = na;
            % primeiro individuo factível
            % % %     if  (pop(tac)< ns*2^nvar && bestfilho(tac)>= ns*2^nvar)
            % % %          evolucao2(le,8) = na;
            % % %          evolucao2(le,9) = nportp;
            % % %          evolucao2(le,10) = nportf;
            % % %     else
            % % %          evolucao2(le,8) = na;
            % % %          evolucao2(le,9) = 0;
            % % %          evolucao2(le,10) = 0;
            % % %     end
            
            
            %     evolucao2(le,6) = size(find(y>tpai),2);% quantos nós ligantes > nom
            %     evolucao2(le,7) = size(find(y<tpai),2);% quantos nós ligantes < nom
            
            %evolucao2(le,8:15) = mutlig(posicao,1:8);
            le=le+1;
        end
        teste2 = randi(10);
        
        if  (bestfilho(tac)> pop(tac)) % se a pontuação best é melhor
            pop = bestfilho; % best entra na população
            y=ybest;
            
            evolucao2
        elseif   (bestfilho(tac)==pop(tac)) && teste2 <= pc
            if  tbest >= tpai    % compara tamanho best e pai
                pop = bestfilho; % best entra na população
                y=ybest;
            end
            
        elseif   (bestfilho(tac)==pop(tac)) && teste2 > pc % cartesiana tradicional
            pop = bestfilho;
            y=ybest;
            
            % elseif (pop(tac)== (ns*2^nvar)-1 && bestfilho(tac) > pop(tac))
            %   pop = bestfilho;
            %   y=ybest;
        else
        end
        
        
        
        % CONTAGEM   NUMERO    NUMERO    PONTUACAO    MEDIANA             NUMERO
        %  RODADA    RODADA   AVALIAÇÃO     BEST      RODADA             LIGANTES
        
        % [contnr       nr      na        pop(tac)' median(na_rodada)   size(y,2)]
        
        
        if nr>r_in+1
            [contnr nr na pop(tac)' median(na_rodada) size(y,2)];
            [ na pop(tac)'];
        else
            [contnr nr na pop(tac)' size(y,2)];
            [ na pop(tac)'];
        end
        na;
        % if (mod(na,2)==0)
        %     [contnr nr na pop(tac)' size(y,2)]
        % end
        % %
        
    end% end while (na<limite)
    nr
    %end
    
    na_rodada(contnr,1)= na;         % numero de avaliaçoes da rodada
    nnos_rodada(contnr,1)=size(y,2); % número de nós do best de cada rodada
    best_rodada(contnr,:)= pop;      % melhor indivíduo de cada rodada
    y_rodada{[contnr]} = y;          % ligantes do melhor individuo de cada rodada
    save b1_evolucao
    
    
    % CONTAGEM   NUMERO    NUMERO    PONTUACAO    MEDIANA   NUMERO
    %  RODADA    RODADA   AVALIAÇÃO     BEST      RODADA   LIGANTES
    [contnr nr na pop(tac)' median(na_rodada) size(y,2)];
    
    if na >= limite
        continf=continf+1; % contagem das rodadas infactiveis
    end
    contnr = contnr+1;
end % end runs

