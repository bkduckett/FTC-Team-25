#pragma config(UserModel, "../pragmas/baemax.h")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define FOUR_WHEEL_DRIVE
#define DEAD_RECKON_GYRO

#include "../../lib/sensors/drivers/hitechnic-sensormux.h"
#include "../../lib/sensors/drivers/hitechnic-protoboard.h"
#include "../../lib/sensors/drivers/hitechnic-irseeker-v2.h"
#include "../../lib/sensors/drivers/hitechnic-gyro.h"

const tMUXSensor HTGYRO  = msensor_S4_3;
bool beacon_done;
int distance_monitor_distance;

#include "../library/baemax_defs.h"
#include "../../lib/baemax_drivetrain_defs.h"
#include "../../lib/drivetrain_square.h"
#include "../../lib/dead_reckon.h"
#include "../../lib/data_log.h"
#include "../../lib/ir_utils.h"
#include "../../lib/us_utils.h"
#include "../../lib/us_cascade_utils.c"
#include "../../lib/limit_switch.h"
#include "../library/auto_utils.h"

const tMUXSensor irr_left = msensor_S4_1;
const tMUXSensor irr_right = msensor_S4_2;
const tMUXSensor HTPB = msensor_S4_4;

#define RIGHT_EYE_STOP 176
#define LEFT_EYE_STOP  80
#define EYE_OFFSET     50

task find_slices()
{
	int lpos, rpos;
	int lstart, rstart;
	int ldelta, rdelta;
	int turn_dir;
	bool ldone, rdone;
	bool lfound, rfound;
	bool rotated;
	int ldir, rdir;
	int dir;

	lstart = LSERVO_CENTER + EYE_OFFSET;
	rstart = RSERVO_CENTER - EYE_OFFSET;

	rotated = false;
	turn_dir = 0;

	while (!rotated) {
		lpos = LSERVO_CENTER + EYE_OFFSET;
		rpos = RSERVO_CENTER - EYE_OFFSET;
		ldone = false;
		rdone = false;

		servo[leftEye] = lpos;
		servo[rightEye] = rpos;
		wait1Msec(300);

		while (!ldone || !rdone) {
			if (!ldone) {
				dir = HTIRS2readACDir(irr_left);
				if (dir == 4) {
					ldone = true;
					lfound = true;
				} else if (lpos <= LEFT_EYE_STOP) {
					ldone = true;
					lfound = false;
				} else {
					lpos -= 1;
					servo[leftEye] = lpos;
				}
				ldir = dir;
			}
			if (!rdone) {
				dir = HTIRS2readACDir(irr_right);
				if (dir == 6) {
					rdone = true;
					rfound = true;
				} else if (rpos >= RIGHT_EYE_STOP) {
					rdone = true;
					rfound = false;
				} else {
					rpos += 1;
					servo[rightEye] = rpos;
				}
				rdir = dir;
			}

			nxtDisplayCenteredBigTextLine(5, "%d %d", ldir, rdir);
			//wait1Msec(250);
		}

		rdelta = abs(rstart - rpos);
		ldelta = abs(lstart - lpos);

		if (rfound && lfound) {
			if (abs(rdelta - ldelta) < 6) {
				nxtDisplayCenteredTextLine(1, "Centered %d, %d", ldelta, rdelta);
				rotated = true;
			} else if (rdelta > ldelta) {
				nxtDisplayCenteredTextLine(1, "Left %d, %d", ldelta, rdelta);
				turn_dir = 1;
			} else {
				nxtDisplayCenteredTextLine(1, "Right %d, %d", ldelta, rdelta);
				turn_dir = -1;
			}
		} else {
			if (!rfound) {
				nxtDisplayCenteredTextLine(1, "R Missing");
			}
			if (!lfound) {
				nxtDisplayCenteredTextLine(2, "L Missing");
			}
		}

		turnEncoder(turn_dir, 30);
	}

	if (turn_dir == 1) {
		turnEncoder(-1, 30);
	} else {
		turnEncoder(1, 30);
	}
}

task main()
{
	startTask(find_slices);

	while (true) {}
}