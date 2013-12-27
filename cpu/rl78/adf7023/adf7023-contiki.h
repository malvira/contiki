#ifndef __ADF7023-contiki_H__
#define __ADF7023-conitki_H__

int adf7023_init(void);

/* Prepare the radio with a packet to be sent. */
int adf7023_prepare(const void *payload, unsigned short payload_len);

/* Send the packet that has previously been prepared. */
int adf7023_transmit(unsigned short transmit_len);

/* Prepare & transmit a packet. */
int adf7023_send(const void *payload, unsigned short payload_len);

/* Prepare & transmit a packet. */
int adf7023_read(void *buf, unsigned short buf_len);

/* Perform a Clear-Channel Assessment (CCA) to find out if there is a packet in the air or not. */
int adf7023_channel_clear(void);

/* Check if the radio driver is currently receiving a packet. */
int adf7023_receiving_packet(void);

/* Check if the radio driver has just received a packet. */
int adf7023_pending_packet(void);

/* Turn the radio on. */
int adf7023_on(void);

/* Turn the radio off. */
int adf7023_off(void);

#endif /* __ADF7023-contiki_H__ */
