#
# This file is part of DGD, https://github.com/dworkin/dgd
# Copyright (C) 1993-2010 Dworkin B.V.
# Copyright (C) 2010-2013 DGD Authors (see the commit log for details)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
HOST=	DARWIN
DEFINES=-D$(HOST)
DEBUG=	-O -g
CCFLAGS=$(DEFINES) $(DEBUG)
CFLAGS=	-I. -I.. $(CCFLAGS)
LINTFLAGS=-abcehpruz
CC=	gcc

ifeq ($(HOST),LINUX)
  SYSV_STYLE=1
endif
ifeq ($(HOST),SOLARIS)
  SYSV_STYLE=1
endif
ifeq ($(HOST),DECALPHA)
  SYSV_STYLE=1
endif
ifeq ($(HOST),GENERIC_SYSV)
  SYSV_STYLE=1
endif

ifneq ($(HOST),WIN32)
  PLATFORM_DIR=unix
  SRC=	local.c dirent.c dload.c time.c connect.c xfloat.c
  SUBOBJ=	local.o dirent.o dload.o time.o crypt.o xfloat.o asn.o
  ifdef SYSV_STYLE
    SRC+=lrand48.c
    SUBOBJ+=lrand48.o
  else
    SRC+=random.c
    SUBOBJ+=random.o
  endif
  OBJ=	$(SUBOBJ) connect.o
else
 PLATFORM_DIR=win32
 SRC= local.c dload.c time.c connect.c xfloat.c dosfile.c windgd.c
 SUBOBJ= local.o dload.o time.o crypt.o xfloat.o asn.o
 OBJ= $(SUBOBJ) connect.o dosfile.o windgd.o
endif

dgd:	$(OBJ)
	@for i in $(OBJ); do echo host/$$i; done > dgd

lint:
	lint $(LINTFLAGS) $(CFLAGS) $(SRC) crypt.c

sub:	$(SUBOBJ)
	@for i in $(SUBOBJ); do echo ../host/$$i; done > sub

clean:
	rm -f dgd sub $(SRC) $(OBJ)


local.c: $(PLATFORM_DIR)/local.c
	cp $(PLATFORM_DIR)/$@ $@

random.c: $(PLATFORM_DIR)/random.c
	cp $(PLATFORM_DIR)/$@ $@

lrand48.c: $(PLATFORM_DIR)/lrand48.c
	cp $(PLATFORM_DIR)/$@ $@

dirent.c: $(PLATFORM_DIR)/dirent.c
	cp $(PLATFORM_DIR)/$@ $@

dload.c: $(PLATFORM_DIR)/dload.c
	cp $(PLATFORM_DIR)/$@ $@

time.c: $(PLATFORM_DIR)/time.c
	cp $(PLATFORM_DIR)/$@ $@

connect.c: $(PLATFORM_DIR)/connect.c
	cp $(PLATFORM_DIR)/$@ $@

dosfile.c: $(PLATFORM_DIR)/dosfile.c
	cp $(PLATFORM_DIR)/$@ $@

windgd.c: $(PLATFORM_DIR)/windgd.c
	cp $(PLATFORM_DIR)/$@ $@

xfloat.c: simfloat.c
	cp simfloat.c $@

$(OBJ):	../dgd.h ../host.h ../config.h ../alloc.h ../error.h
connect.o: ../hash.h ../comm.h
xfloat.o: ../xfloat.h
