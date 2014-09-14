PKGS=glib-2.0
CFLAGS=$(shell pkg-config --cflags $(PKGS))
LIBS=$(shell pkg-config --libs $(PKGS))

edel_cpu_first: edel_cpu_first.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
