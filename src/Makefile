CC	 	= gcc
CLINKER	= gcc

OBJS	= main.o ip.o

.SUFFIXES:	.o .c
.c.o:
	$(CC) -c $(CCLAGS) $<

EXE = main.out

all: $(EXE)

$(EXE): $(OBJS)
	$(CLINKER) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(EXE)

install:
	mv $(EXE) ../bin/.
