function tabela = cria_tab()
global nvar mintermo ns
%cria a tabela verdade referente ao circuito  a ser projetado
% Cria um vetor com o peso de cada bit
nl =1;
termo = zeros(1,nvar);
for j= 1:nvar;
    termo(nvar-j+1) = 2^(j-1); % termo => peso cada bit
    nl = nl + termo(nvar-j+1); % soma os pesos = numero linhas tab
end
% cria a tabela verdade
tabela = zeros(nl, nvar+1+ns); 
%=>            nl,entradas + um + zero + decimal +saidas;

% Cria a tabela verdade usando as conversoes - elementos char
tab_true = zeros(2^nvar,nvar);
for t=1:nl
    tab_true(t,:)= dec2bin(t-1, nvar);
end

% percorre a tabela verdade de caracter e preenche a tab verdade numerica
for i=1:nl
    for j=1:nvar
        if(tab_true(i,j) == '1')
            tabela(i,j) = 1;
        end
       tabela(i, nvar+1) = i-1;  % coloca o numero do mintermo !! +2
    end          
end
%tabela(:,nvar+1) =1; % entrada "tudo_um"
% preenche  a saida da tabela verdade com os mintermo
for n=1:ns
    for i =1:size(mintermo{n},2)
        tabela(mintermo{n}(i)+1, nvar+1+n) = 1;
    end
end
tabela;
%FIM DA CRIAÇAO TABELA VERDADE*******************************************
end

