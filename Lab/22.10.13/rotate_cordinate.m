function T = rotate_cordinate(axis, q)
    C = cos(q);
    S = sin(q);

    if axis == 'x'
        R = [ 1 0 0;
              0 C -S;
              0 S C; ];
    elseif axis == 'y'
        R = [ C 0 S;
              0 1 0;
             -S 0 C; ];
    elseif axis == 'z'
        R = [ C -S 0;
              S  C 0;
              0  0 1;];
    else
        error('Wrong axis');
    end

    T = [R          zeros(3,1);
         zeros(1,3) 1;          ];