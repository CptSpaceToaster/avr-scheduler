/*
 * Note... this isn't a very exhaustive test suite... however, I only needed to fix these edge cases, and the system
 * appeared to operate normally.  Feel free to leave a git issue, or submit a PR highlighting a case that fails...
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "avr-scheduler.h"

int main(int argc, char *argv[]) {
	schedule_t *s = new_schedule();
	// Put stuff in a schedule, and set the ID's to be the order that you expect.  The first one will be 1, second 2, etc...
	schedule_insert(s, 10, 100, 1);
	schedule_insert(s, 20, 110, 2);
	schedule_insert(s, 30, 120, 3);
	schedule_insert(s, 40, 130, 4);
	schedule_test_backwards(s); // This will check the order
	schedule_test_forwards(s);
	schedule_clear(s); //Clear the schedule after every test
	
	schedule_insert(s, 100, 50, 1);
	schedule_insert(s, 100, 150, 2);
	schedule_insert(s, 100, 59850, 3);
	schedule_insert(s, 100, 59950, 4);
	schedule_test_backwards(s);
	schedule_test_forwards(s);
	schedule_clear(s);
	
	schedule_insert(s, 100, 59850, 1);
	schedule_insert(s, 100, 59950, 2);
	schedule_insert(s, 100, 50, 3);
	schedule_insert(s, 100, 150, 4);
	schedule_test_backwards(s);
	schedule_test_forwards(s);
	schedule_clear(s);
	
	schedule_insert(s, 100, 59850, 1);
	schedule_insert(s, 100, 50, 2);
	schedule_insert(s, 100, 150, 3);
	schedule_insert(s, 100, 150, 4);
	schedule_test_backwards(s);
	schedule_test_forwards(s);
	schedule_clear(s);
	
	schedule_insert(s, 100, 59850, 3);
	schedule_insert(s, 50, 59850, 2);
	schedule_insert(s, 20, 59850, 1);
	schedule_insert(s, 100, 50, 4);
	schedule_insert(s, 100, 150, 5);
	schedule_insert(s, 100, 150, 6);
	schedule_test_backwards(s);
	schedule_test_forwards(s);
	schedule_clear(s);
	
	schedule_insert(s, 100, 59950, 3);
	schedule_insert(s, 50, 59950, 2);
	schedule_insert(s, 20, 59950, 1);
	schedule_insert(s, 100, 50, 4);
	schedule_insert(s, 100, 150, 5);
	schedule_insert(s, 100, 150, 6);
	schedule_test_backwards(s);
	schedule_test_forwards(s);
	schedule_clear(s);
	
	schedule_insert(s, 20, 59850, 1);
	schedule_insert(s, 50, 59850, 2);
	schedule_insert(s, 100, 59850, 3);
	schedule_insert(s, 100, 50, 4);
	schedule_insert(s, 100, 150, 5);
	schedule_insert(s, 100, 150, 6);
	schedule_test_backwards(s);
	schedule_test_forwards(s);
	schedule_clear(s);
	
	schedule_insert(s, 20, 59950, 1);
	schedule_insert(s, 50, 59950, 2);
	schedule_insert(s, 100, 59950, 3);
	schedule_insert(s, 100, 50, 4);
	schedule_insert(s, 100, 150, 5);
	schedule_insert(s, 100, 150, 6);
	schedule_test_backwards(s);
	schedule_test_forwards(s);
	schedule_clear(s);
	
	
	
	schedule_insert(s, 150, 59950, 3); //100
	schedule_insert(s, 50, 	59960, 1);  //10
	schedule_insert(s, 100, 59970, 2); //70
	schedule_test_backwards(s);
	schedule_test_forwards(s);
	schedule_clear(s);
	
	schedule_insert(s, 150, 59950, 5); //100
	schedule_insert(s, 50, 59960, 1);  //10
	schedule_insert(s, 100, 59970, 3); //70
	schedule_insert(s, 60, 30, 4);     //90
	schedule_insert(s, 10, 50, 2);     //60
	schedule_insert(s, 100, 150, 6);   //250
	schedule_test_backwards(s);
	schedule_test_forwards(s);
	schedule_clear(s);
}