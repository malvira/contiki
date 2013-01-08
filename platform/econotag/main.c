/*
 * Copyright (c) 2012, Mariano Alvira <mar@devl.org> and other contributors
 * to the MC1322x project (http://mc1322x.devl.org) and Contiki.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

#include <string.h>

/* debug */
#define DEBUG DEBUG_FULL
#include "net/uip-debug.h"

/* contiki */
#include "contiki.h"
#include "dev/button-sensor.h"
#include "net/rime/rimeaddr.h"
#include "net/netstack.h"

/* mc1322x */
#include "mc1322x.h"
#include "config.h"
#include "contiki-uart.h"

/* econotag */
#include "platform_prints.h"

SENSORS(&button_sensor);

#ifndef M12_CONF_SERIAL
#define M12_SERIAL 0x000000
#else
#define M12_SERIAL M12_CONF_SERIAL
#endif

int main(void) {

	mc1322x_init();

	/* m12_init() flips the mux switch */

	/* trims the main crystal load capacitance */
	if (!FORCE_ECONOTAG_I && CRM->SYS_CNTLbits.XTAL32_EXISTS) {
		/* M12 based econotag */
		PRINTF("trim xtal for M12\n\r");
		CRM->XTAL_CNTLbits.XTAL_CTUNE = (M12_CTUNE_4PF << 4) | M12_CTUNE;
		CRM->XTAL_CNTLbits.XTAL_FTUNE = M12_FTUNE;
	} else {
		/* econotag I */
		PRINTF("trim xtal for Econotag I\n\r");
		CRM->XTAL_CNTLbits.XTAL_CTUNE = (ECONOTAG_CTUNE_4PF << 4) | ECONOTAG_CTUNE;
		CRM->XTAL_CNTLbits.XTAL_FTUNE = ECONOTAG_FTUNE;
	}

	{
		/* number of RTC clocks to measure */
		#define RTC_SAMPLE_COUNTS 2500
		#define TIMER_FREQ 24000000/64

		uint32_t start, end;
		uint16_t rtcA, rtcB; 

		global_irq_disable();

		TMR1->ENBLbits.ENBL1 = 0;
		TMR1->CTRLbits = (struct TMR_CTRL) {
			.COUNT_MODE = 1,           /* count rising edges of primary source (should be 24MHz) */
			.PRIMARY_CNT_SOURCE = 0xe, /* primary source divided by 64 */
			.SECONDARY_CNT_SOURCE = 1, /* set to tmr 1, but not used */
			.ONCE = 0,                 /* continuous */
			.LENGTH = 0,               /* roll over counter at end */
			.DIR = 0,                  /* count up */
			.CO_INIT = 0,              /* no co-channels */
			.OUTPUT_MODE = 0,          /* assert output flag while running */
		};
		TMR1->SCTRLbits = (struct TMR_SCTRL) {
			.OEN = 0,                  /* pin is input */
			.OPS = 0,                  /* don't invert */
			.VAL = 0,                  /* default value when forcing (not used) */
			.EEOF = 0,                 /* disable force */
			.MSTR = 0,                 /* no master mode */
			.CAPTURE_MODE = 0,         /* capture disabled */
			.IPS = 0,                  /* don't invert */
			.IEFIE = 0,                /* disable interrupts */
			.TOFIE = 0,                /* no timer overflow interrupt */
			.TCFIE = 0,                /* no compare interrupt */
		};
		TMR1->CSCTRL = 0;                  /* disable all compares */
		TMR1->CSCTRLbits.FILT_EN = 1;      /* enable glitch filters */
		TMR1->LOAD = 0;                    /* load in 0 */
		TMR1->COMP1 = 0XFFFF;              /* set compare to max */
		TMR1->CNTR = 0;                    /* start counter at 0 */
		TMR1->ENBLbits.ENBL1 = 1;          /* enable TMR1 */

		/* wait until rtc changes */
		rtcA = CRM->RTC_COUNT;
		while (CRM->RTC_COUNT <= rtcA) { continue; }
		start = TMR1->CNTR;
		rtcB = CRM->RTC_COUNT;
		while (CRM->RTC_COUNT < (rtcB + RTC_SAMPLE_COUNTS) ) { continue; }
		end = TMR1->CNTR;
		rtcB = CRM->RTC_COUNT;

		global_irq_enable();

		printf("count %d\n\r", (int32_t)(end - start));
		printf("start %d end %d\n\r", start, end);
		printf("rtcA %d rtcB %d\n\r", rtcA, rtcB);
		printf("samp %d\n\r", rtcB - rtcA);

		rtc_freq = TIMER_FREQ * (rtcB - rtcA - 1) / (end - start);
		printf("rtc_freq %d\n\r", rtc_freq);

	}

	/* create mac address if blank*/
	if (mc1322x_config.eui == 0) {
		/* mac address is blank */
		/* construct a new mac address based on IAB or OUI definitions */

		/* if an M12_SERIAL number is not defined */
		/* generate a random extension in the Redwire experimental IAB */
		/* The Redwire IAB (for development only) is: */
		/* OUI: 0x0050C2 IAB: 0xA8C */
		/* plus a random 24-bit extension */
		/* Otherwise, construct a mac based on the M12_SERIAL */
		/* Owners of an Econotag I (not M12 based) can request a serial number from Redwire */
		/* to use here */

		/* M12 mac is of the form "EC473C4D12000000" */
		/* Redwire's OUI: EC473C */
		/* M12: 4D12 */
		/* next six nibbles are the M12 serial number as hex */
		/* e.g. if the barcode reads: "12440021" = BDD1D5 */
		/* full mac is EC473C4D12BDD1D5 */

#if (M12_SERIAL == 0)
                /* use random mac from experimental range */
		mc1322x_config.eui = (0x0050C2A8Cull << 24) | (*MACA_RANDOM & (0xffffff));
#else
		/* construct mac from serial number */
		mc1322x_config.eui = (0xEC473C4D12ull << 24) | M12_SERIAL;
#endif
		mc1322x_config_save(&mc1322x_config);		
	} 
	
	/* configure address on maca hardware and RIME */
	contiki_maca_set_mac_address(mc1322x_config.eui);

#if WITH_UIP6
	memcpy(&uip_lladdr.addr, &rimeaddr_node_addr.u8, sizeof(uip_lladdr.addr));
	queuebuf_init();
	NETSTACK_RDC.init();
	NETSTACK_MAC.init();
	NETSTACK_NETWORK.init();
  #if DEBUG_ANNOTATE
	print_netstack();
  #endif
	process_start(&tcpip_process, NULL);
  #if DEBUG_ANNOTATE
	print_lladdrs();
  #endif
#endif /* endif WITH_UIP6 */

	process_start(&sensors_process, NULL);

	print_processes(autostart_processes); 
	autostart_start(autostart_processes);

	/* Main scheduler loop */
	while(1) {
		check_maca();

		if(uart1_input_handler != NULL) {
			if(uart1_can_get()) {
				uart1_input_handler(uart1_getc());
			}
		}
		
		process_run();
	}
	
	return 0;
}
