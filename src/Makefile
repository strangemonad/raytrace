SOURCES = $(wildcard *.cpp)			 
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)
LDFLAGS = `pkg-config --libs gtkmm-2.0 gtkglextmm-1.0 lua50` -llualib50 -lpng
CPPFLAGS = `pkg-config --cflags gtkmm-2.0 gtkglextmm-1.0 lua50`
CXXFLAGS = $(CPPFLAGS) -W -Wall -g -O3
CXX = g++
MAIN = rt

all: $(MAIN)

depend: $(DEPENDS)

clean:
	rm -rf *.o *.d $(MAIN)

$(MAIN): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $<

%.d: %.cpp
	set -e; $(CC) -M $(CPPFLAGS) $< \
                  | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                [ -s $@ ] || rm -f $@

include $(DEPENDS)

