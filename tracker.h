#ifndef _TRACKER_H_
#define _TRACKER_H_

#include "stdint.h"

typedef struct tracker_packet_s {
    int16_t gyro[3];
    int16_t acc[3];
    int16_t mag[3];
} tracker_packet_t, *tracker_packet_p;
    
typedef struct tracker_s *tracker_p;

tracker_p tracker_init();
int tracker_read(tracker_p tracker, tracker_packet_p packet, int timeout);
void tracker_destroy(tracker_p tracker);
    
#endif /* _TRACKER_H_ */
