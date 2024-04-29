clear
arduino = serialport("COM3", 9600);

j = read(arduino, 3, "uint8");
disp(j);
pause(0.5);
j(3) = (-(j(1).^2) + j(1) + j(2) + -(j(2).^2) );
i = 0;

while true
    h = read(arduino, 3, "uint8");
    h(3) = (-(h(1).^2) + h(1) + h(2) + -(h(2).^2) );
    j = [j;h];
    output = lightLevelGradient(j(:,1),j(:,2),j(:,3));
    i = i+1;
    disp(i);
    if (i >= 50) 
        writeline(arduino, output);
        i = 0;
    end
    pause(0.1);
end
