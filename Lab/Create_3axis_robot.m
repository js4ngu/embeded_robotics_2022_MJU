function R = create_3axis_robot()
    %Create robot
    R = create_robot_structure(3)
    %Init Robot structure
    R.name = '60171851'
    %Set gravity
    R.gravity   = [0; 0; -9.80665];
    R.T_gto0    = [ 1 0 0 0;
                    0 1 0 0;
                    0 0 1 0;
                    0 0 0 1;];
    
    R.link(1).q_offset  = 90/180*pi;
    R.link(1).d         = 0.1;
    R.link(1).l         = 0;
    R.link(1).a         = -90/180*pi;

    R.link(2).q_offset  = 0;
    R.link(2).d         = 0.1;
    R.link(2).l         = 0.3;
    R.link(2).a         = 0;

    R.link(3).q_offset  = 0;
    R.link(3).d         = 0;
    R.link(3).l         = 0.3;
    R.link(3).a         = 0;