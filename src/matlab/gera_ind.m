        function pop = gera_ind(c,tac,lin,col,ret)
        %-----------------------------------------------------------------
        % IMPLEMENTAÇÃO MATRICIAL
        global ns portas nvar pnof nof
          % CRIAÇÃO DA POPULAÇÃO INICIAL
        qno = ret*lin;% quantidade nós retorno
        lim_ret = nvar+qno; % limite de nós retorno
        % se o nó é menor ou igual que "lim_ret" a ligação obedece a "randi(no-1)"
    
        pop = zeros(1,tac);  % tac = 3*c + ns + 1
          for j=3:3:3*c % varredura para inicialiar as portas e entradas portas
                pop(j) = 100*randi(portas);
          end
        
% % % %             for j=3:3:qno*3 % varredura para inicialiar as portas e entradas portas
% % % %                 no = j/3 + nvar;
% % % %                     pop(j-1)= randi(no-1);
% % % %                     pop(j-2)= randi(no-1);
% % % %         %             while ( pop(j-1)== pop(j-2))
% % % %         %             pop(j-2)= randi(nvar);
% % % %         %             end      
% % % %                 %end
% % % %             end
    
            
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
                        pop(k-1)= randi(no_i-1);
                        pop(k-2)= randi(no_i-1);
                  end
               end
            
            
            
            
            % for j=(qno*3)+3:(3*lin):(3*(lin-1)) 
               for contc=(ret+1):col
                  no_i = (contc-1)*lin + nvar + 1;% nó inicio da "coluna" 
                  no_f = (contc*lin)+nvar; % nó final da "coluna" a ser preenchida
                  pno_i =((contc-1)*lin)*3 + 3; % posicao nó inicial
                  pno_f =((contc)*lin)*3;       % posicao nó final
                  no_is = no_i-qno; % nó inicio sorteio
                  no_fs = no_i-1; % nó fim sorteio
                  %%%%no_fs = no_f-qno; % nó fim sorteio
    
                  for k=pno_i:3:pno_f
% % % %                       pop(k-1)=    no_i -qno + randi(qno);
% % % %                       pop(k-2)=    no_i -qno + randi(qno);
                        pop(k-1)=    no_is + randi(qno);
                        pop(k-2)=    no_is + randi(qno);
                  end
               end
               
                for i=1:ns % Determinação nó saída levando consideração retorno
                 pop(pnof+i)= nof-qno + randi(qno);
                end


