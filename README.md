# Yubikey Sniffer
----------

Sniffing the raw byte data from a Yubikey connected to your linux machine.

## prerequisites
yum install libusb

## Install
```
g++ -g -L/usr/local/lib -lusb-1.0 -o yubikeysniffer yubikeysniffer.cpp
```

## Run
```
sudo ./yubikeysniffer
```
