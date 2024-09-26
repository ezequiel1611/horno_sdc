clear all;
close all;
clc;
commandwindow;
pkg load control;

% Cargo los datos exportada de la RTDB
datos = load("datos.m");
plot(datos, 'linewidth',2);
grid 'on';
xlim([0,100]);
ylim([23,33]);
ylabel("Temperatura [°C]");
xlabel("Tiempo [s]");
title("HornoESP32");
