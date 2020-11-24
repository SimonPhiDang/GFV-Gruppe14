

a = [301 300.2 300.7 301.2 301.7 299.7 301.2 300.7 301.2 300.7]

mt = 300
ma = mean(a)
mi = max(a)

%systematisk fejl
Es = ma - mt
%Tilfældige fejl
Er = mi - ma

%% 


a2 = (a)';
pd = fitdist(a2,'Normal')

mu = 300.83;
s = 0.569698;

konfidens = s*1.96

A2min = min(a2)
A2max = max(a2)

x = 295:308;

y_norm = normpdf(x,mu,s);
plot(x,y_norm)

Konfidens95 = 100-konfidens
