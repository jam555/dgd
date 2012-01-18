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

ifeq ($(OS),Windows_NT)
 HOST= WIN32
 PLATFORM_DEFINES= -D_WIN32_WINNT=0x501
else
 HOST=	DARWIN
 PLATFORM_DEFINES=
endif

ifeq ($(OS),Windows_NT)
 LIBS= -lws2_32 -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
else
 LIBS=	-ldl		# FreeBSD may require "make HOST=FREEBSD LIBS="
endif

DMAKE=	make
BIN=	../bin

a.out: always
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' a.out

always:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' always

$(BIN)/driver: a.out
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' $(BIN)/driver

install:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' install

lint:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' lint

comp.sub:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' comp.sub

lex.sub:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' lex.sub

ed.sub:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' ed.sub

comp/parser.h:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' comp/parser.h

comp/a.out:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' comp/a.out

clean:
	$(DMAKE) -f Makefile 'HOST=$(HOST)' 'PLATFORM_DEFINES=$(PLATFORM_DEFINES)' 'LIBS=$(LIBS)' clean
