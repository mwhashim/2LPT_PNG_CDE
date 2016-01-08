from __future__ import division
from numpy import *
from scipy.integrate import quad
from scipy.interpolate import interp1d, UnivariateSpline, InterpolatedUnivariateSpline
from scipy.misc import derivative

from matplotlib import *
from pylab import *


#--: Cosmology !!
Omega = 0.308 ; OmegaLambda = 0.692 

#----------------------
k_piv_camb = 100;  dx_value = 1e-5
#--: Redshift !!
Redshift = linspace(0,30,101)
Redshift_1 = linspace(0,30,1001)
Redshift_num = range(103)[1:-1][::-1]

a = 1/(1 + Redshift)
z_0 = 101

#--: Data loading !!
Tl_k = {}; gl_z = []; gl_z_k = []; Tq_k = {}
for z in Redshift_num:
    k, Tl_k[z] = loadtxt('data_array/lcdm_transfer_%s.dat' %z, unpack=True,usecols = [0,1])
    gl_z_k.append(Tl_k[z]/Tl_k[z_0])
    gl_z.append(Tl_k[z][k_piv_camb]/Tl_k[z_0][k_piv_camb])

Dl_z = array(gl_z); Dl_z_2 = 3/7 * (array(gl_z))**2

order = 5
Dl_z_interp = InterpolatedUnivariateSpline(Redshift, Dl_z, k=order)
Dl_z_interp_2 = InterpolatedUnivariateSpline(Redshift, Dl_z_2,k=order)

Dl_z_prim = derivative(Dl_z_interp, Redshift, dx = dx_value)
fl_z = - (1/(a * Dl_z_interp(Redshift))) * Dl_z_prim

Dl_z_prim_2 = derivative(Dl_z_interp_2, Redshift , dx = dx_value)
fl_z_2 = - (1/(a * Dl_z_interp_2(Redshift))) * Dl_z_prim_2

a = a[::-1]; Dl_z = Dl_z[::-1]; fl_z = fl_z[::-1]; fl_z_2 = fl_z_2[::-1]

#--: Growth function data file !!
Omega_a = Omega/(Omega + a * (1 - Omega - OmegaLambda) + a * a * a * OmegaLambda)
Hubble_a = sqrt(Omega/(a * a * a) + (1 - Omega - OmegaLambda) / (a * a) + OmegaLambda)
growth_int = lambda a: pow(a/(Omega + a * (1 - Omega - OmegaLambda) + a * a * a * OmegaLambda), 1.5)

Dl_zz =  [Hubble_a[i] * quad(growth_int, 0, a[i])[0] for i in range(len(a))]
fl_zz = pow(Omega_a, 0.6); fl_zz_2 = 2 * pow(Omega_a, 4./7.)

#--: Data Writing !!
f = open('growth.dat', 'w')
for i in range(len(a)):
    f.write('%s   %s    %s    %s    %s\n' %(a[i], Hubble_a[i], Dl_z[i], fl_z[i], fl_z_2[i]))

f.close()


figure()
plot(a, Dl_z, '-')
plot(a, Dl_zz, '--')

figure()
plot(a, fl_z, '-')
plot(a, fl_zz, '--')

figure()
plot(a, fl_z_2, '-')
plot(a, fl_zz_2, '--')

show()





#----------------------
#vel_prefac= 1720.42, vel_prefac2= 3440.84,  hubble_a=9579.31 fom=0.999955
#vel_prefac= 1679.59, vel_prefac2= 3359.21,  hubble_a=9579.31 fom=0.976225
#vel_prefac= 1682.98, vel_prefac2= 3365.96,  hubble_a=9579.31 fom=0.978196

