dtapm: model.c view.c vx.h
	cc model.c view.c  -I/usr/local/include/ -I/usr/X11R6/include/ -L/usr/local/lib -L/usr/X11R6/lib -lXm -lXt -lX11 -o vx

clean:
	rm -f vx *.o
