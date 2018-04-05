% % % % % function bloke = sim4(bloke, tac, y)
% % % % % global s ns nof nvar
% % ATEN��O: NVAR2 FOI EXCLUIDO 

% SIMULADOR - VERS�O 3
% Recebe um individuo e os n�s ligantes(vetor y) e retorna o individuo
% avaliado; Nessa vers�o a avalia��o come�a a partir do y(start). No caso
% da popula��o inicial come�a a partir do primeiro n� ligante; ou seja nom
% = y(1); Quando a muta��o for de conex�o tamb�m come�a y(1). Apenas quando
% o individuo for infact�vel e a muta��o for porta podemos fazer
% y(start)~=y(1)
% % % % % for i=1:size(y,2) % seleciona o indice para varrer o vetor dos ligantes
% % % % %     pt = bloke(( y(i) - nvar)*3); % seleciona a porta correspondente ao y(i)
% % % % %     switch pt
% % % % % % %         case 100
% % % % % % %             s(:,y(i)) = s(:,bloke((y(i)-nvar)*3 - 1));
% % % % %         case 100
% % % % %             s(:,y(i)) = s(:,bloke((y(i)-nvar)*3 - 1))& ...
% % % % %                 s(:,bloke((y(i)-nvar)*3 - 2));
% % % % %         case 200
% % % % %             s(:,y(i)) = s(:,bloke((y(i)-nvar)*3 - 1))| ...
% % % % %                 s(:,bloke((y(i)-nvar)*3 - 2));
% % % % %         case 300
% % % % %             s(:,y(i)) = not(s(:,bloke((y(i)-nvar)*3 - 1))& ...
% % % % %                 s(:,bloke((y(i)-nvar)*3 - 2)));
% % % % %         case 400
% % % % %             s(:,y(i)) = not(s(:,bloke((y(i)-nvar)*3 - 1))| ...
% % % % %                 s(:,bloke((y(i)-nvar)*3 - 2)));
% % % % %         otherwise
% % % % %     end % end switch
% % % % % end
% % % % % % PREENCHENDO A COLUNA REFERENTE A SAIDA ENCONTRADA(apenas copiando a saida
% % % % % %do n� output para a coluna nof + ns +1 ...)
% % % % % as = zeros(1,ns); % acerto de cada saida
% % % % % for i=1:ns
% % % % %     s(:,nof+ns+i) = s(:,bloke((nof-nvar)*3+i));
% % % % %     as(i) = sum(not(xor(s(:,nof+i),s(:,nof+ns+i))));
% % % % % end
% % % % % af = sum(as); % acerto final = soma acerto de cada saida
% % % % %     bloke(tac) = af;
% % % % % end
function bloke = sim4(bloke, tac, y)
global s ns nof nvar
% % ATEN��O: NVAR2 FOI EXCLUIDO

% SIMULADOR - VERS�O 3
% Recebe um individuo e os n�s ligantes(vetor y) e retorna o individuo
% avaliado; Nessa vers�o a avalia��o come�a a partir do y(start). No caso
% da popula��o inicial come�a a partir do primeiro n� ligante; ou seja nom
% = y(1); Quando a muta��o for de conex�o tamb�m come�a y(1). Apenas quando
% o individuo for infact�vel e a muta��o for porta podemos fazer
% y(start)~=y(1)
for i=1:size(y,2) % seleciona o indice para varrer o vetor dos ligantes
    pt = bloke(( y(i) - nvar)*3); % seleciona a porta correspondente ao y(i)
    switch pt
        case 100
            s(:,y(i)) = s(:,bloke((y(i)-nvar)*3 - 1)).* ...
                s(:,bloke((y(i)-nvar)*3 - 2));
        case 200
            s(:,y(i)) = s(:,bloke((y(i)-nvar)*3 - 1))+ ...
                s(:,bloke((y(i)-nvar)*3 - 2));
        case 300
            s(:,y(i)) = (s(:,bloke((y(i)-nvar)*3 - 1)));
        case 400
            s(:,y(i)) = not(s(:,bloke((y(i)-nvar)*3 - 1)).* ...
                s(:,bloke((y(i)-nvar)*3 - 2)));
           
        case 500
            s(:,y(i)) = not(s(:,bloke((y(i)-nvar)*3 - 1))+ ...
                s(:,bloke((y(i)-nvar)*3 - 2)));
            
        case 600
            s(:,y(i)) = xor(s(:,bloke((y(i)-nvar)*3 - 1)), ...
                s(:,bloke((y(i)-nvar)*3 - 2)));
        case 700
            s(:,y(i)) = (s(:,bloke((y(i)-nvar)*3 - 1)));
        case 800
            s(:,y(i)) = ones(2^nvar,1);
        case 900
            s(:,y(i)) = zeros(2^nvar,1);           
        otherwise
    end % end switch
end
% PREENCHENDO A COLUNA REFERENTE A SAIDA ENCONTRADA(apenas copiando a saida
%do n� output para a coluna nof + ns +1 ...)
as = zeros(1,ns); % acerto de cada saida


% % % %     s(:,nof+ns+1:nof+ns+ns) = s(:,bloke((nof-nvar)*3+1 : (nof-nvar)*3+ns));
% % % %     as(1:ns) = sum(not(xor(s(:,nof+1:nof+ns),s(:,nof+ns+1:nof+ns+ns))));


for i=1:ns
    s(:,nof+ns+i) = s(:,bloke((nof-nvar)*3+i));
    as(i) = sum(not(xor(s(:,nof+i),s(:,nof+ns+i))));
end
af = sum(as); % acerto final = soma acerto de cada saida
%bloke(tac) = af;
qf = 0; % quantidade de Wire 

if (af < ns*(2^nvar)) % if o individuo � infact�vel
    bloke(tac) = af;
else
    for i=1:size(y,2)
        if ((bloke((y(i)-nvar)*3)) == 700 || ...            
            (bloke((y(i)-nvar)*3)) == 800 ||...
            (bloke((y(i)-nvar)*3)) == 900)
        
            qf = qf+1;
        end
    end
    bloke(tac) = (af + (nof-nvar - size(y,2)) +qf);
     
end
end


