CC = g++


objects =  main.o  CSlist.o CMat.o \
	function.o loc_store.o prediction.o schedule.o \
	road.o input.o gradient_descent.o config.o
main:$(objects)
	$(CC) -o main $(objects)


main.o :config.h road.h input.h prediction.h schedule.h

function.o:config.h

gradient_descent.o : CMat.h config.h

input.o : function.h CSlist.h road.h 

loc_store.o : road.h

prediction.o:function.h input.h road.h gradient_descent.h schedule.h \
			loc_store.h
road.o:config.h function.h input.h

schedule.o:config.h road.h


.PHONY : clean
clean:
	-rm main $(objects)

