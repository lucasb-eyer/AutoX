LDFLAGS=-L/usr/local/lib -lX11
CFLAGS=-I/usr/local/include -O2 -Wall -Wextra -std=c99 -pedantic
DESTDIR=/usr

ax: main.o
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY: clean install
clean:
	$(RM) *.o ax

install: ax
	install ax $(DESTDIR)/bin

uninstall:
	$(RM) $(DESTDIR)/bin/ax
