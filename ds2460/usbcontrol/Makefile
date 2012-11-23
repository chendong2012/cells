VERSION=2.0-alpha003

DEBUG=-g
LDFLAGS=-lusb-1.0
CXXFLAGS+=-O2 -Wall -DVERSION=\"$(VERSION)\" $(DEBUG)

OBJS=sha.o Secrect.o interface.o USBaccessBasic.o USBaccess.o main.o


all: clewarecontrol

clewarecontrol: $(OBJS)
	$(CXX) $(DEBUG) $(OBJS) $(LDFLAGS) -o clewarecontrol

install: clewarecontrol
	cp clewarecontrol $(DESTDIR)/usr/bin
	cp clewarecontrol.1 $(DESTDIR)/usr/share/man/man1/clewarecontrol.1

uninstall: clean
	rm -f $(DESTDIR)/usr/bin/clewarecontrol
	rm -f $(DESTDIR)/usr/share/man/man1/clewarecontrol.1.gz

clean:
	rm -f $(OBJS) clewarecontrol core gmon.out *.da

package: clean
	# source package
	rm -rf clewarecontrol-$(VERSION)*
	mkdir clewarecontrol-$(VERSION)
	cp -a *.c* *.h clewarecontrol.1 Makefile license.txt examples readme.txt clewarecontrol-$(VERSION)
	tar czf clewarecontrol-$(VERSION).tgz clewarecontrol-$(VERSION)
	rm -rf clewarecontrol-$(VERSION)
