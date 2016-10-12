"use strict";
(function( ) {


        var name = "ip_config" ;


        var ip_config = function( ) {

		var parent = new window.MainMenuScreen(name);
		//var parent = new window.Scene( name ) ;
                var self = Object.create( parent ) ;
                

                self.$root.attr( 'id', 'ip_config' ) ;
                
                var $ip_config = $('<div>').addClass( 'ip_config' ).appendTo(self.$root);
		

/* +++++++++++++++++++++++++++++++ this would be for ipv6 ++++++++++++++++++++++++++++++++
		var switchToProtocol = function( item ) {
			console.log( "Seleced Protocol: ", item.name );
			switch( item )
			{
				case 'nettwork settings screen :: ipv4 and ipv6':
					//window.AppManager.getWidget( 'ipv4 config' ).show( ) ;
                                        //window.AppManager.getWidget( 'ipv6 config' ).show( ) ;
					ip_settings.show_list("ipv4");
					ip_settings.show_list("ipv6");
					break;
				case 'nettwork settings screen :: ipv4':
					//window.AppManager.getWidget( 'ipv4 config' ).show( ) ;
					//window.AppManager.getWidget( 'ipv6 config' ).hide( ) ;
					ip_settings.show_list("ipv4");
					ip_settings.hide_list("ipv6");
					break;
				case 'nettwork settings screen :: ipv6':
					//window.AppManager.getWidget( 'ipv4 config' ).hide( ) ;
					//window.AppManager.getWidget( 'ipv6 config' ).show( ) ;
					ip_settings.hide_list("ipv4");
					ip_settings.show_list("ipv6");
					break;
			}
		};

  +++++++++++++++++++++++++++++++++++++++++++++++  end of ipv6 stuff ++++++++++++++++++++++++++++++++++++++ */

  
		var ok_was_pressed = function()
		{
            console.log("you should be in here: ", 1);
            // save all conf's on the box and try connecting
            window.Platform.network.setConnectionDetails_settings( window.Platform.network.MANUAL );
            window.Platform.network.setConnectionDetails_addres( ip_settings.get_element("ipv4").get_item(0).getText() );
            window.Platform.network.setConnectionDetails_mask( ip_settings.get_element("ipv4").get_item(1).getText() );
            window.Platform.network.setConnectionDetails_gateway( ip_settings.get_element("ipv4").get_item(2).getText() );
            window.Platform.network.setConnectionDetails_dns1( ip_settings.get_element("ipv4").get_item(3).getText() );
            window.Platform.network.setConnectionDetails_dns2( ip_settings.get_element("ipv4").get_item(4).getText() );


			window.AppManager.back( );
            window.AppManager.enter("General Settings");

			// if there is also ipv6 the other settengs als need to be read
            var ret = window.Platform.network.auto_apply_settings();
            console.log("trying to aply seetings: ", ret);            
            window.Platform.network.standard_error_reaction( ret );
            if ( ret < 0) {
                //reset settings
                window.AppManager.getScene("General Settings").reload_nettwork_settings();
            }
		};
 /*       
        var canceled = false;
        var tmp;
        self.canceled = function()
        {
            tmp = JSON.parse(JSON.stringify(canceled));
            canceled = false;
            console.log( "0000000000000000000000000000000000:", tmp ) ;

            return tmp;
        };
*/
        
		var cancel_was_pressed = function()
		{
            console.log("return to general settings");
            window.AppManager.getScene("General Settings").set_focus( ) ;

			window.AppManager.back( );
            window.AppManager.enter("General Settings");
		};
        
        

	
/*++++++++++++++++++++++++++++++++++++++++++++++  this would be for ipv6 ++++++++++++++++++++++++++++++++++++++++++
		var ip_selection =
		{
			name: "ip selection list",
			window_function: window.normalised_select_view,
				items: [
					{
						name: "nettwork settings screen :: network settings",
						items: [
							{ name: 'nettwork settings screen :: ipv4 and ipv6', value: 'ipv4 and ipv6', },
							{ name: 'nettwork settings screen :: ipv4', value: 'ipv4', },
							{ name: 'nettwork settings screen :: ipv6', value: 'ipv6', },
						],
						action: switchToProtocol,
					},
				],
                };


		var ipv4_desc =
		{
			name: "ipv4 config",
			window_function: window.InputView,
			items: [
				{ name: 'nettwork settings screen :: ipv4 address', size: 15, type: 'ipv4', },
				{ name: 'nettwork settings screen :: ipv4 network mask', size: 15, type: 'ipv4', },
				{ name: 'nettwork settings screen :: ipv4 gateway', size: 15, type: 'ipv4', },
				{ name: 'nettwork settings screen :: ipv4 DNS 1', size: 15, type: 'ipv4', },
				{ name: 'nettwork settings screen :: ipv4 DNS 2', size: 15, type: 'ipv4', },
			],
		};

		var ipv6_desc =
		{
			name: "ipv6 config",
			window_function: window.InputView,
			items: [
				{ name: 'nettwork settings screen :: ipv6 address', size: 15, type: 'ipv6', },
				{ name: 'nettwork settings screen :: ipv6 network mask', size: 15, type: 'ipv6 netmask', },
				{ name: 'nettwork settings screen :: ipv4 gateway', size: 15, type: 'ipv6', },
				{ name: 'nettwork settings screen :: ipv4 DNS 1', size: 15, type: 'ipv6', },
				{ name: 'nettwork settings screen :: ipv4 DNS 2', size: 15, type: 'ipv6', },
			],
		};

		var button_desc =
		{
                        name: 'ip buttons',
                        left_button: 'nettwork settings screen :: cansel_button',
                        right_button: 'sample_screen :: connect_ok_button',
                        left_callback: ok_was_pressed,
                        right_callback: cancel_was_pressed,
		};
			

                var ip_config =
                {
                        name: "ip config",
                        window_function: [
                                                { name: "ip select", window_function: window.text_field, description: ip_selection },
                                                { name: "ipv4", window_function: window.text_field, description: ipv4_desc },
                                                { name: "ipv6", window_function: window.text_field, description: ipv6_desc },
                                                { name: "buttons", window_function: window.two_buttons, description: button_desc },
                                        ],
                };

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ end of ipv6 stuff +++++++++++++++++++++++++++++++++++++++++++++++ */ 

// +++++++++++++++++++++++++++++++++ this is only ipv4 replace it whit ipv6 if ipv6 is here +++++++++++++++++++++++++++++++++++++++


                var ipv4_desc =
                {
                        name: "ipv4 config",
                        window_function: window.InputView,
                        items: [
                                { name: 'nettwork settings screen :: ipv4 address', size: 15, type: 'ipv4', },
                                { name: 'nettwork settings screen :: ipv4 network mask', size: 15, type: 'ipv4', },
                                { name: 'nettwork settings screen :: ipv4 gateway', size: 15, type: 'ipv4', },
                                { name: 'nettwork settings screen :: ipv4 DNS 1', size: 15, type: 'ipv4', },
                                { name: 'nettwork settings screen :: ipv4 DNS 2', size: 15, type: 'ipv4', },
                        ],
                };


                var button_desc =
                {
                        name: 'ip buttons',
                        left_button: 'IPPV_btn_cancel :: cancel',
                        right_button: 'nettwork settings screen :: connect_ok_button',
                        left_callback: cancel_was_pressed,
                        right_callback: ok_was_pressed,
                };
                        

                var ip_config =
                {
                        name: "ip config",
                        window_function: [
                                                { name: "ipv4", window_function: window.text_field, description: ipv4_desc },
                                                { name: "buttons", window_function: window.two_buttons, description: button_desc },
                                        ],
                };
// +++++++++++++++++++++++++++++++++++++++++++ end of only ipv4 stuff ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



		var ip_settings = new window.list_manager(ip_config);
		ip_settings.$root.appendTo( $ip_config );

		ip_settings.enter("ipv4","start_top");
		ip_settings.show();
		ip_settings.show_list("ipv4");
		ip_settings.show_list("buttons");

		
		// fill imput views whit data from settings
		var settings = window.Platform.network.get_settings();
                ip_settings.get_element("ipv4").get_item(0).get_input().set_text( settings.ip_address  );
                ip_settings.get_element("ipv4").get_item(1).get_input().set_text( settings.mask  );
                ip_settings.get_element("ipv4").get_item(2).get_input().set_text( settings.gateway  );
                ip_settings.get_element("ipv4").get_item(3).get_input().set_text( settings.dns1  );
                ip_settings.get_element("ipv4").get_item(4).get_input().set_text( settings.dns2  );

		

                self.onKey = function( ) {
                        return true ;
                } ;

                self.onKeyUP = function( ) {

			ip_settings.move_up();
			
                } ;

                self.onKeyDOWN = function( ) {

			ip_settings.move_down();
                       
                } ;


         self.onKeyOK = function( ) {
			ip_settings.activate();
        } ;

		self.onKeyNUMBER = function( num ) {

			ip_settings.get_current_element().get_active_item().appendChar( num.toString() );
		};
        
        self.onKeyRIGHT = function()
		{
            ip_settings.get_current_element().get_active_item().get_input().next_section();
        };
        
        self.onKeyBACK = function( ) {
            ip_settings.get_current_element().get_active_item().get_input().removeChar();
        };
        
		self.onKeyLEFT = function()
		{
            ip_settings.get_current_element().get_active_item().get_input().back_section();
		};

	 return self ;
        

	} ;
	
	
        window.AppManager.addScene( name, ip_config ) ;


})( ) ;

