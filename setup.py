import subprocess
from distutils.core import setup

#------------ My mac --------------------
FFTW_INCL = "-I/opt/envhpc/fftw/2.1.5/include"
FFTW_LIBS = "-L/opt/envhpc/fftw/2.1.5/lib"

GSL_LIBS= "-L/opt/envhpc/gsl/1.9/lib/"
GSL_INCL = "-I/opt/envhpc/gsl/1.9/include/"

MPICHLIB = "-L/usr/local/lib/"

FFTW_LIB =  FFTW_LIBS + " -lsrfftw_mpi -lsfftw_mpi -lsrfftw -lsfftw"
LIBS   =   "-lm " + MPICHLIB + FFTW_LIB + GSL_LIBS + " -lgsl -lgslcblas"


# Compile c code
p = subprocess.Popen(["mpicc","-DNO64BITID -DORTOG_FNL", LIBS,
                      "main.c", "grwoth_de.c",
                      "power.c",  "checkchoose.c", "allvars.c", "save.c",  "read_param.c", "read_glass.c",
                      "nrsrc/nrutil.c", "nrsrc/qromb.c", "nrsrc/polint.c", "nrsrc/trapzd.c",
                      
                      "-o", "2LPT_CDE_PNG"]).wait()

subprocess.call(["./2LPT_CDE_PNG"])

setup(name='2LPTNG',
      version='1.0',
      packages=['2LPTNG', '2LPTNG.2LPTNG'],
      )