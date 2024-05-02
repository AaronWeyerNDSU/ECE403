clear
arduino = serialport("COM6", 9600);

j = str2num(readline(arduino));
disp(j);
pause(0.5);

while true
    h = str2num(readline(arduino));
    j = [j;h];
    disp("Longitude: " + j(:, 1));
    disp("Latitude: " + j(:, 2));
    disp("Brightness: " + j(:, 3));
    output = lightLevelGradient(j(:,1),j(:,2),j(:,3));
    disp("Brightest Point Coords: " + output(1) + ", " + output(3));
end
