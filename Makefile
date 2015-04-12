CC     := g++
CFLAGS := -O2 -std=c++0x
TARGET := tunnel
OBJS   := main.o tun.o network.o socket.o 

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	
clean :
	rm -f $(TARGET)
	rm -f *.o
