"use strict";
(function( ) {

	var name = "Internet Icon" ;


	var Internet = function( ) {

		var parent = new window.Widget( name ) ;
		var self = Object.create( parent ) ;

		window.Platform.network.init();
		
		self.$root.attr( 'id', 'internet_icon' ) ;        
        
        //start the network observer for the icon
        self.onShow = function( ) {
            networkIconUpdate();
            startNetworkIconUpdate();            
        };

        //stop the network observer for the icon
        self.onHide = function( ) {
            stopNetworkIconUpdate();
        }; 

		var $internet = $( '<div>' ).addClass( 'debug_text' ).appendTo( self.$root ) ;
		var $img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
		
		// on first run do not make popus
		var first_run = true;
        
        var network_observer_timeout;
        var network_observer_running = false;
        
        //function to update wifi signal strength
        var startNetworkIconUpdate = function( ) {
            if (network_observer_running == false) {
                network_observer_running = true;
               // console.log("start netwok observer");
                network_observer_timeout = window.setInterval(networkIconUpdate, 2500);
            }
        };
        
        //function to update wifi signal strength
        var stopNetworkIconUpdate = function( ) {
            if (network_observer_running == true) {
                network_observer_running = false;
                //console.log("stop netwok observer");
                window.clearInterval(network_observer_timeout);
            }
            
        };
        
        //function to update wifi signal icon
        var networkIconUpdate = function( ) {
            var con = window.Platform.network.getActiveInterface();
            switch (con.type) {
                
            case window.Platform.network.WIRELESS:
                if (!con.configured) { // wifi interface is not configured
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_error' ).appendTo( self.$root );
                } else if (!con.connected || con.signal <= 0) { // we have no connection
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'no_wifi_connection' ).appendTo( self.$root );
                } else if (con.signal <= 25) {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_connection_25' ).appendTo( self.$root );
                } else if (con.signal <= 50) {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_connection_50' ).appendTo( self.$root );
                } else if (con.signal <= 75) {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_connection_75' ).appendTo( self.$root );
                } else if (con.signal <= 100) {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_connection_100' ).appendTo( self.$root );
                }
                break;
            case window.Platform.network.ETHERNET:
                if (!con.connected) { // we have no connection
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
                     window.AppManager.getScene("System Info").judgeLANNotConnect();
                } else {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'lan_connection' ).appendTo( self.$root );
                    window.AppManager.getScene("System Info").judgeLANConnect();
                    break;
                }
            default: // something strange happens, so we have no connection
                $img.remove();
                $img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
                break;
            }
        };
		
		
		var update = function( con ) {

				console.log("active inteface changed callback");
				console.log("interface_info  type= "+ con.type);
				console.log("interface_info  ip_address= "+ con.ip_address);
				console.log("interface_info  linkState= "+ con.linkState);
				console.log("interface_info  status= "+ con.status);
            switch (con.type) {
                
            case window.Platform.network.WIRELESS:
                if (!con.configured) { // wifi interface is not configured
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_error' ).appendTo( self.$root );
                    window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
                } else if (!con.connected || con.signal <= 0) { // we have no connection
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'no_wifi_connection' ).appendTo( self.$root );
                    window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
                } else if (con.signal <= 25) {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_connection_25' ).appendTo( self.$root );
                } else if (con.signal <= 50) {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_connection_50' ).appendTo( self.$root );
                } else if (con.signal <= 75) {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_connection_75' ).appendTo( self.$root );
                } else if (con.signal <= 100) {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'wifi_connection_100' ).appendTo( self.$root );
                }
                break;
            case window.Platform.network.ETHERNET:
                if (!con.connected) { // we have no connection
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
                    window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
                } else {
                    $img.remove();
                    $img = $( '<div>' ).addClass( 'lan_connection' ).appendTo( self.$root );
                    break;
                }
            default: // something strange happens, so we have no connection
                $img.remove();
                $img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
                window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
                break;
            }

        /*
			var settings = window.Platform.network.get_settings();
			if (settings.type == window.Platform.network.NONE) {
				
				// no internet connection was selected put image to no_lan_connection and return
				$img.remove();
				$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
				
				// none in genearal settings is alredy set since it gets set in the settings
				// when the user selects it
				// in every other case none gets only shown to display that there is no internet
				// connection and somthing wen't wrong

				return;
			}
			
			
			if (connection.status != window.Platform.network.CONNECTED) {
				// well we have no internet
				// so lets make a error

				// set general settings to none internet connection
				window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
				
				// also we have no internet connection so put img to no_lan_connection
				$img.remove();
				$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );

				// on first run do not make popus
				if (first_run == true) {
					first_run = false;
					return;
				}
				
				// info popup
				window.AppManager.getScene('alert_box').text(TR('alert msg screen :: connection faild'));
				window.AppManager.enter('alert_box');

				return;
			}
			
			
			if ( connection.ip_address == "" ) {
				// set general settings to none internet connection
				window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
				
				// also we have no internet connection so put img to no_lan_connection
				$img.remove();
				$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );

				// on first run do not make popus
				if (first_run == true) {
					first_run = false;
					return;
				}
				
				// info popup
				window.AppManager.getScene('alert_box').text(TR('alert msg screen :: connection faild'));
				window.AppManager.enter('alert_box');
			}
			else
			{
				switch( connection.type ) {
	
					case window.Platform.network.ETHERNET:
						
						// lets see if the ethernet cable is pluged
						if (connection.linkState == window.Platform.network.UNPLUGGED) {
							// cable is unpluged
							
							// set general settings to none internet connection
							window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
							
							// also we have no internet connection so put img to no_lan_connection
							$img.remove();
							$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
							
							// on first run do not make popus
							if (first_run == true) {
								first_run = false;
								return;
							}
							
							// info popup
							window.AppManager.getScene('alert_box').text(TR('alert msg screen :: cable unplugged'));
							window.AppManager.enter('alert_box');
						}
						else if (connection.linkState == window.Platform.network.PLUGGED) {
							// cable is pluged
							// we have a Ethernet connection
							// set picture
							$img.remove();
							$img = $( '<div>' ).addClass( 'lan_connection' ).appendTo( self.$root );
	
							// set general settings to lan
							window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 1);
	
						}
						else
						{
							// well i have no idea what happen that i landed here but let's act like the box is unplugged
							
							// set general settings to none internet connection
							window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
							
							// also we have no internet connection so put img to no_lan_connection
							$img.remove();
							$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );

							// on first run do not make popus
							if (first_run == true) {
								first_run = false;
								return;
							}
							
							// info popup
							window.AppManager.getScene('alert_box').text(TR('alert msg screen :: network error'));
							window.AppManager.enter('alert_box');
						}
						
						break ;
	
					case window.Platform.network.WIRELESS:
						// we have a Wireless connection
						// set picture
						$img.remove();
						$img = $( '<div>' ).addClass( 'wifi_connection' ).appendTo( self.$root );

						// set general settings to wifi
						window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 0);


						//mabey later
						//startWirelessUpdate( ) ;
						break ;
				}
			}
			*/
		} ;
		
		var interface_error_occurred = function()
		{
			
			console.log("interface_error_occurred callback function");

			
			// we have no internet
			
			// put image to no_lan_connection
			//$img.remove();
			//$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
			
			// set general settings to no internet
			window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
            
            /*
            
			// test if none type was set ( should we have no internet connection ?)
			var settings = window.Platform.network.get_settings();
			if (settings.type == window.Platform.network.NONE) {
				//everthing fine we should not have a internet connection none was selected
				return;
			}
			
            
			// on first run do not make popus
			if (first_run == true) {
				first_run = false;
				return;
			}
			
			// oh somthing wen't wrong
			// info popup
			window.AppManager.getScene('alert_box').text(TR('alert msg screen :: connection faild'));
			window.AppManager.enter('alert_box');
            */
		};

		var on_scan = function( state, data )
		{
			console.log("sacn state is: ", state);
			
			if ( window.Platform.network.WIFI_SCAN_INIT ==  state ||  window.Platform.network.WIFI_SCAN_IN_PROGRESS == state ) {
				
				var connection = window.Platform.network.getActiveInterface();
				
				var settings = window.Platform.network.get_settings();
				if (settings.type == window.Platform.network.NONE) {
					
					// no internet connection was selected put image to no_lan_connection and return
					$img.remove();
					$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
					
					// none in genearal settings is alredy set since it gets set in the settings
					// when the user selects it
					// in every other case none gets only shown to display that there is no internet
					// connection and somthing wen't wrong
					return;
				}
				
				
				if (connection.status != window.Platform.network.CONNECTED) {
					// well we have no internet
					// so lets make a error
	
					// set general settings to none internet connection
					window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
					
					// also we have no internet connection so put img to no_lan_connection
					$img.remove();
					$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
	
					return;
				}
				
				
				if ( connection.ip_address == "" ) {
					// set general settings to none internet connection
					window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
					
					// also we have no internet connection so put img to no_lan_connection
					$img.remove();
					$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
	
				}
				else
				{
					switch( connection.type ) {
		
						case window.Platform.network.ETHERNET:
							
							// lets see if the ethernet cable is pluged
							if (connection.linkState == window.Platform.network.UNPLUGGED) {
								// cable is unpluged
								
								// set general settings to none internet connection
								window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
								
								// also we have no internet connection so put img to no_lan_connection
								$img.remove();
								$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
								
							}
							else if (connection.linkState == window.Platform.network.PLUGGED) {
								// cable is pluged
								// we have a Ethernet connection
								// set picture
								$img.remove();
								$img = $( '<div>' ).addClass( 'lan_connection' ).appendTo( self.$root );
		
								// set general settings to lan
								window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 1);
		
							}
							else
							{
								// well i have no idea what happen that i landed here but let's act like the box is unplugged
								
								// set general settings to none internet connection
								window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
								
								// also we have no internet connection so put img to no_lan_connection
								$img.remove();
								$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
	
							}
							
							break ;
		
						case window.Platform.network.WIRELESS:
							// we have a Wireless connection
							// set picture
							$img.remove();
							$img = $( '<div>' ).addClass( 'wifi_connection' ).appendTo( self.$root );
							// set general settings to wifi
							window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 0);
	
	
							//mabey later
							//startWirelessUpdate( ) ;
							break ;
					}
				}
			}
		} ;
		
		var cable_state = function(connection)
		{
			var settings = window.Platform.network.get_settings();
			if (settings.type == window.Platform.network.NONE) {
				
				// no internet connection was selected put image to no_lan_connection and return
				$img.remove();
				$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
				
				// none in genearal settings is alredy set since it gets set in the settings
				// when the user selects it
				// in every other case none gets only shown to display that there is no internet
				// connection and somthing wen't wrong
				return;
			}
			
			if (connection.linkState == window.Platform.network.UNPLUGGED && connection.configured) {
				// cable is unpluged
				
				// set general settings to none internet connection
				window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 2);
				
				// also we have no internet connection so put img to no_lan_connection
				$img.remove();
				$img = $( '<div>' ).addClass( 'no_lan_connection' ).appendTo( self.$root );
				
				
				// info popup
				//window.AppManager.getScene('alert_box').text(TR('alert msg screen :: cable got unplugged'));
				//window.AppManager.enter('alert_box');
			}
			else if (connection.linkState == window.Platform.network.PLUGGED && connection.ip_address != "" ) {
				// cable is pluged
				// we have a Ethernet connection
				// set picture
				$img.remove();
				$img = $( '<div>' ).addClass( 'lan_connection' ).appendTo( self.$root );

				// set general settings to lan
				window.AppManager.getScene("General Settings").change_item_to("general settings screen :: network", 1);

			}
		};
		
		update( window.Platform.network.getActiveInterface() );
		window.Platform.network.set_onInterfaceChanged_callback( update );

		window.Platform.network.set_onInterfaceConfigurationError_callback( interface_error_occurred );
		window.Platform.network.set_onScanStateChanged_callback( on_scan );
		//window.Platform.network.register_callback_function_scanWifiStart( on_scan.bind(this));
		window.Platform.network.set_onNetworkCableChanged_callback(cable_state);

		return self ;
	} ;
	


	window.AppManager.addWidget( name, Internet ) ;

})( ) ;
