/*
 * config.h
 *
 *  Created on: 23 févr. 2019
 *      Author: francois
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

//------------------------------------------------------------------------
// 4.b) variable defines for the state engine
//------------------------------------------------------------------------

typedef enum {off, forward, backward} tdirection;


typedef enum {RUN_OKAY,     // DCC Running
              RUN_STOP,     // DCC Running, all Engines Emergency Stop
              RUN_OFF,      // Output disabled (2*Taste, PC)
              RUN_SHORT,    // Short circuit;
              RUN_PAUSE,    // DCC Running, all Engines Speed 0
              RUN_REMOTE,	// No local_control, under jmri CONTROL
              PROG_OKAY,    // Prog-Mode running
              PROG_SHORT,   // Prog-Mode + Short circuit
              PROG_OFF,     // Prog-Mode, track off
              PROG_ERROR    // Prog-Mode, Programming Error
              } t_opendcc_state;

extern t_opendcc_state opendcc_state;

//------------------------------------------------------------------------
// define a structure for DCC messages inside OpenDCC
//------------------------------------------------------------------------
#define   MAX_DCC_SIZE  5

typedef enum {is_void,
              is_stop,
              is_loco,
              is_acc,
              is_feedback,
              is_prog,
              is_prog_ack}  tmessage;





#endif /* SRC_CONFIG_H_ */
