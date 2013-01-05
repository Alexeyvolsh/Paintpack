#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hidapi.h"

#define PPVIP 0x0E8F
#define PPPID 0x0025

#define NUMCHANNELS 30  // number of channels
#define DEVICEPOLLINGINTERVAL 5

int main(int argc, char const *argv[]) {

    float input[NUMCHANNELS];
    unsigned char buffer[NUMCHANNELS + 3];
    
    hid_device *handle = NULL;

    while(1) {

        while (handle == NULL) {

            handle = hid_open(PPVIP, PPPID, NULL);

            if (handle == NULL)
                fprintf(stderr, "painpack device not detected\n");
            else {
                fprintf(stderr, "painpack device connected\n");
                break;
            }

            sleep(DEVICEPOLLINGINTERVAL);
        }

        // read channel values from boblight popen
        // dirty and stupid, but safe
        scanf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
            input, input+1, input+2, input+3, input+4, input+5, input+6, input+7, input+8, input+9,
            input+10, input+11, input+12, input+13, input+14, input+15, input+16, input+17, input+18, input+19,
            input+20, input+21, input+22, input+23, input+24, input+25, input+26, input+27, input+28, input+29
        );

        buffer[0] = 0x00; // default ReportID
        buffer[1] = 0x03; // update colors command 
        buffer[2] = 0x00;

        int dataofsset = 3;
        for (int i=0; i < NUMCHANNELS; i++) {
            buffer[dataofsset++] = (unsigned char) input[i] * 255; // float to unsigned char conversion
        }

    #ifdef DEBUG

        fprintf(stderr, "sending data: ");

        for (int i=0; i < 33; i++) {
            fprintf(stderr, "%i ", buffer[i]);
        }

        fprintf(stderr, "\n");

    #endif

        int result = hid_write(handle, buffer, sizeof(buffer));

        if (result == -1) {
            fprintf(stderr, "error on writing buffer to the painpack device, trying to reconnect\n");
            hid_close(handle);

            handle = NULL;
        }
    }
}