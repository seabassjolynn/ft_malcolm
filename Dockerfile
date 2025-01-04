FROM alpine:latest

RUN apk update

#utility can intercept arp packets
#it is used in the project for debugging, e.g. to verify that it is possible to intercept ARP
#packets on host B, sent from host A 
RUN apk add tcpdump

#next 2 lines install make, gcc some libraries and header files needed for project
RUN apk add build-base
RUN apk add linux-headers

RUN mkdir /app
COPY /app /app

WORKDIR /app 
RUN make

CMD ["/bin/sh"]