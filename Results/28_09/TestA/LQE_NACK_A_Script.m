%% Results Analysis %%
%% Jo�o Patr�cio, N�MEC 76330 %%

clc;
clear all;
close all;

display('Jo�o Patr�cio ')
display('MIEET, DETI, UA')
display(' ')

%% biblio
%https://www.mathworks.com/help/matlab/ref/fscanf.html

%% Treating Delivery Ratio case A
close all; clear;clc; 
Nnodes=2;   %N� nodes generating msgs every second
%getting file

Ft_recNACK = fopen('timeRec02.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
t_recNACK = fscanf(Ft_recNACK,formatSpec);
fclose(Ft_recNACK);
%
count=0:1:length(t_recNACK)-1;
i=1;
j=1;

while i<length(t_recNACK)+1
   
    drNACK(i)=count(i)/(Nnodes*100)*100;
    i=i+1;
end
display('End calc DR')
plot(t_recNACK,drNACK)
xlabel('Elapsed time (s)');
ylabel('Delivery Ratio (%)');
title('Test A');
legend('LQE-NACK');
grid on;

%

hold off;
savefig('plotA_DR')
%
%Calculate delay
%
figure;
Ft_sentNACK = fopen('timeSent02.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
t_sentNACK = fscanf(Ft_sentNACK,formatSpec);
fclose(Ft_sentNACK);
%
count3=0:1:length(t_sentNACK)-1;
i=1;
j=1;

while i<length(t_sentNACK)+1
    delayNACK(i)=t_recNACK(i)-t_sentNACK(i);
    i=i+1;
end
display('End calc delay')
meanD_NACK=mean(delayNACK)
% % % %
%% % % METHOD 2
%http://matlab.cheme.cmu.edu/2011/08/27/introduction-to-statistical-data-analysis/
ybar = mean(delayNACK)
s = std(delayNACK)
ci = 0.95;
alpha = 1 - ci;

n = length(delayNACK); %number of elements in the data vector
T_multiplier = tinv(1-alpha/2, n-1)
% the multiplier is large here because there is so little data. That means
% we do not have a lot of confidence on the true average or standard
% deviation
ci95 = T_multiplier*s/sqrt(n)

% confidence interval
sprintf('The confidence interval is %1.1f +- %1.1f',ybar,ci95)
[ybar - ci95, ybar + ci95]

% %% %

display('Mean Delay:')
meanD_NACK
%mean delay
bar(1,meanD_NACK)
xlabel('Gateway Node ID');
ylabel('Network Mean Delay (s)');
legend('LQE-NACK');
title('Test A');
grid on;  
% % %
hold off;
savefig('plotA_delayB')


% Delay through time
figure;

plot(t_recNACK,delayNACK)
xlabel('Elapsed time (s)');
ylabel('Delay (s)');
legend('LQE-NACK');
title('Test A');
grid on; 
% % %
hold off;
savefig('plotA_delay')


%% Calculate Redudant Overhead 
clear; clc;

% Node[0]
n0_sent=textread('dataNameSent00.txt','%s');
i=1;
j=1;
count=0;
while i<length(n0_sent)
    j=i;
    while j<length(n0_sent)
        if (strcmp(n0_sent(i),n0_sent(j)) && i~=j && ~strcmp(n0_sent(i),{''}))
            n0_sent(i);          
            count=count+1;
            n0_sent(j)={''};
        end       
        j=j+1;
    end
    i=i+1;
end
count
% Node[1]
n1_sent=textread('dataNameSent01.txt','%s');
i=1;
j=1;
count1=0;
while i<length(n1_sent)
    j=i;
    while j<length(n1_sent)
        if (strcmp(n1_sent(i),n1_sent(j)) && i~=j && ~strcmp(n1_sent(i),{''}))
            n1_sent(i);          
            count1=count1+1;
            n1_sent(j)={''};
        end       
        j=j+1;
    end
    i=i+1;
end
count1

countT=count+count1


% % %
figure
bar(0,countT)

xlabel('Forwarding strategies)');
ylabel('Overhead (n�)');
legend('LQE-NACK');
title('Test A');
grid on; 
hold off;
savefig('plotA_overhead')
