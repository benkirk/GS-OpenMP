CC	= gcc
LD	= $(CC)

CFLAGS	= -g -Wall 
CFLAGS	+= -I. 
LDFLAGS	= 

OBJS	= 
LIBS    = -lm -fopenmp

SRCS	= $(patsubst %.o,%.c,$(OBJS))

PRGS	= gs_seq gs_openmp 

all: $(PRGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $*.c $(INCLUDE) -o $@ -fopenmp

$(PRGS): $(OBJS)
$(PRGS): 
$(PRGS): % : %.o
	$(CC) $(CFLAGS) -o $@ $< $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	-rm -f *.o  *~ $(PRGS)

