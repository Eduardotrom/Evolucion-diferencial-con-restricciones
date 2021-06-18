T1 = exe
T5 = Build
CF = g++

OP =  -O2
OPG = $(OP) -march=sandybridge -static
LIBC=$(PWD)/Librerias
PD=$(PWD)/Main
all: dir bld comp clean 

dir: 
	mkdir -p $(T1) $(T2) $(T5)
dir_c:
	mkdir -p $(T3) $(T4) $(T5)
bld: dir 
	$(CF) -I $(LIBC) -c $(LIBC)/*.cpp  $(OP)
	mv *.o $(T5)
bldg: dir 
	$(CF) -I $(LIBC) -c $(LIBC)/*.cpp  $(OPG)
	mv *.o $(T5)
comp: bld
	$(CF) -I $(LIBC) -o $(T1)/optimizador_exe  $(PD)/De-rest.cpp $(T5)/*.o $(OP)


.PHONY: clean
clean:
	rm -r $(T5)
	clear