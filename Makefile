# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

all: options dwm

options:
	@echo dwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	cp config.def.h $@

dwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwm ${OBJ} dwm-${VERSION}.tar.gz

dist: clean
	mkdir -p dwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		dwm.1 drw.h util.h ${SRC} dwm.png transient.c dwm-${VERSION}
	tar -cf dwm-${VERSION}.tar dwm-${VERSION}
	gzip dwm-${VERSION}.tar

deps: 
	mkdir -p /usr/share/drwm/backgrounds/
	cp -f wal.jpeg /usr/share/drwm/backgrounds/
	cp -f bg /usr/share/drwm/
	cp -f drwm.desktop /usr/share/xsessions/drwm.desktop
	git clone https://github.com/Super-Yojan/bar /usr/share/drwm/drwm-bar

install: all deps
	cp -f drwm /usr/bin
	chmod 755 /usr/bin/drwm
	cp -f dwm /usr/bin
	chmod 755 /usr/bin/dwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < dwm.1 > ${DESTDIR}${MANPREFIX}/man1/dwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${MANPREFIX}/man1/dwm.1
	rm -rf dwm-${VERSION}
	rm -rf /usr/bin/drwm
	rm -rf /usr/share/drwm



.PHONY: all options clean dist install uninstall
