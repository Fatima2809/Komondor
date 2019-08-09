/* Komondor IEEE 802.11ax Simulator
 *
 * Copyright (c) 2017, Universitat Pompeu Fabra.
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007

 * Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
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
 * File description: this is the main Komondor file
 *
 * - This file defines a NOTIFICATION and provides basic displaying methods
 */

#ifndef _AUX_PERFORMANCE_
#define _AUX_PERFORMANCE_

struct Performance
{
	// Time of the last measurement
	double last_time_measured;
	double sum_time_channel_idle;

	// Throughput statistics
	double throughput;
	double throughput_loss;
	double max_bound_throughput;

	// Frames statistics
	int data_packets_acked;
	int data_frames_acked;
	int data_packets_sent;
	int data_packets_lost;
	int rts_cts_sent;
	int rts_cts_lost;
	int rts_lost_slotted_bo;

	// Buffer statistics
	int num_packets_generated;
	int num_packets_dropped;
	int num_delay_measurements;
	double sum_delays;
	double average_delay;
	double average_rho;
	double average_utilization;
	double generation_drop_ratio;

	// Channel occupancy
	double expected_backoff;
	int num_new_backoff_computations;
	int *num_trials_tx_per_num_channels;
	double average_waiting_time;
	double bandwidth_used_txing;
	double *total_time_transmitting_per_channel;
	double *total_time_transmitting_in_num_channels;
	double *total_time_lost_per_channel;
	double *total_time_lost_in_num_channels;
	double *total_time_spectrum_per_channel;
	double time_in_nav;

	// Per-STA statistics
	double *throughput_per_sta;
	int *data_packets_sent_per_sta;
	int *rts_cts_sent_per_sta;
	int *data_packets_lost_per_sta;
	int *rts_cts_lost_per_sta;
	int *data_packets_acked_per_sta;
	int *data_frames_acked_per_sta;

	// Other
	int num_tx_init_tried;
	int num_tx_init_not_possible;
	double prob_slotted_bo_collision;
	double *rssi_list;
	double *received_power_array;

//	// Function to print the node's report
//	void PrintReport(void){
//		printf("%s Report (last measurement in %f):\n", LOG_LVL4, last_time_measured);
//		printf("%s throughput = %f\n", LOG_LVL5, throughput);
//		printf("%s max_bound_throughput = %f\n", LOG_LVL5, max_bound_throughput);
//		printf("%s data_packets_sent = %d\n", LOG_LVL5, data_packets_sent);
//		printf("%s data_packets_lost = %d\n", LOG_LVL5, data_packets_lost);
//		printf("%s rts_cts_packets_sent = %d\n", LOG_LVL5, rts_cts_packets_sent);
//		printf("%s rts_cts_packets_lost = %d\n", LOG_LVL5, rts_cts_packets_lost);
//		printf("%s num_packets_generated = %f\n", LOG_LVL5, num_packets_generated);
//		printf("%s num_packets_dropped = %f\n", LOG_LVL5, num_packets_dropped);
//		printf("\n");
//	}

	/*
	 * SetSizeOfChannelLists(): sets the size of the arrays in which channel-related information is stored
	 */
	void SetSizeOfChannelLists(int total_channels_number){
		num_trials_tx_per_num_channels = new int[total_channels_number];
		total_time_transmitting_per_channel = new double[total_channels_number];
		total_time_transmitting_in_num_channels = new double[total_channels_number];
		total_time_lost_per_channel = new double[total_channels_number];
		total_time_lost_in_num_channels = new double[total_channels_number];
		total_time_spectrum_per_channel = new double[total_channels_number];
		for(int i = 0; i < total_channels_number; ++i){
			num_trials_tx_per_num_channels[i] = 0;
			total_time_transmitting_per_channel[i] = 0;
			total_time_transmitting_in_num_channels[i] = 0;
			total_time_lost_per_channel[i] = 0;
			total_time_lost_in_num_channels[i] = 0;
			total_time_spectrum_per_channel[i] = 0;
		}
	}

	/*
	 * SetSizeOfStaList(): sets the size of the arrays in which STA-related information is stored
	 */
	void SetSizeOfStaList(int num_stas){
		throughput_per_sta = new double[num_stas];
		data_packets_sent_per_sta = new int[num_stas];
		rts_cts_sent_per_sta = new int[num_stas];
		data_packets_lost_per_sta = new int[num_stas];
		rts_cts_lost_per_sta = new int[num_stas];
		data_packets_acked_per_sta = new int[num_stas];
		data_frames_acked_per_sta = new int[num_stas];
		for(int i = 0; i < num_stas; ++i){
			throughput_per_sta[i] = 0;
			data_packets_sent_per_sta[i] = 0;
			rts_cts_sent_per_sta[i] = 0;
			data_packets_lost_per_sta[i] = 0;
			rts_cts_lost_per_sta[i] = 0;
			data_packets_acked_per_sta[i] = 0;
			data_frames_acked_per_sta[i] = 0;
		}
	}

	/*
	 * SetSizeOfRssiList(): sets the size of the array list_id_aggregated
	 */
	void SetSizeOfRssiList(int total_wlans_number){
		rssi_list = new double[total_wlans_number];
		for(int i = 0; i < total_wlans_number; ++i){
			rssi_list[i] = 0;
		}
	}

	/*
	 * SetSizeOfRxPowerList(): sets the size of the array list_id_aggregated
	 */
	void SetSizeOfRxPowerList(int total_nodes_number){
		received_power_array = new double[total_nodes_number];
		for(int i = 0; i < total_nodes_number; ++i){
			received_power_array[i] = 0;
		}
	}

};

#endif
