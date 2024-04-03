CXX         = g++
CXXFLAGS    = -Wall -Os -s -Wno-ignored-qualifiers
#CXXFLAGS    = -std=c++17 -pedantic -Wall -Wextra -Werror -Wshadow -Wsign-conversion -g
LDFLAGS     = -I.
SRCDIR     := src
OBJDIR     := obj
EXECUTABLE  = gfaq
#SOURCE      = $(shell find $(SRCDIR) -name *.cc)
SOURCE     := $(wildcard $(SRCDIR)/*.cc $(SRCDIR)/*/*.cc)
#OBJECTS     = $(subst src, $(OBJDIR), $(addsuffix .o, $(basename $(SOURCE))))
OBJECTS := $(SOURCE:.cc=.o)
#DEPENDENTS := $(OBJECTS:.o=.d)
#INCDIRS    := $(shell find $(SRCDIR) -type d)
#INCFLAGS   := $(addprefix -I, $(INCDIRS))
#CPPFLAGS   := $(INCFLAGS) -MMD -MP


.DELETE_ON_ERROR:
all: head $(EXECUTABLE) foot clean

head:
	@ECHO ========================================
	@mkdir -p $(dir $(OBJECTS))

foot:
	@ECHO
	@ECHO ========================================
	@ECHO Build complete.

$(OBJDIR)/%.o: $(SOURCE)
	@ECHO \ \ \ \ Building $@
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	@ECHO
	@ECHO \ \ \ \ Building $@
	@ECHO \ \ \ \ Requires $^
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

.PHONY: clean cleanest

clean:
	@ECHO Cleaning...
#	del $(OBJECTS)
	rm -r $(OBJECTS)

cleanest: clean
	del $(EXECUTABLE)

# -include $(DEPS)