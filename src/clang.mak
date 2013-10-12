CLANGINC=-Id:/bin/mingw64/include/c++/4.8.1 -Id:/bin/mingw64/include/c++/4.8.1/x86_64-w64-mingw32 -pedantic -fsyntax-only
INCLUDE=$(CLANGINC) -Inarl -I../externals/catch/single_include
OBJDIR=tmp
SRCDIR=tests/testnarl

OBJS=	$(OBJDIR)/testnarl.oclang \
		$(OBJDIR)/test_concatenating_range.oclang \
		$(OBJDIR)/test_diffing_range.oclang \
		$(OBJDIR)/test_filtering_range.oclang \
		$(OBJDIR)/test_grouping_range.oclang \
		$(OBJDIR)/test_intersecting_range.oclang \
		$(OBJDIR)/test_iterable_range.oclang \
		$(OBJDIR)/test_partial_range.oclang \
		$(OBJDIR)/test_range_accumulate.oclang \
		$(OBJDIR)/test_range_factory.oclang \
		$(OBJDIR)/test_range_generator.oclang \
		$(OBJDIR)/test_range_predicate.oclang \
		$(OBJDIR)/test_reversed_range.oclang \
		$(OBJDIR)/test_sorted_range.oclang \
		$(OBJDIR)/test_transforming_range.oclang \
		$(OBJDIR)/test_unioning_range.oclang \
		$(OBJDIR)/test_zipped_range.oclang \

CPPFLAGS=-std=c++11 -Wall -Wextra -Wpedantic
CC=clang++ 

testnarlclang.exe: $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^

include $(OBJS:.oclang=.dclang)

$(OBJDIR):
	-mkdir $(OBJDIR)

$(OBJDIR)/%.oclang: $(SRCDIR)/%.cpp 
	$(CC) $(CPPFLAGS) -c -o $@ $< $(INCLUDE)

$(OBJDIR)/%.dclang: $(SRCDIR)/%.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $(INCLUDE) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
