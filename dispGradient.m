function [output] = dispGradient(xCoords,yCoords, lightLevels)
    X = xCoords;
    Y = yCoords;
    Z = lightLevels;
    
    states = [X X.^2 Y Y.^2];

    coeff = states\Z;

    minX = min(X); maxX = max(X); deltaX = (maxX-minX)/20; 
    minY = min(Y); maxY = max(Y); deltaY = (maxY-minY)/20;

    [XX, YY] = meshgrid([minX:deltaX:maxX], [minY:deltaY:maxY]);

    ZZ = coeff(1).*XX + coeff(2).*XX.^2 + coeff(3).*YY + coeff(4).*YY.^2;
    
    %Demo
    figure(2)
    surf(XX,YY,ZZ)
    hold on;
    xlabel('X')
    ylabel('Y')
    zlabel('Light Level')

    fx = @(X) coeff(1) + 2.*coeff(2).*X;
    fy = @(Y) coeff(3) + 2.*coeff(4).*Y;

    xh = xCoords(end);
    yh = yCoords(end);
    oldzh = coeff(1).*xh + coeff(2).*xh.^2 + coeff(3).*yh + coeff(4).*yh.^2;
    diffzh = 0.01;

    h = 0.01;

    %Gradient Ascent (High Point)
    while diffzh > 0.00001 && diffzh < 0.5 %While the graph isn't rapidly declining
        xh = xh + h*fx(xh);
        yh = yh + h*fy(yh);
        zh = coeff(1).*xh + coeff(2).*xh.^2 + coeff(3).*yh + coeff(4).*yh.^2;
        diffzh = zh - oldzh;
        oldzh = zh;

        %Demo
        plot3(xh, yh, zh, 'x')
        drawnow;
    end

    xl = xCoords(end);
    yl = yCoords(end);
    oldzl = coeff(1).*xl + coeff(2).*xl.^2 + coeff(3).*yl + coeff(4).*yl.^2;
    diffzl = 0.01;

    %Gradient Descent (Low Point)
    while diffzl > 0.0001 && diffzl < 0.1 
        xl = xl - h*fx(xl);
        yl = yl - h*fy(yl);
        zl = coeff(1).*xl + coeff(2).*xl.^2 + coeff(3).*yl + coeff(4).*yl.^2;
        diffzl = oldzl - zl;
        oldzl = zl;

        %Demo
        plot3(xl, yl, zl, 'x')
        drawnow;
    end
    
    output = [xh, yh; xl, yl];
    
end