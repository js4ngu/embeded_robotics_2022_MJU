function T = translate_coordinate(axis, d)
    if axis == 'X'
        P = [d;
             0;
             0];
    elseif axis == 'Y'
        P = [0;
             d;
             0];
    elseif axis == 'Z'
        P = [0;
             0;
             d];

    else
        error('wrong AXIS')
    end

    T = [zeros(3,3), P;
         zeros(1,3), 1];