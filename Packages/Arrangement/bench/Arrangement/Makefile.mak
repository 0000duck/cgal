BASEDIR =.

include $(ROOT)/include/make/cgaldef.mak

BASENAME = benchPmwx
INSTALLDIR0 = $(BINDIR)
CPPSOURCES = $(BASENAME).C
TARGET0 = $(BASENAME)
LCPPDEFS+= -DCGAL_NO_PM_DEFAULT_POINT_LOCATION

LOBJDIR =
ifeq ($(USE_CONIC_TRAITS), 1)
LCPPDEFS+= -DUSE_CONIC_TRAITS
TARGET0 := $(TARGET0)Conics
LOBJDIR :=$(LOBJDIR)_conics
else

ifeq ($(USE_LEDA_KERNEL), 1)
LCPPDEFS+= -DUSE_LEDA_KERNEL
TARGET0 := $(TARGET0)LedaKernel
LOBJDIR :=$(LOBJDIR)_leda_kernel
else
ifeq ($(USE_MY_KERNEL), 1)
LCPPDEFS+= -DUSE_MY_KERNEL
TARGET0 := $(TARGET0)MyKernel
LOBJDIR :=$(LOBJDIR)_my_kernel
else
ifeq ($(USE_LAZY_RAT), 1)
LCPPDEFS+= -DUSE_LAZY_RAT
TARGET0 := $(TARGET0)LazyRat
LOBJDIR :=$(LOBJDIR)_lazy_rat
else
ifeq ($(USE_QUOTIENT_MP_FLOAT), 1)
LCPPDEFS+= -DUSE_QUOTIENT_MP_FLOAT
TARGET0 := $(TARGET0)MPFloat
LOBJDIR :=$(LOBJDIR)_mp_float
else
ifeq ($(USE_LAZY_QUOTIENT), 1)
LCPPDEFS+= -DUSE_LAZY_QUOTIENT
TARGET0 := $(TARGET0)LazyQuotient
LOBJDIR :=$(LOBJDIR)_lazy_quotient
else
ifeq ($(USE_GMPQ), 1)
LCPPDEFS+= -DUSE_GMPQ
TARGET0 := $(TARGET0)Gmpq
LOBJDIR :=$(LOBJDIR)_gmpq
else
ifeq ($(USE_LAZY_GMPQ), 1)
LCPPDEFS+= -DUSE_LAZY_GMPQ
TARGET0 := $(TARGET0)LazyGmpq
LOBJDIR :=$(LOBJDIR)_lazy_gmpq
else
ifeq ($(USE_DOUBLE), 1)
LCPPDEFS+= -DUSE_DOUBLE
TARGET0 := $(TARGET0)Double
LOBJDIR :=$(LOBJDIR)_double
endif
endif
endif
endif
endif
endif
endif
endif
endif

ifeq ($(USE_CACHED_TRAITS), 1)
LCPPDEFS+= -DUSE_CACHED_TRAITS
TARGET0 := $(TARGET0)Cached
LOBJDIR :=$(LOBJDIR)_cached
endif

ifeq ($(USE_LEDA_SEGMENT_TRAITS), 1)
LCPPDEFS+= -DUSE_LEDA_SEGMENT_TRAITS
TARGET0 := $(TARGET0)LedaSegment
LOBJDIR :=$(LOBJDIR)_leda_segment
endif

ifeq ($(USE_INSERT_OLD), 1)
LCPPDEFS+= -DUSE_INSERT_OLD
TARGET0 :=$(TARGET0)Old
LOBJDIR :=$(LOBJDIR)_old
endif

TARGET0 := $(TARGET0)$(EXEFILESUFFIX)
LOBJDIR := $(LOBJDIR)_$(COMPILER)$(COMPILER_VER)

LCPPINCS = -I$(BASEDIR)
LCPPINCS+= -I$(BASEDIR)/../../include
LCPPINCS+= -I$(BASEDIR)/../../../Benchmark/include
LCPPINCS+= -I$(BASEDIR)/../../../Planar_map/include
LCPPINCS+= -I$(BASEDIR)/../../../Trapezoidal_decomposition/include
LCPPINCS+= -I$(BASEDIR)/../../../Sweep_line_2/include
LCPPINCS+= -I$(BASEDIR)/../../../Leda_rat_kernel/include
LCPPINCS+= $(CGALINCS)

include $(ROOT)/include/make/cgalrul.mak

cartesian:
	$(MAKEF)

leda_kernel:
	$(MAKEF) "USE_LEDA_KERNEL=1"

lazy_rat:
	$(MAKEF) "USE_LAZY_RAT=1"

quotient_mp_float:
	$(MAKEF) "USE_QUOTIENT_MP_FLOAT=1"

lazy_quotient_mp_float:
	$(MAKEF) "USE_LAZY_QUOTIENT=1"

gmpq:
	$(MAKEF) "USE_GMPQ=1"

lazy_gmpq:
	$(MAKEF) "USE_LAZY_GMPQ=1"

double:
	$(MAKEF) "USE_DOUBLE=1"

# 
insert_old:
	$(MAKEF) "USE_INSERT_OLD=1"

my_kernel:
	$(MAKEF) "USE_MY_KERNEL=1"

conic_traits:
	$(MAKEF) "USE_CONIC_TRAITS=1"

# Cached:

cached_traits:
	$(MAKEF) "USE_CACHED_TRAITS=1"

leda_kernel_cached_traits:
	$(MAKEF) "USE_LEDA_KERNEL=1" "USE_CACHED_TRAITS=1"

lazy_rat_cached_traits:
	$(MAKEF) "USE_LAZY_RAT=1" "USE_CACHED_TRAITS=1"

quotient_mp_float_cached_traits:
	$(MAKEF) "USE_QUOTIENT_MP_FLOAT=1" "USE_CACHED_TRAITS=1"

lazy_quotient_mp_float_cached_traits:
	$(MAKEF) "USE_LAZY_QUOTIENT=1" "USE_CACHED_TRAITS=1"

gmpq_cached_traits:
	$(MAKEF) "USE_GMPQ=1" "USE_CACHED_TRAITS=1"

lazy_gmpq_cached_traits:
	$(MAKEF) "USE_LAZY_GMPQ=1" "USE_CACHED_TRAITS=1"

double_cached_traits:
	$(MAKEF) "USE_DOUBLE=1" "USE_CACHED_TRAITS=1"

all_non_cached: cartesian \
	leda_kernel \
	lazy_rat \
	quotient_mp_float \
	lazy_quotient_mp_float \
	gmpq \
	lazy_gmpq \
	double

all_cached: cached_traits \
	leda_kernel_cached_traits \
	quotient_mp_float_cached_traits \
	lazy_rat_cached_traits \
	lazy_quotient_mp_float_cached_traits \
	gmpq_cached_traits \
	lazy_gmpq_cached_traits \
	double_cached_traits

# install:

cartesian_inst:
	$(MAKEF) install

leda_kernel_inst:
	$(MAKEF) "USE_LEDA_KERNEL=1" install

lazy_rat_inst:
	$(MAKEF) "USE_LAZY_RAT=1" install

quotient_mp_float_inst:
	$(MAKEF) "USE_QUOTIENT_MP_FLOAT=1" install

lazy_quotient_mp_float_inst:
	$(MAKEF) "USE_LAZY_QUOTIENT=1" install

gmpq_inst:
	$(MAKEF) "USE_GMPQ=1" install

lazy_gmpq_inst:
	$(MAKEF) "USE_LAZY_GMPQ=1" install

double_inst:
	$(MAKEF) "USE_DOUBLE=1" install

# 
insert_old_inst:
	$(MAKEF) "USE_INSERT_OLD=1" install

my_kernel_inst:
	$(MAKEF) "USE_MY_KERNEL=1" install

conic_traits_inst:
	$(MAKEF) "USE_CONIC_TRAITS=1" install

# Cached:

cached_traits_inst:
	$(MAKEF) "USE_CACHED_TRAITS=1" install

leda_kernel_cached_traits_inst:
	$(MAKEF) "USE_LEDA_KERNEL=1" "USE_CACHED_TRAITS=1" install

lazy_rat_cached_traits_inst:
	$(MAKEF) "USE_LAZY_RAT=1" "USE_CACHED_TRAITS=1" install

quotient_mp_float_cached_traits_inst:
	$(MAKEF) "USE_QUOTIENT_MP_FLOAT=1" "USE_CACHED_TRAITS=1" install

lazy_quotient_mp_float_cached_traits_inst:
	$(MAKEF) "USE_LAZY_QUOTIENT=1" "USE_CACHED_TRAITS=1" install

gmpq_cached_traits_inst:
	$(MAKEF) "USE_GMPQ=1" "USE_CACHED_TRAITS=1" install

lazy_gmpq_cached_traits_inst:
	$(MAKEF) "USE_LAZY_GMPQ=1" "USE_CACHED_TRAITS=1" install

double_cached_traits_inst:
	$(MAKEF) "USE_DOUBLE=1" "USE_CACHED_TRAITS=1" install

#
all_non_cached_inst: cartesian_inst \
	leda_kernel_inst \
	quotient_mp_float_inst \
	lazy_rat_inst \
	lazy_quotient_mp_float_inst \
	gmpq_inst \
	lazy_gmpq_inst \
	double_inst

all_cached_inst: cached_traits_inst \
	leda_kernel_cached_traits_inst \
	quotient_mp_float_cached_traits_inst \
	lazy_rat_cached_traits_inst \
	lazy_quotient_mp_float_cached_traits_inst \
	gmpq_cached_traits_inst \
	lazy_gmpq_cached_traits_inst \
	double_cached_traits_inst
