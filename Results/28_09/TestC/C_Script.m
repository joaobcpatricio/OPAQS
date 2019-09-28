%% Results Analysis %%
%% João Patrício, NºMEC 76330 %%

clc;
clear all;
close all;

display('João Patrício ')
display('MIEET, DETI, UA')
display(' ')

%% biblio
%https://www.mathworks.com/help/matlab/ref/fscanf.html

%% Treating Delivery Ratio case C
close all; clear;clc; 
Nnodes=3;   %Nº nodes generating msgs every second
maxT_sen=91;    %Time when nodes stop generating msgs+1
%getting file
Ft_rec = fopen('C_timeRecEpidemic.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
t_rec = fscanf(Ft_rec,formatSpec);
fclose(Ft_rec);
%
count=0:1:length(t_rec)-1;
i=1;
j=1;

while i<length(t_rec)+1
    j=fix(t_rec(i))+1;
    if fix(t_rec(i))>maxT_sen
        j=maxT_sen;
    end
    dr(i)=count(i)/(Nnodes*j)*100;
    i=i+1;
end
display('End calc DR')
plot(t_rec,dr)
xlabel('Elapsed time (s)');
ylabel('Delivery Ratio (%)');
title('Test C');
legend('Epidemic');
grid on;  
hold on;
% % %
Ft_recNACK = fopen('C_timeRecNACK.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
t_recNACK = fscanf(Ft_recNACK,formatSpec);
fclose(Ft_recNACK);

%
count=0:1:length(t_recNACK)-1;
i=1;
j=1;

while i<length(t_recNACK)+1
    j=fix(t_recNACK(i))+1;
    if fix(t_recNACK(i))>maxT_sen
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
legend('EPIDEMIC','LQE-NACK');
grid on;

%

hold off;
savefig('plotC_DR')
%%
%Calculate delay
%
Ft_sent = fopen('C_timeSentEpidemic.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
t_sent = fscanf(Ft_sent,formatSpec);
fclose(Ft_sent);
%
count2=0:1:length(t_sent)-1;
i=1;
j=1;

while i<length(t_sent)+1
    delay(i)=t_rec(i)-t_sent(i);
    i=i+1;
end
display('End calc delay')
meanD=mean(delay);
display('Mean Delay:')
meanD
%mean delay
figure
bar(0,meanD)
xlabel('Gateway Node ID');
ylabel('Network Mean Delay (s)');
legend('Epidemic');
title('Test C');
grid on;  
% % %
hold on;
Ft_sentNACK = fopen('C_timeSentNACK.txt','r');    %Name of the file with the receveid times
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
display('Mean Delay:')
meanD_NACK
%mean delay
bar(1,meanD_NACK)
xlabel('Gateway Node ID');
ylabel('Network Mean Delay (s)');
legend('Epidemic','LQE-NACK');
title('Test C');
grid on;  
% % %
hold off;
savefig('plotC_delayB')


% Delay through time
figure;
%plot(count2,delay)
plot(t_rec,delay)
xlabel('Elapsed time (s)');
ylabel('Delay (s)');
legend('Epidemic');
title('Test C');
grid on;  
% % %
hold on;
plot(t_recNACK,delayNACK)
xlabel('Elapsed time (s)');
ylabel('Delay (s)');
legend('Epidemic','LQE-NACK');
title('Test C');
grid on; 
% % %
hold off;
savefig('plotC_delay')


%% Calculate Redudant Overhead 
clear; clc;

% Node[0]
n0_sent=textread('C_N0_sentEpidemic.txt','%s');
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
n1_sent=textread('C_N1_sentEpidemic.txt','%s');
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
% Node[0]
n0_sentNACK=textread('C_N0_sentNACK.txt','%s');
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
n1_sentNACK=textread('C_N1_sentNACK.txt','%s');
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

countTNACK=countNACK+countNACK1
% % %
figure
bar(0,countT)
hold on;
bar(1,countTNACK)
xlabel('Forwarding strategies)');
ylabel('Overhead (nº)');
legend('Epidemic','LQE-NACK');
title('Test C');
grid on; 
hold off;
savefig('plotC_overhead')

%% 
% Calculate LQE between nodes[0]->[2]
%
fLQE_time = fopen('C_LQEtime0_2.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
LQE_time = fscanf(fLQE_time,formatSpec);
fclose(fLQE_time);
%
fLQE_w = fopen('C_LQEw0_2.txt','r');    %Name of the file with the receveid times
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
