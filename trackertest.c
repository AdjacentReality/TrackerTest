#include <stdio.h>
#include "tracker.h"

int main(void)
{
    int ret = 0;
    tracker_p tracker = tracker_init();
    if (!tracker) {
        fprintf(stderr, "Failed to init Tracker.\n");
        return 1;
    }
    
    tracker_packet_t packet = {{0}};
    while (1) {
        ret = tracker_read(tracker, &packet, 1000);
        printf("tracker_read %d. [%d, %d, %d], [%d, %d, %d], [%d, %d, %d]\n", ret, 
            packet.gyro[0], packet.gyro[1], packet.gyro[2],
            packet.acc[0], packet.acc[1], packet.acc[2],
            packet.mag[0], packet.mag[1], packet.mag[2]);
    }
    
    tracker_destroy(tracker);
    
    return 0;
}

