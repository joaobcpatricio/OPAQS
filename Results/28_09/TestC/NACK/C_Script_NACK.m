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

%% Treating Delivery Ratio case C
close all; clear;clc; 
Nnodes=3;   %N� nodes generating msgs every second
maxT_sen=91;    %Time when nodes stop generating msgs+1
%getting file

Ft_recNACK = fopen('timeRec02.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
t_recNACK = fscanf(Ft_recNACK,formatSpec);
fclose(Ft_recNACK);

%
count=1:1:length(t_recNACK);
i=1;
j=1;

while i<length(t_recNACK)+1
    j=fix(t_recNACK(i))+1;
    if fix(t_recNACK(i))>=maxT_sen
        j=maxT_sen;
    end
    drNACK(i)=count(i)/(Nnodes*j)*100;
    i=i+1;
end
display('End calc DR')
plot(t_recNACK,drNACK)
xlabel('Elapsed time (s)');
ylabel('Delivery Ratio (%)');
title('Test B');
legend('LQE-NACK');
grid on;

%

hold off;
savefig('plotC_DR_NACK')
%%
%Calculate delay
%

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
meanD_NACK=mean(delayNACK);

%% intervalo de confiança METHOD 1
figure
x=1:1:length(delayNACK);                            % Create Independent Variable
y = delayNACK;                                      % Create Dependent Variable ‘Experiments’ Data
N = size(delayNACK)                               % Number of ‘Experiments’ In Data Set
yMean = mean(y);                                    % Mean Of All Experiments At Each Value Of ‘x’
ySEM = std(y)/sqrt(N(2));                              % Compute ‘Standard Error Of The Mean’ Of All Experiments At Each Value Of ‘x’
CI95 = tinv([0.025 0.975], N(2)-1);                    % Calculate 95% Probability Intervals Of t-Distribution
yCI95 = bsxfun(@times, ySEM, CI95(:));              % Calculate 95% Confidence Intervals Of All Experiments At Each Value Of ‘x’
figure
plot(1, yMean)                                      % Plot Mean Of All Experiments
hold on
plot(2, yCI95+yMean)                                % Plot 95% Confidence Intervals Of All Experiments
hold off
grid
%errorbar( length(delayNACK)-1, mean(delayNACK), confidence_intervals( delayNACK, 95 ) )
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

% we can say with 95% confidence that the true mean lies between these two
% values.

% categories: Data analysis
%% % %

display('Mean Delay:')
meanD_NACK
%mean delay
figure;
bar(1,meanD_NACK)
xlabel('Gateway Node ID');
ylabel('Network Mean Delay (s)');
legend('LQE-NACK');
title('Test C');
grid on;  
% % %
hold off;
savefig('plotC_delayB_NACK')


% Delay through time
figure;

plot(t_recNACK,delayNACK)
xlabel('Elapsed time (s)');
ylabel('Delay (s)');
legend('LQE-NACK');
title('Test C');
grid on; 
% % %
hold off;
savefig('plotC_delay_NACK')


%% Calculate Redudant Overhead 
clear; clc;


% % %
% Node[0]
n0_sentNACK=textread('dataNameSent00.txt','%s');
i=1;
j=1;
countNACK=0;
while i<length(n0_sentNACK)
    j=i;
    while j<length(n0_sentNACK)
        if (strcmp(n0_sentNACK(i),n0_sentNACK(j)) && i~=j && ~strcmp(n0_sentNACK(i),{''}))
            n0_sentNACK(i);          
            countNACK=countNACK+1;
            n0_sentNACK(j)={''};
        end       
        j=j+1;
    end
    i=i+1;
end
countNACK
% Node[1]
n1_sentNACK=textread('dataNameSent01.txt','%s');
i=1;
j=1;
countNACK1=0;
while i<length(n1_sentNACK)
    j=i;
    while j<length(n1_sentNACK)
        if (strcmp(n1_sentNACK(i),n1_sentNACK(j)) && i~=j && ~strcmp(n1_sentNACK(i),{''}))
            n1_sentNACK(i);          
            countNACK1=countNACK1+1;
            n1_sentNACK(j)={''};
        end       
        j=j+1;
    end
    i=i+1;
end
countNACK1
% Node[0]
n3_sentNACK=textread('dataNameSent03.txt','%s');
i=1;
j=1;
countNACK3=0;
while i<length(n3_sentNACK)
    j=i;
    while j<length(n3_sentNACK)
        if (strcmp(n3_sentNACK(i),n3_sentNACK(j)) && i~=j && ~strcmp(n3_sentNACK(i),{''}))
            n3_sentNACK(i);          
            countNACK=countNACK+1;
            n3_sentNACK(j)={''};
        end       
        j=j+1;
    end
    i=i+1;
end
countNACK3

countTNACK=countNACK+countNACK1+countNACK3;
% % %
figure
bar(1,countTNACK)
xlabel('Forwarding strategies)');
ylabel('Overhead (n�)');
legend('LQE-NACK');
title('Test C');
grid on; 
hold off;
savefig('plotC_overhead_NACK')

%% 
% Calculate LQE between nodes[0]->[2]
%
fLQE_time = fopen('LQEwtime00_02.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
LQE_time = fscanf(fLQE_time,formatSpec);
fclose(fLQE_time);
%
fLQE_w = fopen('LQEweight00_02.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
LQE_w = fscanf(fLQE_w,formatSpec);
fclose(fLQE_w);
%
%count3=0:1:length(t_sent)-1;
figure;
plot(LQE_time,LQE_w)
xlabel('Elapsed time (s)');
ylabel('LQE quality (%)');
legend('LQE-NACK');
title('Test C: links nodes 0-2');
grid on;
hold off;
savefig('plotC_LQE0_2')
