%%
% resultados exemplos ppsn 
clc
clear all
load b1_evolucao
n1=1;
n2=1;
for i=1: size(evolucao2,1)
    if evolucao2(i,6)==0 && evolucao2(i,7)==1
        res(n1,:)= evolucao2(i,:);
        n1=n1+1;
    end
     if evolucao2(i,6)==0 && evolucao2(i,7)== 0 && i>1  && ...
        evolucao2(i-1,6)==01 && evolucao2(i-1,7)== 1
    res2(n2,:) = evolucao2(i-1,:);
    n2=n2+1;
     end
    
end
nfact = size(res,1);
mean (res2(:,5))