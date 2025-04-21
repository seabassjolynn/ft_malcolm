FROM debian:latest

RUN apt update

#utility can intercept arp packets
#it is used in the project for debugging, e.g. to verify that it is possible to intercept ARP
#packets on host B, sent from host A 
RUN apt install tcpdump -y
RUN apt install make
RUN apt install gcc -y
RUN apt install iputils-ping -y
RUN apt install arping -y
RUN apt install tcpdump -y
RUN apt install net-tools -y # Install net-tools to provide the arp utility for network debugging
RUN apt install iproute2 -y # Install iproute2 to provide the ip utility for network configuration and debugging

RUN mkdir /app
COPY /app /app

WORKDIR /app 
RUN make

CMD ["/bin/sh"]