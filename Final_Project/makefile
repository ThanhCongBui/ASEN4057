all:
	gcc -c delVmin_opt.c delVmin_opt.h
	gcc -c delVtime_opt.c delVtime_opt.h
	gcc -c integrator.c integrator.h
	gcc -c forces.c forces.h
	gcc -c myrename.c myrename.h
	gcc -o main main.c delVmin_opt.o delVtime_opt.o integrator.o forces.o myrename.o -lm		
clear:
	rm delVmin_opt.o delVtime.o integrator.o forces.o myrename.o main
