#include <stdio.h>
#include <time.h>
#include <math.h>
#include "tracker.h"
#include "MadgwickAHRS.h"

#define L3G_SO 70.0
#define GYRO_TO_RADIANS(a) ((float)a*M_PI*L3G_SO/(180.0*1000.0))

int main(void)
{
    int ret = 0;
    tracker_p tracker = tracker_init();
    if (!tracker) {
        fprintf(stderr, "Failed to init Tracker.\n");
        return 1;
    }
    
    tracker_packet_t packet = {{0}};
    double avg = 500.0;
    struct timespec prev, now;
    clock_gettime(CLOCK_REALTIME, &prev);
    
    while (1) {
        ret = tracker_read(tracker, &packet, 1000);
        
        clock_gettime(CLOCK_REALTIME, &now);
        struct timespec diff;
        if ((now.tv_nsec-prev.tv_nsec)<0) {
            diff.tv_nsec = 1000000000+now.tv_nsec-prev.tv_nsec;
        } else {
            diff.tv_nsec = now.tv_nsec-prev.tv_nsec;
        }
        prev = now;

        double ddiff = diff.tv_nsec / 1000000000.0;
        double hz = 1.0/ddiff;
        avg = 0.99*avg+0.01*hz;
        
        MadgwickAHRSupdate(avg, GYRO_TO_RADIANS(packet.gyro[0]), 
            GYRO_TO_RADIANS(packet.gyro[1]), GYRO_TO_RADIANS(packet.gyro[2]),
            packet.acc[0], packet.acc[1], packet.acc[2],
            packet.mag[0], packet.mag[1], packet.mag[2]);
/*        printf("tracker_read %d. [%d, %d, %d], [%d, %d, %d], [%d, %d, %d]\n", ret, 
            packet.gyro[0], packet.gyro[1], packet.gyro[2],
            packet.acc[0], packet.acc[1], packet.acc[2],
            packet.mag[0], packet.mag[1], packet.mag[2]);*/
        printf("%d %f %f [%f, %f, %f, %f]\n", ret, hz, avg, q0, q1, q2, q3);
    }
    
    tracker_destroy(tracker);
    
    return 0;
}

