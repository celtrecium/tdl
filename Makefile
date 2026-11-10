NAME=tdl

CC      ?= gcc
CFLAGS   = -O3 
INCLUDES = -Iinclude

SOURCES         = $(wildcard src/*.c)
HEADERS_MAIN    = $(wildcard include/*.h)
HEADERS_TDL     = $(wildcard include/tdl/*.h)
HEADERS_U8UTILS = $(wildcard include/tdl/u8utils/*.h)
OBJECTS         = $(SOURCES:.c=.o)
TARGET_STATIC   = lib$(NAME).a
TARGET_SHARED   = lib$(NAME).so

PREFIX     ?= /usr
LIBDIR      = $(PREFIX)/lib64
INCLUDEDIR  = $(PREFIX)/include

.PHONY: all static shared clean

all: shared static

.c.o:
	$(CC) -c -fPIC $(INCLUDES) $(CFLAGS) $< -o $@

$(TARGET_STATIC): $(OBJECTS)
	ar rcs $(TARGET_STATIC) $(OBJECTS)

$(TARGET_SHARED): $(OBJECTS)
	$(CC) -shared $^ -o $@

static: $(TARGET_STATIC)

shared: $(TARGET_SHARED)

install: $(TARGET_STATIC) $(TARGET_SHARED)
	install -d $(DESTDIR)$(LIBDIR)
	install -d $(DESTDIR)$(INCLUDEDIR)
	install -d $(DESTDIR)$(INCLUDEDIR)/tdl
	install -d $(DESTDIR)$(INCLUDEDIR)/tdl/u8utils
	
	install $(TARGET_SHARED) $(DESTDIR)$(LIBDIR)
	install $(TARGET_STATIC) $(DESTDIR)$(LIBDIR)
	
	install $(HEADERS_MAIN) $(DESTDIR)$(INCLUDEDIR)
	install $(HEADERS_TDL) $(DESTDIR)$(INCLUDEDIR)/tdl
	install $(HEADERS_U8UTILS) $(DESTDIR)$(INCLUDEDIR)/tdl/u8utils

uninstall:
	rm -f  $(DESTDIR)$(LIBDIR)/$(TARGET_STATIC)
	rm -f  $(DESTDIR)$(LIBDIR)/$(TARGET_SHARED)
	rm -rf $(DESTDIR)$(INCLUDEDIR)/tdl
	rm -f  $(DESTDIR)$(INCLUDEDIR)/tdl.h

clean:
	rm $(OBJECTS)
