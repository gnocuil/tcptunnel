CC     := gcc
CFLAGS := -O2
TARGET := tcpsrv
OBJS   := main_srv.o

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
	
clean :
	rm -f $(TARGET)
	rm -f *.o
