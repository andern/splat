SHELL:= /bin/bash

include config.mk

SRC = splat.c log.c debug.c util.c gl_math.c
OBJ = $(SRC:.c=.o)

all: $(PROGRAM_NAME)

.c.o:
	gcc -c $(CFLAGS) $<

$(OBJ): config.mk

$(PROGRAM_NAME): $(OBJ)
#	gcc -lm $(OBJ) -o $@
	gcc -lGLEW -lglfw -lGL -lm $(OBJ) -o $@

clean:
	rm -f $(PROGRAM_NAME) $(OBJ) $(PROGRAM_NAME)-$(VERSION).tar.gz

dist: clean
	@mkdir $(PROGRAM_NAME)-$(VERSION)
	@cp COPYING Makefile config.mk $(PROGRAM_NAME).1 $(SRC)\
		$(PROGRAM_NAME)-$(VERSION)/.
	tar -zcf $(PROGRAM_NAME)-$(VERSION).tar.gz $(PROGRAM_NAME)-$(VERSION)
	@rm -rf $(PROGRAM_NAME)-$(VERSION)

install: $(PROGRAM_NAME)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(PROGRAM_NAME) $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed 's/VERSION/$(VERSION)/g' < $(PROGRAM_NAME).1\
		> $(DESTDIR)$(MANPREFIX)/man1/$(PROGRAM_NAME).1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/$(PROGRAM_NAME).1

uninstall:
	rm -rf $(DESTDIR)$(PREFIX)/bin/$(PROGRAM_NAME)
	rm -rf $(DESTDIR)$(MANPREFIX)/man1/$(PROGRAM_NAME).1

.PHONY: clean dist uninstall install
