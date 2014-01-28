CLANGINC=-Id:/bin/mingw64/include/c++/4.8.1 -Id:/bin/mingw64/include/c++/4.8.1/x86_64-w64-mingw32 -pedantic
INCLUDE=-Inarl -I../externals/catch/single_include
OBJDIR=tmp
SRCDIR=tests/testnarl

# * aggregate
# * all 
# * any
# * concat
# * count
# * distinct
# * except
# * groupby
# * join
# * intersect
# * range
# * reverse
# * select
# * selectmany
# * sequenceequal
# * skip
# * skipwhile
# * sorted
# * take
# * takewhile
# * to...
# * union
# * where
# * zip
# - average ( aggregate )
# - cast ( select )
# - default ( n/a )
# - defaultifempty ( n/a )
# - elementat ( where? )
# - empty
# - first ( * )
# - max
# - min
# - oftype ( where )
# - repeat ( special case of generate )
# - sum ( aggregate )
# - contains ( same as any( predicatee ) )
# groupjoin
# last
# single

OBJS=	$(OBJDIR)/testnarl.o \
		$(OBJDIR)/test_concatenating_range.o \
		$(OBJDIR)/test_diffing_range.o \
		$(OBJDIR)/test_distinct_range.o \
		$(OBJDIR)/test_filtering_range.o \
		$(OBJDIR)/test_grouping_range.o \
		$(OBJDIR)/test_intersecting_range.o \
		$(OBJDIR)/test_iterable_range.o \
		$(OBJDIR)/test_joined_range.o \
		$(OBJDIR)/test_merging_range.o \
		$(OBJDIR)/test_partial_range.o \
		$(OBJDIR)/test_range_accumulate.o \
		$(OBJDIR)/test_range_factory.o \
		$(OBJDIR)/test_range_generator.o \
		$(OBJDIR)/test_range_predicate.o \
		$(OBJDIR)/test_reversed_range.o \
		$(OBJDIR)/test_selectmany_range.o \
		$(OBJDIR)/test_sorted_range.o \
		$(OBJDIR)/test_tocontainer_range.o \
		$(OBJDIR)/test_transforming_range.o \
		$(OBJDIR)/test_unioning_range.o \
		$(OBJDIR)/test_zipped_range.o \
		$(OBJDIR)/narl_stl.o \

CPPFLAGS=-std=c++11 -Wall -Wextra -Wpedantic
CC=g++ 

.PHONY: tests
tests: testnarl.exe
	.\testnarl.exe

.PHONY: clean
clean:
	-rm $(OBJS)
	-rm testnarl.exe

.PHONY: release
release:
	echo $(CC) $(CPPFLAGS) $(INCLUDE) -O3 -o narl_tests_opt.exe $(SRCDIR)/*.cpp

testnarl.exe: $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^

include $(OBJS:.o=.d)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	$(CC) $(CPPFLAGS) -c -o $@ $< $(INCLUDE)

$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	-mkdir $(OBJDIR)
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $(INCLUDE) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJDIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
