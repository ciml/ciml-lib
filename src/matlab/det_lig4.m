function y = det_lig4(pop2)

% % % % ATENÇÃO: NVAR2 FOI EXCLUIDO 
% % 
% % % det_lig3 fornece um veto y contendo os nós ligantes do individuo, det_lig
% % % fornecia um estrutura com os ligantes da população. Tinha que ser
% % % estrutura pois o tamanho do vetor ligante é diferente para cada
% % % indivíduo. Mas dete_lig3 é chamada para cada individuo e não pela
% % % população.
% % % DETECÇÃO DE QUAIS NÓS ESTÃO LIGADOS (são guradados no vetor v)**********
% % % concatena o vetor v de cada saida
% % % essa função recebe uma pop2 para identificar os nós ligantes, essa
% % % pop2 pode ser a inicial (pop) ou de qq geração.
% % global nvar ns portas nof pnof 
% % %global nvar nvar mintermo ns tabela portas  nof pnof 
% %     v =zeros(1,nof);%criação do vetor que guarda os nós a serem avaliados
% %     no = zeros(1,ns); % vetor que contem os nós output
% %     for t =1:ns
% %         no(t) = pop2(1,pnof+t); % saida a ser avaliada
% %         v(no(t)) = no(t);
% %         for i=no(t):-1:nvar+1
% % %             if(v(i)>0)
% % %                     if pop2(1,(i-nvar)*3)==100% | (pop2(1,(i-nvar2)*3)==500))
% % %                     v(pop2(1,(i-nvar)*3 -1)) = pop2(1,(i-nvar)*3 -1);
% % %                     else
% % %                     v(pop2(1,(i-nvar)*3 -1)) = pop2(1,(i-nvar)*3 -1);
% % %                     v(pop2(1,(i-nvar)*3 -2)) = pop2(1,(i-nvar)*3 -2);
% % %                     end
% % %             end
% % %             
% %             
% %                 if(v(i)>0)
% %                     v(pop2(1,(i-nvar)*3 -1)) = pop2(1,(i-nvar)*3 -1);
% %                     v(pop2(1,(i-nvar)*3 -2)) = pop2(1,(i-nvar)*3 -2);              
% %                 end
% %         end %end for
% %     end % ent t
% %     v(1,1:nvar)= (1:nvar); % preenchemos o vetor v de 1 a nvar; depois será deletado
% %     y2 = find(v); 
% %     y = y2(nvar+1:size(y2,2));% ARMAZENA OS NÓS LIGANTES DE CADA INDIVIDUO (sem entradas nvar)

%% ATENÇÃO: FOI ANALISADA PARA 9 PORTAS (FIO=1 E FIO = 0) FORAM EXCLUIDOS
% % ATENÇÃO: NVAR2 FOI EXCLUIDO 

% det_lig3 fornece um veto y contendo os nós ligantes do individuo, det_lig
% fornecia um estrutura com os ligantes da população. Tinha que ser
% estrutura pois o tamanho do vetor ligante é diferente para cada
% indivíduo. Mas dete_lig3 é chamada para cada individuo e não pela
% população.
% DETECÇÃO DE QUAIS NÓS ESTÃO LIGADOS (são guradados no vetor v)**********
% concatena o vetor v de cada saida
% essa função recebe uma pop2 para identificar os nós ligantes, essa
% pop2 pode ser a inicial (pop) ou de qq geração.
global nvar ns portas nof pnof 
%global nvar nvar mintermo ns tabela portas  nof pnof 
    v =zeros(1,nof);%criação do vetor que guarda os nós a serem avaliados
    no = zeros(1,ns); % vetor que contem os nós output
    for t =1:ns
        no(t) = pop2(1,pnof+t); % saida a ser avaliada
        v(no(t)) = no(t);
        for i=no(t):-1:nvar+1
            if(v(i)>0)
                    if ((pop2(1,(i-nvar)*3)==300 || (pop2(1,(i-nvar)*3)==700)))
                    v(pop2(1,(i-nvar)*3 -1)) = pop2(1,(i-nvar)*3 -1);
                    
                    elseif ((pop2(1,(i-nvar)*3)==800 || (pop2(1,(i-nvar)*3)==900)))%%mo
                    
                    elseif ((pop2(1,(i-nvar)*3)==100 || (pop2(1,(i-nvar)*3)==200)) ||...
                            (pop2(1,(i-nvar)*3)==400 || (pop2(1,(i-nvar)*3)==500)) || ...
                            (pop2(1,(i-nvar)*3)==600))
                        
                    v(pop2(1,(i-nvar)*3 -1)) = pop2(1,(i-nvar)*3 -1);
                    v(pop2(1,(i-nvar)*3 -2)) = pop2(1,(i-nvar)*3 -2);
                    else
                    end
            end
            
            
%                 if(v(i)>0)
%                     v(pop2(1,(i-nvar)*3 -1)) = pop2(1,(i-nvar)*3 -1);
%                     v(pop2(1,(i-nvar)*3 -2)) = pop2(1,(i-nvar)*3 -2);              
%                 end
        end %end for
    end % ent t
    v(1,1:nvar)= (1:nvar); % preenchemos o vetor v de 1 a nvar; depois será deletado
    y2 = find(v); 
    y = y2(nvar+1:size(y2,2));% ARMAZENA OS NÓS LIGANTES DE CADA INDIVIDUO (sem entradas nvar)







