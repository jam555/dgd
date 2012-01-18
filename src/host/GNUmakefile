#
# This file is part of DGD, http://dgd-osr.sourceforge.net/
# Copyright (C) 1993-2010 Dworkin B.V.
# Copyright (C) 2010-2011 DGD Authors (see the file Changelog for details)
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

# This is mostly just a wrapper file. It's purpose is to provide an easy
# way to compile DGD on the MSYS enviroment for Windows.
HOST=	DARWIN
DEBUG=	-O -g

ifeq ($(HOST),WIN32)
 DEFINES=-D$(HOST)

 SRC= connect.c dload.c dosfile.c local.c time.c windgd.c	xfloat.c
 OBJ= connect.o dload.o dosfile.o local.o time.o windgd.o	crypt.o xfloat.o asn.o
 SUBOBJ=	connect.o dload.o dosfile.o local.o time.o windgd.o crypt.o xfloat.o asn.o
else
 DEFINES=-D$(HOST)

 SRC=	local.c random.c dirent.c dload.c time.c connect.c xfloat.c
 OBJ=	local.o random.o dirent.o dload.o time.o connect.o crypt.o xfloat.o asn.o
 SUBOBJ=	local.o random.o dirent.o dload.o time.o crypt.o xfloat.o asn.o
endif

CCFLAGS=$(DEFINES) $(DEBUG)
CFLAGS=	-I. -I.. $(CCFLAGS)
LINTFLAGS=-abcehpruz
CC=	gcc
DMAKE=	make


$(OBJ):	../dgd.h ../host.h ../config.h ../alloc.h ../error.h

connect.o: ../hash.h ../comm.h

xfloat.o: ../xfloat.h


ifeq ($(HOST),WIN32)

dgd:	$(OBJ)
	@for i in $(OBJ); do echo host/$$i; done > dgd

lint:
	lint $(LINTFLAGS) $(CFLAGS) $(SRC) crypt.c

sub:	$(SUBOBJ)
	@for i in $(SUBOBJ); do echo ../host/$$i; done > sub

clean:
	rm -f dgd sub $(SRC) $(OBJ)

connect.c: win32/connect.c
	cp win32/$@ $@

dload.c: win32/dload.c
	cp win32/$@ $@

dosfile.c: win32/dosfile.c
	cp win32/$@ $@

local.c: win32/local.c
	cp win32/$@ $@

time.c: win32/time.c
	cp win32/$@ $@

windgd.c: win32/windgd.c
	cp win32/$@ $@

xfloat.c: simfloat.c
	cp simfloat.c $@

else

dgd:	$(OBJ)
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' dgd

lint:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' lint

sub:	$(SUBOBJ)
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' sub

clean:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' clean

endif
