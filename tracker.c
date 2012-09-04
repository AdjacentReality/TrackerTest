#include "tracker.h"
#include <stdlib.h>
#include <stdio.h>
#include <hidapi/hidapi.h>

#define REPORT_SIZE 16

// sign extending trick from http://graphics.stanford.edu/~seander/bithacks.html#FixedSignExtend
struct {int16_t x:12;} s;

typedef struct tracker_s {
    hid_device *hid;
} tracker_t ;

tracker_p tracker_init()
{
    tracker_p tracker = (tracker_p)calloc(1, sizeof(tracker_t));
    if (tracker) {
        // Open with the vendor and product IDs we are temporarily using
        tracker->hid = hid_open(0x03EB, 0x2044, NULL);
        if (!tracker->hid) {
            fprintf(stderr, "Failed to open Tracker device.\n");
            free(tracker);
            return NULL;
        }
    } else {
        fprintf(stderr, "Failed to allocate Tracker memory.\n");
        return NULL;
    }
    
    return tracker;
}

int tracker_read(tracker_p tracker, tracker_packet_p packet, int timeout)
{
    int ret = 0;

    if (tracker && packet) {
        unsigned char buf[REPORT_SIZE];
        
        ret = hid_read_timeout(tracker->hid, buf, REPORT_SIZE, timeout);
        if (ret == REPORT_SIZE) {
            // USB is little endian, so just unpack manually
            packet->gyro[0] = buf[0] | (buf[1] << 8);
            packet->gyro[1] = buf[2] | (buf[3] << 8);
            packet->gyro[2] = buf[4] | (buf[5] << 8);
            
            // The accelerometer and magnetometer data is 12bit, packed.
            // The accelerometer values are in the high 12 bits
            packet->acc[0] = (buf[6] << 8) | (buf[7] & 0xF0);
            packet->acc[1] = ((buf[7] & 0x0F) << 12) | (buf[8] << 4);
            packet->acc[2] = (buf[9] << 8) | (buf[10] & 0xF0);
            
            // Magnetometer values are in the low 12 bits, so the sign needs
            // to be extended when unpacking
            packet->mag[0] = s.x = ((buf[10] & 0x0F) << 8) | buf[11];
            packet->mag[1] = s.x = (buf[12] << 4) | (buf[13] >> 4);
            packet->mag[2] = s.x = ((buf[13] & 0x0F) << 8) | buf[14];
        } else {
            ret = 0;
        }
    }
    
    return ret;
}

void tracker_destroy(tracker_p tracker)
{
    if (tracker) {
        if (tracker->hid) hid_close(tracker->hid);
        free(tracker);
    }
}

