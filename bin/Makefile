############################################################
# Description:	Makefile for generating the three binaries
#		hlenc, hldec and hllist.
#
# Author(s):	Anders Henja
#
# Copyright:	Swedish Meteorological and Hydrological Institute, 2000
#
# History:	2000-03-22 Created by Anders Henja
############################################################
-include ../def.mk

CFLAGS = $(OPTS) $(DEFS) $(HDF5_INCDIR) $(ZLIB_INCDIR) $(SZLIB_INCDIR) -I../hlhdf
LDFLAGS=  -L. -L../hlhdf $(HDF5_LIBDIR) $(ZLIB_LIBDIR) $(SZLIB_LIBDIR)

ifeq ($(GOT_SZ_COMPRESS),yes)
  LIB_SZLIB=-lsz
else
  LIB_SZLIB=
endif

LIBRARIES= $(LD_FORCE_STATIC) -lhlhdf $(LD_FORCE_SHARE) -lhdf5 -lz $(LIB_SZLIB) -lm

TARGET_HLDEC=hldec
SOURCES_HLDEC=hldec.c
OBJECTS_HLDEC=$(SOURCES_HLDEC:.c=.o)

TARGET_HLENC=hlenc
SOURCES_HLENC=hlenc.c
OBJECTS_HLENC=$(SOURCES_HLENC:.c=.o)

TARGET_HLLIST=hllist
SOURCES_HLLIST=hllist.c
OBJECTS_HLLIST=$(SOURCES_HLLIST:.c=.o)

all: $(TARGET_HLDEC) $(TARGET_HLENC) $(TARGET_HLLIST)

$(TARGET_HLDEC): $(OBJECTS_HLDEC) ../hlhdf/libhlhdf.a
	$(CC) -o $@ $(LDFLAGS) $(OBJECTS_HLDEC) $(LIBRARIES)

$(TARGET_HLENC): $(OBJECTS_HLENC) ../hlhdf/libhlhdf.a
	$(CC) -o $@ $(LDFLAGS) $(OBJECTS_HLENC) $(LIBRARIES)

$(TARGET_HLLIST): $(OBJECTS_HLLIST) ../hlhdf/libhlhdf.a
	$(CC) -o $@ $(LDFLAGS) $(OBJECTS_HLLIST) $(LIBRARIES)

.PHONY: clean
clean:
	@\rm -f *.o
	@\rm -f *~
	@\rm -f so_locations

.PHONY: distclean
distclean:
	@\rm -f *.o
	@\rm -f $(TARGET_HLDEC)
	@\rm -f $(TARGET_HLENC)
	@\rm -f $(TARGET_HLLIST)
	@\rm -f *~ core
	@\rm -f so_locations

.PHONY: distribution
distribution:
	@echo "Would bring the latest revision upto date"

.PHONY: test
test:
	@echo "Nothing to test"

.PHONY: install
install:
	@"$(HL_INSTALL)" -f -C $(TARGET_HLDEC) "$(DESTDIR)$(prefix)/bin/$(TARGET_HLDEC)"
	@"$(HL_INSTALL)" -f -C $(TARGET_HLENC) "$(DESTDIR)$(prefix)/bin/$(TARGET_HLENC)"
	@"$(HL_INSTALL)" -f -C $(TARGET_HLLIST) "$(DESTDIR)$(prefix)/bin/$(TARGET_HLLIST)"

