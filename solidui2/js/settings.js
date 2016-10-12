"use strict";
(function( ) {

	window.settings	= {

		platform: 'pc',  // Possible values are: anything in the "js/platforms" folder.
		template: 'hkc', // Possible values are: anything in the "js/templates" folder.
		language: 'en',  // Possible values are: anything in the "js/translations" folder.

		required_key_mappings: [
			"OK",
			"LEFT",
			"RIGHT",
			"UP",
			"DOWN",
			"BACK",
		],
		
		monitor_adjust_top: 20, // Adjust the monitor margin top: value in px
		monitor_adjust_left: 40, // Adjust the monitor margin left: value in px  
		monitor_adjust_bottom: 0, // Adjust the monitor margin bottom: value in px
		monitor_adjust_right: 15, // Adjust the monitor margin right: value in px  

		wireless_update_interval: 5000,

		volume_bar_autohide_delay: 2000,
		channel_number_autohide_delay: 5000,
		info_bar_autohide_delay: 5000,
		

	} ;

})( ) ;
