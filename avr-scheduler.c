/*
 * avr-scheduler.c
 *
 * Created: 11/22/2014 2:44:29 PM
 * Author: CaptainSpaceToaster
 */ 

#include "avr-scheduler.h"

event_t event_t_error = {
	.ms_duration = 0,
	.ms_time = 65535,
	.id = 255,
	.next = NULL,
	.previous = NULL,
};

/* Creates an event_t in memory, and returns its pointer. */
event_t* new_event(uint16_t new_duration, uint16_t current_time, uint8_t new_id) {
	event_t *ret = (event_t *)malloc(sizeof(event_t));
	if (ret == NULL) {
		//Didn't get anything from malloc... most likely out of memory
		return NULL;
	} if (new_duration > 65535 - SCHEDULE_TIME_CEILING) {
		return NULL;
	}
	ret->ms_duration = new_duration;
	ret->ms_time = current_time;
	ret->id = new_id;
	ret->next = NULL;
	ret->previous = NULL;
	return ret;
}

/* Creates a schedule_t in memory, and returns its pointer.  Sets the first and last element to NULL nodes, and size value to zero. */
schedule_t* new_schedule() {
	schedule_t *ret = (schedule_t *)malloc(sizeof(schedule_t));
	if (ret == NULL) {
		//Didn't get anything from malloc... most likely out of memory
		return NULL;
	}
	ret->first_event = NULL;
	ret->last_event = NULL;
	ret->size = 0;
	return ret;
}

/* Empties the given schedule */
uint8_t schedule_clear(schedule_t *s) {
	if (s == NULL) {
		return SCHEDULE_NOT_FOUND; // return empty if it's not found
	} else if (schedule_is_empty(s)) {
		return 0;
	}
	event_t *temp = s->last_event;
	while(temp != NULL) {
		temp = s->last_event->previous;
		free(s->last_event);
		s->last_event = temp;
		if (s->last_event != NULL) {
			s->last_event->next = NULL;
		}
		s->size--;
		
	}
	return 0;
}

/* Checks the count, and returns true if the count is zero */
bool schedule_is_empty(schedule_t *s) {
	if (s == NULL) {
		return SCHEDULE_NOT_FOUND; // return empty if it's not found
	}
	return (s->size == 0);
}

/* Adds an object to the front of the schedule, regardless of its ms_time 
 *
 * WARNING: This means that no matter what, the pushed event won't show up until
 * all of the previous events to its entry are cleared...
 * 
 * This is likely to ruin a precise schedule, unless you know what you're doing. */
uint8_t schedule_push(schedule_t *s, event_t *input, uint16_t current_time) {
	if (s == NULL) {
		return SCHEDULE_NOT_FOUND;
	} if (input->ms_time > SCHEDULE_TIME_CEILING) {
		return DURATION_TOO_LONG;
	} if (input->ms_time > SCHEDULE_TIME_CEILING) {
		return TIME_OVERFLOW;
	}
	
	input->next = s->first_event;
	
	if (s->size == 0) {
		s->first_event = input;	
		s->last_event = input;
	} else if (s->size == 1) {
		input->next = s->first_event;
		s->first_event->previous = input;
		s->first_event = input;
		s->last_event->previous = s->first_event;
	} else {
		input->next = s->first_event;
		s->first_event->previous = input;
		s->first_event = input;
	}
	
	s->size ++;
	return 0;
}

/* Adds an event_t pointer to the schedule, and places it according to its ms_duration and ms_time */
uint8_t schedule_insert(schedule_t *s, uint16_t duration, uint16_t current_time, uint8_t id) {
	if (s == NULL) {
		return SCHEDULE_NOT_FOUND;
	} else if (s->size >= SCHEDULE_SIZE) {
		return SCHEDULE_OVERFLOW;
	} if (duration > 65535-SCHEDULE_TIME_CEILING) {
		return DURATION_TOO_LONG;
	}
	
	event_t *input = new_event(duration, current_time, id);
	if (input == NULL) {
		return NULL_EVENT_ERROR;
	}
	
	
	if (s->size == 0) {
		s->first_event = input;
		s->last_event = input;
	} else if (s->size == 1) {
		if ((duration + current_time) >= s->first_event->ms_duration + s->first_event->ms_time) {
			input->next = s->first_event;
			s->first_event->previous = input;
			s->first_event = input;
			s->last_event->previous = s->first_event;
		} else {
			input->previous = s->last_event;
			s->last_event->next = input;
			s->last_event = input;
			s->first_event->next = s->last_event;
		}
	} else {
		event_t *temp = s->first_event;
		while(1) {
			if (temp == NULL) { // temp iterated to the end of the list
				input->previous = s->last_event;
				s->last_event->next = input;
				s->last_event = input;
				break;
			} else if ((duration + current_time) >= temp->ms_duration + temp->ms_time) {
				input->next = temp;	
			
				if (temp->previous == NULL) { // temp is the beginning of the list
					s->first_event = input;
				} else {
					input->previous = temp->previous;
					temp->previous->next = input;
				}
				temp->previous = input;
				break;
			} else {
				temp = temp->next;
			}
		}
	}
	s->size++;
	return 0;
}

/* Removes the last element from the schedule and returns a copy... this destroys the element */
event_t schedule_pop(schedule_t *s) {
	if (s == NULL) {
		return event_t_error; // SCHEDULE_NOT_FOUND
	} else if (schedule_is_empty(s)) {
		return event_t_error;
	}
	event_t ret = *(s->last_event);
	event_t *temp = s->last_event->previous;
	free(s->last_event);
	s->last_event = temp;
	if (s->last_event != NULL) {
		s->last_event->next = NULL;
	}
	s->size--;
	return ret;
}

/* Peeks at the last element from the schedule and returns it */
event_t schedule_peek(schedule_t *s) {
	if (s == NULL) {
		return event_t_error; // SCHEDULE_NOT_FOUND
	} else if (schedule_is_empty(s)) {
		return event_t_error;
	}
	return *(s->last_event);
}