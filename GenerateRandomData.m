clear all
close all
clc

%generate random data for testing purposes
 X = rand(100, 1);
 Y = rand(100, 1); 
 err = 1+0.7.*rand(100,1); % add some random noise
 act_coef = [.82, .45, 1.33, .5];
 Z = (-X.^2 + X + Y + -Y.^2 ) .* err;
 
figure(1)
plot3(X,Y,Z,'*')
xlabel('X')
ylabel('Y')
zlabel('Light Level')
grid on


