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

# Added by Jared Maddox, Jan 2, 2013
# Note: This was basically just copied from my old DGD-for-MSys version,
# expect untested bugs.
#
ifneq ($(HOST),WIN32)
  # This was just indented from what was already in this file, so it
  # should be fine.
  #

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
 # This, in comparison, was copied from my previous implementation. Don't
 # depend on it being bug-free.
 #

 SRC= local.c dload.c time.c connect.c xfloat.c dosfile.c windgd.c
 OBJ= $(SUBOBJ) connect.o dosfile.o windgd.o
 SUBOBJ= local.o dload.o time.o crypt.o xfloat.o asn.o
endif

dgd:	$(OBJ)
	@for i in $(OBJ); do echo host/$$i; done > dgd

lint:
	lint $(LINTFLAGS) $(CFLAGS) $(SRC) crypt.c

sub:	$(SUBOBJ)
	@for i in $(SUBOBJ); do echo ../host/$$i; done > sub

clean:
	rm -f dgd sub $(SRC) $(OBJ)

# Next, we have the copy rules.
#
ifneq ($(HOST),WIN32)
  # These are the versions that already existed here, but wrapped to
  # control their use.
  #

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

else
  # And here we have the forms that I've copied from my old
  # implementation.
  #

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

endif
  # Finally, we go on to what we can count on for both versions.
  #


xfloat.c: simfloat.c
	cp simfloat.c $@

$(OBJ):	../dgd.h ../host.h ../config.h ../alloc.h ../error.h
connect.o: ../hash.h ../comm.h
xfloat.o: ../xfloat.h
