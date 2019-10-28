%% Calculate Redudant Overhead 
close all; clear;clc; 
Nnodes=5;   %N� nodes generating msgs every second
maxT_sen=100;    %Time when nodes stop generating msgs+1


%% 

% Node[0]
%n0_sentNACK=textread('GwTimeRec.txt','%s');
name_rec=textread('ReachedGwName.txt','%s');
%
time_recC = fopen('GwTimeRec.txt','r');    %Name of the file with the receveid times
formatSpec = '%f';
time_rec = fscanf(time_recC,formatSpec);
fclose(time_recC);
%
%
i=1;
j=1;
countNACK=0;
gg=0;
while i<length(name_rec)
    j=i;
    while j<length(name_rec)
        if (strcmp(name_rec(i),name_rec(j)) && i~=j && ~strcmp(name_rec(i),{''}))
            %name_rec(i);          
            countNACK=countNACK+1;
            %name_rec(j)={''};
            %display('if');
            
            time_rec(j)=[];
            name_rec(j)=[];
        %end 
        else
            j=j+1;
        end
        %display('while2');
    end
    i=i+1;
    %display('while1');
end
display('ok');
%
count=1:1:length(time_rec);
i=1;
j=1;
while i<length(time_rec)+1
    j=fix(time_rec(i))+1;
    if fix(time_rec(i))>=maxT_sen
        j=maxT_sen;
    end
    drGW(i)=count(i)/(Nnodes*j)*100;
    i=i+1;
end
display('End calc DR')
plot(time_rec,drGW)
xlabel('Elapsed time (s)');
ylabel('Delivery Ratio (%)');
title('Test C');
legend('LQE-NACK');
grid on;
%
hold off;
savefig('plotC_DR_GW')
display('Success:');
drGW(length(drGW))
display('Data Overhead:');
countNACK

%% 
%Verificar nº de gws
% % %
% Node[0]
%n0_sentNACK=textread('GwTimeRec.txt','%s');
name_recs=textread('ReachedGwSpcs.txt','%s');
%
name_recss=name_recs;
name_recsDoub=str2double(name_recs);
%
i=1;
j=1;
countREP=0;
gg=0;
%gw=1:1:Nnodes;
while i<length(name_recs)
    j=i;
    while j<length(name_recs)
        if (strcmp(name_recs(i),name_recs(j)) && i~=j && ~strcmp(name_recs(i),{''}))
            %name_rec(i);          
            countREP=countREP+1;    %counts how many times theres a repetition
            name_recs(j);
            %i
            %gw(name_recsDoub(i)+1)=gw(name_recsDoub(i)+1)+1
            name_recs(j)=[];
            name_recsDoub(j)=[];
           % gw(name_recs(i))=gw(name_recs(i))+1
        %end       
        else
            j=j+1;
        %display('while2');
        end
    end
    i=i+1;
    %display('while1');
end
display('Number of gws:');
length(name_recs)-1

%% Plot gw sending over time
figure
%0
gw_rec0=textread('GwTimeRec00.txt','%s');
gw_re0=str2double(gw_rec0);
countgw_re0=1:1:length(gw_rec0);
plot(gw_re0, countgw_re0);
hold on;
%1
gw_rec1=textread('GwTimeRec01.txt','%s');
gw_re1=str2double(gw_rec1);
countgw_re1=1:1:length(gw_rec1);
plot(gw_re1, countgw_re1);
hold on;
% 2
gw_rec2=textread('GwTimeRec02.txt','%s');
gw_re2=str2double(gw_rec2);
countgw_re2=1:1:length(gw_rec2);
plot(gw_re2, countgw_re2);
hold on;
% %3
% gw_rec3=textread('GwTimeRec03.txt','%s');
% gw_re3=str2double(gw_rec3);
% countgw_re3=1:1:length(gw_rec3);
% plot(gw_re3, countgw_re3);
% hold on;
% %4
% gw_rec4=textread('GwTimeRec04.txt','%s');
% gw_re4=str2double(gw_rec4);
% countgw_re4=1:1:length(gw_rec4);
% plot(gw_re4, countgw_re4);
% hold on;
% %5
% gw_rec5=textread('GwTimeRec05.txt','%s');
% gw_re5=str2double(gw_rec5);
% countgw_re5=1:1:length(gw_rec5);
% plot(gw_re5, countgw_re5);
% hold on;
%6
% gw_rec6=textread('GwTimeRec06.txt','%s');
% gw_re6=str2double(gw_rec6);
% countgw_re6=1:1:length(gw_rec6);
% plot(gw_re6, countgw_re6);
% hold on;
% %7
% gw_rec7=textread('GwTimeRec07.txt','%s');
% gw_re7=str2double(gw_rec7);
% countgw_re7=1:1:length(gw_rec7);
% plot(gw_re7, countgw_re7);
% hold on;
% %8
% gw_rec8=textread('GwTimeRec08.txt','%s');
% gw_re8=str2double(gw_rec8);
% countgw_re8=1:1:length(gw_rec8);
% plot(gw_re8, countgw_re8);
% hold on;
% % %9
% gw_rec9=textread('GwTimeRec09.txt','%s');
% gw_re9=str2double(gw_rec9);
% countgw_re9=1:1:length(gw_rec9);
% plot(gw_re9, countgw_re9,'Linewidth',2);
% hold on;
% %10
% gw_rec10=textread('GwTimeRec10.txt','%s');
% gw_re10=str2double(gw_rec10);
% countgw_re10=1:1:length(gw_rec10);
% plot(gw_re10, countgw_re10,'Linewidth',2);
% hold on;
%end
xlabel('Elapsed time (s)');
ylabel('GW Msgs sent');
title('Test C');
legend('Node0', 'Node1', 'Node2');
% legend('Node0','Node1','Node2','Node3');
savefig('GwsNo')
%%
figure
%0
gw_rec0=textread('GwTimeRec00.txt','%s');
gw_re0=str2double(gw_rec0);
countgw_re0=zeros(length(gw_rec0),1);
scatter(gw_re0, countgw_re0, 'filled');
hold on;
%1
gw_rec1=textread('GwTimeRec01.txt','%s');
gw_re1=str2double(gw_rec1);
countgw_re1=1+zeros(length(gw_rec1),1);
scatter(gw_re1, countgw_re1, 'filled');
hold on;
%2
gw_rec2=textread('GwTimeRec02.txt','%s');
gw_re2=str2double(gw_rec2);
countgw_re2=2+zeros(length(gw_rec2),1);
scatter(gw_re2, countgw_re2, 'filled');
hold on;
% %3
% gw_rec3=textread('GwTimeRec03.txt','%s');
% gw_re3=str2double(gw_rec3);
% countgw_re3=3+zeros(length(gw_rec3),1);
% scatter(gw_re3, countgw_re3, 'filled');
% hold on;
% %4
% gw_rec4=textread('GwTimeRec04.txt','%s');
% gw_re4=str2double(gw_rec4);
% countgw_re4=4+zeros(length(gw_rec4),1);
% scatter(gw_re4, countgw_re4, 'filled');
% hold on;

xlabel('Elapsed time (s)');
ylabel('GW ID');
title('Test C');
legend('Node0','Node1','Node2');
% legend('Node0','Node1','Node2','Node3');
grid on
grid minor
savefig('GesTemp')

%
axis([0 110 0 3])
set(gca,'ytick',[0 1 2])

%%
%sum of hops
gw_hop=textread('GwMsgHops.txt','%s');
gw_hops=str2double(gw_hop);

h=1;
sum=0;
while h<length(gw_hops)+1
    sum=sum+gw_hops(h);
    h=h+1;
end
display('Total hops:');
sum