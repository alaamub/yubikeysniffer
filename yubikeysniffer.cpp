#include <iostream>
#include <stdio.h>
#include </usr/local/include/libusb-1.0/libusb.h>
          
using namespace std;
          
int main() {
    libusb_device **devs;
    libusb_device_handle *dev_handle;
    libusb_context *ctx = NULL;
    libusb_config_descriptor *config;
    uint8_t config_index;
    int r; // for return values
    ssize_t cnt; // holding number of devices in list
    
    r = libusb_init(&ctx);
    if(r < 0) {
        cout<<"Init Error "<<r<<endl;
        return 1;
    }

    libusb_set_debug(ctx, 3);
    cnt = libusb_get_device_list(ctx, &devs);
    cout<<"No. of devices = "<<cnt<<endl;

    if(cnt < 0) {
        cout<<"Getting Device Error"<<endl;
        return 1;
     }
    
    cout<<cnt<<" Devices in list."<<endl;
    dev_handle = libusb_open_device_with_vid_pid(ctx,  0x1050 , 0x0111 ); //these are vendorID and productID I found for my usb device
    if(dev_handle == NULL)
        cout<<"Cannot open device"<<endl;
    else
        cout<<"Device Opened"<<endl;

    libusb_free_device_list(devs, 1); //free the list, unref the devices in it

    if(libusb_kernel_driver_active(dev_handle, 1) == 1) { //find out if kernel driver is attached
        cout<<"Kernel Driver Active"<<endl;
        if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
            cout<<"Kernel Driver Detached!"<<endl;
        }
        r = libusb_claim_interface(dev_handle, 1); //claim interface 1 (the first) of device (mine had jsut 1)
        if(r < 0) {
            cout<<"Cannot Claim Interface with return value = "<< r <<endl;
            return 1;
    }
    
    // this where the actual dump happen
    unsigned char ENDPOINT_DOWN = 0x2;
    int actual_length;
    unsigned char data[256];
    char buffer[256];
    libusb_bulk_transfer(dev_handle, ENDPOINT_DOWN, data, sizeof(data), &actual_length, 0);
    if ( actual_length == sizeof(data)) {
        printf("read Successful!: ");
        for(int j=0;j<256;j++) {
            printf("%02x ",data[j]);
        }
        printf("\n");
    }
    // end if dump
    r = libusb_release_interface(dev_handle, 1); //release the claimed interface
    if(r!=0) {
        cout<<"Cannot Release Interface"<<endl;
        return 1;
    }
    cout<<"Released Interface"<<endl;
          
    libusb_close(dev_handle); //close the device we opened
    libusb_exit(ctx); 
    return 0;
}
