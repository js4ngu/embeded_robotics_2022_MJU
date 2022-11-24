%2022.12.20
function compute_forward_kinematics(R)
    n = R.n;
    if n < 0 || n > 10
        error('Wrong dimension');
    end

    T_gtokm1 = R.T_gto0;
    for k = 1:n
        q = R.link(k).q + R.link(k).q_offset;
        d = R.link(k).d;
		R.link(k).l;
		R.link(k).a;

        R.link(k).T_km1tok = homogeneous_tranfrormation(q,d,l,a);
        R.link(k).T_kmitok = T_kmitok;
        R.link(k).T_gtok = T_gtokm1 * T_km1tok;

        R.link(k)._gtokm1 = R.link(k).T_gtok;
    end
end