/* TODO: DEFINE copyright headers.*/

/* This is just an sketch of what our Komondor headers should look like.
 *
 * Copyright (c) 2017, Universitat Pompeu Fabra.
 * All rights reserved.
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
 *
 * -----------------------------------------------------------------
 *
 * Author  : Sergio Barrachina-Muñoz and Francesc Wilhelmi
 * Created : 2016-12-05
 * Updated : $Date: 2017/03/20 10:32:36 $
 *           $Revision: 1.0 $
 *
 * -----------------------------------------------------------------
 * File description: Node is a component representing any device in the
 * network (i.e., AP, STA, etc.). Node contains all the logic and algorithms
 * for simulating nodes operation.
 *
 * - Bla bla bla...
 */

#include <math.h>
#include <algorithm>
#include <stddef.h>

#include "../list_of_macros.h"
#include "../methods/auxiliary_methods.h"
#include "../methods/power_channel_methods.h"
#include "../methods/backoff_methods.h"
#include "../methods/modulations_methods.h"
#include "../methods/notification_methods.h"
#include "../methods/time_methods.h"
#include "../structures/notification.h"
#include "../structures/logical_nack.h"
#include "../structures/wlan.h"
#include "../structures/logger.h"

// Node component: "TypeII" represents components that are aware of the existence of the simulated time.
component Node : public TypeII{

	// Methods
	public:

		// COST
		void Setup();
		void Start();
		void Stop();

		// Generic
		void InitializeVariables();
		void RestartNode(int called_by_time_out);
		void CallRestartSta();
		void CallSensing();
		void PrintNodeInfo(int info_detail_level);
		void WriteNodeInfo(Logger node_logger, int info_detail_level, char *header_string);
		void PrintOrWriteNodeStatistics(int write_or_print);
		void HandleSlottedBackoffCollision();
		void StartSavingLogs();
		void RecoverFromCtsTimeout();

		// Packets
		Notification GenerateNotification(int packet_type, int destination_id, double tx_duration);
		void SelectDestination();
		void SendResponsePacket();
		void AckTimeout();
		void CtsTimeout();
		void DataTimeout();
		void NavTimeout();
		void RequestMCS();
		void NewPacketGenerated();
		void TrafficGenerator();
		void StartTransmission();

		// NACK
		void SendLogicalNack(LogicalNack logical_nack);
		void CleanNack();

		// Backoff
		void PauseBackoff();
		void ResumeBackoff();

	// Public items (entered by nodes constructor in Komondor simulation)
	public:

		// Specific to a node
		int node_id; 				// Node identifier
		double x;					// X position coordinate
		double y;					// Y position coordinate
		double z;					// Z position coordinate
		char *node_code;			// Name of the Node (only for information displaying purposes)
		int node_type;				// Node type (e.g., AP, STA, ...)
		int destination_id;			// Destination node id (for nodes not belonging to any WLAN)
		double lambda;				// Average notification generation rate [notification/s]
		int ieee_protocol;			// IEEE protocol type
		int primary_channel;		// Primary channel
		int min_channel_allowed;	// Min. allowed channel
		int max_channel_allowed;	// Max. allowed channel
		int num_channels_allowed;	// Maximum number of channels allowed to TX in
		double tpc_min;				// Min. power transmission [pW]
		double tpc_default;			// Default power transmission [pW]
		double tpc_max;				// Max. power transmission [pW]
		double cca_min;				// Min. CCA	("sensitivity" threshold) [pW]
		double cca_default;			// Default CCA	("sensitivity" threshold) [pW]
		double cca_max;				// Max. CCA ("sensitivity" threshold)
		double tx_gain;				// Antenna transmission gain [linear]
		double rx_gain;				// Antenna reception gain [linear]
		int channel_bonding_model;	// Channel bonding model (definition of models in function GetTxChannelsByChannelBonding())
		int modulation_default;		// Default modulation

		// WLAN
		char *wlan_code;			// Code of the WLAN to which the node belongs
		Wlan wlan;					// Wlan to which the node belongs

		/* Same default in every node (parameters from system input and console arguments) */
		// Generic
		double simulation_time_komondor;	// Observation time (time when the simulation stops)
		int total_nodes_number;				// Total number of nodes
		int collisions_model;				// Collisions model (TODO: implement different collision models)
		double capture_effect;				// Capture effect threshold [linear ratio]
		double constant_per;				// Constant PER for correct transmissions
		int save_node_logs;					// Flag for activating the log writting of nodes
		int print_node_logs;				// Flag for activating the printing of node logs
		char *simulation_code;				// Simulation code

		// Channel
		int basic_channel_bandwidth;		// Channel unit bandwidth [Hz]
		int num_channels_komondor;			// Number of subchannels composing the whole channel
		int adjacent_channel_model;			// Adjacent channel interference model (definition of models in function UpdateChannelsPower())
		int pifs_activated;					// PIFS mechanism activation

		// Transmissions
		int default_destination_id;			// Current destination node ID
		double noise_level;					// Environment noise [pW]
		int current_modulation;				// Current_modulation used by nodes
		int channel_max_intereference;		// Channel of interest suffering maximum interference
		double SIFS;						// SIFS [s]
		double DIFS;						// DIFS [s]
		double PIFS;						// PIFS [s]
		double central_frequency;			// Central frequency (Hz)
		int cw_min;							// Backoff minimum Contention Window
		int cw_stage_max;							// Backoff maximum Contention Window
		int pdf_backoff;					// Probability distribution type of the backoff (0: exponential, 1: deterministic)
		int path_loss_model;				// Path loss model (0: free-space, 1: Okumura-Hata model - Uban areas)

		// Data rate - modulations
		int modulation_rates[4][12];		// Modulation rates in bps used in IEEE 802.11ax
		int err_prob_modulation[4][12];		// BER associated to each modulation (TO BE FILLED!!)

		// Packets
		int pdf_tx_time;					// Probability distribution type of the transmission time (0: exponential, 1: deterministic)
		int packet_length;					// Notification length [bits]
		int num_packets_aggregated;			// Number of packets aggregated in one transmission
		int ack_length;						// ACK length [bits]
		int rts_length;						// RTS length [bits]
		int cts_length;						// CTS length [bits]
		int traffic_model;					// Traffic model (0: full buffer, 1: poisson, 2: deterministic)
		int backoff_type;					// Type of Backoff (0: Slotted 1: Continuous)
		int cw_adaptation;					// CW adaptation (0: constant, 1: bineary exponential backoff)

	// Statistics (accessible when simulation finished through Komondor simulation class)
	public:

		int data_packets_sent;
		int rts_cts_sent;
		double *total_time_transmitting_per_channel;		// Time transmitting per channel;
		double *total_time_transmitting_in_num_channels;	// Time transmitting in (ix 0: 1 channel, ix 1: 2 channels...)
		double *total_time_lost_per_channel;				// Time transmitting per channel unsuccessfully;
		double *total_time_lost_in_num_channels;			// Time transmitting in (ix 0: 1 channel, ix 1: 2 channels...) unsuccessfully
		double throughput;									// Throughput [Mbps]
		double throughput_loss;								// Throughput of lost packets [Mbps]
		int data_packets_lost;									// Own packets that have been collided or lost
		int *num_trials_tx_per_num_channels;				// Number of txs trials per number of channels
		int rts_cts_lost;
		int *nacks_received;								// Counter of the type of Nacks received
		int num_tx_init_tried;								// Number of TX initiations tried (whenever transmitter try to acces the channel)
		int num_tx_init_not_possible;						// Number of TX initiations that have been not possible due to channel state and DCB model
		int rts_lost_slotted_bo;							// Number of RTS packets lost due to slotted BO
		double prob_slotted_bo_collision;					// Probability of slotted BO collision

	// Private items (just for node operation)
	private:

		// Komondor environment
		double *channel_power;				// Channel power detected in each sub-channel [pW] (Pico watts for resolution issues)
		int *channels_free;					// Channels that are found free for the beginning TX (i.e. power sensed < CCA)
		int *channels_for_tx;				// Channels that are used in the beginning TX (depend on the channel bonding model)

		// File for writting node logs
		FILE *output_log_file;				// File for logs in which the node is involved
		char own_file_path[32];				// Name of the file for node logs
		Logger node_logger;					// struct containing the attributes needed for writting logs in a file
		char *header_string;				// Header string for the logger

		// State and timers
		int node_state;						// Node's internal state (0: sensing the channel, 1: transmitting, 2: receiving notification)
		double remaining_backoff;			// Remaining backoff
		int progress_bar_counter;			// Counter for displaying the progress bar

		// Transmission parameters
		int node_is_transmitter;			// Flag for determining if node is able to tranmsit packet (e.g., AP in downlink)
		int current_left_channel;			// Left channel used in current TX
		int current_right_channel;			// Right channel used in current TX
		double current_tpc;					// Transmission power used in current TX [dBm]
		double current_cca;					// Current CCA (variable "sensitivity")	[dBm]
		int current_destination_id;			// Current destination node ID
		double current_tx_duration;			// Duration of the TX being done [s]
		double current_nav_time;					// Current NAV duration
		int packet_id;						// Notification ID
		int rts_cts_id;						// Id for RTS/CTS transactions
		double current_sinr;				// SINR perceived in current TX [linear ratio]
		int loss_reason;	// Packet loss reason (if any)

		// Notifications
		Notification rts_notification;		// RTS to be filled before sending it
		Notification cts_notification;		// CTS to be filled before sending it
		Notification data_notification;		// DATA notification to be filled before sending it
		Notification ack_notification;		// ACK to be filled before sending it
		Notification incoming_notification; // Notification of interest being received

		// Last notification that made the node change state or reamin in NAV. It is used for detecting simultaneous events.
		Notification nav_notification;

		// NAV notification sent in a different primary channel. Store it for detecting BO collisions when using CB.
		Notification outrange_nav_notification;

		int default_modulation;				// Default MCS identifier
		double data_rate;					// Data rate [bits per duration of OFDM symbol] being used currently
		double data_rate_20mhz;				// Data rate in one channel (legacy mode)
		int cw_current;						// Contention Window being used currently
		int cw_stage_current;				// Current CW stage

		double data_duration;
		double ack_duration;
		double rts_duration;
		double cts_duration;

		int **mcs_per_node;					// Modulation selected for each of the nodes (only transmitting nodes)
		int *change_modulation_flag;		// Flag for changig the MCS of any of the potential receivers
		int *mcs_response;					// MCS response received from receiver

		// Sensing and Reception parameters
		LogicalNack nack;					// NACK to be filled in case node is the destination of tx loss
		double max_pw_interference;			// Maximum interference detected in range of interest [pW]
		int channel_max_interference;		// Channel of maximum interference detected in range of interest [pW]
		int *nodes_transmitting;			// IDs of the nodes which are transmitting to any destination
		double *power_received_per_node;	// Power received from each node in the network [pW]
		double power_rx_interest;			// Power received from a TX destined to the node [pW]
		int receiving_from_node_id;			// ID of the node that is transmitting to the node (-1 if node is not receiing)
		int receiving_packet_id;			// ID of the notification that is being transmitted to me
		int *hidden_nodes_list;				// Store nodes that for sure are hidden (1 indicates that node "i" is hidden)
		int *potential_hidden_nodes;		// Maintain a list of the times a node participated in a collision by hidden node
		int collisions_by_hidden_node; 		// Number of noticed collisions by hidden node (maintained by the transmitter)
		double BER;							// Bit error rate (deprecated)
		double PER;							// Packet error rate (deprecated)
		int num_packets_in_buffer;			// Number of packets in the buffer pending to be transmitted
		double *timestampt_channel_becomes_free;	// Timestamp when channel becomes free (when P(channel) < CCA)
		double time_to_trigger;				// Auxiliar time to trigger an specific trigger (used for almost every .Set() function)

	// Connections and timers
	public:

		// INPORT connections for receiving notifications
		inport void inline InportSomeNodeStartTX(Notification &notification);
		inport void inline InportSomeNodeFinishTX(Notification &notification);
		inport void inline InportNackReceived(LogicalNack &logical_nack_info);

		inport void inline InportMCSRequestReceived(Notification &notification);
		inport void inline InportMCSResponseReceived(Notification &notification);

		// OUTPORT connections for sending notifications
		outport void outportSelfStartTX(Notification &notification);
		outport void outportSelfFinishTX(Notification &notification);
		outport void outportSendLogicalNack(LogicalNack &logical_nack_info);

		outport void outportAskForTxModulation(Notification &notification);
		outport void outportAnswerTxModulation(Notification &notification);

		// Triggers
		Timer <trigger_t> trigger_sim_time;				// Timer for displaying the exectuion time status (progress bar)
		Timer <trigger_t> trigger_end_backoff; 			// Duration of current trigger_end_backoff. Triggers outportSelfStartTX()
		Timer <trigger_t> trigger_start_backoff;		// Timer for the DIFS
		Timer <trigger_t> trigger_toFinishTX; 			// Duration of current notification transmission. Triggers outportSelfFinishTX()
		Timer <trigger_t> trigger_SIFS;					// Timer for the SIFS
		Timer <trigger_t> trigger_ACK_timeout;			// Trigger when ACK hasn't arrived in time
		Timer <trigger_t> trigger_CTS_timeout;			// Trigger when CTS hasn't arrived in time
		Timer <trigger_t> trigger_DATA_timeout; 		// Trigger when DATA TX could not start due to RTS/CTS failure
		Timer <trigger_t> trigger_NAV_timeout;  		// Trigger for the NAV
		Timer <trigger_t> trigger_new_packet_generated; // Trigger for new packets generation
		Timer <trigger_t> trigger_preoccupancy; 		// Trigger for delaying 1 ps the occupancy of channels after channel range selection
		Timer <trigger_t> trigger_restart_sta; 			// Trigger for retarding the STA restart enough time to handle same time RTS finish events
		Timer <trigger_t> trigger_wait_collisions; 		// Trigger for waiting just in case more RTS collisions are detected at the same time
		Timer <trigger_t> trigger_start_saving_logs; 	// Trigger for starting saving logs
		Timer <trigger_t> trigger_recover_cts_timeout; 	// Trigger for waiting part of EIFS after CTS timeout detected


		// Every time the timer expires execute this
		inport inline void EndBackoff(trigger_t& t1);
		inport inline void MyTxFinished(trigger_t& t1);
		inport inline void PrintProgressBar(trigger_t& t1);
		inport inline void ResumeBackoff(trigger_t& t1);
		inport inline void SendResponsePacket(trigger_t& t1);
		inport inline void AckTimeout(trigger_t& t1);
		inport inline void CtsTimeout(trigger_t& t1);
		inport inline void DataTimeout(trigger_t& t1);
		inport inline void NavTimeout(trigger_t& t1);
		inport inline void NewPacketGenerated(trigger_t& t1);
		inport inline void StartTransmission(trigger_t& t1);
		inport inline void CallRestartSta(trigger_t& t1);
		inport inline void CallSensing(trigger_t& t1);
		inport inline void StartSavingLogs(trigger_t& t1);
		inport inline void RecoverFromCtsTimeout(trigger_t& t1);

		// Connect timers to methods
		Node () {
			connect trigger_end_backoff.to_component,EndBackoff;
			connect trigger_toFinishTX.to_component,MyTxFinished;
			connect trigger_sim_time.to_component,PrintProgressBar;
			connect trigger_start_backoff.to_component,ResumeBackoff;
			connect trigger_SIFS.to_component,SendResponsePacket;
			connect trigger_ACK_timeout.to_component,AckTimeout;
			connect trigger_CTS_timeout.to_component,CtsTimeout;
			connect trigger_DATA_timeout.to_component,DataTimeout;
			connect trigger_NAV_timeout.to_component,NavTimeout;
			connect trigger_new_packet_generated.to_component,NewPacketGenerated;
			connect trigger_preoccupancy.to_component,StartTransmission;
			connect trigger_restart_sta.to_component,CallRestartSta;
			connect trigger_wait_collisions.to_component,CallSensing;
			connect trigger_start_saving_logs.to_component,StartSavingLogs;
			connect trigger_recover_cts_timeout.to_component,RecoverFromCtsTimeout;
		}
};

/*
 * Setup()
 */
void Node :: Setup(){
	// Do nothing
};

/*
 * Start()
 */
void Node :: Start(){

	// Initialize variables
	InitializeVariables();

	// if(print_node_logs) printf("%s(N%d) Start\n", node_code, node_id);

	// Create node logs file if required
	if(save_node_logs) {
		// Name node log file accordingly to the node_id
		sprintf(own_file_path,"%s_%s_N%d_%s.txt","./output/logs_output_", simulation_code, node_id, node_code);
		remove(own_file_path);
		output_log_file = fopen(own_file_path, "at");
		node_logger.save_logs = save_node_logs;
		node_logger.file = output_log_file;
		node_logger.SetVoidHeadString();
	}

	if(save_node_logs) fprintf(node_logger.file,"%.18f;N%d;S%d;%s;%s Start()\n",
			SimTime(), node_id, STATE_UNKNOWN, LOG_B00, LOG_LVL1);

	// Write node info
	header_string = (char *) malloc(LOG_HEADER_NODE_SIZE * sizeof(*header_string));
	sprintf(header_string,"%.15f;N%d;S%d;%s;%s", SimTime(), node_id, node_state, LOG_Z00, LOG_LVL3);
	if(save_node_logs) WriteNodeInfo(node_logger, INFO_DETAIL_LEVEL_2, header_string);

	// Start backoff procedure only if node is able to transmit
	if(node_is_transmitter) {
		TrafficGenerator();
	} else {
		current_destination_id = wlan.ap_id;	// TODO: for uplink traffic. Set STAs destination to the GW
	}

	// Progress bar (trick: it is only printed by node with id 0)
	if(PROGRESS_BAR_DISPLAY){
		if(node_id == 0){
			if(print_node_logs) printf("%s PROGRESS BAR:\n", LOG_LVL1);
			trigger_sim_time.Set(SimTime() + PICO_VALUE);
		}
	}

//	double pw_1m = ComputePowerReceived(1, tpc_default, tx_gain, rx_gain, central_frequency, PATHLOSS_5GHZ_OFFICE_BUILDING);
//	double pw_3m = ComputePowerReceived(3, tpc_default, tx_gain, rx_gain, central_frequency, PATHLOSS_5GHZ_OFFICE_BUILDING);
//	double pw_5m = ComputePowerReceived(5, tpc_default, tx_gain, rx_gain, central_frequency, PATHLOSS_5GHZ_OFFICE_BUILDING);
//	double pw_10m = ComputePowerReceived(10, tpc_default, tx_gain, rx_gain, central_frequency, PATHLOSS_5GHZ_OFFICE_BUILDING);
//	double pw_50m = ComputePowerReceived(50, tpc_default, tx_gain, rx_gain, central_frequency, PATHLOSS_5GHZ_OFFICE_BUILDING);
//
//	double pw_1m_free = ComputePowerReceived(1, tpc_default, tx_gain, rx_gain, central_frequency, PATH_LOSS_LFS);
//	double pw_3m_free = ComputePowerReceived(3, tpc_default, tx_gain, rx_gain, central_frequency, PATH_LOSS_LFS);
//	double pw_5m_free = ComputePowerReceived(5, tpc_default, tx_gain, rx_gain, central_frequency, PATH_LOSS_LFS);
//	double pw_10m_free = ComputePowerReceived(10, tpc_default, tx_gain, rx_gain, central_frequency, PATH_LOSS_LFS);
//	double pw_50m_free = ComputePowerReceived(50, tpc_default, tx_gain, rx_gain, central_frequency, PATH_LOSS_LFS);
//
//	printf("pw_1m = %f pw (%f dBm) - Free: pw_1m_free = %f pw (%f dBm)\n"
//			"pw_3m = %f pw (%f dBm) - Free: pw_3m_free = %f pw (%f dBm)\n"
//			"pw_5m = %f pw (%f dBm) - Free: pw_5m_free = %f pw (%f dBm)\n"
//			"pw_10m = %f pw (%f dBm) - Free: pw_10m_free = %f pw (%f dBm)\n"
//			"pw_50m = %f pw (%f dBm) - Free: pw_50m_free = %f pw (%f dBm)\n",
//			pw_1m, ConvertPower(PW_TO_DBM, pw_1m), pw_1m_free, ConvertPower(PW_TO_DBM, pw_1m_free),
//			pw_3m, ConvertPower(PW_TO_DBM, pw_3m), pw_3m_free, ConvertPower(PW_TO_DBM, pw_3m_free),
//			pw_5m, ConvertPower(PW_TO_DBM, pw_5m), pw_5m_free, ConvertPower(PW_TO_DBM, pw_5m_free),
//			pw_10m, ConvertPower(PW_TO_DBM, pw_10m), pw_10m_free, ConvertPower(PW_TO_DBM, pw_10m_free),
//			pw_50m, ConvertPower(PW_TO_DBM, pw_50m), pw_50m_free, ConvertPower(PW_TO_DBM, pw_50m_free));


	// Write in log from a given timestamp on
//     save_node_logs = FALSE;
//     trigger_start_saving_logs.Set(SimTime() + 656);

	if(save_node_logs) fprintf(node_logger.file,"\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");

	// if(save_node_logs) fprintf(node_logger.file, "%f;N%d;S%d;%s;%s Start() END\n", SimTime(), node_id, node_state, LOG_B01, LOG_LVL1);
};

/*
 * Stop()
 */
void Node :: Stop(){

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Node Stop()\n",
			SimTime(), node_id, node_state, LOG_C00, LOG_LVL1);

	// Print and write node statistics if required
	PrintOrWriteNodeStatistics(PRINT_LOG);
	PrintOrWriteNodeStatistics(WRITE_LOG);

	// Close node logs file
	if(save_node_logs) fclose(node_logger.file);

	// if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Node info:\n", SimTime(), node_id, node_state, LOG_C01, LOG_LVL1);
};

/*
 * InportSomeNodeStartTX(): called when some node (this one included) starts a TX
 * Input arguments:
 * - notification: notification containing the information of the transmission start perceived
 */
void Node :: InportSomeNodeStartTX(Notification &notification){

	if(save_node_logs) fprintf(node_logger.file,
			"%.15f;N%d;S%d;%s;%s InportSomeNodeStartTX(): N%d to N%d sends packet type %d in range %d-%d\n",
			SimTime(), node_id, node_state, LOG_D00, LOG_LVL1,
			notification.source_id, notification.tx_info.destination_id, notification.packet_type,
			notification.left_channel, notification.right_channel);

	if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s Nodes transmitting: ",
				SimTime(), node_id, node_state, LOG_D00, LOG_LVL3);

	// Identify node that has started the transmission as transmitting node in the array
	nodes_transmitting[notification.source_id] = TRUE;

	if(save_node_logs) PrintOrWriteNodesTransmitting(WRITE_LOG, save_node_logs,
			print_node_logs, node_logger, total_nodes_number, nodes_transmitting);

	if(notification.source_id == node_id){ // If OWN NODE IS THE TRANSMITTER, do nothing

//		if(save_node_logs) fprintf(node_logger.file,
//				"%.15f;N%d;S%d;%s;%s I have started a TX of packet #%d (type %d) to N%d in channels %d - %d of duration %.9f us\n",
//				SimTime(), node_id, node_state, LOG_D02, LOG_LVL2, notification.tx_info.packet_id,
//				notification.packet_type, notification.tx_info.destination_id,
//				notification.left_channel, notification.right_channel, notification.tx_info.tx_duration * pow(10,6));

	} else {	// If OTHER NODE IS THE TRANSMITTER

//		if(save_node_logs) fprintf(node_logger.file,
//				"%.15f;N%d;S%d;%s;%s N%d has started a TX of packet #%d (type %d) to N%d in channels %d - %d\n",
//				SimTime(), node_id, node_state, LOG_D02, LOG_LVL2, notification.source_id,
//				notification.tx_info.packet_id,	notification.packet_type, notification.tx_info.destination_id,
//				notification.left_channel, notification.right_channel);

		// Update the power sensed at each channel
		UpdateChannelsPower(x, y, z, &channel_power, notification, TX_INITIATED,
				central_frequency, num_channels_komondor, path_loss_model, rx_gain, adjacent_channel_model,
				node_id);

		if(save_node_logs) fprintf(node_logger.file,
					"%.15f;N%d;S%d;%s;%s Power sensed per channel: ",
					SimTime(), node_id, node_state, LOG_E18, LOG_LVL3);

		PrintOrWriteChannelPower(WRITE_LOG, save_node_logs, node_logger, print_node_logs,
						&channel_power, num_channels_komondor);

		// Call UpdatePowerSensedPerNode() ONLY for adding power (some node started)
		UpdatePowerSensedPerNode(primary_channel, power_received_per_node, notification, x, y, z,
				rx_gain, central_frequency, path_loss_model, TX_INITIATED);

//		if(save_node_logs) fprintf(node_logger.file,
//					"%.15f;N%d;S%d;%s;%s START Channel before updating: ",
//					SimTime(), node_id, node_state, LOG_E18, LOG_LVL3);
//
//		PrintOrWriteChannelPower(WRITE_LOG, save_node_logs, node_logger, print_node_logs,
//						channel_power, num_channels_komondor);

		UpdateTimestamptChannelFreeAgain(timestampt_channel_becomes_free, &channel_power,
				current_cca, num_channels_komondor, SimTime());

		if(save_node_logs) {

			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s timestampt_channel_becomes_frees: ",
										SimTime(), node_id, node_state, LOG_F02, LOG_LVL3);

			for(int i = 0; i < num_channels_komondor; i++){

				fprintf(node_logger.file, "%.9f  ", timestampt_channel_becomes_free[i]);

			}
			fprintf(node_logger.file, "\n");

			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s difference times: ",
										SimTime(), node_id, node_state, LOG_F02, LOG_LVL3);

			for(int i = 0; i < num_channels_komondor; i++){

				fprintf(node_logger.file, "%.9f  ", SimTime() - timestampt_channel_becomes_free[i]);

			}

			fprintf(node_logger.file, "\n");

		}

		// Decide action according to current state and Notification initiated
		switch(node_state){

			/* STATE_SENSING:
			 * - If node IS destination and notification is data packet:
			 *   * check if packet can be received
			 *      - If packet lost: generateNack and handle backoff
			 *      - If packet can be received: state = RECEIVE_DATA and pause backoff
			 * - If node IS NOT destination: handle backoff
			 */
			case STATE_SENSING:{

				if(notification.tx_info.destination_id == node_id){	// Node IS THE DESTINATION

					// Update power received of interest
					power_rx_interest = power_received_per_node[notification.source_id];

					current_left_channel = notification.left_channel;
					current_right_channel = notification.right_channel;

					if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s I am the TX destination (N%d). Checking if notification can be received.\n",
							SimTime(), node_id, node_state, LOG_D07, LOG_LVL3,
							notification.tx_info.destination_id);

					// Compute max interference (the highest one perceived in the reception channel range)
					ComputeMaxInterference(&max_pw_interference, &channel_max_intereference,
							notification, node_state, power_received_per_node, &channel_power);

					if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s P[%d] = %f dBm - P_st = %f dBm - P_if = %f dBm\n",
							SimTime(), node_id, node_state, LOG_D08, LOG_LVL5,
							channel_max_intereference,
							ConvertPower(PW_TO_DBM, channel_power[channel_max_intereference]),
							ConvertPower(PW_TO_DBM, power_rx_interest),
							ConvertPower(PW_TO_DBM, max_pw_interference));

					if(notification.packet_type == PACKET_TYPE_RTS) {	// Notification CONTAINS an RTS PACKET

						current_sinr = UpdateSINR(power_rx_interest, noise_level, max_pw_interference);

						if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s SINR = %.2f dBm\n",
							SimTime(), node_id, node_state, LOG_D08, LOG_LVL5,
							ConvertPower(LINEAR_TO_DB, current_sinr));

						// Check if notification has been lost due to interferences or weak signal strength
						loss_reason = IsPacketLost(primary_channel, notification, notification,
								current_sinr, capture_effect, current_cca,
								power_rx_interest, constant_per, hidden_nodes_list, node_id);

						if(loss_reason != PACKET_NOT_LOST) {	// If RTS IS LOST, send logical Nack

							// Check if lost due to BO collision
							if(loss_reason == PACKET_LOST_INTERFERENCE){

								if(fabs(outrange_nav_notification.timestampt - notification.timestampt)
										< MAX_DIFFERENCE_SAME_TIME){
									loss_reason = PACKET_LOST_BO_COLLISION;
								}

							}

							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Reception of notification %d from N%d CANNOT be started because of reason %d\n",
									SimTime(), node_id, node_state, LOG_D15, LOG_LVL4, notification.tx_info.packet_id,
									notification.source_id, loss_reason);

							// Send logical NACK to transmitter sending incoming notification for indicating the loss reason
							LogicalNack logical_nack = GenerateLogicalNack(notification.packet_type, notification.tx_info.packet_id,
									node_id, notification.source_id, NODE_ID_NONE, loss_reason, BER, current_sinr);

							SendLogicalNack(logical_nack);

							if(node_is_transmitter){

								int pause = HandleBackoff(PAUSE_TIMER, &channel_power, primary_channel, current_cca,
										num_packets_in_buffer);

								// Check if node has to freeze the BO (if it is not already frozen)
								if (pause) PauseBackoff();
							}

						} else {	// Data packet IS NOT LOST (it can be properly received)

							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Reception of RTS %d from N%d CAN be started (SINR = %f dB)\n",
									SimTime(), node_id, node_state, LOG_D16, LOG_LVL4, notification.tx_info.packet_id,
									notification.source_id, ConvertPower(LINEAR_TO_DB, current_sinr));

							/*
							 * Save incoming notification. This is kept in order to compare new notifications to the current
							 * one for determining if it is lost and stuff like that.
							 */
							incoming_notification = notification;

							// Change state and update receiving info
							data_duration = notification.tx_info.data_duration;
							ack_duration = notification.tx_info.ack_duration;
							rts_duration = notification.tx_info.rts_duration;
							cts_duration = notification.tx_info.cts_duration;

							current_left_channel = notification.left_channel;
							current_right_channel = notification.right_channel;

							node_state = STATE_RX_RTS;
							receiving_from_node_id = notification.source_id;
							receiving_packet_id = notification.tx_info.packet_id;

							// Pause backoff as node has began a reception
							if(node_is_transmitter) PauseBackoff();

						}

					} else {	//	Notification does NOT CONTAIN an RTS
						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Unexpected packet type (%d) received!\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4, notification.packet_type);
					}

				} else {	// Node IS NOT THE DESTINATION

					// See if node has to change to NAV
					if(notification.packet_type == PACKET_TYPE_RTS
							|| notification.packet_type == PACKET_TYPE_CTS){

						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s I am not the TX destination (N%d to N%d). Checking if RTS/CTS can be decoded.\n",
								SimTime(), node_id, node_state, LOG_D07, LOG_LVL2,
								notification.source_id, notification.tx_info.destination_id);

						// Can RTS or CTS packet be decoded?
						power_rx_interest = power_received_per_node[notification.source_id];

						// Compute max interference (the highest one perceived in the reception channel range)
						ComputeMaxInterference(&max_pw_interference, &channel_max_intereference,
							notification, node_state, power_received_per_node, &channel_power);

						current_sinr = UpdateSINR(power_rx_interest, noise_level, max_pw_interference);


						if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s Pmax_int[%d] = %f dBm - P_st = %f dBm - P_if = %f dBm, sinr = %f dB\n",
							SimTime(), node_id, node_state, LOG_D08, LOG_LVL5,
							channel_max_intereference, ConvertPower(PW_TO_DBM, channel_power[channel_max_intereference]),
							ConvertPower(PW_TO_DBM, power_rx_interest),
							ConvertPower(PW_TO_DBM, max_pw_interference),
							ConvertPower(LINEAR_TO_DB,current_sinr));

						loss_reason = IsPacketLost(primary_channel, notification, notification, current_sinr,
								capture_effect, current_cca, power_rx_interest, constant_per, hidden_nodes_list, node_id);

						if(loss_reason == PACKET_NOT_LOST) { // RTS/CTS can be decoded

							if (notification.packet_type == PACKET_TYPE_CTS) {
								if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s CTS can be decoded\n",
									SimTime(), node_id, node_state, LOG_D07, LOG_LVL3);
							} else if (notification.packet_type == PACKET_TYPE_RTS){
								if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s RTS can be decoded\n",
									SimTime(), node_id, node_state, LOG_D07, LOG_LVL3);
							}

							// Save NAV notifcation for comparing timestamps in case of need
							nav_notification = notification;

							if(node_is_transmitter){

								int pause = HandleBackoff(PAUSE_TIMER, &channel_power, primary_channel, current_cca,
										num_packets_in_buffer);

								// Check if node has to freeze the BO (if it is not already frozen)
								if (pause) PauseBackoff();

							}

							current_nav_time = notification.tx_info.nav_time;

							// SERGIO on 28/09/2017:
							// - Ensure NAV TO finishes at same time (or before) than other's WLAN ACK transmission.
							// time_to_trigger = SimTime() + current_nav_time + TIME_OUT_EXTRA_TIME;
							time_to_trigger = SimTime() + current_nav_time - TIME_OUT_EXTRA_TIME;

							// SERGIO_TRIGGER
							// trigger_NAV_timeout.Set(round_to_digits(time_to_trigger,12));
							trigger_NAV_timeout.Set(fix_time_offset(time_to_trigger,13,12));

							if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Entering in NAV and setting NAV timeout to %.12f\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL3,
								trigger_NAV_timeout.GetTime());

							if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s current_nav_time = %.12f\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4,
								current_nav_time);

							node_state = STATE_NAV;

						} else { // RTS/CTS cannot be decoded.

							if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s RTS/CTS sent from N%d could not be decoded for reason %d\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL3,
								notification.source_id, loss_reason);

							// Save NAV notifcation for comparing timestamps in case of need
							outrange_nav_notification = notification;

							// Check if DIFS or BO must be stopped
							if(node_is_transmitter){

								int pause = HandleBackoff(PAUSE_TIMER, &channel_power, primary_channel, current_cca,
										num_packets_in_buffer);

								// Check if node has to freeze the BO (if it is not already frozen)
								if (pause) PauseBackoff();

							}

						}

					} else if (notification.packet_type == PACKET_TYPE_DATA ||
							   notification.packet_type == PACKET_TYPE_ACK){

						if(node_is_transmitter){

							int pause = HandleBackoff(PAUSE_TIMER, &channel_power, primary_channel, current_cca,
									num_packets_in_buffer);

							// Check if node has to freeze the BO (if it is not already frozen)
							if (pause) PauseBackoff();
						}

					}

				}

				break;
			}

			/* STATE_TX_NAV
			 * - If node IS destination: generateNack to incoming notification transmitter
			 * - If node IS NOT destination: check if NAV trigger must be updated
			 */
			case STATE_NAV:{

				if(notification.tx_info.destination_id == node_id){	// Node IS THE DESTINATION

					// If two or more packets sent at the same time
					if(fabs(nav_notification.timestampt - notification.timestampt) < MAX_DIFFERENCE_SAME_TIME){

						if(notification.packet_type == PACKET_TYPE_RTS) {	// Notification CONTAINS an RTS PACKET

							if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s RTS from my AP N%d sent simultaneously\n",
								SimTime(), node_id, node_state, LOG_D16, LOG_LVL4,
								notification.source_id);

							// Update power received of interest
							power_rx_interest = power_received_per_node[notification.source_id];

							// Compute max interference (the highest one perceived in the reception channel range)
							ComputeMaxInterference(&max_pw_interference, &channel_max_intereference,
								notification, node_state, power_received_per_node, &channel_power);

							current_sinr = UpdateSINR(power_rx_interest, noise_level, max_pw_interference);


							if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s P[%d] = %f dBm - P_st = %f dBm - P_if = %f dBm\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL5, channel_max_intereference,
								ConvertPower(PW_TO_DBM, channel_power[channel_max_intereference]),
								ConvertPower(PW_TO_DBM, power_rx_interest),
								ConvertPower(PW_TO_DBM, max_pw_interference));

							// Check if notification has been lost due to interferences or weak signal strength
							loss_reason = IsPacketLost(primary_channel, notification, notification,
									current_sinr, capture_effect, current_cca,
									power_rx_interest, constant_per, hidden_nodes_list, node_id);

							if(loss_reason != PACKET_NOT_LOST) {	// If RTS IS LOST, send logical Nack

								loss_reason = PACKET_LOST_BO_COLLISION;

								if(!node_is_transmitter) {

									// Sergio 18/09/2017:
									// NAV is no longer valid. It cannot be decoded due to interferences.
									// Wait MAX_DIFFERENCE_SAME_TIME to detect more transmissions sent at the "same" time
									// Trigger the restart then.

									// Sergio on 27/09/2017. Review this case
									if(save_node_logs) fprintf(node_logger.file,
										"%.15f;N%d;S%d;%s;%s RTS from my AP CANNOT be decoded\n",
										SimTime(), node_id, node_state, LOG_D08, LOG_LVL5);

									trigger_NAV_timeout.Cancel();
									time_to_trigger = SimTime() + MAX_DIFFERENCE_SAME_TIME;
									// trigger_NAV_timeout.Set(fix_time_offset(time_to_trigger,13,12));
									trigger_restart_sta.Set(fix_time_offset(time_to_trigger,13,12));

								} else {

									printf("ALARM! KOTIN\n");

								}
								// EOF HandleSlottedBackoffCollision();

								if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s RTS cannot be decoded (SINR = %f dB) -> Sending NACK corresponding to BO collision to N%d\n",
									SimTime(), node_id, node_state, LOG_D16, LOG_LVL5,
									ConvertPower(LINEAR_TO_DB, current_sinr), notification.source_id);

								LogicalNack logical_nack = GenerateLogicalNack(notification.packet_type,
									notification.tx_info.packet_id, node_id, notification.source_id,
									NODE_ID_NONE, loss_reason, BER, current_sinr);

								SendLogicalNack(logical_nack);

							} else {	// Data packet IS NOT LOST (it can be properly received)

								if(save_node_logs) fprintf(node_logger.file,
										"%.15f;N%d;S%d;%s;%s Reception of RTS %d from N%d CAN be started (SINR = %f dB)\n",
										SimTime(), node_id, node_state, LOG_D16, LOG_LVL4, notification.tx_info.packet_id,
										notification.source_id, ConvertPower(LINEAR_TO_DB, current_sinr));

								trigger_NAV_timeout.Cancel();

								// Change state and update receiving info
								data_duration = notification.tx_info.data_duration;
								ack_duration = notification.tx_info.ack_duration;
								rts_duration = notification.tx_info.rts_duration;
								cts_duration = notification.tx_info.cts_duration;

								current_left_channel = notification.left_channel;
								current_right_channel = notification.right_channel;

								node_state = STATE_RX_RTS;
								receiving_from_node_id = notification.source_id;
								receiving_packet_id = notification.tx_info.packet_id;

								incoming_notification = notification;

								// Pause backoff as node has began a reception
								if(node_is_transmitter) PauseBackoff();

							}

						} else {	//	Notification does NOT CONTAIN an RTS
							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Unexpected packet type (%d) received!\n",
									SimTime(), node_id, node_state, LOG_D08, LOG_LVL4, notification.packet_type);
						}

					} else { // Notification not detected to happen at the sime time

						// Send logical NACK to transmitter sending incoming notification for indicating the loss reason
						LogicalNack logical_nack = GenerateLogicalNack(notification.packet_type, notification.tx_info.packet_id,
								node_id, notification.source_id, NODE_ID_NONE, PACKET_LOST_RX_IN_NAV, BER, current_sinr);

						SendLogicalNack(logical_nack);

					}

				} else { // Node IS NOT THE DESTINATION

					if(notification.packet_type == PACKET_TYPE_RTS ||
							notification.packet_type == PACKET_TYPE_CTS) {	// PACKET TYPE RTS OR CTS

						// TODO: determine if decodificable!
						if(fabs(nav_notification.timestampt - notification.timestampt) < MAX_DIFFERENCE_SAME_TIME){

							if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s NAV collision detected\n",
								SimTime(), node_id, node_state, LOG_D07, LOG_LVL2);

							// SERGIO HandleSlottedBackoffCollision();
							loss_reason = PACKET_LOST_BO_COLLISION;

							if(!node_is_transmitter) {

								trigger_NAV_timeout.Cancel();
								time_to_trigger = SimTime() + MAX_DIFFERENCE_SAME_TIME;
								trigger_NAV_timeout.Set(fix_time_offset(time_to_trigger,13,12));

								if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s (workaround) setting NAV trigger to %.12f\n",
									SimTime(), node_id, node_state, LOG_D07, LOG_LVL3,
									trigger_NAV_timeout.GetTime());

							} else {

								if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Waiting just in case of more collisions.\n",
									SimTime(), node_id, node_state, LOG_D07, LOG_LVL4);

								trigger_NAV_timeout.Cancel();

								// Sergio on 27/09/2017
								// - An AP must wait EIFS after the last packet of external RTSs collisions is finished.

								time_to_trigger =
										SimTime() + MAX_DIFFERENCE_SAME_TIME + SIFS +
										notification.tx_info.cts_duration + DIFS;

								trigger_wait_collisions.Set(fix_time_offset(time_to_trigger,13,12));

								// node_state = STATE_SLEEP;

							}

							// Do not send NACK because node is not the destination

							// EOF HandleSlottedBackoffCollision();


						} else { // No collision

							// Check if it can be decoded to update NAV time if required
							current_sinr = UpdateSINR(power_rx_interest, noise_level, max_pw_interference);


							loss_reason = IsPacketLost(primary_channel, incoming_notification, notification,
									current_sinr, capture_effect, current_cca,
									power_rx_interest, constant_per, hidden_nodes_list, node_id);

							if (loss_reason == PACKET_NOT_LOST &&
								ConvertPower(PW_TO_DBM, channel_power[primary_channel]) > current_cca) {

								nav_notification = notification;

								// Update NAV trigger
								if(trigger_NAV_timeout.GetTime() < notification.tx_info.nav_time) {

									time_to_trigger = SimTime() +  notification.tx_info.nav_time + TIME_OUT_EXTRA_TIME;

									trigger_NAV_timeout.Set(fix_time_offset(time_to_trigger,13,12));

									if(save_node_logs) fprintf(node_logger.file,
										"%.15f;N%d;S%d;%s;%s Updating NAV timeout to the more restrictive one: From %.12f to %.12f\n",
										SimTime(), node_id, node_state, LOG_D07, LOG_LVL4,
										trigger_NAV_timeout.GetTime(), time_to_trigger);

								}

								if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s New RTS/CTS arrived from (N%d). Setting NAV to new value %.18f\n",
									SimTime(), node_id, node_state, LOG_D07, LOG_LVL3,
									notification.source_id, trigger_NAV_timeout.GetTime());
							}
						}
					}
				}

				break;
			}

			/* STATE_TX_DATA || STATE_TX_ACK:
			 * - If node IS destination: generateNack to incoming notification transmitter
			 * - If node IS NOT destination: do nothing
			 */
			case STATE_TX_DATA:
			case STATE_TX_ACK:
			case STATE_TX_RTS:
			case STATE_TX_CTS:{

				if(notification.tx_info.destination_id == node_id){ // Node IS THE DESTINATION

					if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s I am the TX destination (N%d)\n",
							SimTime(), node_id, node_state, LOG_D07, LOG_LVL3,
							notification.tx_info.destination_id);

					if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s I am transmitting, packet cannot be received\n",
							SimTime(), node_id, node_state, LOG_D18, LOG_LVL3);

					// Send logical NACK to incoming notification transmitter due to receiver (node) was already receiving
					LogicalNack logical_nack = GenerateLogicalNack(notification.packet_type, notification.tx_info.packet_id,
							node_id, notification.source_id, NODE_ID_NONE, PACKET_LOST_DESTINATION_TX, BER, current_sinr);

					SendLogicalNack(logical_nack);

				} else {	// Node IS NOT THE DESTINATION, do nothing

//					if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s I am NOT the TX destination (N%d)\n",
//							SimTime(), node_id, node_state, LOG_D08, LOG_LVL3, notification.tx_info.destination_id);

				}

				break;
			}

			/* STATE_RX_DATA || STATE_RX_ACK:
			 * - If node IS destination: generateNack to incoming notification transmitter
			 * - Check if ongoing packet reception is lost:
			 *   * If packet lost: generate NACK to source of ongoing reception and restart node
			 *   * If packet NOT lost: do nothing
			 */
			case STATE_RX_DATA:
			case STATE_RX_ACK:
			case STATE_RX_RTS:
			case STATE_RX_CTS:{

				if(notification.tx_info.destination_id == node_id){	// Node IS THE DESTINATION

//					if(save_node_logs) fprintf(node_logger.file,
//							"%.15f;N%d;S%d;%s;%s I am the TX destination (N%d)\n",
//							SimTime(), node_id, node_state, LOG_D07, LOG_LVL3, notification.tx_info.destination_id);

					// Check if ongoing notification has been lost due to interferences caused by new transmission
					current_sinr = UpdateSINR(power_rx_interest, noise_level, max_pw_interference);

					loss_reason = IsPacketLost(primary_channel, incoming_notification, notification,
							current_sinr, capture_effect, current_cca,
							power_rx_interest, constant_per, hidden_nodes_list, node_id);

					if(loss_reason != PACKET_NOT_LOST
							&& loss_reason != PACKET_LOST_OUTSIDE_CH_RANGE)  {	// If ongoing data packet IS LOST

							// Pure collision (two nodes transmitting to me with enough power)
							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Pure collision! Already receiving from N%d\n",
									SimTime(), node_id, node_state, LOG_D19, LOG_LVL4, receiving_from_node_id);

							loss_reason = PACKET_LOST_PURE_COLLISION;

							// If two or more packets sent at the same time
							if(fabs(notification.timestampt - incoming_notification.timestampt) < MAX_DIFFERENCE_SAME_TIME){

								// SERGIO HandleSlottedBackoffCollision();
								loss_reason = PACKET_LOST_BO_COLLISION;

								if(!node_is_transmitter) {

									time_to_trigger = SimTime() + MAX_DIFFERENCE_SAME_TIME;

									trigger_NAV_timeout.Set(fix_time_offset(time_to_trigger,13,12));

								} else {

									// TODO!!!!!!!
									// In case STAs can send to AP
									printf("ALARM 2! TU TU\n");
								}

								// EOF HandleSlottedBackoffCollision();

							}

							// Send NACK to both ongoing transmitter and incoming interferer nodes
							LogicalNack logical_nack = GenerateLogicalNack(notification.packet_type, nav_notification.tx_info.packet_id,
									node_id, nav_notification.source_id, notification.source_id, loss_reason, BER, current_sinr);

							SendLogicalNack(logical_nack);


					} else {	// If ongoing data packet IS NOT LOST (incoming transmission does not affect ongoing reception)

						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Low strength signal received while already receiving from N%d\n",
							SimTime(), node_id, node_state, LOG_D20, LOG_LVL4, receiving_from_node_id);

						// Send logical NACK to incoming transmitter indicating that node is already receiving
						LogicalNack logical_nack = GenerateLogicalNack(notification.packet_type, receiving_from_node_id,
								node_id, notification.source_id, NODE_ID_NONE, PACKET_LOST_LOW_SIGNAL_AND_RX, BER, current_sinr);

						SendLogicalNack(logical_nack);

					}

				} else {	// Node is NOT THE DESTINATION

//					if(save_node_logs) fprintf(node_logger.file,
//							"%.15f;N%d;S%d;%s;%s I am NOT the TX destination (N%d)\n",
//							SimTime(), node_id, node_state, LOG_D08, LOG_LVL3, notification.tx_info.destination_id);

					// Compute max interference (the highest one perceived in the reception channel range)
					ComputeMaxInterference(&max_pw_interference, &channel_max_intereference,
							incoming_notification, node_state, power_received_per_node, &channel_power);

					// Check if the ongoing reception is affected
					current_sinr = UpdateSINR(power_rx_interest, noise_level, max_pw_interference);


					if(save_node_logs) fprintf(node_logger.file,
						"%.15f;N%d;S%d;%s;%s P[%d] = %f dBm - P_st = %f dBm - P_if = %f dBm - current_sinr = %.2f dBm\n",
						SimTime(), node_id, node_state, LOG_D08, LOG_LVL5, channel_max_intereference,
						ConvertPower(PW_TO_DBM, channel_power[channel_max_intereference]),
						ConvertPower(PW_TO_DBM, power_rx_interest),
						ConvertPower(PW_TO_DBM, max_pw_interference),
						ConvertPower(LINEAR_TO_DB, current_sinr));

					// Check if the notification that was already being received is lost due to new notification
					loss_reason = IsPacketLost(primary_channel, incoming_notification, notification,
							current_sinr, capture_effect, current_cca,
							power_rx_interest, constant_per, hidden_nodes_list, node_id);

					if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s loss_reason = %d\n",
						SimTime(), node_id, node_state, LOG_D19, LOG_LVL4, loss_reason);

					if(loss_reason != PACKET_NOT_LOST) { 	// If ongoing packet reception IS LOST

						// Collision by hidden node
						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Collision by interferences!\n",
								SimTime(), node_id, node_state, LOG_D19, LOG_LVL4);

						// Sergio on 18 Oct 2017:
						// - I decide to remove this source of collision because it is not a regular BO collision
						// - The CE at the AP may be not achieved by the sum of several transmitters at the same time.
						// -  ****** Old code begins ******
							// // If two or more packets sent at the same time
							// if(fabs(notification.timestampt - incoming_notification.timestampt) < MAX_DIFFERENCE_SAME_TIME){
							//
							//	 loss_reason = PACKET_LOST_BO_COLLISION;
							//
							//	if(!node_is_transmitter) {
							//
							//		trigger_NAV_timeout.Cancel();
							//		trigger_NAV_timeout.Set(SimTime());
							//
							//	} else {
							//
							//		// Collision by hidden node
							//		if(save_node_logs) fprintf(node_logger.file,
							//			"%.15f;N%d;S%d;%s;%s WHYO2?\n",
							//			SimTime(), node_id, node_state, LOG_D19, LOG_LVL4);
							//
							//	}
							//
							//	// EOF HandleSlottedBackoffCollision();
							//
							// }
						/// -  ****** Old code finishes ******


						 // If two or more packets sent at the same time
						if(node_state == STATE_RX_RTS && notification.packet_type == PACKET_TYPE_RTS){

							if(fabs(notification.timestampt - incoming_notification.timestampt) < MAX_DIFFERENCE_SAME_TIME){

								 loss_reason = PACKET_LOST_BO_COLLISION;


//								if(!node_is_transmitter) {
//
//									trigger_NAV_timeout.Cancel();
//									trigger_NAV_timeout.Set(SimTime());
//
//								} else {
//
//									// Collision by hidden node
//									if(save_node_logs) fprintf(node_logger.file,
//										"%.15f;N%d;S%d;%s;%s WHYO2?\n",
//										SimTime(), node_id, node_state, LOG_D19, LOG_LVL4);
//
//								}

								// EOF HandleSlottedBackoffCollision();

							}
						}
						// Send logical NACK to ongoing transmitter
						LogicalNack logical_nack = GenerateLogicalNack(incoming_notification.packet_type,
								incoming_notification.tx_info.packet_id, node_id, incoming_notification.source_id,
								NODE_ID_NONE, loss_reason, BER, current_sinr);

						SendLogicalNack(logical_nack);

						RestartNode(FALSE);
					}
				}
				break;
			}

			/* STATE_WAIT_ACK:
			 * - If node IS destination and packet is ACK:
			 *   * Check if ACK packet can be received:
			 *     - If packet lost: generate Nack to source of ongoing ACK reception and do nothing until ACK timeout expires
			 *     - If packet NOT lost: cancel ACK timeout and state = STATE_RX_ACK
			 * - If node IS NOT destination: do nothing
			 */
			case STATE_WAIT_ACK:{

				if(notification.tx_info.destination_id == node_id){	// Node is the destination

					power_rx_interest = power_received_per_node[notification.source_id];

					incoming_notification = notification;

//					if(save_node_logs) fprintf(node_logger.file,
//							"%.15f;N%d;S%d;%s;%s I am the TX destination (N%d). Checking if notification can be received.\n",
//							SimTime(), node_id, node_state, LOG_D07, LOG_LVL3, notification.tx_info.destination_id);

					if(notification.packet_type == PACKET_TYPE_ACK){	// ACK packet transmission started

						// Compute max interference (the highest one perceived in the reception channel range)
						ComputeMaxInterference(&max_pw_interference, &channel_max_intereference,
							incoming_notification, node_state, power_received_per_node, &channel_power);

						// Check if notification has been lost due to interferences or weak signal strength
						current_sinr = UpdateSINR(power_rx_interest, noise_level, max_pw_interference);

						loss_reason = IsPacketLost(primary_channel, incoming_notification, notification,
								current_sinr, capture_effect, current_cca,
								power_rx_interest, constant_per, hidden_nodes_list, node_id);

						if(loss_reason != PACKET_NOT_LOST
								&& loss_reason != PACKET_LOST_OUTSIDE_CH_RANGE) {	// If ACK packet IS LOST, send logical Nack

							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Reception of notification %d from N%d CANNOT be started because of reason %d\n",
									SimTime(), node_id, node_state, LOG_D15, LOG_LVL4, notification.tx_info.packet_id,
									notification.source_id, loss_reason);

							// Send logical NACK to ACK transmitter
							LogicalNack logical_nack = GenerateLogicalNack(incoming_notification.packet_type, incoming_notification.tx_info.packet_id,
									node_id, receiving_from_node_id, NODE_ID_NONE, loss_reason, BER, current_sinr);

							SendLogicalNack(logical_nack);

							// Do nothing until ACK timeout is triggered

						} else {	// If ACK packet IS NOT LOST (it can be properly received)

							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Reception of ACK %d from N%d CAN be started\n",
									SimTime(), node_id, node_state, LOG_D16, LOG_LVL4, notification.tx_info.packet_id, notification.source_id);

							// Cancel ACK timeout and go to STATE_RX_ACK while updating receiving info
							trigger_ACK_timeout.Cancel();
							node_state = STATE_RX_ACK;
							receiving_from_node_id = notification.source_id;
							receiving_packet_id = notification.tx_info.packet_id;

//							if(save_node_logs) fprintf(node_logger.file,
//									"%.15f;N%d;S%d;%s;%s I am the TX destination (N%d)\n",
//									SimTime(), node_id, node_state, LOG_D16, LOG_LVL4, notification.tx_info.destination_id);

//							if(save_node_logs) fprintf(node_logger.file,
//									"%.15f;N%d;S%d;%s;%s current_sinr = %f dB\n",
//									SimTime(), node_id, node_state, LOG_D16, LOG_LVL5,
//									ConvertPower(LINEAR_TO_DB,current_sinr));

						}

					}  else {	//	Some packet type received that is not ACK
						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Unexpected packet type received!\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4);
					}

				} else {	// Node IS NOT THE DESTINATION, do nothing
//
//					if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s I am NOT the TX destination (N%d)\n",
//								SimTime(), node_id, node_state, LOG_D08, LOG_LVL3, notification.tx_info.destination_id);

				}
				break;
			}

			/* STATE_WAIT_CTS:
			 * - If node IS destination and packet is CTS:
			 *   * Check if CTS packet can be received:

			 * - If node IS NOT destination: do nothing
			 */
			case STATE_WAIT_CTS:{

				if(notification.tx_info.destination_id == node_id){	// Node is the destination

					incoming_notification = notification;

//					if(save_node_logs) fprintf(node_logger.file,
//							"%.15f;N%d;S%d;%s;%s I am the TX destination (N%d). Checking if notification can be received.\n",
//							SimTime(), node_id, node_state, LOG_D07, LOG_LVL3, notification.tx_info.destination_id);

					if(notification.packet_type == PACKET_TYPE_CTS){	// CTS packet transmission started

						power_rx_interest = power_received_per_node[notification.source_id];

						// Compute max interference (the highest one perceived in the reception channel range)
						ComputeMaxInterference(&max_pw_interference, &channel_max_intereference,
								incoming_notification, node_state, power_received_per_node, &channel_power);

						// Check if notification has been lost due to interferences or weak signal strength
						current_sinr = UpdateSINR(power_rx_interest, noise_level, max_pw_interference);


//						if(save_node_logs) fprintf(node_logger.file,
//							"%.15f;N%d;S%d;%s;%s P_sn = %f dBm (%f pW) - P_st= %f dBm (%f pW)"
//							"- P_if = %f dBm (%f pW)\n",
//							SimTime(), node_id, node_state, LOG_D08, LOG_LVL5,
//							ConvertPower(PW_TO_DBM, channel_power[channel_max_intereference]), channel_power[channel_max_intereference],
//							ConvertPower(PW_TO_DBM, power_rx_interest), power_rx_interest, ConvertPower(PW_TO_DBM, max_pw_interference),
//							max_pw_interference);

						loss_reason = IsPacketLost(primary_channel, incoming_notification, notification,
								current_sinr, capture_effect, current_cca,
								power_rx_interest, constant_per, hidden_nodes_list, node_id);

						if(loss_reason != PACKET_NOT_LOST
								&& loss_reason != PACKET_LOST_OUTSIDE_CH_RANGE)  {	// If CTS packet IS LOST, send logical Nack

							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Reception of notification %d from N%d CANNOT be started because of reason %d\n",
									SimTime(), node_id, node_state, LOG_D15, LOG_LVL4, notification.tx_info.packet_id,
									notification.source_id, loss_reason);

							// Send logical NACK to ACK transmitter
							LogicalNack logical_nack = GenerateLogicalNack(notification.packet_type, notification.tx_info.packet_id,
									node_id, notification.source_id,
									NODE_ID_NONE, loss_reason, BER, current_sinr);

							SendLogicalNack(logical_nack);

							// Do nothing until ACK timeout is triggered

						} else {	// If CTS packet IS NOT LOST (it can be properly received)

							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Reception of CTS #%d from N%d CAN be started\n",
									SimTime(), node_id, node_state, LOG_D16, LOG_LVL4, notification.tx_info.packet_id, notification.source_id);

							// Cancel ACK timeout and go to STATE_RX_ACK while updating receiving info
							trigger_CTS_timeout.Cancel();
							node_state = STATE_RX_CTS;
							receiving_from_node_id = notification.source_id;
							receiving_packet_id = notification.tx_info.packet_id;

							// Change state and update receiving info
							data_duration = notification.tx_info.data_duration;
							ack_duration = notification.tx_info.ack_duration;
							cts_duration = notification.tx_info.cts_duration;

//							if(save_node_logs) fprintf(node_logger.file,
//									"%.15f;N%d;S%d;%s;%s I am the TX destination (N%d)\n",
//									SimTime(), node_id, node_state, LOG_D16, LOG_LVL4, notification.tx_info.destination_id);

//							if(save_node_logs) fprintf(node_logger.file,
//									"%.15f;N%d;S%d;%s;%s current_sinr = %f dB\n",
//									SimTime(), node_id, node_state, LOG_D16, LOG_LVL5, ConvertPower(LINEAR_TO_DB,current_sinr));

						}

					}  else {	//	Some packet type received that is not CTS
						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Unexpected packet type received!\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4);
					}

				} else {	// Node IS NOT THE DESTINATION, do nothing

//					if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s I am NOT the TX destination (N%d)\n",
//								SimTime(), node_id, node_state, LOG_D08, LOG_LVL3, notification.tx_info.destination_id);

				}
				break;
			}

			/* STATE_WAIT_DATA:
			 * - If node IS destination and packet is DATA:
			 *   * Check if DATA packet can be received:
			 * - If node IS NOT destination: do nothing
			 */
			case STATE_WAIT_DATA:{

				if(notification.tx_info.destination_id == node_id){	// Node is the destination

					power_rx_interest = power_received_per_node[notification.source_id];
					incoming_notification = notification;

//					if(save_node_logs) fprintf(node_logger.file,
//							"%.15f;N%d;S%d;%s;%s I am the TX destination (N%d). Checking if notification can be received.\n",
//							SimTime(), node_id, node_state, LOG_D07, LOG_LVL3, notification.tx_info.destination_id);

					if(notification.packet_type == PACKET_TYPE_DATA){	// DATA packet transmission started

						// Compute max interference (the highest one perceived in the reception channel range)
						ComputeMaxInterference(&max_pw_interference, &channel_max_intereference,
							incoming_notification, node_state, power_received_per_node, &channel_power);

						// Check if notification has been lost due to interferences or weak signal strength
						current_sinr = UpdateSINR(power_rx_interest, noise_level, max_pw_interference);

						if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s P[%d] = %f dBm - P_st = %f dBm - P_if = %f dBm - current_sinr = %.2f dBm\n",
							SimTime(), node_id, node_state, LOG_D08, LOG_LVL5, channel_max_intereference,
							ConvertPower(PW_TO_DBM, channel_power[channel_max_intereference]),
							ConvertPower(PW_TO_DBM, power_rx_interest),
							ConvertPower(PW_TO_DBM, max_pw_interference),
							ConvertPower(LINEAR_TO_DB, current_sinr));

						loss_reason = IsPacketLost(primary_channel, incoming_notification, notification,
								current_sinr, capture_effect, current_cca,
								power_rx_interest, constant_per, hidden_nodes_list, node_id);

						if(loss_reason != PACKET_NOT_LOST
							&& loss_reason != PACKET_LOST_OUTSIDE_CH_RANGE)  {	// If DATA packet IS LOST, send logical Nack

							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Reception of notification %d from N%d CANNOT be started because of reason %d\n",
									SimTime(), node_id, node_state, LOG_D15, LOG_LVL4, notification.tx_info.packet_id,
									notification.source_id, loss_reason);

							// Send logical NACK to DATA transmitter
							LogicalNack logical_nack = GenerateLogicalNack(notification.packet_type, notification.tx_info.packet_id,
									node_id, notification.source_id,
									NODE_ID_NONE, loss_reason, BER, current_sinr);

							SendLogicalNack(logical_nack);


						} else {	// If DATA packet IS NOT LOST (it can be properly received)

							if(save_node_logs) fprintf(node_logger.file,
									"%.15f;N%d;S%d;%s;%s Reception of DATA %d from N%d CAN be started\n",
									SimTime(), node_id, node_state, LOG_D16, LOG_LVL4, notification.tx_info.packet_id, notification.source_id);

							// Cancel DATA timeout and go to STATE_RX_DATA while updating receiving info
							trigger_DATA_timeout.Cancel();
							node_state = STATE_RX_DATA;
							receiving_from_node_id = notification.source_id;
							receiving_packet_id = notification.tx_info.packet_id;

							// Change state and update receiving info
							data_duration = notification.tx_info.data_duration;
							ack_duration = notification.tx_info.ack_duration;

						}

					}  else {	//	Some packet type received that is not ACK
						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Unexpected packet type received!\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4);
					}

				} else {	// Node IS NOT THE DESTINATION, do nothing

//					if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s I am NOT the TX destination (N%d)\n",
//								SimTime(), node_id, node_state, LOG_D08, LOG_LVL3, notification.tx_info.destination_id);

				}
				break;
			}

			case STATE_SLEEP:{
				// do nothing
				break;
			}

			default:{
				printf("ERROR: %d is not a correct state\n", node_state);
				exit(EXIT_FAILURE);
				break;
			}
		}
	}
	// if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s InportSomeNodeStartTX() END\n", SimTime(), node_id, node_state, LOG_D01, LOG_LVL1);
};

/*
 * InportSomeNodeFinishTX(): called when some node finishes a packet TX (data or ACK)
 * Input arguments:
 * - notification: notification containing the information of the transmission that has finished
 */
void Node :: InportSomeNodeFinishTX(Notification &notification){

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s InportSomeNodeFinishTX(): N%d to N%d (type %d)"
			"at range %d-%d "
			"- nodes transmitting: ",
		SimTime(), node_id, node_state, LOG_E00, LOG_LVL1,
		notification.source_id, notification.tx_info.destination_id, notification.packet_type,
		notification.left_channel, notification.right_channel);

	// Identify node that has finished the transmission as non-transmitting node in the array
	nodes_transmitting[notification.source_id] = FALSE;
	if(save_node_logs) PrintOrWriteNodesTransmitting(WRITE_LOG, save_node_logs,
			print_node_logs, node_logger, total_nodes_number, nodes_transmitting);

	if(notification.source_id == node_id){	// Node is the TX source: do nothing

//		if(save_node_logs) fprintf(node_logger.file,
//				"%.15f;N%d;S%d;%s;%s I have finished the TX of packet #%d (type %d) in channel range: %d - %d\n",
//				SimTime(), node_id, node_state, LOG_E18, LOG_LVL2, notification.tx_info.packet_id,
//				notification.packet_type, notification.left_channel, notification.right_channel);

	} else {	// Node is not the TX source

//		if(save_node_logs) fprintf(node_logger.file,
//				"%.15f;N%d;S%d;%s;%s N%d has finished the TX of packet #%d (type %d) in channel range: %d - %d\n",
//				SimTime(), node_id, node_state, LOG_E18, LOG_LVL2, notification.source_id,
//				notification.tx_info.packet_id, notification.packet_type, notification.left_channel,
//				notification.right_channel);



//		if(save_node_logs) fprintf(node_logger.file,
//			"%.15f;N%d;S%d;%s;%s Channel before updating: ",
//			SimTime(), node_id, node_state, LOG_E18, LOG_LVL3);
//
//		PrintOrWriteChannelPower(WRITE_LOG, save_node_logs, node_logger, print_node_logs,
//				channel_power, num_channels_komondor);

		// Update the power sensed at each channel
		UpdateChannelsPower(x, y, z, &channel_power, notification, TX_FINISHED,
				central_frequency, num_channels_komondor, path_loss_model, rx_gain, adjacent_channel_model,
				node_id);

//		int cas = 0;
//		for(int i = 0; i < total_nodes_number; i++){
//			if(nodes_transmitting[i] == 1){
//				cas ++;
//			}
//		}
//		if(cas == 0){
//
//			// printf("EDED\n");
//
//			for(int i = 0; i < num_channels_komondor; i++){
//				channel_power[i] = 0;
//			}
//
//		}

		if(save_node_logs) fprintf(node_logger.file,
			"%.15f;N%d;S%d;%s;%s Power sensed per channel: ",
			SimTime(), node_id, node_state, LOG_E18, LOG_LVL3);

		PrintOrWriteChannelPower(WRITE_LOG, save_node_logs, node_logger, print_node_logs,
				&channel_power, num_channels_komondor);

		// Call UpdatePowerSensedPerNode() ONLY for adding power (some node started)
		UpdatePowerSensedPerNode(primary_channel, power_received_per_node, notification, x, y, z,
				rx_gain, central_frequency, path_loss_model, TX_FINISHED);

		UpdateTimestamptChannelFreeAgain(timestampt_channel_becomes_free, &channel_power,
				current_cca, num_channels_komondor, SimTime());

		if(save_node_logs) {

			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s timestampt_channel_becomes_free: ",
										SimTime(), node_id, node_state, LOG_F02, LOG_LVL3);

			for(int i = 0; i < num_channels_komondor; i++){

				fprintf(node_logger.file, "%.9f  ", timestampt_channel_becomes_free[i]);

			}
			fprintf(node_logger.file, "\n");

			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s difference times: ",
										SimTime(), node_id, node_state, LOG_F02, LOG_LVL3);

			for(int i = 0; i < num_channels_komondor; i++){

				fprintf(node_logger.file, "%.9f  ", SimTime() - timestampt_channel_becomes_free[i]);

			}

			fprintf(node_logger.file, "\n");

		}



		switch(node_state){

			/* STATE_SENSING:
			 * - handle backoff
			 */
			case STATE_SENSING:{	// Do backoff process

				if(node_is_transmitter) {

					if(!trigger_end_backoff.Active()){	// BO was paused

						int resume = HandleBackoff(RESUME_TIMER, &channel_power, primary_channel, current_cca,
								num_packets_in_buffer);

						if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s P[%d] = %f dBm (%f)\n",
							SimTime(), node_id, node_state, LOG_D08, LOG_LVL5,
							primary_channel, ConvertPower(PW_TO_DBM, channel_power[primary_channel]), channel_power[primary_channel]);

						if (resume) {	// BO can be resumed

							// Sergio on 26/09/2017. EIFS vs NAV.
							// - To identify if previous packet lost to trigger the EIFS
							// - If not, just resume the backoff

							time_to_trigger = SimTime() + DIFS;
							// time_to_trigger = SimTime() + SIFS + notification.tx_info.cts_duration + DIFS;

							trigger_start_backoff.Set(fix_time_offset(time_to_trigger,13,12));

							if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s BO will be resumed after DIFS at %.12f.\n",
									SimTime(), node_id, node_state, LOG_E11, LOG_LVL4,
									trigger_start_backoff.GetTime());

//							if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s EIFS started.\n",
//														SimTime(), node_id, node_state, LOG_E11, LOG_LVL4);

						} else {	// BO cannot be resumed

							if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s EIFS cannot be started.\n",
								SimTime(), node_id, node_state, LOG_E11, LOG_LVL4);

						}
					} else {	// BO was already active

						if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s BO was already active.\n",
								SimTime(), node_id, node_state, LOG_E11, LOG_LVL4);

					}
				}

				break;
			}

			/* STATE_RX_DATA:
			 * - If node IS the destination and data packet transmission finished:
			 *   * start SIFS and generate ACK
			 *   * state = STATE_TX_ACK
			 * - If node IS NOT the destination: do nothing
			 */
			case STATE_RX_DATA:{	// Check if the current reception is finished

				if(notification.tx_info.destination_id == node_id){ 	// Node IS THE DESTINATION

					if(notification.packet_type == PACKET_TYPE_DATA){	// Data packet transmission finished

						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Packet #%d reception from N%d is finished successfully.\n",
								SimTime(), node_id, node_state, LOG_E14, LOG_LVL3, notification.tx_info.packet_id,
								notification.source_id);

						// Generate and send ACK to transmitter after SIFS
						node_state = STATE_TX_ACK;
						current_tx_duration = ack_duration;
						current_destination_id = notification.source_id;
						ack_notification = GenerateNotification(PACKET_TYPE_ACK, current_destination_id, current_tx_duration);

						// triggers the SendResponsePacket() function after SIFS
						time_to_trigger = SimTime() + SIFS;
						trigger_SIFS.Set(fix_time_offset(time_to_trigger,13,12));

//						if(save_node_logs) fprintf(node_logger.file,
//							"%.15f;N%d;S%d;%s;%s Not rounded: %.18f, %.18f\n",
//							SimTime(), node_id, node_state, LOG_E14, LOG_LVL4,
//							time_to_finish, SIFS);
//
//						double time_to_finish_v2 = round_to_digits(time_to_finish + FEMTO_VALUE, 15);
//
//						if(save_node_logs) fprintf(node_logger.file,
//							"%.15f;N%d;S%d;%s;%s Rounded: %.18f\n",
//							SimTime(), node_id, node_state, LOG_E14, LOG_LVL4,
//							time_to_finish_v2);
//
//						// time_to_finish = truncate_Sergio(time_to_finish, 15);
//
//						if(save_node_logs) fprintf(node_logger.file,
//							"%.15f;N%d;S%d;%s;%s Truncated: %.18f\n",
//							SimTime(), node_id, node_state, LOG_E14, LOG_LVL4,
//							truncate_Sergio(time_to_finish_v2, 14));
//
//						trigger_SIFS.Set(time_to_finish_v2); // triggers the SendResponsePacket() function after SIFS

						if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s SIFS will be triggered in %.12f\n",
							SimTime(), node_id, node_state, LOG_E14, LOG_LVL3,
							trigger_SIFS.GetTime());

					} else {	// Other packet type transmission finished
						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Unexpected packet type transmission finished!\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4);
					}

				} else {	// Node IS NOT THE DESTINATION, do nothing

					if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s Still noticing a packet transmission (#%d) from N%d.\n",
							SimTime(), node_id, node_state, LOG_E15, LOG_LVL3, notification.tx_info.packet_id,
							notification.source_id);

				}

				break;
			}

			/* STATE_RX_ACK:
			 * - If node IS the destination and ACK packet transmission finished:
			 *   * decrease contention window and restart node
			 *   * state = STATE_SENSING (implicit on restart)
			 * - If node IS NOT the destination: do nothing
			 */
			case STATE_RX_ACK:{	// Check if the current reception is finished

				if(notification.tx_info.destination_id == node_id){	// Node IS THE DESTINATION

					if(notification.packet_type == PACKET_TYPE_ACK){	// ACK packet transmission finished

						// TODO: packet_acked ++

						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s ACK #%d reception from N%d is finished successfully.\n",
								SimTime(), node_id, node_state, LOG_E14, LOG_LVL3, notification.tx_info.packet_id,
								notification.source_id);

						current_tx_duration += (notification.tx_info.tx_duration + SIFS);	// Add ACK time to tx_duration

						// Sergio on 20/09/2017:
						// - Transmission succeeded ---> reset CW if binary exponential backoff is implemented
						HandleContentionWindow(
								cw_adaptation, RESET_CW, &cw_current, cw_min, &cw_stage_current, cw_stage_max);

						// Restart node (implicitly to STATE_SENSING)

						// Extra slot for successful transmissions
						RestartNode(FALSE);

					} else {	// Other packet type transmission finished
						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Unexpected packet type transmission finished!\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4);
					}

				} else {	// Node IS NOT THE DESTINATION

					if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Still receiving packet #%d reception from N%d.\n",
							SimTime(), node_id, node_state, LOG_E15, LOG_LVL3, incoming_notification.tx_info.packet_id,
							incoming_notification.source_id);
				}

				break;
			}

			/* STATE_TX_DATA || STATE_TX_ACK:
			 * - Do nothing
			 */
			case STATE_TX_DATA:
			case STATE_TX_ACK:
			case STATE_WAIT_ACK:
			case STATE_TX_RTS:
			case STATE_TX_CTS:
			case STATE_WAIT_CTS:
			case STATE_WAIT_DATA:
			case STATE_NAV:{	// Do nothing
				break;
			}

			/* STATE_RX_RTS:
			 * -
			 */
			case STATE_RX_RTS:{

				if(notification.tx_info.destination_id == node_id){	// Node IS THE DESTINATION

					if(notification.packet_type == PACKET_TYPE_RTS){	// RTS packet transmission finished

						if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s RTS #%d reception from N%d is finished successfully.\n",
							SimTime(), node_id, node_state, LOG_E14, LOG_LVL3, notification.tx_info.packet_id,
							notification.source_id);

						// Check channel availability in order to send the CTS
						if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s Checking if CTS can be sent: P_sen = %f dBm, CCA = %f dBm.\n",
							SimTime(), node_id, node_state, LOG_E14, LOG_LVL3,
							ConvertPower(PW_TO_DBM, channel_power[primary_channel]),
							ConvertPower(PW_TO_DBM, current_cca));

						// TODO: check max interference sensed in TX channel range instead
						// of doing "ConvertPower(PW_TO_DBM, channel_power[primary_channel]) < current_cca"

						if(ConvertPower(PW_TO_DBM, channel_power[primary_channel]) < current_cca) {

							if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Channel(s) is (are) clear! Sending CTS to N%d (STATE = %d) ...\n",
								SimTime(), node_id, node_state, LOG_E14, LOG_LVL3, current_destination_id, node_state);

							node_state = STATE_TX_CTS;
							// Generate and send CTS to transmitter after SIFS
							current_destination_id = notification.source_id;

							current_tx_duration = cts_duration;

							current_nav_time = ComputeNavTime(node_state, rts_duration, cts_duration, data_duration, ack_duration, SIFS);

							time_to_trigger = SimTime() + SIFS;
							trigger_SIFS.Set(fix_time_offset(time_to_trigger,13,12)); // triggers the SendResponsePacket() function after SIFS

							if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s SIFS will be triggered in %.12f\n",
								SimTime(), node_id, node_state, LOG_E14, LOG_LVL3,
								trigger_SIFS.GetTime());

							cts_notification = GenerateNotification(PACKET_TYPE_CTS, current_destination_id, current_tx_duration);

						} else {
							// CANNOT START PACKET TX

							if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s NO PUEDE PASAR!\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4);

							/*
							 * STAs should wait MAX_DIFFERENCE_SAME_TIME in order to avoid entering in NAV when it is not required.
							 * E.g. STA A is sensing and is able to decode a packet from AP A. At the same time AP B transmits and
							 * harms AP A - STA A transmission. STA A is restarted. Again, at the same time AP C transmits. Then,
							 * in order to avoid entering in NAV when in fact a slotted BO collision did happen, STA A should not
							 * listen to AP C packet. After MAX_DIFFERENCE_SAME_TIME, no same time events are ensured and STA A can
							 * start sensing again.
							 */
							if(!node_is_transmitter) {
								time_to_trigger = SimTime() + MAX_DIFFERENCE_SAME_TIME;
								trigger_restart_sta.Set(fix_time_offset(time_to_trigger,13,12));
							} else {
								RestartNode(FALSE);
							}
						}

					} else {	// Other packet type transmission finished
						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Unexpected packet type transmission finished!\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4);
					}

				} else {	// Node IS NOT THE DESTINATION

					if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Still receiving packet #%d reception from N%d.\n",
							SimTime(), node_id, node_state, LOG_E15, LOG_LVL3, incoming_notification.tx_info.packet_id,
							incoming_notification.source_id);
				}

				break;
			}


			/* STATE_RX_CTS:
			 * -
			 */
			case STATE_RX_CTS:{

				if(notification.tx_info.destination_id == node_id){	// Node IS THE DESTINATION

					if(notification.packet_type == PACKET_TYPE_CTS){	// RTS packet transmission finished

						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s CTS #%d reception from N%d is finished successfully.\n",
								SimTime(), node_id, node_state, LOG_E14, LOG_LVL3, notification.tx_info.packet_id,
								notification.source_id);

						node_state = STATE_TX_DATA;

						// Generate and send DATA to transmitter after SIFS
						current_destination_id = notification.source_id;

						// Sergio on 20/09/2017. CW only must be changed when ACK received or loss detected.
						// cw_current = HandleContentionWindow(RESET_CW, cw_current, cw_min, cw_stage_max);


						current_tx_duration = data_duration;	// This duration already computed in EndBackoff
						time_to_trigger = SimTime() + SIFS;

						trigger_SIFS.Set(fix_time_offset(time_to_trigger,13,12));

						if(save_node_logs) fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s SIFS will be triggered in %.12f\n",
							SimTime(), node_id, node_state, LOG_E14, LOG_LVL3,
							trigger_SIFS.GetTime());

						data_notification = GenerateNotification(PACKET_TYPE_DATA, current_destination_id, current_tx_duration);


					} else {	// Other packet type transmission finished
						if(save_node_logs) fprintf(node_logger.file,
								"%.15f;N%d;S%d;%s;%s Unexpected packet type transmission finished!\n",
								SimTime(), node_id, node_state, LOG_D08, LOG_LVL4);
					}

				} else {	// Node IS NOT THE DESTINATION

					if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Still receiving packet #%d reception from N%d.\n",
							SimTime(), node_id, node_state, LOG_E15, LOG_LVL3, incoming_notification.tx_info.packet_id,
							incoming_notification.source_id);
				}

				break;
			}

			case STATE_SLEEP:{
				// do nothing
				break;
			}

			default:{
				printf("ERROR: %d is not a correct state\n", node_state);
				exit(EXIT_FAILURE);
				break;
			}
		}
	}
	// if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s InportSomeNodeFinishTX() END",	SimTime(), node_id, node_state, LOG_E01, LOG_LVL1);
};

/*
 * InportNackReceived(): called when some node sends a logical NACK
 * Input arguments:
 * - logical_nack_info: NACK information
 */
void Node :: InportNackReceived(LogicalNack &logical_nack){

	int nack_reason;

//	if(save_node_logs) fprintf(node_logger.file,
//			"%.15f;N%d;S%d;%s;%s InportNackReceived(): N%d to N%d (A) and N%d (B)\n",
//			SimTime(), node_id, node_state, LOG_H00, LOG_LVL1, logical_nack.source_id,
//			logical_nack.node_id_a, logical_nack.node_id_b);

	// If node is involved in the NACK
	if(logical_nack.source_id != node_id &&
			(node_id == logical_nack.node_id_a || node_id == logical_nack.node_id_b)){

		if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s NACK of packet #%d received from N%d sent to a:N%d (and b:N%d) with reason %d\n",
				SimTime(), node_id, node_state, LOG_H00, LOG_LVL2, logical_nack.packet_id, logical_nack.source_id,
				logical_nack.node_id_a, logical_nack.node_id_b, logical_nack.loss_reason);

		// Process logical NACK for statistics purposes
		nack_reason = ProcessNack(logical_nack, node_id, node_logger, node_state, save_node_logs,
				SimTime(), nacks_received, hidden_nodes_list, potential_hidden_nodes,
				total_nodes_number, nodes_transmitting);

		if(nack_reason == PACKET_LOST_BO_COLLISION){
			rts_lost_slotted_bo ++;

			if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s ++++++++++++++++++++++++++++++++\n",
				SimTime(), node_id, node_state, LOG_H00, LOG_LVL2);

			if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s rts_lost_slotted_bo ++\n",
				SimTime(), node_id, node_state, LOG_H00, LOG_LVL2);

			if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s ++++++++++++++++++++++++++++++++\n",
				SimTime(), node_id, node_state, LOG_H00, LOG_LVL2);
		}

	} else {	// Node is the NACK transmitter, do nothing

//		if(save_node_logs) fprintf(node_logger.file,
//				"%.15f;N%d;S%d;%s;%s NACK of packet #%d sent to a) N%d and b) N%d with reason %d\n",
//				SimTime(), node_id, node_state, LOG_H00, LOG_LVL2, logical_nack.packet_id,
//				logical_nack.node_id_a, logical_nack.node_id_b, logical_nack.loss_reason);

	}

	// if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;G01;%s InportNackReceived() END\n", SimTime(), node_id, LOG_LVL1);
}

/*
 * InportMCSRequestReceived(): called when some node asks (logically) the receiver for selectable MCS configurations
 * Input arguments:
 * - notification: notification containing the MCS request
 */
void Node :: InportMCSRequestReceived(Notification &notification){

	if(notification.tx_info.destination_id == node_id) {	// If node IS THE DESTINATION

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s MCS request received from N%d\n",
				SimTime(), node_id, node_state, LOG_F00, LOG_LVL1, notification.source_id);

		// Compute distance and power received from transmitter
		double distance = ComputeDistance(x, y, z, notification.tx_info.x,
				notification.tx_info.y, notification.tx_info.z);

		double power_rx_interest = ComputePowerReceived(distance,
				notification.tx_info.tx_power, tx_gain, rx_gain,
				central_frequency, path_loss_model);

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s I am at distance: %.2f m (sensing P_rx = %.2f dBm)\n",
						SimTime(), node_id, node_state, LOG_F00, LOG_LVL2,
						distance, ConvertPower(PW_TO_DBM, power_rx_interest));


		// Select the modulation according to the SINR perceived corresponding to incoming transmitter
		SelectMCSResponse(mcs_response, power_rx_interest);

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s mcs_response for 1, 2, 4 and 8 channels: ",
			SimTime(), node_id, node_state, LOG_F00, LOG_LVL3);

		PrintOrWriteArrayInt(mcs_response, 4, WRITE_LOG, save_node_logs,
						print_node_logs, node_logger);

		// Fill and send MCS response
		Notification response_mcs  = GenerateNotification(PACKET_TYPE_MCS_RESPONSE,
				notification.source_id, TX_DURATION_NONE);

		outportAnswerTxModulation(response_mcs);

	} else { 	// If node IS NOT THE DESTINATION
		// Do nothing
	}
}

/*
 * InportMCSResponseReceived(): called when some node answers back to a MCS request
 * Input arguments:
 * - notification: notification containing the MCS response
 */
void Node :: InportMCSResponseReceived(Notification &notification){

	if(notification.tx_info.destination_id == node_id) {	// If node IS THE DESTINATION


		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s InportMCSResponseReceived()\n",
				SimTime(), node_id, node_state, LOG_F00, LOG_LVL1);

		int ix_aux = current_destination_id - wlan.list_sta_id[0];	// Auxiliary index for correcting the node id offset

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s MCS per number of channels: ",
				SimTime(), node_id, node_state, LOG_F00, LOG_LVL2);

		// printf("- N%d MCS per number of channels: ", current_destination_id);

		// Set receiver modulation to the received one
		for (int i = 0; i < NUM_OPTIONS_CHANNEL_LENGTH; i++){

			mcs_per_node[ix_aux][i] = notification.tx_info.modulation_schemes[i];
			if(save_node_logs) fprintf(node_logger.file, "%d ", mcs_per_node[ix_aux][i]);
			// printf("%d ", mcs_per_node[ix_aux][i]);
		}

		if(save_node_logs) fprintf(node_logger.file, "\n");

		// printf("\n");

		// TODO: ADD LOGIC TO HANDLE WRONG SITUATIONS (cannot transmit over none of the channel combinations)
		if(mcs_per_node[ix_aux][0] == -1) {

			// CANNOT TX EVEN FOR 1 CHANNEL
			if(current_tpc < MAX_POWER) {

				current_tpc ++;
				change_modulation_flag[ix_aux] = TRUE;

			} else {

				// NODE UNREACHABLE
				if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Unreachable node: transmissions to N%d are cancelled\n",
						SimTime(), node_id, node_state, LOG_G00, LOG_LVL3, current_destination_id);
				// TODO: unreachable_nodes[current_destination_id] = TRUE;
			}
		}

	} else {	// If node IS NOT THE DESTINATION
		// Do nothing
	}
}

/*
 * TrafficGenerator(): called each time a packet is generated to start a new packet generation
 */
void Node :: TrafficGenerator() {

	double time_for_next_packet = 0;

	switch(traffic_model) {

		// 0
		case TRAFFIC_FULL_BUFFER:{

			num_packets_in_buffer = PACKET_BUFFER_SIZE;

			if(node_is_transmitter){

				// Identify if backoff can be resumed
				int resume = HandleBackoff(RESUME_TIMER, &channel_power, primary_channel, current_cca,
						num_packets_in_buffer);

				if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Start DFIS (%.12f)\n",
						SimTime(), node_id, node_state, LOG_G00, LOG_LVL3, DIFS);

				if (resume) {
					time_to_trigger = SimTime() + DIFS;
					trigger_start_backoff.Set(fix_time_offset(time_to_trigger,13,12));
				}
			}

			break;
		}

		// 1
		case TRAFFIC_POISSON:{
			time_for_next_packet = Exponential(1/lambda);
			time_to_trigger = SimTime() + time_for_next_packet;
			trigger_new_packet_generated.Set(fix_time_offset(time_to_trigger,13,12));
			break;
		}

		// 2
		case TRAFFIC_DETERMINISTIC:{
			time_for_next_packet = 1/lambda;
			time_to_trigger = SimTime() + time_for_next_packet;
			trigger_new_packet_generated.Set(fix_time_offset(time_to_trigger,13,12));
			break;
		}

		default:{
			printf("Wrong traffic model!\n");
			exit(EXIT_FAILURE);
			break;
		}

	}

}

/*
 * NewPacketGenerated(): triggered by trigger_new_packet_generated
 */
void Node :: NewPacketGenerated(trigger_t &){

	if(node_is_transmitter){

		if (num_packets_in_buffer < PACKET_BUFFER_SIZE) {

			num_packets_in_buffer++;

//			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s A new packet has been generated (%d/%d)\n",
//					SimTime(), node_id, node_state, LOG_F00, LOG_LVL4, num_packets_in_buffer,PACKET_BUFFER_SIZE);

			// Attempt to restart BO only if node didn't have any packet before a new packet was generated
			if(node_state == STATE_SENSING && num_packets_in_buffer == 1) {

				if(trigger_end_backoff.Active()) remaining_backoff =
						ComputeRemainingBackoff(backoff_type, trigger_end_backoff.GetTime() - SimTime());

				int resume = HandleBackoff(RESUME_TIMER, &channel_power, primary_channel, current_cca,
						num_packets_in_buffer);

				if (resume) {
					time_to_trigger = SimTime() + DIFS;
					trigger_start_backoff.Set(fix_time_offset(time_to_trigger,13,12));
				}

			}

		} else {
			// Buffer overflow - new packet is lost
		}

	}

	TrafficGenerator();

}

/*
 * Pre-occupancy calls this
 */
void Node :: StartTransmission(trigger_t &){
	outportSelfStartTX(rts_notification);
}

/*
 * EndBackoff(): called when backoff finishes
 * Input arguments:
 * - trigger_t: backoff trigger
 */
void Node :: EndBackoff(trigger_t &){

	if(save_node_logs) fprintf(node_logger.file, "\n----------------------------------------------------------\n");
	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s EndBackoff()\n",
			SimTime(), node_id, node_state, LOG_F00, LOG_LVL1);

	// Cancel NAV TO trigger
	trigger_NAV_timeout.Cancel();

	// Cancel trigger for safety
	trigger_recover_cts_timeout.Cancel();

	// Check if MCS already defined for every potential receiver
	for(int n = 0; n < wlan.num_stas; n++) {
		current_destination_id = wlan.list_sta_id[n];
		// Receive the possible MCS to be used for each number of channels
		if (change_modulation_flag[n]) {

			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Requesting MCS to N%d\n",
				SimTime(), node_id, node_state, LOG_F02, LOG_LVL2, current_destination_id);

			RequestMCS();
		}
	}

	// Pick one receiver from the pool of potential receivers
	SelectDestination();

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Trying to start TX to STA N%d\n",
				SimTime(), node_id, node_state, LOG_F02, LOG_LVL2, current_destination_id);

//	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Selected destination: N%d\n",
//				SimTime(), node_id, node_state, LOG_F02, LOG_LVL2, current_destination_id);

	// Identify free channels
	num_tx_init_tried ++;

	GetChannelOccupancyByCCA(primary_channel, pifs_activated, channels_free, min_channel_allowed, max_channel_allowed,
			&channel_power, current_cca, timestampt_channel_becomes_free, SimTime(), PIFS);


	if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s Power sensed per channel: ",
				SimTime(), node_id, node_state, LOG_E18, LOG_LVL3);

	PrintOrWriteChannelPower(WRITE_LOG, save_node_logs, node_logger, print_node_logs,
			&channel_power, num_channels_komondor);

	if(save_node_logs) {

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s timestampt_channel_becomes_frees: ",
									SimTime(), node_id, node_state, LOG_F02, LOG_LVL3);

		for(int i = 0; i < num_channels_komondor; i++){

			fprintf(node_logger.file, "%.9f  ", timestampt_channel_becomes_free[i]);

		}
		fprintf(node_logger.file, "\n");

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s difference times: ",
									SimTime(), node_id, node_state, LOG_F02, LOG_LVL3);

		for(int i = 0; i < num_channels_komondor; i++){

			fprintf(node_logger.file, "%.9f  ", SimTime() - timestampt_channel_becomes_free[i]);

		}

		fprintf(node_logger.file, "\n");

	}

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Channels founds free (mind PIFS if activated): ",
			SimTime(), node_id, node_state, LOG_F02, LOG_LVL3);


	PrintOrWriteChannelsFree(WRITE_LOG, save_node_logs, print_node_logs, node_logger,
			num_channels_komondor, channels_free);

	// Identify the channel range to TX in depending on the channel bonding scheme and free channels
	int ix_mcs_per_node = current_destination_id - wlan.list_sta_id[0];

	GetTxChannelsByChannelBonding(channels_for_tx, channel_bonding_model, channels_free,
			min_channel_allowed, max_channel_allowed, primary_channel,
			mcs_per_node, ix_mcs_per_node, num_channels_komondor);

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Channels for transmitting: ",
			SimTime(), node_id, node_state, LOG_F02, LOG_LVL2);

	PrintOrWriteChannelForTx(WRITE_LOG, save_node_logs, print_node_logs, node_logger,
			num_channels_komondor, channels_for_tx);

	// Act according to possible (not possible) transmission
	if(channels_for_tx[0] != TX_NOT_POSSIBLE){	// Transmission IS POSSIBLE

		// Start RTS/CTS exchange (in case it is active)
		node_state = STATE_TX_RTS;

		// Generate Notification
		current_left_channel = GetFirstOrLastTrueElemOfArray(FIRST_TRUE_IN_ARRAY, channels_for_tx, num_channels_komondor);

		current_right_channel = GetFirstOrLastTrueElemOfArray(LAST_TRUE_IN_ARRAY, channels_for_tx, num_channels_komondor);

		int num_channels_tx = current_right_channel - current_left_channel + 1;

		num_trials_tx_per_num_channels[(int)log2(num_channels_tx)]++;

		if(save_node_logs) fprintf(node_logger.file,
			"%.15f;N%d;S%d;%s;%s Transmission is possible in range: %d - %d\n",
			SimTime(), node_id, node_state, LOG_F04, LOG_LVL3, current_left_channel, current_right_channel);

		// Compute all packets durations (RTS, CTS, DATA and ACK) and NAV time
		int ix_num_channels_used = log2(num_channels_tx);

		current_modulation = mcs_per_node[ix_mcs_per_node][ix_num_channels_used];


		// Sergio on 5 Oct 2017:
		// - Allow computing time in the IEEE 802.11ax

		switch(ieee_protocol){

			case IEEE_NOT_SPECIFIED:{

				data_rate =  Mcs_array::mcs_array[ix_num_channels_used][current_modulation-1];
				rts_duration = ComputeTxTime(rts_length, data_rate, pdf_tx_time);
				cts_duration = ComputeTxTime(cts_length, data_rate, pdf_tx_time);
				data_duration = ComputeTxTime(packet_length * num_packets_aggregated, data_rate, pdf_tx_time);
				ack_duration = ComputeTxTime(ack_length, data_rate, pdf_tx_time);

				break;
			}

			case IEEE_802_11_AX:{

				// data rate at 20 MHz is 52 * ym * yc
				data_rate_20mhz = 52 * Mcs_array::modulation_bits[current_modulation-1] *
						Mcs_array::coding_rates[current_modulation-1];

				// data rate depending on CB and streams: Nsc * ym * yc * SUSS
				data_rate =  getNumberSubcarriers(num_channels_tx) *
						Mcs_array::modulation_bits[current_modulation-1] *
						Mcs_array::coding_rates[current_modulation-1] *
						IEEE_AX_SU_SPATIAL_STREAMS;

				rts_duration = computeRtsTxTime80211ax(data_rate_20mhz);
				cts_duration = computeCtsTxTime80211ax(data_rate_20mhz);
				data_duration = computeDataTxTime80211ax(num_packets_aggregated, packet_length, data_rate);
				ack_duration = computeAckTxTime80211ax(data_rate_20mhz);

//				printf("current_modulation = %d\n"
//						"data_rate_20mhz = %f\n"
//						"data_rate = %f\n"
//						"rts_duration = %f\n"
//						"cts_duration = %f\n"
//						"data_duration = %f\n"
//						"ack_duration = %f\n"
//						"Nsc = %d\n", current_modulation, data_rate_20mhz, data_rate,
//						rts_duration, cts_duration, data_duration, ack_duration, getNumberSubcarriers(num_channels_tx));

				break;
			}

		}

		if(save_node_logs) fprintf(node_logger.file,
			"%.15f;N%d;S%d;%s;%s Transmitting in %d channels using modulation %d (%.0f bits per OFDM symbol ---> %.2f Mbps) \n",
			SimTime(), node_id, node_state, LOG_F04, LOG_LVL4,
			(int) pow(2,ix_num_channels_used), current_modulation, data_rate,
			data_rate/IEEE_AX_OFDM_SYMBOL_DURATION * pow(10,-6));



		current_tx_duration = rts_duration;
		current_nav_time = ComputeNavTime(node_state, rts_duration, cts_duration, data_duration, ack_duration, SIFS);


		// current_nav_time = round_to_digits(current_nav_time, 6);
		current_nav_time = fix_time_offset(current_nav_time,13,12);

		if(save_node_logs) fprintf(node_logger.file,
					"%.15f;N%d;S%d;%s;%s RTS duration: %.12f s - NAV duration = %.12f s\n",
					SimTime(), node_id, node_state, LOG_F04, LOG_LVL5,
					rts_duration, current_nav_time);

		/*
		 * IMPORTANT: to avoid synchronization problems in Slotted BO, we put a
		 * random time epsilon trigger before sending the channel occupancy notification.
		 * E.g. If two APs A and B finish their BO at the very same time, both A and B will
		 * find the channel free and will pick the corresponding channels accordingly. This
		 * way we are able to capture slotted BO collisions.
		 */
		double time_rand_value = 0;
		if(backoff_type == BACKOFF_SLOTTED){

			int rand_number = 2 + rand() % (MAX_NUM_RAND_TIME-2);	// in [2, MAX_NUM_RAND_TIME]

			time_rand_value = (double) rand_number * MAX_DIFFERENCE_SAME_TIME/MAX_NUM_RAND_TIME; // in [FEMTO_SECOND, MAX_DIFFERENCE_SAME_TIME]

			// Sergio on 28/09/2017
			// time_rand_value = round_to_digits(time_rand_value, 15);
			time_rand_value = fix_time_offset(time_rand_value,13,12);

			current_nav_time -= time_rand_value;

			if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s time_rand_value = %.12f s - corrected NAV time = %.12f s\n",
				SimTime(), node_id, node_state, LOG_F04, LOG_LVL5,
				time_rand_value, current_nav_time);
		}

//		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Setting NAV time from %.12f to %.12f\n",
//			SimTime(), node_id, node_state, LOG_F04, LOG_LVL4,
//			(current_nav_time + time_rand_value), current_nav_time);

		rts_notification = GenerateNotification(PACKET_TYPE_RTS, current_destination_id, current_tx_duration);

//		if(save_node_logs) fprintf(node_logger.file,
//				"%.15f;N%d;S%d;%s;%s Transmission of RTS #%d started\n",
//				SimTime(), node_id, node_state, LOG_F04, LOG_LVL3, rts_cts_id);

		// Send RTS notification and trigger to finish transmission

		if(backoff_type == BACKOFF_SLOTTED){
			time_to_trigger = SimTime() + time_rand_value;
			trigger_preoccupancy.Set(fix_time_offset(time_to_trigger,13,12));
		} else {
			outportSelfStartTX(rts_notification);
		}

		time_to_trigger = SimTime() + current_tx_duration;

		if(save_node_logs) fprintf(node_logger.file,
			"%.15f;N%d;S%d;%s;%s time_to_trigger = %.12f s - fix_time_offset = %.12f s\n",
			SimTime(), node_id, node_state, LOG_F04, LOG_LVL5,
			time_to_trigger, fix_time_offset(time_to_trigger,13,12));

		trigger_toFinishTX.Set(fix_time_offset(time_to_trigger,13,12));
		rts_cts_id++;
		rts_cts_sent ++;
		trigger_start_backoff.Cancel();	// Safety instruction

	} else {	// Transmission IS NOT POSSIBLE, compute a new backoff.

		num_tx_init_not_possible ++;

		// Compute a new backoff and trigger a new DIFS
		remaining_backoff = ComputeBackoff(pdf_backoff, cw_current, backoff_type);
		// time_to_trigger = SimTime() + DIFS;
		// trigger_start_backoff.Set(round_to_digits(time_to_trigger,15));
		// Remain on STATE_SENSING (redundant)
		node_state = STATE_SENSING;

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Transmission is NOT possible\n",
				SimTime(), node_id, node_state, LOG_F03, LOG_LVL3);

	}

	// if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s EndBackoff() END\n", SimTime(), node_id, node_state, LOG_F01, LOG_LVL1);
};

/*
 * MyTxFinished(): called when own transmission is finished
 * Input arguments:
 * - trigger_t: transmission trigger
 */
void Node :: MyTxFinished(trigger_t &){

//	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s MyTxFinished()\n",
//			SimTime(), node_id, node_state, LOG_G00, LOG_LVL1);

	switch(node_state){

		case STATE_TX_RTS:{		// Wait for CTS

			// Set CTS timeout and change state to STATE_WAIT_CTS
			// time_to_trigger = SimTime() + SIFS + TIME_OUT_EXTRA_TIME;



			Notification notification = GenerateNotification(PACKET_TYPE_RTS, current_destination_id, TX_DURATION_NONE);
			outportSelfFinishTX(notification);

			// Sergio on 27/09/2017:
			// - Modify timeout time after sending RTS. It should simply be SIFS + TIME_OUT_EXTRA_TIME

			// time_to_trigger = SimTime() + SIFS + notification.tx_info.cts_duration + DIFS + TIME_OUT_EXTRA_TIME;
			// time_to_trigger = SimTime() + SIFS + notification.tx_info.cts_duration + DIFS;
			time_to_trigger = SimTime() + SIFS + notification.tx_info.cts_duration + DIFS;

			trigger_CTS_timeout.Set(fix_time_offset(time_to_trigger,13,12));

			node_state = STATE_WAIT_CTS;

			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s RTS tx finished. Waiting for CTS until %.12f\n",
						SimTime(), node_id, node_state, LOG_G00, LOG_LVL2,
						trigger_CTS_timeout.GetTime());

			break;
		}

		case STATE_TX_CTS:{		// Wait for Data

			Notification notification = GenerateNotification(PACKET_TYPE_CTS, current_destination_id, TX_DURATION_NONE);

			outportSelfFinishTX(notification);

			// Set CTS timeout and change state to STATE_WAIT_DATA
			time_to_trigger = SimTime() + SIFS + TIME_OUT_EXTRA_TIME;
			trigger_DATA_timeout.Set(fix_time_offset(time_to_trigger,13,12));
			node_state = STATE_WAIT_DATA;

			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s CTS tx finished. Waiting for DATA...\n",
						SimTime(), node_id, node_state, LOG_G00, LOG_LVL2);

			break;
		}

		case STATE_TX_DATA:{ 	// Change state to STATE_WAIT_ACK

			Notification notification = GenerateNotification(PACKET_TYPE_DATA, current_destination_id, TX_DURATION_NONE);
			outportSelfFinishTX(notification);

			// Set ACK timeout and change state to STATE_WAIT_ACK
			time_to_trigger = SimTime() + SIFS + TIME_OUT_EXTRA_TIME;
			trigger_ACK_timeout.Set(fix_time_offset(time_to_trigger,13,12));
			node_state = STATE_WAIT_ACK;

			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s DATA tx finished. Waiting for ACK...\n",
						SimTime(), node_id, node_state, LOG_G00, LOG_LVL2);

			break;
		}

		case STATE_TX_ACK:{		// Restart node

			Notification notification = GenerateNotification(PACKET_TYPE_ACK, current_destination_id, TX_DURATION_NONE);
			outportSelfFinishTX(notification);

			if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s ACK tx finished. Restarting node...\n",
						SimTime(), node_id, node_state, LOG_G00, LOG_LVL2);

			RestartNode(FALSE);

			break;
		}



		default:
			break;
	}

	// if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;  MyTxFinished()\n", SimTime(), node_id, node_state, LOG_G01, LOG_LVL1);
};

/*
 * RequestMCS(): performs a negotiation of the MCS to be used according to the tx power sensed by the receiver
 */
void Node :: RequestMCS(){

//	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s RequestMCS() to N%d\n",
//				SimTime(), node_id, node_state, LOG_G00, LOG_LVL1, current_destination_id);

	// Only one channel required (logically!)
	// Receiver is able to determine the power received when transmitter uses more than one channel by its own
	current_left_channel = primary_channel;
	current_right_channel = primary_channel;

	if(node_type == NODE_TYPE_OTHER) {
		// Send request MCS notification
		Notification request_modulation = GenerateNotification(PACKET_TYPE_MCS_REQUEST, default_destination_id, TX_DURATION_NONE);
		outportAskForTxModulation(request_modulation);

	} else {

		// Send request MCS notification
		Notification request_modulation = GenerateNotification(PACKET_TYPE_MCS_REQUEST, current_destination_id, TX_DURATION_NONE);
		outportAskForTxModulation(request_modulation);

		int ix_aux = current_destination_id - wlan.list_sta_id[0];	// Auxiliary variable for correcting the node id offset
		// MCS of receiver is not pending anymore
		change_modulation_flag[ix_aux] = FALSE;
	}


	// if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s RequestMCS() END\n", SimTime(), node_id, node_state, LOG_G00, LOG_LVL1);

}

/*
 * SelectDestination(): pick destination node
 */
void Node :: SelectDestination(){

//	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s SelectDestination()\n",
//			SimTime(), node_id, node_state, LOG_G00, LOG_LVL1);

	if(node_type == NODE_TYPE_OTHER) {
		current_destination_id = default_destination_id;
	}

	current_destination_id = PickRandomElementFromArray(wlan.list_sta_id, wlan.num_stas);

	// if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s SelectDestination() END\n", SimTime(), node_id, node_state, LOG_G00, LOG_LVL1);
}

/*********************/
/*********************/
/*  HANDLE PACKETS   */
/*********************/
/*********************/

/*
 * GenerateNotification: generates a Notification
 **/
Notification Node :: GenerateNotification(int packet_type, int destination_id, double tx_duration){


	Notification notification;

	notification.packet_type = packet_type;
	notification.source_id = node_id;
	notification.left_channel = current_left_channel;
	notification.right_channel = current_right_channel;
	notification.packet_length = -1;
	notification.modulation_id = -1;
	notification.timestampt = SimTime();

	TxInfo tx_info;
	tx_info.SetSizeOfMCS(4);

	tx_info.packet_id = packet_id;
	tx_info.destination_id = destination_id;
	tx_info.tx_duration = tx_duration;
	tx_info.data_duration = data_duration;
	tx_info.ack_duration = ack_duration;
	tx_info.rts_duration = rts_duration;
	tx_info.cts_duration = cts_duration;
	int num_channels_tx = current_right_channel - current_left_channel + 1;
	tx_info.tx_power = ComputeTxPowerPerChannel(current_tpc, num_channels_tx);
	tx_info.tx_gain = tx_gain;
	tx_info.data_rate = data_rate;
	tx_info.x = x;
	tx_info.y = y;
	tx_info.z = z;
	tx_info.nav_time = 0;

	switch(packet_type){

		case PACKET_TYPE_DATA:{
			notification.packet_length = packet_length;
			break;
		}

		case PACKET_TYPE_ACK:{
			notification.packet_length = ack_length;
			break;
		}

		case PACKET_TYPE_MCS_REQUEST:{
			// Do nothing
			break;
		}

		case PACKET_TYPE_MCS_RESPONSE:{
			for(int i = 0; i < 4; i++) {
				tx_info.modulation_schemes[i] = mcs_response[i];
			}
			break;
		}

		case PACKET_TYPE_RTS:{
			notification.packet_length = rts_length;
			tx_info.nav_time = current_nav_time;
			break;
		}

		case PACKET_TYPE_CTS:{
			notification.packet_length = cts_length;
			tx_info.nav_time = current_nav_time;
			break;
		}

		default:{
			printf("ERROR: Packet type unknown\n");
			exit(EXIT_FAILURE);
			break;
		}
	}

	notification.tx_info = tx_info;
	return notification;
}

/*
 * SendLogicalNack: Sends a NACK notification
 */
void Node :: SendLogicalNack(LogicalNack logical_nack){

	outportSendLogicalNack(logical_nack);

	if(save_node_logs) fprintf(node_logger.file,
			"%.15f;N%d;S%d;%s;%s NACK of packet type %d sent to a:N%d (and b:N%d) with reason %d\n",
			SimTime(), node_id, node_state, LOG_I00, LOG_LVL4, logical_nack.packet_type,
			logical_nack.node_id_a, logical_nack.node_id_b, logical_nack.loss_reason);

}

/*
 * CleanNack(): re-initializes the Nack info.
 */
void Node :: CleanNack(){

	nack.source_id = NODE_ID_NONE;
	nack.packet_id = NO_PACKET_ID;
	nack.loss_reason = PACKET_NOT_LOST;
	nack.node_id_a = NODE_ID_NONE;
	nack.node_id_b = NODE_ID_NONE;

}

/*
 * SendResponsePacket(): Sends the response according to the current state (called when SIFS is triggered)
 */
void Node :: SendResponsePacket(trigger_t &){

	switch(node_state){

		case STATE_TX_ACK:{

			if(save_node_logs) fprintf(node_logger.file,
					"%.15f;N%d;S%d;%s;%s SIFS completed after receiving DATA, sending ACK...\n",
					SimTime(), node_id, node_state, LOG_I00, LOG_LVL3);

			outportSelfStartTX(ack_notification);

			// trigger_toFinishTX.Set(SimTime() + current_tx_duration);
			// time_to_trigger = truncate_Sergio(SimTime() + FEMTO_VALUE,12) + current_tx_duration;
			time_to_trigger = SimTime() + current_tx_duration;
			trigger_toFinishTX.Set(fix_time_offset(time_to_trigger,13,12));

			if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s truncate_Sergio = %.12f - current_tx_duration = %.12f - trigger_toFinishTX = %.12f\n",
				SimTime(), node_id, node_state, LOG_I00, LOG_LVL3,
				truncate_Sergio(SimTime() + FEMTO_VALUE,12), current_tx_duration, trigger_toFinishTX.GetTime());

			break;
		}

		case STATE_TX_CTS:{
			if(save_node_logs) fprintf(node_logger.file,
					"%.15f;N%d;S%d;%s;%s SIFS completed after receiving RTS, sending CTS (duration = %f)\n",
					SimTime(), node_id, node_state, LOG_I00, LOG_LVL3, current_tx_duration);
			outportSelfStartTX(cts_notification);

			time_to_trigger = SimTime() + current_tx_duration;
			trigger_toFinishTX.Set(fix_time_offset(time_to_trigger,13,12));
			break;
		}

		case STATE_TX_DATA:{
			if(save_node_logs) fprintf(node_logger.file,
					"%.15f;N%d;S%d;%s;%s SIFS completed after receiving CTS, sending DATA...\n",
					SimTime(), node_id, node_state, LOG_I00, LOG_LVL3);
			outportSelfStartTX(data_notification);
			time_to_trigger = SimTime() + current_tx_duration;
			trigger_toFinishTX.Set(fix_time_offset(time_to_trigger,13,12));
			data_packets_sent++;
			if(save_node_logs) fprintf(node_logger.file,
					"%.15f;N%d;S%d;%s;%s Data TX will be finished at %.15f\n",
					SimTime(), node_id, node_state, LOG_I00, LOG_LVL3,
					trigger_toFinishTX.GetTime());

			// Remove 1 packet from the queue
			if(traffic_model != TRAFFIC_FULL_BUFFER) num_packets_in_buffer --;

			break;
		}
	}
}

/*********************/
/*********************/
/*     TIMEOUTS      */
/*********************/
/*********************/

/*
 * AckTimeout(): handles ACK timeout. It is called when ACK timeout is triggered.
 */
void Node :: AckTimeout(trigger_t &){

	current_tx_duration += SIFS + TIME_OUT_EXTRA_TIME;		// Add ACK timeout to tx_duration

	for(int c = current_left_channel; c <= current_right_channel; c++){
		total_time_transmitting_per_channel[c] += SIFS + TIME_OUT_EXTRA_TIME;
	}

	handlePacketLoss(PACKET_TYPE_DATA, total_time_lost_in_num_channels, total_time_lost_per_channel,
			data_packets_lost, rts_cts_lost, current_right_channel, current_left_channel,current_tx_duration);
	
	// Sergio on 16 July 2018: [AGENTS] add data packet lost for partial throughput computations
	last_measurement_data_packets_lost++;

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s  ACK TIMEOUT! Data packet %d lost\n",
					SimTime(), node_id, node_state, LOG_D17, LOG_LVL4,
					packet_id);

	// Sergio on 20/09/2017. CW only must be changed when ACK received or loss detected.
	HandleContentionWindow(
				cw_adaptation, INCREASE_CW, &cw_current, cw_min, &cw_stage_current, cw_stage_max);

	RestartNode(TRUE);
}

/*
 * CtsTimeout(): handles CTS timeout. It is called when CTS timeout is triggered (after sending RTS).
 */
void Node :: CtsTimeout(trigger_t &){

	handlePacketLoss(PACKET_TYPE_CTS, total_time_lost_in_num_channels, total_time_lost_per_channel,
			data_packets_lost, rts_cts_lost, current_right_channel, current_left_channel,current_tx_duration);

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s ---------------------------------------------\n",
						SimTime(), node_id, node_state, LOG_D17, LOG_LVL1);
	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s CTS TIMEOUT! RTS-CTS packet lost\n",
					SimTime(), node_id, node_state, LOG_D17, LOG_LVL2);


	// Sergio on 20/09/2017. CW only must be changed when ACK received or loss detected.
	HandleContentionWindow(
			cw_adaptation, INCREASE_CW, &cw_current, cw_min, &cw_stage_current, cw_stage_max);

	// RestartNode(TRUE);

	// *******************************************

	// Update TX time statistics

	total_time_transmitting_in_num_channels[(int)log2(current_right_channel - current_left_channel + 1)] += current_tx_duration;
	for(int c = current_left_channel; c <= current_right_channel; c++){
		total_time_transmitting_per_channel[c] += current_tx_duration;
	}

	// Reinitialize parameters
	current_tx_duration = 0;
	power_rx_interest = 0;
	max_pw_interference = 0;
	node_state = STATE_SENSING;
	receiving_from_node_id = NODE_ID_NONE;
	receiving_packet_id = NO_PACKET_ID;

	// Generate new BO in case of being a TX node
	if(node_is_transmitter){

		// Set the ID of the next packet
		packet_id++;

		trigger_end_backoff.Cancel(); // Cancel BO timeout for safety

		// In case of being an AP
		remaining_backoff = ComputeBackoff(pdf_backoff, cw_current, backoff_type);

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s New backoff computed: %f (%.0f slots).\n",
								SimTime(), node_id, node_state, LOG_Z00, LOG_LVL3,
								remaining_backoff, remaining_backoff/SLOT_TIME);

		// FRANKY
//		remaining_backoff += SLOT_TIME;
//
//		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Extra slot added: %f.\n",
//						SimTime(), node_id, node_state, LOG_Z00, LOG_LVL4, remaining_backoff);

		// Sergio on 25 Oct 2017:
		// - Start DIFS after the expected CTS transmission time is finished.



		time_to_trigger = SimTime() + incoming_notification.tx_info.cts_duration;
		trigger_recover_cts_timeout.Set(fix_time_offset(time_to_trigger,13,12));


		// Freeze backoff immediately if primary channel is occupied
//		int resume = HandleBackoff(RESUME_TIMER, channel_power, primary_channel, current_cca,
//				num_packets_in_buffer);
//
//		if (resume) trigger_start_backoff.Set(SimTime());

	}

	CleanNack();

	// Cancel timeout triggers for safety
	trigger_ACK_timeout.Cancel();			// Trigger when ACK hasn't arrived in time
	trigger_CTS_timeout.Cancel();			// Trigger when CTS hasn't arrived in time
	trigger_DATA_timeout.Cancel();			// Trigger when DATA TX could not start due to RTS/CTS failure
	trigger_NAV_timeout.Cancel();  			// Trigger for the NAV

	// *******************************************
}

/*
 * DataTimeout(): handles Data timeout. It is called when data timeout (after sending CTS) is triggered.
 */
void Node :: DataTimeout(trigger_t &){

	handlePacketLoss(PACKET_TYPE_CTS, total_time_lost_in_num_channels, total_time_lost_per_channel,
			data_packets_lost, rts_cts_lost, current_right_channel, current_left_channel, current_tx_duration);

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s DATA TIMEOUT! RTS-CTS packet lost\n",
					SimTime(), node_id, node_state, LOG_D17, LOG_LVL4);

	// Sergio on 20/09/2017. CW only must be changed when ACK received or loss detected.
	// cw_current = HandleContentionWindow(INCREASE_CW, cw_current, cw_min, cw_stage_max);

	RestartNode(TRUE);
}

/*
 * NavTimeout(): handles ACK timeout. It is called when ACK timeout is triggered.
 */
void Node :: NavTimeout(trigger_t &){

	if(save_node_logs) fprintf(node_logger.file, "\n **********************************************************************\n");

	if(save_node_logs) fprintf(node_logger.file,
			"%.15f;N%d;S%d;%s;%s NAV TIMEOUT!\n",
			SimTime(), node_id, node_state, LOG_D17, LOG_LVL1);

	if(node_is_transmitter){

		node_state = STATE_SENSING;

		int resume = HandleBackoff(RESUME_TIMER, &channel_power, primary_channel, current_cca,
				num_packets_in_buffer);

		// Update BO value according to TO extra time
		if (resume) {

			time_to_trigger = SimTime() + DIFS - TIME_OUT_EXTRA_TIME;

			// time_to_trigger = SimTime() + DIFS;

			trigger_start_backoff.Set(fix_time_offset(time_to_trigger,13,12));

			if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s Starting new DIFS to finsih in %.12f\n",
				SimTime(), node_id, node_state, LOG_D17, LOG_LVL3,
				trigger_start_backoff.GetTime());

		} else {
			if(save_node_logs) fprintf(node_logger.file,
				"%.15f;N%d;S%d;%s;%s New DIFS cannot be started\n",
				SimTime(), node_id, node_state, LOG_D17, LOG_LVL3);

			// printf("- %.12f; N%d cannot start DIFS\n", SimTime(), node_id);
		}

	} else {

		RestartNode(TRUE);

	}

}

/************************/
/************************/
/*  BACKOFF MANAGEMENT  */
/************************/
/************************/

/*
 * PauseBackoff(): pauses the backoff
 * */
void Node :: PauseBackoff(){

	if(trigger_start_backoff.Active()){
		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Cancelling DIFS. BO still frozen at %.9f (%.2f slots)\n",
				SimTime(), node_id, node_state, LOG_F00, LOG_LVL3,
				remaining_backoff * pow(10,6), remaining_backoff / SLOT_TIME);

		trigger_start_backoff.Cancel();
	} else {

		if(trigger_end_backoff.Active()){	// If backoff trigger is active, freeze it

			/*
			 * TODO: Pausing the backoff (I think we should use modulus). Solved through extra time condition I?
			 */
			// if((trigger_end_backoff.GetTime() - SimTime()) > MAX_DIFFERENCE_SAME_TIME){

				if(save_node_logs) fprintf(node_logger.file,
					"%.15f;N%d;S%d;%s;%s BO is active. Freezing it from %.9f (%.2f slots)...\n",
					SimTime(), node_id, node_state, LOG_F00, LOG_LVL3,
					(trigger_end_backoff.GetTime() - SimTime()) * pow(10,6), (trigger_end_backoff.GetTime() - SimTime())/SLOT_TIME);

				remaining_backoff = ComputeRemainingBackoff(backoff_type, trigger_end_backoff.GetTime() - SimTime());

				if(save_node_logs) fprintf(node_logger.file,
					"%.15f;N%d;S%d;%s;%s ... to %.9f (%.2f slots)\n",
					SimTime(), node_id, node_state, LOG_F00, LOG_LVL3,
					remaining_backoff * pow(10,6), remaining_backoff/SLOT_TIME);

	//			if(save_node_logs) fprintf(node_logger.file,
	//								"%.15f;N%d;S%d;%s;%s Original remaining BO: %.9f us\n",
	//								SimTime(), node_id, node_state, LOG_F00, LOG_LVL3,
	//								(trigger_end_backoff.GetTime() - SimTime())*pow(10,6));

	//			if(save_node_logs) fprintf(node_logger.file,
	//					"%.15f;N%d;S%d;%s;%s Backoff is active --> freeze it at %.9f us (%.2f slots)\n",
	//					SimTime(), node_id, node_state, LOG_F00, LOG_LVL3,
	//					remaining_backoff * pow(10,6), remaining_backoff/SLOT_TIME);

				trigger_end_backoff.Cancel();
			// }

		} else {	// If backoff trigger is frozen

			if(save_node_logs) fprintf(node_logger.file,
					"%.15f;N%d;S%d;%s;%s Backoff is NOT active - it is already frozen at %.9f us (%.2f slots)\n",
					SimTime(), node_id, node_state, LOG_F00, LOG_LVL3,
					remaining_backoff * pow(10,6), remaining_backoff / SLOT_TIME);

			trigger_end_backoff.Cancel(); // Redundant (for safety)

		}

	}
}

/*
 * ResumeBackoff(): resumes the backoff (triggered after DIFS is completed)
 * */
void Node :: ResumeBackoff(trigger_t &){

//	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s DIFS finished\n",
//					SimTime(), node_id, node_state, LOG_F00, LOG_LVL2);

	time_to_trigger = SimTime() + remaining_backoff;

	trigger_end_backoff.Set(fix_time_offset(time_to_trigger,13,12));

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Resuming backoff at %.9f us (%.2f slots)\n",
				SimTime(), node_id, node_state, LOG_F00, LOG_LVL3,
				(remaining_backoff * pow(10,6)), (remaining_backoff / (double) SLOT_TIME));

//	if(save_node_logs) fprintf(node_logger.file,
//				"%.15f;N%d;S%d;%s;%s DIFS: active = %d, t_DIFS = %f - backoff: active = %d - t_back = %f\n",
//				SimTime(), node_id, node_state, LOG_D02, LOG_LVL3,
//				trigger_start_backoff.Active(), trigger_start_backoff.GetTime() - SimTime(),
//				trigger_end_backoff.Active(), trigger_end_backoff.GetTime() - SimTime());

}

/***********************/
/***********************/
/* AUXILIARY FUNCTIONS */
/***********************/
/***********************/

/*
 * CallRestartSta(): calls RestartNode() when called by the trigger. It is useful to handle transmission
 * ocurring at the same time.
 * STAs should wait MAX_DIFFERENCE_SAME_TIME in order to avoid entering in NAV when it is not required.
 * E.g. STA A is sensing and is able to decode a packet from AP A. At the same time AP B transmits and
 * harms AP A - STA A transmission. STA A is restarted. Again, at the same time AP C transmits. Then,
 * in order to avoid entering in NAV when in fact a slotted BO collision did happen, STA A should not
 * listen to AP C packet. After MAX_DIFFERENCE_SAME_TIME, no same time events are ensured and STA A can
 * start sensing again.
 */
void Node :: CallRestartSta(trigger_t &){

	RestartNode(FALSE);

}

/*
 * RestartNode(): re-initializes the nodes. Puts it in the initial state (sensing and decreasing BO)
 */
void Node :: RestartNode(int called_by_time_out){

	if(save_node_logs) fprintf(node_logger.file, "\n **********************************************************************\n");
	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Node Restarted (%d)\n",
			SimTime(), node_id, node_state, LOG_Z00, LOG_LVL1,
			called_by_time_out);

	// Update TX time statistics
	total_time_transmitting_in_num_channels[(int)log2(current_right_channel - current_left_channel + 1)] += current_tx_duration;
	for(int c = current_left_channel; c <= current_right_channel; c++){
		total_time_transmitting_per_channel[c] += current_tx_duration;
	}

	// Reinitialize parameters
	current_tx_duration = 0;
	power_rx_interest = 0;
	max_pw_interference = 0;
	node_state = STATE_SENSING;
	receiving_from_node_id = NODE_ID_NONE;
	receiving_packet_id = NO_PACKET_ID;

	// Generate new BO in case of being a TX node
	if(node_is_transmitter){

		// Set the ID of the next packet
		packet_id++;

		trigger_end_backoff.Cancel(); // Cancel BO timeout for safety

		// Cancel trigger for safety
		trigger_recover_cts_timeout.Cancel();

		// In case of being an AP
		remaining_backoff = ComputeBackoff(pdf_backoff, cw_current, backoff_type);

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Starting DIFS. New backoff computed: %f (%.0f slots).\n",
						SimTime(), node_id, node_state, LOG_Z00, LOG_LVL3,
						remaining_backoff, remaining_backoff/SLOT_TIME);

		// FRANKY
		remaining_backoff += SLOT_TIME;

		if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Extra slot added: %f\n",
						SimTime(), node_id, node_state, LOG_Z00, LOG_LVL4, remaining_backoff);

		// Freeze backoff immediately if primary channel is occupied
		int resume = HandleBackoff(RESUME_TIMER, &channel_power, primary_channel, current_cca,
				num_packets_in_buffer);

		// Check if node has to freeze the BO (if it is not already frozen)
		if(called_by_time_out){
			if (resume) {
				// time_to_trigger = SimTime() + DIFS - TIME_OUT_EXTRA_TIME;
				time_to_trigger = SimTime() + DIFS;
				trigger_start_backoff.Set(fix_time_offset(time_to_trigger,13,12));
			}
		} else {
			if (resume) {
				time_to_trigger = SimTime() + DIFS;
				trigger_start_backoff.Set(fix_time_offset(time_to_trigger,13,12));
			}
		}


	}

	CleanNack();

	// Cancel timeout triggers for safety
	trigger_ACK_timeout.Cancel();			// Trigger when ACK hasn't arrived in time
	trigger_CTS_timeout.Cancel();			// Trigger when CTS hasn't arrived in time
	trigger_DATA_timeout.Cancel();			// Trigger when DATA TX could not start due to RTS/CTS failure
	trigger_NAV_timeout.Cancel();  			// Trigger for the NAV

}

void Node:: CallSensing(trigger_t &){

	node_state = STATE_SENSING;


}

// Starts saving logs from a given initial value
void Node:: StartSavingLogs(trigger_t &){

	save_node_logs = TRUE;

}

void Node:: HandleSlottedBackoffCollision() {

	/*
	 * STAs should wait MAX_DIFFERENCE_SAME_TIME in order to avoid entering in NAV when it is not required.
	 * E.g. STA A is sensing and is able to decode a packet from AP A. At the same time AP B transmits and
	 * harms AP A - STA A transmission. STA A is restarted. Again, at the same time AP C transmits. Then,
	 * in order to avoid entering in NAV when in fact a slotted BO collision did happen, STA A should not
	 * listen to AP C packet. After MAX_DIFFERENCE_SAME_TIME, no same time events are ensured and STA A can
	 * start sensing again.
	 */

	// Slotted BO collision (case where STA is receiving)
	loss_reason = PACKET_LOST_BO_COLLISION;

	if(!node_is_transmitter) {

		node_state = STATE_SLEEP; // avoid listening to notifications until restart
		time_to_trigger = SimTime() + MAX_DIFFERENCE_SAME_TIME;
		trigger_restart_sta.Set(fix_time_offset(time_to_trigger,13,12));

	} else {
		// In case STAs can send to AP
		RestartNode(FALSE);
	}
}

void Node:: RecoverFromCtsTimeout(trigger_t &) {

	// Sergio on 25 Oct 2017
	// - Just restart the node to start the DIFS

	if(save_node_logs) fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s RecoverFromCtsTimeout\n",
							SimTime(), node_id, node_state, LOG_Z00, LOG_LVL3);

	trigger_recover_cts_timeout.Cancel();

	RestartNode(TRUE);

}



/************************/
/************************/
/*  PRINT INFORMATION   */
/************************/
/************************/

/*
 * PrintNodeInfo(): prints Node info
 * Input arguments:
 * - info_detail_level: level of detail of the info printed
 */
void Node :: PrintNodeInfo(int info_detail_level){

	printf("%s Node %s info:\n", LOG_LVL3, node_code);
	printf("%s node_id = %d\n", LOG_LVL4, node_id);
	printf("%s node_type = %d\n", LOG_LVL4, node_type);
	printf("%s position = (%.2f, %.2f, %.2f)\n", LOG_LVL4, x, y, z);
	printf("%s primary_channel = %d\n", LOG_LVL4, primary_channel);
	printf("%s min_channel_allowed = %d\n", LOG_LVL4, min_channel_allowed);
	printf("%s max_channel_allowed = %d\n", LOG_LVL4, max_channel_allowed);
	printf("%s channel_bonding_model = %d\n", LOG_LVL4, channel_bonding_model);
	printf("%s cw_min = %d\n", LOG_LVL4, cw_min);

	if(info_detail_level > INFO_DETAIL_LEVEL_0){
		printf("%s wlan:\n", LOG_LVL4);
		printf("%s wlan code = %s\n", LOG_LVL5, wlan.wlan_code);
		printf("%s wlan id = %d\n", LOG_LVL5, wlan.wlan_id);
		printf("%s wlan AP id = %d\n", LOG_LVL5, wlan.ap_id);
		printf("%s STAs in WLAN (%d): ", LOG_LVL5, wlan.num_stas);
		wlan.PrintStaIds();
	}

	if(info_detail_level > INFO_DETAIL_LEVEL_1){
		printf("%s lambda = %f packets/s\n", LOG_LVL4, lambda);
		printf("%s cw_min = %d\n", LOG_LVL4, cw_min);
		printf("%s cw_stage_max = %d\n", LOG_LVL4, cw_stage_max);
		printf("%s destination_id = %d\n", LOG_LVL4, destination_id);
		printf("%s tpc_min = %f pW (%f dBm)\n", LOG_LVL4, tpc_min, ConvertPower(PW_TO_DBM, tpc_min));
		printf("%s tpc_default = %f pW (%f dBm)\n", LOG_LVL4, tpc_default, ConvertPower(PW_TO_DBM, tpc_default));
		printf("%s tpc_max = %f pW (%f dBm)\n", LOG_LVL4, tpc_max, ConvertPower(PW_TO_DBM, tpc_max));
		printf("%s cca_min = %f pW (%f dBm)\n", LOG_LVL4, cca_min, ConvertPower(PW_TO_DBM, cca_min));
		printf("%s cca_default = %f pW (%f dBm)\n", LOG_LVL4, cca_default, ConvertPower(PW_TO_DBM, cca_default));
		printf("%s cca_max = %f pW (%f dBm)\n", LOG_LVL4, cca_max, ConvertPower(PW_TO_DBM, cca_max));
		printf("%s tx_gain = %f (%f dBi)\n", LOG_LVL4, tx_gain, ConvertPower(LINEAR_TO_DB, tx_gain));
		printf("%s rx_gain = %f (%f dBi)\n", LOG_LVL4, rx_gain, ConvertPower(LINEAR_TO_DB, rx_gain));
		printf("%s modulation_default = %d\n", LOG_LVL4, modulation_default);
		printf("%s central_frequency = %f Hz (%f GHz)\n", LOG_LVL4, central_frequency, central_frequency * pow(10,-9));
		printf("%s lambda = %f packets/s\n", LOG_LVL4, lambda);
	}
	printf("\n");
}

/*
 * WriteNodeInfo(): writes Node info
 */
void Node :: WriteNodeInfo(Logger node_logger, int info_detail_level, char *header_string){

	if(header_string == NULL){
		header_string = (char *) malloc(1);
		sprintf(header_string, " ");
	}

	fprintf(node_logger.file, "%s Node %s info:\n", header_string, node_code);
	fprintf(node_logger.file, "%s - node_id = %d\n", header_string, node_id);
	fprintf(node_logger.file, "%s - node_type = %d\n", header_string, node_type);
	fprintf(node_logger.file, "%s - position = (%.2f, %.2f, %.2f)\n", header_string, x, y, z);
	fprintf(node_logger.file, "%s - primary_channel = %d\n", header_string, primary_channel);
	fprintf(node_logger.file, "%s - min_channel_allowed = %d\n", header_string, min_channel_allowed);
	fprintf(node_logger.file, "%s - max_channel_allowed = %d\n", header_string, max_channel_allowed);
	fprintf(node_logger.file, "%s - channel_bonding_model = %d\n", header_string, channel_bonding_model);

	if(info_detail_level > INFO_DETAIL_LEVEL_0){

		wlan.WriteWlanInfo(node_logger, header_string);

	}

	if(info_detail_level > INFO_DETAIL_LEVEL_1){
		fprintf(node_logger.file, "%s - lambda = %f packets/s\n", header_string, lambda);
		fprintf(node_logger.file, "%s - cw_min = %d\n", header_string, cw_min);
		fprintf(node_logger.file, "%s - cw_stage_max = %d\n", header_string, cw_stage_max);
		fprintf(node_logger.file, "%s - destination_id = %d\n", header_string, destination_id);
		fprintf(node_logger.file, "%s - tpc_default = %f pW\n", header_string, tpc_default);
		fprintf(node_logger.file, "%s - cca_default = %f pW\n", header_string, cca_default);
		fprintf(node_logger.file, "%s - lambda = %f\n", header_string, lambda);
	}

}

/*
 * PrintProgressBar(): prints the progress bar of the Komondor simulation
 * Input arguments:
 * - trigger_t: trigger of the progress bar
 */
void Node :: PrintProgressBar(trigger_t &){

	// if(print_node_logs) printf("* %d %% *\n", progress_bar_counter * PROGRESS_BAR_DELTA);
	printf("* %d %% *\n", progress_bar_counter * PROGRESS_BAR_DELTA);
	trigger_sim_time.Set(round_to_digits(SimTime() + simulation_time_komondor / (100/PROGRESS_BAR_DELTA),15));

	// End progress bar
	if(node_id == 0 && progress_bar_counter == (100/PROGRESS_BAR_DELTA)-1){
		trigger_sim_time.Set(round_to_digits(SimTime() + simulation_time_komondor/(100/PROGRESS_BAR_DELTA) - MIN_VALUE_C_LANGUAGE,15));
	}
	progress_bar_counter ++;
}

/*
 * PrintOrWriteNodeStatistics(): prints (or writes) final statistics at the given node
 */
void Node :: PrintOrWriteNodeStatistics(int write_or_print){

	// Process statistics
	double data_packets_lost_percentage = 0;
	double rts_cts_lost_percentage = 0;
	double tx_init_failure_percentage = 0;
	double rts_lost_bo_percentage = 0;

	tx_init_failure_percentage = double(num_tx_init_not_possible * 100)/double(num_tx_init_tried);

	if (data_packets_sent > 0) {
		data_packets_lost_percentage = double(data_packets_lost * 100)/double(data_packets_sent);
		rts_cts_lost_percentage = double(rts_cts_lost * 100)/double(rts_cts_sent);
		rts_lost_bo_percentage = double(rts_lost_slotted_bo *100)/double(rts_cts_sent);
		prob_slotted_bo_collision = rts_lost_bo_percentage / 100;
	}
	throughput = (((double)(data_packets_sent-data_packets_lost) * packet_length * num_packets_aggregated))
			/ SimTime();
	int hidden_nodes_number = 0;
	for(int i = 0; i < total_nodes_number; i++){
		if(hidden_nodes_list[i] == 1) hidden_nodes_number++;
	}

	switch(write_or_print){

		case PRINT_LOG:{

			if (node_is_transmitter && print_node_logs) {
				printf("------- %s (N%d) ------\n", node_code, node_id);

				// Throughput
				printf("%s Throughput = %f Mbps\n", LOG_LVL2, throughput * pow(10,-6));

				// RTS/CTS sent and lost
				printf("%s RTS/CTS sent = %d - RTS/CTS lost = %d  (%.2f %% lost)\n",
						LOG_LVL2, rts_cts_sent, rts_cts_lost, rts_cts_lost_percentage);

				// RTS/CTS sent and lost
				printf("%s RTS lost due to slotted BO = %d (%f %%)\n",
						LOG_LVL3, rts_lost_slotted_bo, rts_lost_bo_percentage);

				// Data packets sent and lost
				printf("%s Data packets sent = %d - Data packets lost = %d  (%f %% lost)\n",
						LOG_LVL2, data_packets_sent, data_packets_lost, data_packets_lost_percentage);

				// Number of trials to transmit
				printf("%s num_tx_init_tried = %d - num_tx_init_not_possible = %d (%f %% failed)\n",
						LOG_LVL2, num_tx_init_tried, num_tx_init_not_possible, tx_init_failure_percentage);

				// Time EFFECTIVELY transmitting in a given number of channels (no losses)
				printf("%s Time EFFECTIVELY transmitting in N channels:", LOG_LVL3);
				for(int n = 0; n < num_channels_allowed; n++){
					printf("\n%s - %d: %f s (%.2f %%)",
							LOG_LVL3, (int) pow(2,n),
							total_time_transmitting_in_num_channels[n] - total_time_lost_in_num_channels[n],
							((total_time_transmitting_in_num_channels[n] -
									total_time_lost_in_num_channels[n])) * 100 /SimTime());

					if((int) pow(2,n) == num_channels_komondor) break;
				}
				printf("\n");

				// Time EFFECTIVELY transmitting in each of the channels (no losses)
				printf("%s Time EFFECTIVELY transmitting in each channel:", LOG_LVL3);
				double time_effectively_txing;
				for(int c = 0; c < num_channels_komondor; c++){

					time_effectively_txing = total_time_transmitting_per_channel[c] -
							total_time_lost_per_channel[c];

					printf("\n%s - %d = %.2f s (%.2f %%)",
							LOG_LVL3, c, time_effectively_txing,
							(time_effectively_txing * 100 /SimTime()));
				}
				printf("\n");

//				// Time LOST transmitting in a given number of channels
//				printf("%s Time LOST transmitting in N channels:", LOG_LVL3);
//				for(int n = 0; n < num_channels_allowed; n++){
//					printf("\n%s - %d: %f",
//							LOG_LVL3, n, total_time_lost_in_num_channels[n]);
//				}
//				printf("\n");
//
//				// Time LOST transmitting in each of the channels
//				printf("%s Time LOST transmitting in each channel:", LOG_LVL3);
//				for(int c = 0; c < num_channels_komondor; c++){
//					printf("\n%s - %d = %.2f s (%.2f %%)",
//						LOG_LVL3, c, total_time_lost_per_channel[c],
//						(total_time_lost_per_channel[c] * 100 / SimTime()));
//				}
//				printf("\n");

				// Time tx trials in each number of channels
				printf("%s Number of tx trials per number of channels:", LOG_LVL3);
				for(int n = 0; n < num_channels_komondor; n++){

					printf("\n%s - %d: %d (%.2f %%)",
							LOG_LVL3, (int) pow(2,n),
							num_trials_tx_per_num_channels[n],
							(((double) num_trials_tx_per_num_channels[n] * 100) / (double) (rts_cts_sent)));

					if((int) pow(2,n) == num_channels_komondor) break;
				}
				printf("\n");


				// Number of TX initiations that have been not possible due to channel state and DCB model
				printf("%s num_tx_init_not_possible = %d\n", LOG_LVL2, num_tx_init_not_possible);

//				// Hidden nodes
//				printf("%s Total number of hidden nodes: %d\n", LOG_LVL2, hidden_nodes_number);
//				printf("%s Hidden nodes list: ", LOG_LVL2);
//				for(int i = 0; i < total_nodes_number; i++){
//					printf("%d  ", hidden_nodes_list[i]);
//				}
//				printf("\n");
//
//				printf("%s Times a node was implied in a collision by hidden node: ",LOG_LVL2);
//				for(int i=0; i < total_nodes_number; i++) {
//					printf("%d ", potential_hidden_nodes[i]);
//				}

				printf("\n\n");
			}
			break;
		}

		case WRITE_LOG:{

			if (save_node_logs){

				if (node_is_transmitter) {
					// Throughput
					fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Throughput = %f Mbps\n",
							SimTime(), node_id, node_state, LOG_C02, LOG_LVL2, throughput * pow(10,-6));

					// Data packets sent and lost
					fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s Data packets sent: %d\n",
							SimTime(), node_id, node_state, LOG_C03, LOG_LVL2, data_packets_sent);
					fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s Data packets lost: %d\n",
							SimTime(), node_id, node_state, LOG_C04, LOG_LVL2, data_packets_lost);
					fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s Loss ratio: %f\n",
							SimTime(), node_id, node_state, LOG_C05, LOG_LVL2, data_packets_lost_percentage);

					// Time EFFECTIVELY transmitting in a given number of channels (no losses)
					fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Time EFFECTIVELY transmitting in N channels: ",
							SimTime(), node_id, node_state, LOG_C06, LOG_LVL2);
					for(int n = 0; n < num_channels_allowed; n++){
						fprintf(node_logger.file, "(%d) %f  ",
								n+1, total_time_transmitting_in_num_channels[n] - total_time_lost_in_num_channels[n]);
					}
					fprintf(node_logger.file, "\n");

					// Time EFFECTIVELY transmitting in each of the channels (no losses)
					fprintf(node_logger.file,
							"%.15f;N%d;S%d;%s;%s Time EFFECTIVELY transmitting in each channel: ",
							SimTime(), node_id, node_state, LOG_C07, LOG_LVL2);
					for(int c = 0; c < num_channels_komondor; c++){
						fprintf(node_logger.file, "(#%d) %f ",
								c, total_time_transmitting_per_channel[c] - total_time_lost_per_channel[c]);
					}
					fprintf(node_logger.file, "\n");

					// Time LOST transmitting in a given number of channels
					fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Time LOST transmitting in N channels: ",
							SimTime(), node_id, node_state, LOG_C08, LOG_LVL2);
					for(int n = 0; n < num_channels_allowed; n++){
						fprintf(node_logger.file, "(%d) %f  ", n+1, total_time_lost_in_num_channels[n]);
					}
					fprintf(node_logger.file, "\n");

					// Time LOST transmitting in each of the channels
					fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Time LOST transmitting in each channel: ",
							SimTime(), node_id, node_state, LOG_C09, LOG_LVL2);
					for(int c = 0; c < num_channels_komondor; c++){
						fprintf(node_logger.file, "(#%d) %f ", c, total_time_lost_per_channel[c]);
					}
					fprintf(node_logger.file, "\n");

					// Number of TX initiations that have been not possible due to channel state and DCB model
					fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s num_tx_init_not_possible = %d\n",
							SimTime(), node_id, node_state, LOG_C09, LOG_LVL2, num_tx_init_not_possible);

				}

				// Hidden nodes
				int hidden_nodes_number = 0;
				for(int n = 0; n < total_nodes_number; n++){
					if(hidden_nodes_list[n]) hidden_nodes_number++;
				}
				fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Total hidden nodes: %d\n",
						SimTime(), node_id, node_state, LOG_C10, LOG_LVL2, hidden_nodes_number);

				fprintf(node_logger.file, "%.15f;N%d;S%d;%s;%s Hidden nodes list: ",
						SimTime(), node_id, node_state, LOG_C11, LOG_LVL2);
				for(int i = 0; i < total_nodes_number; i++){
					fprintf(node_logger.file, "%d  ", hidden_nodes_list[i]);
				}
			}
			break;
		}
	}
}

/*****************************/
/*****************************/
/*  VARIABLE INITIALISATION  */
/*****************************/
/*****************************/

/*
 * InitializeVariables(): initializes all the necessary variables
 */
void Node :: InitializeVariables() {

	current_sinr = 0;
	max_pw_interference = 0;
	rts_lost_slotted_bo = 0;

	// Output file - logger
	node_logger.save_logs = save_node_logs;
	node_logger.file = node_logger.file;

	// Arrays and other
	channel_power = (double *) malloc(num_channels_komondor * sizeof(*channel_power));
	num_channels_allowed = (max_channel_allowed - min_channel_allowed + 1);
	total_time_transmitting_per_channel = (double *) malloc(num_channels_komondor
			* sizeof(*total_time_transmitting_per_channel));
	channels_free = (int *) malloc(num_channels_komondor * sizeof(*channels_free));
	channels_for_tx = (int *) malloc(num_channels_komondor * sizeof(*channels_for_tx));
	total_time_lost_per_channel = (double *) malloc(num_channels_komondor
			* sizeof(*total_time_lost_per_channel));
	timestampt_channel_becomes_free = (double *) malloc(num_channels_komondor
			* sizeof(*timestampt_channel_becomes_free));

	num_trials_tx_per_num_channels = (int *) malloc(num_channels_komondor
			* sizeof(*num_trials_tx_per_num_channels));

	for(int i = 0; i < num_channels_komondor; i++){
		channel_power[i] = 0;
		total_time_transmitting_per_channel[i] = 0;
		channels_free[i] = FALSE;
		channels_for_tx[i] = FALSE;
		total_time_lost_per_channel[i] = 0;
		timestampt_channel_becomes_free[i] = 0;
		num_trials_tx_per_num_channels[i] = 0;
	}

	total_time_transmitting_in_num_channels = (double *) malloc(num_channels_allowed
			* sizeof(*total_time_transmitting_in_num_channels));
	total_time_lost_in_num_channels = (double *) malloc(num_channels_allowed
			* sizeof(*total_time_lost_in_num_channels));
	for(int i = 0; i < num_channels_allowed; i++){
		total_time_transmitting_in_num_channels[i] = 0;
		total_time_lost_in_num_channels[i] = 0;
	}

	power_received_per_node = (double *) malloc(total_nodes_number * sizeof(*power_received_per_node));
	nodes_transmitting = (int *) malloc(total_nodes_number * sizeof(*nodes_transmitting));
	for(int n = 0; n < total_nodes_number; n++){
		power_received_per_node[n] = 0;
		nodes_transmitting[n] = FALSE;
	}

	// List of hidden nodes (1 indicates hidden nodes, 0 indicates the opposite)
	hidden_nodes_list = (int *) malloc(total_nodes_number * sizeof(*hidden_nodes_list));
	// Counter for the times a node was implied in a collision by hidden node
	potential_hidden_nodes = (int *) malloc(total_nodes_number * sizeof(*potential_hidden_nodes));
	for(int i = 0; i < total_nodes_number; i++){
		hidden_nodes_list[i] = FALSE;
		potential_hidden_nodes[i] = 0;
	}
	potential_hidden_nodes[node_id] = -1; // To indicate that the node cannot be hidden from itself
	nacks_received = (int *) malloc(7 * sizeof(*nacks_received));
	for(int i = 0; i < 7; i++){
		nacks_received[i] = 0;
	}

	// Rest of variables
	power_rx_interest = 0;
	progress_bar_counter = 0;
	current_left_channel =  min_channel_allowed;
	current_right_channel = max_channel_allowed;


	node_state = STATE_SENSING;
	current_modulation = modulation_default;
	cw_current = cw_min;
	cw_stage_current = 0;
	packet_id = 0;
	rts_cts_id = 0;
	num_packets_in_buffer = 0;


	if(node_type == NODE_TYPE_AP) {
		node_is_transmitter = TRUE;
		remaining_backoff = ComputeBackoff(pdf_backoff, cw_current, backoff_type);
		// printf("N%d remaining_backoff = %.15f\n", node_id, remaining_backoff);

		// HARDCODED BY SERGIO TO TEST
		// remaining_backoff = 0;
		// if(node_id == 0) remaining_backoff = SLOT_TIME;
		// END OF HARDCODING

	} else {
		node_is_transmitter = FALSE;
	}

	current_tpc = tpc_default;
	current_cca = cca_default;
	channel_max_intereference = primary_channel;

	data_duration = 0;
	ack_duration = 0;
	rts_duration = 0;
	cts_duration = 0;

	default_modulation = MODULATION_NONE;

	mcs_response = (int *) malloc(4 * sizeof(int));
	for(int n = 0; n < 4; n++){
		mcs_response[n] = 0;
	}

	int *modulations_list = (int*)calloc(4, sizeof(int));

	mcs_per_node = (int**)calloc(wlan.num_stas, sizeof(int*));
	change_modulation_flag = (int *) malloc(wlan.num_stas * sizeof(int));
	for(int n = 0; n < wlan.num_stas; n++){
		mcs_per_node[n] = modulations_list;
		change_modulation_flag[n] = TRUE;
	}

	/* NULL notification for Valgrind issues */
	Notification null_notification;

	null_notification.source_id = -1;
	null_notification.packet_type = -1;
	null_notification.left_channel = -1;
	null_notification.right_channel = -1;
	null_notification.packet_length = -1;
	null_notification.modulation_id = -1;
	null_notification.timestampt = -1;

	TxInfo null_tx_info;

	null_tx_info.packet_id = -1;
	null_tx_info.destination_id = -1;
	null_tx_info.tx_duration = -1;
	null_tx_info.data_duration = 0;
	null_tx_info.ack_duration = 0;
	null_tx_info.rts_duration = 0;
	null_tx_info.cts_duration = 0;
	null_tx_info.tx_power = 0;
	null_tx_info.tx_gain = 0;
	null_tx_info.data_rate = 0;
	null_tx_info.SetSizeOfMCS(4);
	null_tx_info.x = 0;
	null_tx_info.y = 0;
	null_tx_info.z = 0;
	null_tx_info.nav_time = 0;

	null_notification.tx_info = null_tx_info;

	data_notification = null_notification;
	ack_notification= null_notification;
	incoming_notification = null_notification;
	rts_notification = null_notification;
	cts_notification = null_notification;


	// Statistics
	data_packets_sent = 0;
	rts_cts_sent = 0;
	throughput = 0;
	throughput_loss = 0;
	data_packets_lost = 0;
	rts_cts_lost = 0;
	num_tx_init_not_possible = 0;
	num_tx_init_tried = 0;

}

double truncate_Sergio(double number, int floating_position){

    double x = pow(10,floating_position) * number;
    double y = x / pow(10,floating_position);
    return y;

}

double round_to_digits(double value, int digits)
{
    if (value == 0.0) // otherwise it will return 'nan' due to the log10() of zero
        return 0.0;

    // denominator
    double factor = pow(10.0, digits);

    double rounded_numerator =  round(value * factor);

    double rounded_value = rounded_numerator / factor;

    // printf("%.24f - %.24f - %.24f\n", value, rounded_numerator, rounded_value);

    return rounded_value;
}

double round_to_digits_float(float value, int digits)
{
    if (value == 0.0) // otherwise it will return 'nan' due to the log10() of zero
        return 0.0;

    // denominator
    float factor = pow(10.0, digits);

    float rounded_numerator =  round(value * factor);

    float rounded_value = rounded_numerator / factor;

    printf("%.24f - %.24f - %.24f\n", value, rounded_numerator, rounded_value);

//    printf("------------------------\n");
//    printf(" - value = %.18f\n", value);
//    printf(" - digits = %d\n", digits);
    //printf(" - fabs = %.18f\n", fabs(value));
    //printf(" - log10 = %.18f\n", log10(fabs(value)));
    //printf(" - ceil = %.18f\n", ceil(log10(fabs(value))));
    //printf(" - factor = %.18f\n", factor);
//    printf(" - rounded_value = %.18f\n", rounded_value);



    // truncate_Sergio(rounded_value,16)

    return rounded_value;
}

double fix_time_offset(double time_value, int trunc_pos, int round_pos){


	double truncated_value = 0;
	double rounded_value = 0;
	double fixed_time_value = 0;
	// double diff = 0;

	if (trunc_pos != 0) {

		truncated_value = truncate_Sergio(time_value, trunc_pos);

		rounded_value = round_to_digits(truncated_value,round_pos);

		// fixed_time_value = rounded_value;

		// diff = time_value - fixed_time_value;
//
//		if(diff<0) fixed_time_value = fixed_time_value + diff;
//		if(diff>0) fixed_time_value = fixed_time_value - diff;

		fixed_time_value = rounded_value;

//		printf("---------------------------------\n");
//		printf("- time_value = %.15f \n- truncated_value = %.15f \n- rounded_value = %.15f"
//				"\n- diff = %.15f\n- fixed_time_value = %.15f\n",
//				time_value, truncated_value, rounded_value, diff, fixed_time_value);


	} else {
		rounded_value = round_to_digits(time_value,round_pos);
		fixed_time_value = rounded_value;
	}

	// double fixed_time_value = truncated_value;

//	printf("---------------------------------\n");
//	printf("- time_value = %.15f \n- truncated_value = %.15f -\n -diff = %.15f\n- fixed_time_value = %.15f\n",
//			time_value, truncated_value, diff, fixed_time_value);

	return fixed_time_value;
}
