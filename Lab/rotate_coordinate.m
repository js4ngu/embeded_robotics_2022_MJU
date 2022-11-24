function T = rotate_coordinate(axis, q)
    C = cos(q);
    S = sin(q);

    if axis == 'X'
        R = [ 1 0 0;
              0 C -S;
              0 S C; ];
    elseif axis == 'Y'
        R = [ C 0 S;
              0 1 0;
             -S 0 C; ];
    elseif axis == 'Z'
        R = [ C -S 0;
              S  C 0;
              0  0 1;];
    else
        error('Wrong axis');
    end

    T = [R          zeros(3,1);
         zeros(1,3) 1;          ];