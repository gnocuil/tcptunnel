CC     := gcc
CFLAGS := -O2 -lpthread
TARGET_SRV := tcpsrv
TARGET_CLI := tcpcli
OBJS   :=  tcp.o tun.o

all: $(TARGET_SRV) $(TARGET_CLI)

$(TARGET_CLI) : $(OBJS) main_cli.o
	$(CC)  $(OBJS) main_cli.o -o $(TARGET_CLI) $(CFLAGS)

$(TARGET_SRV) : $(OBJS) main_srv.o
	$(CC)  $(OBJS) main_srv.o -o $(TARGET_SRV) $(CFLAGS)

%.o: %.c
	$(CC) -c  $< -o $@ $(CFLAGS)
	
clean :
	rm -f $(TARGET_SRV) $(TARGET_CLI)
	rm -f *.o
