#
# This file is part of DGD, https://github.com/dworkin/dgd
# Copyright (C) 1993-2010 Dworkin B.V.
# Copyright (C) 2010,2012 DGD Authors (see the commit log for details)
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

dgd:	$(OBJ)
	@for i in $(OBJ); do echo host/$$i; done > dgd

lint:
	lint $(LINTFLAGS) $(CFLAGS) $(SRC) crypt.c

sub:	$(SUBOBJ)
	@for i in $(SUBOBJ); do echo ../host/$$i; done > sub

clean:
	rm -f dgd sub $(SRC) $(OBJ)


local.c: unix/local.c
	cp unix/$@ $@

random.c: unix/random.c
	cp unix/$@ $@

lrand48.c: unix/lrand48.c
	cp unix/$@ $@

dirent.c: unix/dirent.c
	cp unix/$@ $@

dload.c: unix/dload.c
	cp unix/$@ $@

time.c: unix/time.c
	cp unix/$@ $@

connect.c: unix/connect.c
	cp unix/$@ $@

xfloat.c: simfloat.c
	cp simfloat.c $@

$(OBJ):	../dgd.h ../host.h ../config.h ../alloc.h ../error.h
connect.o: ../hash.h ../comm.h
xfloat.o: ../xfloat.h
