clear;clc;
[X_100,X_108,X_119,X_162,X_199,X_219,X_239,X_285]=wyniki();


%Pojemnoœæ zmierzona
C_100=X_100(:,1);
C_108=X_108(:,1);
C_119=X_119(:,1);
C_162=X_162(:,1);
C_199=X_199(:,1);
C_219=X_219(:,1);
C_239=X_239(:,1);
C_285=X_285(:,1);

%Czasy
t_100=X_100(:,2);
t_108=X_108(:,2);
t_119=X_119(:,2);
t_162=X_162(:,2);
t_199=X_199(:,2);
t_219=X_219(:,2);
t_239=X_239(:,2);
t_285=X_285(:,2);

%pojemnosc po korekcji
C_p_100=80.77;
C_p_108=110.20;
C_p_119=113.61;
C_p_162=162.29;
C_p_199=199.72;
C_p_219=219.41;
C_p_239=243.00;
C_p_285=282.70;

%C dokladne
v_100=100.06;
v_108=108.94;
v_119=119.82;
v_162=162.97;
v_199=199.94;
v_219=219.65;
v_239=239.97;
v_285=285.85;

%b³¹d bezwzglêdny wartosci zmierzonej
bb_100=abs(v_100-C_100);
bb_108=abs(v_108-C_108);
bb_119=abs(v_119-C_119);
bb_162=abs(v_162-C_162);
bb_199=abs(v_199-C_199);
bb_219=abs(v_219-C_219);
bb_239=abs(v_239-C_239);
bb_285=abs(v_285-C_285);

%b³¹d wzglêdny wartosci zmierzonej
bw_100=100*bb_100/v_100;
bw_108=100*bb_108/v_108;
bw_119=100*bb_119/v_119;
bw_162=100*bb_162/v_162;
bw_199=100*bb_199/v_199;
bw_219=100*bb_219/v_219;
bw_239=100*bb_239/v_239;
bw_285=100*bb_285/v_285;

%b³¹d bezwzglêdny wartosci po korekcji
bb_p_100=abs(v_100-C_p_100);
bb_p_108=abs(v_108-C_p_108);
bb_p_119=abs(v_119-C_p_119);
bb_p_162=abs(v_162-C_p_162);
bb_p_199=abs(v_199-C_p_199);
bb_p_219=abs(v_219-C_p_219);
bb_p_239=abs(v_239-C_p_239);
bb_p_285=abs(v_285-C_p_285);

%b³¹d wzglêdny wartosci po korekcji
bw_p_100=100*bb_p_100/v_100;
bw_p_108=100*bb_p_108/v_108;
bw_p_119=100*bb_p_119/v_119;
bw_p_162=100*bb_p_162/v_162;
bw_p_199=100*bb_p_199/v_199;
bw_p_219=100*bb_p_219/v_219;
bw_p_239=100*bb_p_239/v_239;
bw_p_285=100*bb_p_285/v_285;


%wartosci srednie
m_C_100=mean(C_100);
m_C_108=mean(C_108);
m_C_119=mean(C_119);
m_C_162=mean(C_162);
m_C_199=mean(C_199);
m_C_219=mean(C_219);
m_C_239=mean(C_239);
m_C_285=mean(C_285);

m_t_100=mean(t_100);
m_t_108=mean(t_108);
m_t_119=mean(t_119);
m_t_162=mean(t_162);
m_t_199=mean(t_199);
m_t_219=mean(t_219);
m_t_239=mean(t_239);
m_t_285=mean(t_285);

m_bb_100=mean(bb_100);
m_bb_108=mean(bb_108);
m_bb_119=mean(bb_119);
m_bb_162=mean(bb_162);
m_bb_199=mean(bb_199);
m_bb_219=mean(bb_219);
m_bb_239=mean(bb_239);
m_bb_285=mean(bb_285);

m_bw_100=mean(bw_100);
m_bw_108=mean(bw_108);
m_bw_119=mean(bw_119);
m_bw_162=mean(bw_162);
m_bw_199=mean(bw_199);
m_bw_219=mean(bw_219);
m_bw_239=mean(bw_239);
m_bw_285=mean(bw_285);

%wykresy
%blad bezwzgledny wartosci zmierzonej w funkcji wartosci wartosci dokladnej
figure(1);
plot(v_100,bb_100,'r.');
hold on;
plot(v_108,bb_108,'b.');
plot(v_119,bb_119,'k.');
plot(v_162,bb_162,'g.');
plot(v_199,bb_199,'m.');
plot(v_219,bb_219,'y.');
plot(v_239,bb_239,'c.');
plot(v_285,bb_285,'g.');
xlabel('Wartosc dokladna[pF]');
ylabel('Blad bezwzgledny[pF]');
hold off;

%wartosc zmierzona pojemnosci w funkcji wartosci dokladnej
figure(2);
plot(v_100,m_C_100,'rx');
hold on;
plot(v_108,m_C_108,'rx');
plot(v_119,m_C_119,'rx');
plot(v_162,m_C_162,'rx');
plot(v_199,m_C_199,'rx');
plot(v_219,m_C_219,'rx');
plot(v_239,m_C_239,'rx');
plot(v_285,m_C_285,'rx');
xlabel('Wartosc dokladna pojemnosci[pF]');
ylabel('Wartosc zmierzona pojemnosci[pF]');
hold off;

%blad wzgledny dla wartosci zmierzonej w funkcji wartosci dokladnej
figure(3);
plot(v_100,m_bw_100,'bx');
hold on;
plot(v_108,m_bw_108,'bx');
plot(v_119,m_bw_119,'bx');
plot(v_162,m_bw_162,'bx');
plot(v_199,m_bw_199,'bx');
plot(v_219,m_bw_219,'bx');
plot(v_239,m_bw_239,'bx');
plot(v_285,m_bw_285,'bx');
xlabel('Wartosc dokladna pojemnosci[pF]');
ylabel('blad wzgledny[%]');
hold off;

%wartosc zmierzona pojemnosci po korekcji w funkcji wartosci dokladnej
figure(4);
plot(v_100,C_p_100,'mx');
hold on;
plot(v_108,C_p_108,'mx');
plot(v_119,C_p_119,'mx');
plot(v_162,C_p_162,'mx');
plot(v_199,C_p_199,'mx');
plot(v_219,C_p_219,'mx');
plot(v_239,C_p_239,'mx');
plot(v_285,C_p_285,'mx');
xlabel('Wartosc dokladna pojemnosci[pF]');
ylabel('Wartosc pojemnosci po korekcji[pF]');
hold off;

%blad wzgledny dla wartosci zmierzonej po korekcji w funkcji wartosci dokladnej
figure(5);
plot(v_100,bw_p_100,'kx');
hold on;
plot(v_108,bw_p_108,'kx');
plot(v_119,bw_p_119,'kx');
plot(v_162,bw_p_162,'kx');
plot(v_199,bw_p_199,'kx');
plot(v_219,bw_p_219,'kx');
plot(v_239,bw_p_239,'kx');
plot(v_285,bw_p_285,'kx');
xlabel('Wartosc dokladna pojemnosci[pF]');
ylabel('blad wzgledny[%]');
hold off;

saveas(figure(1),'rozstrzal bledu.png');
saveas(figure(2),'pojemnosc(dokladna).png');
saveas(figure(3),'b_pojemnosc(dokladna).png');
saveas(figure(4),'korekcja(dokladna).png');
saveas(figure(5),'b_korekcja(dokladna).png');