#ifndef MACS_H
#define MACS_H

/* divide time into slots */
/* units are 802.15.4 clock clicks (250kHz) */
/* must be powers of 2 */
#define SLOT_WIDTH 4096 /* a full 133 byte packet is 1064 clks long, 4096 is about 4 full packets wide, or 16ms slots */
/* group a number of slots into a mac period */
#define SLOTS_PER_PERIOD 256 /* 128 slots x 4096 width is a mac period is about 1 second */

#define BEACON_PERIOD SLOT_WIDTH * SLOTS_PER_PERIOD

/* set the beacon interval in terms of SLOTS_PER_PERIOD */
/* actual beacon interval is SLOT_PER_PERIOD * 2^BEACON_INTERVAL */
/* or put another way, 2^BEACON_INTERVAL - 1 is the number of slot periods to skip before beconing again */
#define BEACON_INTERVAL 0

/* value of rx_time is RX_TIME_DELAY maca clocks delayed from actual */
/* this is measured from testing the maca */
#define RX_TIME_DELAY 10

/* number of beacons to keep queued at one time */
#define NUM_BEACONS_QUEUED 4

/* period to queue up beacons */
#define FILL_BEACON_PERIOD (1 * CLOCK_SECOND) * NUM_BEACONS_QUEUED/2

/* if a node has more that DENSE_NODE_THRESH fresh beacons then it's considered "well connected" */
#define DENSE_NODE_THRESH 3

/* a well connected node with too much skew can destabilize a network with it's beacons */
/* because it retransmits that skew to everyone */
/* surpress beacon generation on well connected nodes if the average skew is above this value */
#define BEACON_SURPRESS_SKEW BEACON_PERIOD/32

#define NUM_MAC_ENTRIES 16

typedef struct mac_entry {
	uint64_t addr;
	uint32_t last_rx; /* last maca clock we got a beacon */
	int32_t skew; /* how far this beacon is skewed from a grid boundary */
	uint8_t fresh;
} mac_entry_t;

extern mac_entry_t macs[NUM_MAC_ENTRIES];

#endif
