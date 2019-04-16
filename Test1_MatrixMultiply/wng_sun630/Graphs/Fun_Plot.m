function Fun_Plot

numP = [1,2,4,8,16];
t_rowDistri = [27.2757, 13.4578, 6.5548, 4.09674, 1.97949];
t_mkl = [1.24298, 0.42297, 0.287631, 0.214842, 0.127659];

numP_cannon = [1, 4, 16];
t_cannon = [27.4036, 6.29411, 1.86352];

%%
figure(1);
clf('reset');

plot(numP, t_rowDistri,'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','k');
hold on;
plot(numP_cannon, t_cannon,'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','r');
plot(numP, t_mkl,'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color',[30 144 255]/255);

set(gca,'fontsize',16);
xlabel('Number of processors','FontSize',16);
ylabel('Execution time ( s )','FontSize',16);
legend('Row distributed','Cannon', 'MKL' ,"Location","northeast");
hold off;
savefig(1,'Fig_ExecutionTime_ConstantSize.fig');

%%
t_sequen = 18.1071;
figure(2);
clf('reset');

plot(numP, t_sequen./t_rowDistri,'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','k');
hold on;
plot(numP_cannon, t_sequen./t_cannon,'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','r');

set(gca,'fontsize',16);
xlabel('Number of processors','FontSize',16);
ylabel('Speedup','FontSize',16);
legend('Row distributed','Cannon',"Location","northwest");
hold off;
savefig(2,'Fig_Speedup_ConstantSize.fig');

%%
figure(3);
clf('reset');

plot(numP, t_sequen./t_rowDistri./numP,'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','k');
hold on;
plot(numP_cannon, t_sequen./t_cannon./numP_cannon,'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','r');

set(gca,'fontsize',16);
xlabel('Number of processors','FontSize',16);
ylabel('Efficiency','FontSize',16);
legend('Row distributed','Cannon',"Location","northeast");
hold off;
savefig(3,'Fig_Efficiency_ConstantSize.fig');

figure(31);
clf('reset');

plot(numP, t_rowDistri.*numP-t_sequen,'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','k');
hold on;
plot(numP_cannon, t_cannon.*numP_cannon-t_sequen,'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','r');

set(gca,'fontsize',16);
xlabel('Number of processors','FontSize',16);
ylabel('Total Overhead T0','FontSize',16);
legend('Row distributed','Cannon',"Location","northeast");
hold off;
savefig(31,'Fig_Overhead_ConstantSize.fig');

%%
figure(4);
clf('reset');

plot(numP, (t_rowDistri./t_sequen-1./numP)./(1-1./numP),'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','k');
hold on;
plot(numP_cannon, (t_cannon./t_sequen-1./numP_cannon)./(1-1./numP_cannon),'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','r');

set(gca,'fontsize',16);
xlabel('Number of processors','FontSize',16);
ylabel('Karp-Flatt e','FontSize',16);
legend('Row distributed','Cannon',"Location","northeast");
hold off;
savefig(4,'Fig_KFe_ConstantSize.fig');

%% Scaled Problem Size
numP = [1,2,4,8,16];
t_rowDistri = [2.34665	0.951703	0.689691	0.375425	0.167259;
    17.4985	9.16869	5.21004	2.90943	1.54816];

numP_cannon = [1, 4, 16];
t_cannon = [2.25013		0.55816		0.172257;
    18.2531		4.77207		1.3632];
figure(11);
clf('reset');

plot(numP, t_rowDistri(1,:),'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','k');
hold on;
plot(numP_cannon, t_cannon(1,:),'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','r');

plot(numP, t_rowDistri(2,:),'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','k');
hold on;
plot(numP_cannon, t_cannon(2,:),'linestyle','-','marker','o','linewidth',2,'MarkerSize',3,'color','r');

set(gca,'fontsize',16);
xlabel('Number of processors','FontSize',16);
ylabel('Execution time ( s )','FontSize',16);
legend('Row distributed','Cannon' ,"Location","northeast");
hold off;
savefig(11,'Fig_ExecutionTime_ScaledSize.fig');
end