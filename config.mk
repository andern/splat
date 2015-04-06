PROGRAM_NAME = splat
VERSION = 0.1

DESTDIR = /usr
MANPREFIX = /share/man

CWARN = -Wall -Wextra\
        -Wdouble-promotion -Wformat=2 -Winit-self -Wmissing-include-dirs\
        -Wswitch-default -Wswitch-enum -Wuninitialized\
        -Wstrict-overflow=5 -Wno-div-by-zero -Wtrampolines -Wundef\
        -Wshadow -Wcast-align -Wwrite-strings -Wconversion -Wlogical-op\
        -Wmissing-declarations\
        -Wmissing-field-initializers -Wpacked -Winline -Wvla\
        -Wno-missing-braces -Wstack-protector

#        -Waggregate-return -Wmissing-declarations\

CFLAGS = -std=c11 -pedantic -O0 ${CWARN}\
		 -DVERSION=\"${VERSION}\" -DPROGRAM_NAME=\"${PROGRAM_NAME}\"
