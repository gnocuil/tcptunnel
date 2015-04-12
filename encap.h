#pragma once

#define BUF_LEN 2000

class Encap {
public:
	virtual const char* name() = 0;
	virtual char* readbuf() = 0;
	virtual int readbuflen() = 0;
	virtual int makepacket(int len) = 0;
	virtual int init_socket() = 0;
	virtual int handle_socket() = 0;
	int sendlen() { return send_len; }
	int send4len() { return send4_len; }
	virtual char* sendbuf() = 0;
	virtual char* send4buf() = 0;
protected:
	int send_len;
	int send4_len;
};
