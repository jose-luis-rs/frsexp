
#ifndef __LAND_USER_HH__
#define __LAND_USER_HH__

void recover_tcal_info(uint32 tcal_time_charge_info,
		       double &tcal_time,
		       double &tcal_charge);

void analyse_random_tcal(const char *host,RANDOM_TCAL &rtcal);

void analyse_ntp_message(const char *host,NTP_MESSAGE &msg);

//

void land_pos_sampler_user_function(unpack_event *event,
				    MASTER_VME   &master_vme,
				    raw_event    *raw_event);

void land_pos_sampler_user_init();

void land_pos_sampler_user_exit();

void land_pos_sampler_user_usage_command_line_options();

bool land_pos_sampler_user_handle_command_line_option(const char *arg);

//

#endif//__LAND_USER_HH__
