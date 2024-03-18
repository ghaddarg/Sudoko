CXX	= gcc
AR	= ar
##################################################################################
SRCDIR	= src
APP		= sudoko_checker

MKDIR	= mkdir -p
OUTDIR	= exe
LIBDIR  = lib

SRCEXT	= c
OBJEXT	= o
##################################################################################
#                                   SQLITE
##################################################################################
CPPSRCS := \
	$(SRCDIR)/$(APP).$(SRCEXT)

OBJS := $(CPPSRCS:.$(SRCEXT)=.$(OBJEXT))

%.o: %.$(SRCEXT)
	$(CXX) $(CXXFLAGS) -c $^ -o $@
##################################################################################
#                                   GXX FLAGS
##################################################################################
CFLAGS := \
	-fexceptions -Wall -Wextra -Werror -O0 -g

LDFLAGS := 
##################################################################################
#                              MAIN MAKEFILE
##################################################################################
.PHONY: all
all: dir $(APP)

lib$(APP).a: $(OBJS)
	$(AR) rcs $(LIBDIR)/lib$(APP).a $(OBJS)

$(APP): lib$(APP).a
	$(CXX) $(CFLAGS) $(SRCDIR)/$@.c -o $(OUTDIR)/$@ $(LIBDIR)/lib$(APP).a $(LDFLAGS)

dir: dir_lib
	${MKDIR} ${OUTDIR}

dir_lib:
	${MKDIR} ${LIBDIR}

check:
	./$(OUTDIR)/$(APP)

clean:
	rm -rf $(OUTDIR) \
		rm -rf ${LIBDIR} \
		$(SRCDIR)/*.$(OBJEXT)