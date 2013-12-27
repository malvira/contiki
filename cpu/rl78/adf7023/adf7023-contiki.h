
int adf7023_init(void);

int adf7023_prepare(const void *payload, unsigned short payload_len);
/* Prepare the radio with a packet to be sent. */

int adf7023_transmit(unsigned short transmit_len);
/* Send the packet that has previously been prepared. */

int adf7023_send(const void *payload, unsigned short payload_len);
/* Prepare & transmit a packet. */

int adf7023_read(void *buf, unsigned short buf_len);
/* Prepare & transmit a packet. */

int adf7023_channel_clear(void);
/* Perform a Clear-Channel Assessment (CCA) to find out if there is a packet in the air or not. */

int adf7023_receiving_packet(void);
/* Check if the radio driver is currently receiving a packet. */

int adf7023_pending_packet(void);
/* Check if the radio driver has just received a packet. */

int adf7023_on(void);
/* Turn the radio on. */

int adf7023_off(void);
/* Turn the radio off. */
