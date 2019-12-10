#
#  Copyright (C) 2018 Savoir-Faire Linux Inc.
#
#  Authors:
#      Gaël PORTAY <gael.portay@savoirfairelinux.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

PREFIX ?= /usr/local
DESTDIR ?=
TARGET ?= linux

.PHONY: all
all: src/vodisp/vodisp

src/vodisp/vodisp:
	$(MAKE) -C src/vodisp -f Makefile.$(TARGET)

.PHONY: install
install:
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 src/vodisp/vodisp $(DESTDIR)$(PREFIX)/bin/
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 755 lib/$(TARGET)/libvodisp.so $(DESTDIR)$(PREFIX)/lib/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/vodisp
	rm -f $(DESTDIR)$(PREFIX)/lib/libvodisp.so

.PHONY: clean
clean:
	rm -f src/vodisp/vodisp

# ex: filetype=makefile
