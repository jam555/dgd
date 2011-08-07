#
# This file is part of DGD, http://dgd-osr.sourceforge.net/
# Copyright (C) 1993-2010 Dworkin B.V.
# Copyright (C) 2010 DGD Authors (see the file Changelog for details)
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
DEBUG=	-O -g
CCFLAGS=$(DEFINES) $(DEBUG)
CFLAGS=	-I. -I.. $(CCFLAGS)
LINTFLAGS=-abcehpruz
CC=	gcc
DMAKE=	make

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

dgd:	$(OBJ)
	@for i in $(OBJ); do echo host/$$i; done > dgd

lint:
	lint $(LINTFLAGS) $(CFLAGS) $(SRC) crypt.c

sub:	$(SUBOBJ)
	@for i in $(SUBOBJ); do echo ../host/$$i; done > sub

clean:
	rm -f dgd sub $(SRC) $(OBJ)


ifeq ($(HOST),WIN32)

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

else

local.c: unix/local.c
	cp unix/$@ $@

random.c: unix/random.c
	cp unix/$@ $@

dirent.c: unix/dirent.c
	cp unix/$@ $@

dload.c: unix/dload.c
	cp unix/$@ $@

time.c: unix/time.c
	cp unix/$@ $@

connect.c: unix/connect.c
	cp unix/$@ $@

endif

xfloat.c: simfloat.c
	cp simfloat.c $@


$(OBJ):	../dgd.h ../host.h ../config.h ../alloc.h ../error.h

connect.o: ../hash.h ../comm.h

xfloat.o: ../xfloat.h
