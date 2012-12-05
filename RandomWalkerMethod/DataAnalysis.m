% Programmer:  James L. Armes
%
% Written to plot data generated from C++

clear all; clc;

Data(:,1) = csvread('10N100W.csv', 1, 0);
Data(:,2) = csvread('10N1000W.csv', 1, 0);
Data(:,3) = csvread('10N5000W.csv', 1, 0);
Data(:,4) = csvread('10N25000W.csv', 1, 0);
Data(:,5) = csvread('10N50000W.csv', 1, 0);

Data(:,6) = csvread('50N100W.csv', 1, 0);
Data(:,7) = csvread('50N1000W.csv', 1, 0);
Data(:,8) = csvread('50N5000W.csv', 1, 0);
Data(:,9) = csvread('50N25000W.csv', 1, 0);
Data(:,10) = csvread('50N50000W.csv', 1, 0);

Data(:,11) = csvread('100N100W.csv', 1, 0);
Data(:,12) = csvread('100N1000W.csv', 1, 0);
Data(:,13) = csvread('100N5000W.csv', 1, 0);
Data(:,14) = csvread('100N25000W.csv', 1, 0);
Data(:,15) = csvread('100N50000W.csv', 1, 0);

Data(:,16) = csvread('250N100W.csv', 1, 0);
Data(:,17) = csvread('250N1000W.csv', 1, 0);
Data(:,18) = csvread('250N5000W.csv', 1, 0);
Data(:,19) = csvread('250N25000W.csv', 1, 0);
Data(:,20) = csvread('250N50000W.csv', 1, 0);

Data(:,21) = csvread('500N100W.csv', 1, 0);
Data(:,22) = csvread('500N1000W.csv', 1, 0);
Data(:,23) = csvread('500N5000W.csv', 1, 0);
Data(:,24) = csvread('500N25000W.csv', 1, 0);
Data(:,25) = csvread('500N50000W.csv', 1, 0);


for i = 1:min(size(Data))
   averages(i) = mean(Data(:,i)); 
   stds(i) = std(Data(:,i));
end

% 100 Walkers
avg_T_100 = [averages(1) averages(6) averages(11) averages(16) averages(21)];
std_100 = [stds(1) stds(6) stds(11) stds(16) stds(21)];

% 1000 Walkers
avg_T_1000 = [averages(2) averages(7) averages(12) averages(17) averages(22)];
std_1000 = [stds(2) stds(7) stds(12) stds(17) stds(22)];

% 25000 Walkers, function of average temperature and nodes
avg_T_25000 = [averages(4) averages(9) averages(14) averages(19) averages(24)];
std_25000 = [stds(4) stds(9) stds(14) stds(19) stds(24)];

nodes = [10 50 100 250 500];
figure(1); hold on; grid on; 

errorbar(nodes,avg_T_100,std_100,'ro-', 'LineWidth', 2);
errorbar(nodes,avg_T_1000,std_1000,'ko-', 'LineWidth', 2);
errorbar(nodes,avg_T_25000,std_25000,'o-', 'LineWidth', 2);

xlabel('Number of Nodes'); ylabel('Average Temperature of 50 trials, K');
legend('100 Walkers', '1000 Walkers', '25000 Walkers');

% Plot 2
figure(2); hold on; grid on; xlabel('Trial #'); ylabel('Temperature, K');
std_1 = ones(1,50);
std_2 = ones(1,50);

std_1 = std_1 * stds(21);
std_2 = std_2 * stds(25);

errorbar(1:50, Data(:,21), std_1, 'ro-', 'LineWidth', 2); 
errorbar(1:50, Data(:,25), std_2, 'ko-', 'LineWidth', 2); 

legend('100 Walkers', '50000 Walkers');

% Plot 3 -Effect of # of Walkers 250 nodes, 
avg_T_Walkers = averages(21:25);
std_Walkers = stds(21:25);
figure(3); hold on; grid on;
xlabel('Random Walkers'); ylabel('Average Temperature, K');
errorbar([100 1000 5000 25000 50000], avg_T_Walkers, std_Walkers, 'ko-', 'LineWidth', 2);