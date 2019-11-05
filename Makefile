CFLAGS = -std=c99 -Wall -Wextra
CPPFLAGS = -I./rd_route
LDFLAGS = -shared

.PHONY: all clean

all: reversion.dylib

clean:
	git clean -fdx
	git submodule foreach git clean -fdx


reversion.dylib: reversion.o rd_route/rd_route.o
	$(CC) $(LDFLAGS) -o $@ $^
