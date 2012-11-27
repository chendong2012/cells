VERSION=2.0-alpha003

DEBUG=-g
LDFLAGS=-lusb-1.0
CXXFLAGS+=-O2 -Wall -DVERSION=\"$(VERSION)\" $(DEBUG)

OBJS=sha.o Secrect.o interface.o  main.o


all: uwaycontrol

uwaycontrol: $(OBJS)
	$(CXX) $(DEBUG) $(OBJS) $(LDFLAGS) -o uwaycontrol 

install: uwaycontrol
	cp uwaycontrol $(DESTDIR)/usr/bin
	cp uwaycontrol.1 $(DESTDIR)/usr/share/man/man1/uwaycontrol.1

uninstall: clean
	rm -f $(DESTDIR)/usr/bin/uwaycontrol
	rm -f $(DESTDIR)/usr/share/man/man1/uwaycontrol.1.gz

clean:
	rm -f $(OBJS) uwaycontrol core gmon.out *.da

package: clean
	# source package
	rm -rf uwaycontrol-$(VERSION)*
	mkdir uwaycontrol-$(VERSION)
	cp -a *.c* *.h uwaycontrol.1 Makefile license.txt examples readme.txt uwaycontrol-$(VERSION)
	tar czf uwaycontrol-$(VERSION).tgz uwaycontrol-$(VERSION)
	rm -rf uwaycontrol-$(VERSION)
