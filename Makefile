EXEC   = 2LPT_CDE_PNG

OBJS   = main.o grwoth_de.o power.o checkchoose.o allvars.o save.o read_param.o  read_glass.o  \
         nrsrc/nrutil.o nrsrc/qromb.o nrsrc/polint.o nrsrc/trapzd.o

INCL   = allvars.h proto.h  nrsrc/nrutil.h  Makefile



#OPT   +=  -DPRODUCEGAS   # Set this to automatically produce gas particles 
                          # for a single DM species in the input file by interleaved by a half a grid spacing
                          # only for Gaussian and ZA

#OPT   +=  -DMULTICOMPONENTGLASSFILE  # set this if the initial glass file contains multiple components
                                      # only for Gaussian and ZA

#OPT   +=  -DDIFFERENT_TRANSFER_FUNC  # set this if you want to implement a transfer function that depends on
                                      # particle type
                                      # only for Gaussian and ZA

OPT   +=  -DNO64BITID     # switch this on if you want normal 32-bit IDs

#OPT  +=  -DCORRECT_CIC  # only switch this on if particles start from a glass (as opposed to grid)
                         # only for Gaussian and ZA

#OPT += -DONLY_ZA    # swith this on if you want ZA initial conditions (2LPT otherwise)

#OPT += -DONLY_GAUSSIAN # shwich this if you want gaussian initial conditions (fnl otherwise) 

#OPT += -DLOCAL_FNL #switch this if you want only local non-gaussianities

#OPT += -DEQUIL_FNL #switch this if you want equilateral Fnl

OPT += -DORTOG_FNL #switch this if you want ortogonal Fnl

OPTIONS =  $(OPT)

#------------ My mac --------------------
FFTW_INCL = -I/opt/envhpc/fftw/2.1.5/include
FFTW_LIBS = -L/opt/envhpc/fftw/2.1.5/lib

GSL_LIBS= -L/opt/envhpc/gsl/1.9/lib/
GSL_INCL = -I/opt/envhpc/gsl/1.9/include/

CC       = mpicc   # sets the C-compiler (default)
MPICHLIB = -L/usr/local/lib/


# ----- Chichipio/astro values ----
#FFTW_INCL = -I/opt/envhpc/fftw/2.1.5//include
#FFTW_LIBS = -L/opt/envhpc/fftw/2.1.5//lib

#GSL_LIBS= -L/opt/envhpc/gsl/1.9/lib/
#GSL_INCL = -I/opt/envhpc/gsl/1.9/include/


#-------- Sciama setting ----------------
#ifeq ($(SYSTYPE),"Sciama")
#CC       =  mpicc
#OPTIMIZE =  -O3 -Wall

#GSL_INCL =  -I/opt/apps/libs/gsl/1.16/intel-64.v13.1.046/include
#GSL_LIBS =  -L/opt/apps/libs/gsl/1.16/intel-64.v13.1.046/lib

#FFTW_INCL=  -I/opt/apps/libs/fftw2/2.1.5/intel-64.v13.1.046/include
#FFTW_LIBS=  -L/opt/apps/libs/fftw2/2.1.5/intel-64.v13.1.046/lib

#MPICHLIB =  -L/opt/apps/mpi/openmpi/1.4.3/intel-64.v13.1.046/lib
#endif

#ifneq (HAVE_HDF5,$(findstring HAVE_HDF5,$(OPT)))
#HDF5INCL = -I/opt/gridware/pkg/apps/hdf5_mpi/1.8.11/gcc-4.4.7+openmpi-1.8.1/include
#HDF5LIB  = -L/opt/gridware/pkg/apps/hdf5_mpi/1.8.11/gcc-4.4.7+openmpi-1.8.1/lib
#endif

#---------------------------------------
FFTW_LIB =  $(FFTW_LIBS) -lsrfftw_mpi -lsfftw_mpi -lsrfftw -lsfftw

LIBS   =   -lm  $(MPICHLIB)  $(FFTW_LIB)  $(GSL_LIBS)  -lgsl -lgslcblas

CFLAGS =   $(OPTIONS)  $(OPTIMIZE)  $(FFTW_INCL) $(GSL_INCL)

#---------------------------------------
$(EXEC): $(OBJS) 
	$(CC) $(OPTIMIZE) $(OBJS) $(LIBS)   -o  $(EXEC)  

$(OBJS): $(INCL) 


.PHONY : clean
clean:
	rm -f $(OBJS) $(EXEC)



