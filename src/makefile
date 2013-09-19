INCLUDE=-Inarl -I../externals/catch/single_include
OBJDIR=tmp
SRCDIR=tests/testnarl

OBJS=	$(OBJDIR)/testnarl.o \
		$(OBJDIR)/test_transforming_range.o \
		$(OBJDIR)/test_filtering_range.o \
		$(OBJDIR)/test_partial_range.o \
		$(OBJDIR)/test_range_factory.o \
		$(OBJDIR)/test_range_generator.o \

CC=g++
CPPFLAGS=-std=c++11 

.PHONY: tests
tests: testnarl.exe
	.\testnarl.exe

.PHONY: clean
clean:
	-rm $(OBJS)
	-rm testnarl.exe

testnarl.exe: $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^

$(OBJDIR):
	-mkdir $(OBJDIR)

include $(OBJS:.o=.d)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	$(CC) $(CPPFLAGS) -c -o $@ $< -Wall $(INCLUDE)

$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $(INCLUDE) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
