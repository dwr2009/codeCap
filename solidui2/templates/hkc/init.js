"use strict";
(function( ) {

	var prefix = 'templates/hkc/' ;

	var loading_job = window.Loader.loadStyles( [
		prefix + 'css/font.css',
		prefix + 'css/style.css',
		prefix + 'css/live_tv.css',
		prefix + 'css/volume_bar.css',
		prefix + 'css/top_bar.css',
		prefix + 'css/main_menu.css',
		prefix + 'css/panel.css',
		prefix + 'css/settings.css',
		prefix + 'css/drives.css',
		prefix + 'css/epg.css',
		prefix + 'css/recordings.css',
		prefix + 'css/messages.css',
		prefix + 'css/channels.css',
		prefix + 'css/service_info.css',
		prefix + 'css/statument.css',
		prefix + 'css/tem_account.css',
		prefix + 'css/info_bar.css',
		prefix + 'css/progress_bar.css',
		prefix + 'css/scroll_bar.css',
		prefix + 'css/ca_menu.css',
		prefix + 'css/input_view.css',
		prefix + 'css/label_view.css',
		prefix + 'css/button_view.css',
		prefix + 'css/select_view.css',
		prefix + 'css/process_bar.css',
		prefix + 'css/manual_scan_c.css',
		prefix + 'css/auto_scan_c.css',
		prefix + 'css/keyinput_PopDlg.css',
		prefix + 'css/parental_lock.css',
		prefix + 'css/audiotrack_selectdlg.css',

		prefix + 'css/camenu/pin_code.css',
		prefix + 'css/camenu/parental_rating.css',
		prefix + 'css/camenu/smart_card.css',
		prefix + 'css/camenu/work_time.css',
		prefix + 'css/camenu/email.css',
		prefix + 'css/camenu/stb_info.css',
		prefix + 'css/camenu/ca_info.css',
		prefix + 'css/camenu/nstv_ca.css',
		prefix + 'css/camenu/fingerPrinter.css',
		prefix + 'css/camenu/popDlg.css',
		prefix + 'css/camenu/buyMsgpopDlg.css',
		prefix + 'css/system_info.css',
		prefix + 'css/camenu/OSDMsg.css',
		prefix + 'css/camenu/emailNotifyIcon.css',
		prefix + 'css/camenu/curtainNotify.css',
		prefix + 'css/camenu/selectPopDlg.css',
		prefix + 'css/camenu/confirmPopDlg.css',
		prefix + 'css/camenu/ippv_bar.css',
		prefix + 'css/scroll_list_view.css',
		prefix + 'css/camenu/progressStrip.css',
		prefix + 'css/camenu/page_turning_list_view.css',
		prefix + 'css/ok_bar.css',
		prefix + 'css/scroll_list_view.css',
		prefix + 'css/timeshift_bar.css',
		prefix + 'css/channel_bar.css',
	  	prefix + 'css/favPopDlg.css',
	  	prefix + 'css/favorite_management.css',
		prefix + 'css/reminder_pop.css',
		prefix + 'css/program_reminder.css',
		prefix + 'css/reminder_screen.css',
	  	prefix + 'css/network_conf/wifi_config.css',
	  	prefix + 'css/network_conf/wifi_key_input.css',
		prefix + 'css/network_conf/lan_overlay.css',
		prefix + 'css/confirm_box.css',
		prefix + 'css/timeshift_icon.css',
		prefix + 'css/alert_box.css',
	] ) ;


	var Template = new function( ) {

		this.prefix = prefix ;

		this.DefaultLanguage = "ch"; 
	        this.VolumeMinLimit  = 85;
	        
	        this.EnableFrqHunting = true;
	        
	        this.ChannelMaxLimit = 200;

                this.enhance = {
            	    "Enable"   : true,
             	    "Settings" : [53, 48, 38, 50, 50],
                }
            
	        this.frequenceList = [
			/* from_freq, to_freq, freq_step, bandwidth, symbolrate*/
			[ 234, 234, 1, 5, 5833 ], 
			[ 234, 234, 1, 7, 5833 ],
            
			[ 258, 258, 1, 5, 5833 ],
			[ 258, 258, 1, 7, 5833 ],
            
			[ 266, 266, 1, 5, 5833 ],
			[ 266, 266, 1, 7, 5833 ],
            
			[ 306, 306, 1, 5, 5833 ],
			[ 306, 306, 1, 7, 5833 ],
            
			[ 250, 250, 1, 5, 5833 ],
			[ 250, 250, 1, 7, 5833 ],
            
			[ 298, 298, 1, 5, 5833 ],
			[ 298, 298, 1, 7, 5833 ],
            
			[ 194, 194, 1, 5, 5833 ],
			[ 194, 194, 1, 7, 5833 ],
            
			[ 346, 346, 1, 5, 5833 ],
			[ 346, 346, 1, 7, 5833 ],
            
			[ 210, 210, 1, 5, 5833 ],
			[ 210, 210, 1, 7, 5833 ],    
            
			[ 202, 202, 1, 5, 5833 ],
			[ 202, 202, 1, 7, 5833 ], 

			[ 314, 314, 1, 5, 5833 ],
			[ 314, 314, 1, 7, 5833 ], 

			[ 322, 322, 1, 5, 5833 ],
			[ 322, 322, 1, 7, 5833 ], 
            
			[ 330, 330, 1, 5, 5833 ],
			[ 330, 330, 1, 7, 5833 ],     

			[ 386, 386, 1, 5, 5833 ],
			[ 386, 386, 1, 7, 5833 ],  
            
			[ 378, 378, 1, 5, 5833 ],
			[ 378, 378, 1, 7, 5833 ],  
            
			[ 370, 370, 1, 5, 5833 ],
			[ 370, 370, 1, 7, 5833 ],  
            
			[ 362, 362, 1, 5, 5833 ],
			[ 362, 362, 1, 7, 5833 ],  
             
			[ 354, 354, 1, 5, 5833 ],
			[ 354, 354, 1, 7, 5833 ],                                                                                                                                                                                        
	        ];

		this.init = function( callback ) {
			loading_job.onDone( callback ) ;

			/*$( '<div>' ).text( 'Template is HKC' ).css( {
				'position': 'fixed',
				'bottom': '15px',
				'right': '15px',
			 } ).appendTo( 'body' ) ;*/
		} ;
		this.customersetting = {
			"channelnumbertype" : "lcn", //possible value lcn or index
		},

		
		this.formatsetting =  {
			"Date and Time" : {
				"DateTime": {
					"ch" : "yyyy年M月dd日 (EEEE)  ah.mm",
					"default" : "dd-MMM-yyyy,EEE | HH:mm",
				}, 				
			},		
			"Programme Guide" : {
				"Date": {
					"ch" : "yyyy年M月dd日",
					"default" : "dd-MMM-yyyy",
				}, 				
				"Time": {
					"ch" : "ah:mm",
					"default" : "HH:mm",
				},
				"DateTime": {
					"ch" : "yyyy年M月dd日 (EEEE)  ah.mm",
					"default" : "dd-MMM-yyyy,EEE | HH:mm",
				}, 				
			},
			"Timeshift Bar" : {
				"Time": {
					"ch" : "ah:mm:ss",
					"default" : "HH:mm:ss",
				},
			},
			"Info Bar" : {
				"Date": {
					"ch" : "yyyy年M月dd日",
					"default" : "dd-MMM-yyyy",
				}, 				
				"Time": {
					"ch" : "ah:mm",
					"default" : "HH:mm",
				}, 				
			},
			"progressbar" : {
				"Date": {
					"ch" : "yyyy年M月dd日",
					"default" : "dd-MMM-yyyy",
				}, 				
				"Time": {
					"ch" : "ah:mm",
					"default" : "HH:mm",
				}, 				
			},
			"Reminder Pop" : {
				"Date": {
					"ch" : "yyyy年M月dd日",
					"default" : "dd-MMM-yyyy",
				}, 				
				"Time": {
					"ch" : "ah:mm",
					"default" : "HH:mm",
				},
			},
			"Programme Reminder" : {
				"Date": {
					"ch" : "yyyy年M月dd日",
					"default" : "dd-MMM-yyyy",
				}, 				
				"Time": {
					"ch" : "ah:mm",
					"default" : "HH:mm",
				},
			},
			"Reminder Screen" : {
				"Date": {
					"ch" : "yyyy年M月dd日",
					"default" : "dd-MMM-yyyy",
				}, 				
				"Time": {
					"ch" : "ah:mm",
					"default" : "HH:mm",
				},
			},
		};
		this.EnableMaster = false;
		this.EnableAdultChannels = true;
		this.generalSetting = ["general settings screen :: language",
							   "general settings screen :: network",
							   //"general settings screen :: network settings",
							   "general settings screen :: aspectratio",
							   "general settings screen :: videoformat",
							   "general settings screen :: audiooutput",
							   "general settings screen :: subtitle",
							   "general settings screen :: videoresolution",
							   "general settings screen :: audiopreference",
							   //"general settings screen :: timezone",
                                     ]; 
		this.mainListName = ["All Channels",
		                     "所有頻道",
                                    ];
                                    
		this.EnableNoInformation = false;
	} ;

	window.Template = Template ;

})( ) ;
