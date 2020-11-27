ifdef KERNEL_SIZE
KERNEL_SIZE := $(KERNEL_SIZE)
else
KERNEL_SIZE := 256
endif

ifdef RPI_MODEL
RPI_MODEL := $(RPI_MODEL)
else
RPI_MODEL := 3
endif

ifeq ($(RPI_MODEL), 3)
  CPU_FLAG := cortex-a53
else ifeq ($(RPI_MODEL), 4)
  CPU_FLAG := cortex-a72
endif
FPU_FLAG := neon-fp-armv8

CFLAGS=-mcpu=$(CPU_FLAG) -mfloat-abi=hard -mfpu=$(FPU_FLAG)

all: cpp

assembly:
	as -g $(CFLAGS) --defsym KERNEL_SIZE=$(KERNEL_SIZE) \
	   -o mac_simd.o mac_simd.S

cpp: assembly
	g++ -O3 -g $(CFLAGS) -DKERNEL_SIZE=$(KERNEL_SIZE) \
		main.cpp mac_simd.o -fopenmp -lgomp

clean:
	rm -f a.out
	rm -r *.o
