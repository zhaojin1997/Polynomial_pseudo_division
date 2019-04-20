NVCC =/usr/local/cuda/bin/nvcc
NVCC_FLAGS=-lcufft
all : poly.exe
poly.exe:main.o support.o add.o mult.o device.o cpu.o 
	$(NVCC) $(NVCC_FLAGS) $^ -o $@ 

main.o:main.cu support.h struct.h
	$(NVCC) $(NVCC_FLAGS) -c $< -o $@

support.o:support.cpp support.h struct.h
	$(NVCC) $(NVCC_FLAGS) -c $< -o $@

add.o:add.cu support.h struct.h
	$(NVCC) $(NVCC_FLAGS) -c $< -o $@

mult.o:mult.cu add.h support.h struct.h
	$(NVCC) $(NVCC_FLAGS) -c $< -o $@

device.o:device.cpp mult.h add.h support.h struct.h
	$(NVCC) $(NVCC_FLAGS) -c $< -o $@

cpu.o:ComputeCpu.cpp support.h struct.h
	$(NVCC) $(NVCC_FLAGS) -c $< -o $@

clean:
	-rm -f *.o *.exe
