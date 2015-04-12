CC     := gcc
CFLAGS := -O2
TARGET_SRV := tcpsrv
TARGET_CLI := tcpcli
OBJS   :=  tcp.o tun.o

all: $(TARGET_SRV) $(TARGET_CLI)

$(TARGET_CLI) : $(OBJS) main_cli.o
	$(CC) $(CFLAGS) $(OBJS) main_cli.o -o $(TARGET_CLI)

$(TARGET_SRV) : $(OBJS) main_srv.o
	$(CC) $(CFLAGS) $(OBJS) main_srv.o -o $(TARGET_SRV)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
	
clean :
	rm -f $(TARGET_SRV) $(TARGET_CLI)
	rm -f *.o
