screen ./tcpcli
ifconfig tcptnl up

sysctl net.ipv4.ip_forward=1
iptables -t mangle -A FORWARD -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --set-mss 1412
ip addr add 10.41.0.2/24 dev tcptnl

