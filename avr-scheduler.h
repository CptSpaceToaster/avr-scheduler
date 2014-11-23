/*
 * avr-scheduler.h
 *
 * Created: 11/22/2014 2:44:29 PM
 * Author: CaptainSpaceToaster
 */ 
#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCHEDULE_SIZE				50 // Maximum number of stored events
#define SCHEDULE_TIME_CEILING       60000
#define GET_EVENT_TIME(EVENT_T)     ((uint16_t)((EVENT_T.ms_duration + EVENT_T.ms_time)%SCHEDULE_TIME_CEILING))

typedef struct event {
	uint16_t ms_duration;
	uint16_t ms_time;
	uint8_t id;
	struct event *next;
	struct event *previous;
} event_t;

typedef struct {
	event_t *first_event;               // Position of first event (root)
	event_t *last_event;                // Position of last event
	uint8_t size;                       // Number of scheduled events
} schedule_t;

#define SCHEDULE_NOT_FOUND      0x01    //This can't be changed, as 1 equates to true, and is_empty() returns this error
#define SCHEDULE_OVERFLOW       0x02
#define TIME_OVERFLOW           0x03
#define DURATION_TOO_LONG       0x04
#define NULL_EVENT_ERROR        0x05

/* Creates an event_t in memory, and returns its pointer. */
event_t* new_event(uint16_t duration, uint16_t current_time, uint8_t new_id);

/* Creates a schedule_t in memory, and returns its pointer.  Sets the first and last element to NULL nodes, and size value to zero. */
schedule_t* new_schedule();

/* Checks the count, and returns true if the count is zero */
bool schedule_is_empty(schedule_t *s);

/* Adds an object to the front of the schedule, regardless of its ms_time 
 *
 * WARNING: This means that no matter what, the pushed event won't show up until
 * all of the previous events to its entry are cleared...
 * 
 * This is likely to ruin a precise schedule, unless you know what you're doing. */
uint8_t schedule_push(schedule_t *s, event_t *input, uint16_t current_time);

/* Adds an event_t pointer to the schedule, and places it according to its ms_duration and ms_time */
uint8_t schedule_insert(schedule_t *s, uint16_t duration, uint16_t current_time, uint8_t id);

/* Removes the last element from the schedule and returns a copy... this destroys the element */
event_t schedule_pop(schedule_t *s);

/* Peeks at the last element from the schedule and returns it */
event_t schedule_peek(schedule_t *s);

#endif // _SCHEDULE_H_