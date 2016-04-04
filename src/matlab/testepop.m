%%
% IMPLEMENTAÇÃO MATRICIAL
clear all clc
% CRIAÇÃO DA POPULAÇÃO INICIAL
ns = 3; % numero de saídas do circuito
portas=4;
nvar=4;

  % CRIAÇÃO DA POPULAÇÃO INICIAL
    c = 24;
    lin = 4;   % numero de linhas da matriz
    col = 6;  % numero de colunas da matriz
    ret = 2;
    qno = ret*lin;% quantidade nós retorno
    lim_ret = nvar+qno; % limite de nós retorno
    
    tac = 3*c + ns +1; % número de nós + ligação de cada saida + pont total
nof = nvar +c;     % número (rótulo) do ultimo nó
pnof = 3*c;        % posição do ultimo nó (posição na matriz pop)

    
    % se o nó é menor ou igual que "lim_ret" a ligação obedece a "randi(no-1)"

    pop = zeros(1,tac);  % tac = 3*c + ns + 1
      for j=3:3:3*c % varredura para inicialiar as portas e entradas portas
            pop(j) = 100*randi(portas);
      end
    
        for j=3:3:qno*3 % varredura para inicialiar as portas e entradas portas
            no = j/3 + nvar;
                pop(j-1)= randi(nvar);
                pop(j-2)= randi(nvar);
    %             while ( pop(j-1)== pop(j-2))
    %             pop(j-2)= randi(no-1);
    %             end      
            %end
        end

        % for j=(qno*3)+3:(3*lin):(3*(lin-1)) 
           for contc=(ret+1):col
              no_i = (contc-1)*lin + nvar + 1;% nó inicio da "coluna" 
              no_f = (contc*lin)+nvar; % nó final da "coluna" a ser preenchida
              pno_i =((contc-1)*lin)*3 + 3; % posicao nó inicial
              pno_f =((contc)*lin)*3;       % posicao nó final
              no_is = no_i-qno; % nó inicio sorteio
              no_fs = no_f-qno; % nó fim sorteio

              for k=pno_i:3:pno_f
                  pop(k-1)= no_i -qno + randi(qno);
                  pop(k-2)= no_i -qno + randi(qno);
              end
           end

            for i=1:ns
            pop(pnof+i) = randi(nvar+c); 
            end




% se o nó é menor ou igual que "lim_ret" a ligação obedece a "randi(no-1)"
% % % 
% % % pop = zeros(1,tac);  % tac = 3*c + ns + 1
% % %     for j=3:3:3*c % varredura para inicialiar as portas e entradas portas
% % %         no = j/3 + nvar;
% % %         pop(j) = 100*randi(portas);  
% % %         % pop(j)=500;
% % %         if(no <=lim_ret)
% % %             pop(j-1)= randi(no-1);
% % %             pop(j-2)= randi(no-1);
% % %             while ( pop(j-1)== pop(j-2))
% % %             pop(j-2)= randi(no-1);
% % %             end      
% % %         end
% % %     end
% % %     
% % %     % for j=(qno*3)+3:(3*lin):(3*(lin-1)) 
% % %        for contc=(ret+1):col
% % %            no_is = (contc-ret)*lin + nvar; % nó inicio sorteio
% % %            no_fs = (contc-1)*lin + nvar; % nó fim sorteio
% % %            pno_ip = (contc-1)*lin +3; % posicao no inicio preenchimento
% % %            pno_fp = (contc)*lin; % posicao no fim preenchimento
% % %            xx = [nvar no_is:no_fs];
% % %          for k = pno_ip:3:pno_fp
% % %             index1 = randi(size(xx,2));
% % %             index2 = randi(size(xx,2));
% % %             pop(k-1) = xx(index1);
% % %             pop(k-2) = xx(index2);
% % %              while ( pop(k-1)== pop(k-2))
% % %             index2 = randi(size(xx,2));
% % %             pop(k-2) = xx(index2);
% % %              end 
% % %          end
% % %      end
% % %         %%%end% end if
% % %     %%%end % end 3:3:3c fim varredura inicializa portas e ent1 ent2
% % %   %  x = (nvar+c) - ns  + randperm(ns);
% % %     for i=1:ns
% % %         pop(pnof+i) = randi(nvar+c);
% % % %       pop(ind, pnof+i) = randi(nvar+c);     
% % %      end
% % %   %  pop(pnof+1:pnof+ns) = x;
% % %    % pop(pnof+1) = randi(c+nvar);
% % % pop
% % % 
% % % 


