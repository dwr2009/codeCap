"use strict";
(function( ) {

        var connection_status = new window.Class(

        //parent
        window.Widget,
        
        
        //constructor
        function() {
                Widget.call(this, 'connection_status' ) ;

                this.$root.addClass( 'connection_status' ) ;
                this.$connection_text = $('<div>').addClass('connection_text').appendTo(this.$root);
        
                /*var $con_show = $('<div>').addClass('connection status').appendTo(this.$connection_text);
                this.$con_show = $con_show;
                this.$con_show.text(TR("general settings screen :: ip_status_unknown") );*/
                
                var $con_status = $TR('general settings screen :: ip_status_unknown');
          			this.$con_status = $con_status;
                var $con_show = $('<div>').addClass('connectionStatus').append($con_status).appendTo(this.$connection_text);
                
                var $con_ipaddress = $('<div>').addClass('connectionStatus').appendTo(this.$connection_text);
                this.$con_ipaddress = $con_ipaddress;
                
                this.update_status( window.Platform.network.getActiveInterface() );
                //window.Platform.network.set_onInterfaceChanged_callback( this.update_status.bind(this) );
                
                //window.Platform.network.register_callback_function_wifi_connect(this.set_connection_status_to_connect_to_wifi.bind(this));
                //window.Platform.network.register_callback_function_lan_connect(this.set_connection_status_to_connect_to_lan.bind(this));
                //window.Platform.network.set_onInterfaceConfigurationError_callback( this.on_interface_error.bind(this) );
        	//window.Platform.network.set_onNetworkCableChanged_callback( this.cable_state.bind(this));
                
                this.wififlag = 0;

        },

        {
                cable_state: function( connection )
                {
                    var settings = window.Platform.network.get_settings();
                    if (settings.type == window.Platform.network.NONE) {
                        
                        return;
                    }
                    
                    if (connection.linkState == window.Platform.network.UNPLUGGED && connection.configured) {
                        // cable is unpluged
                        TR_UPDATE(this.$con_status, "general settings screen :: ip_status_disconnected (Ethernet cable is unpluged)");
                        this.$con_ipaddress.text("");
                        //this.$con_show.text(TR("general settings screen :: ip_status_disconnected (Ethernet cable is unpluged)"));

                    }
                    else if (connection.linkState == window.Platform.network.PLUGGED && connection.ip_address != "" ) {
                        // cable is pluged
                        // we have a Ethernet connection
                        TR_UPDATE(this.$con_status, "general settings screen :: ip_status_connected");
                        this.$con_ipaddress.text(" ip:" + connection.ip_address);
                        //this.$con_show.text(TR("general settings screen :: ip_status_connected") + " " + connection.ip_address);
                    }
                },
                
                update_status: function( connection )
                {
                        
                        
                        
                        
                        console.log("update status");
                        console.log("interface_info  type= "+ connection.type);
                        console.log("interface_info  ip_address= "+ connection.ip_address);
                        console.log("interface_info  linkState= "+ connection.linkState);
                        console.log("interface_info  status= "+ connection.status);

                        //var $con_show = null;
                        
                        var settings = window.Platform.network.get_settings();
                        if (settings.type == window.Platform.network.NONE) {
                            		TR_UPDATE(this.$con_status, "general settings screen :: ip_status_disconnected");
                            		this.$con_ipaddress.text("");
                                //this.$con_show.text(TR("general settings screen :: ip_status_disconnected"));
                                return;
                        }
                        
                        
                        if (connection.status != window.Platform.network.CONNECTED) {
                                // well we have no internet
                                // so we display it
                                TR_UPDATE(this.$con_status, "general settings screen :: ip_status_disconnected");
                                this.$con_ipaddress.text("");
                                //this.$con_show.text(TR("general settings screen :: ip_status_disconnected"));
                                return;
                        }
                        
                        
                        if ( connection.ip_address == "" ) {
                        				TR_UPDATE(this.$con_status, "general settings screen :: ip_status_disconnected");
                        				this.$con_ipaddress.text("");
                                //this.$con_show.text(TR("general settings screen :: ip_status_disconnected"));
                        }
                        else
                        {
                            switch( connection.type ) {
                
                                case window.Platform.network.ETHERNET:
                                    
                                        // lets see if the ethernet cable is pluged
                                        if (connection.linkState == window.Platform.network.UNPLUGGED && connection.configured) {
                                        				TR_UPDATE(this.$con_status, "general settings screen :: ip_status_disconnected (Ethernet cable is unpluged)");
                                        				this.$con_ipaddress.text("");
                                                //this.$con_show.text(TR("general settings screen :: ip_status_disconnected (Ethernet cable is unpluged)"));
                                        }
                                        else if (connection.linkState == window.Platform.network.PLUGGED) {
                                                console.log("update status Pluged in !");
                                                TR_UPDATE(this.$con_status, "general settings screen :: ip_status_connected");
                                                this.$con_ipaddress.text(" ip:" + connection.ip_address);
                                                //this.$con_show.text(TR("general settings screen :: ip_status_connected") + " " + connection.ip_address);
                                        
                                        }
                                        else
                                        {
                                        				TR_UPDATE(this.$con_status, "general settings screen :: ip_status_disconnected");
                                        				this.$con_ipaddress.text("");
                                                //this.$con_show.text(TR("general settings screen :: ip_status_disconnected"));
                                        }
                                        
                                        break ;
                
                                case window.Platform.network.WIRELESS:
                                				TR_UPDATE(this.$con_status, "general settings screen :: ip_status_connected");
                                				this.$con_ipaddress.text(" ip:" + connection.ip_address + " ssid: " + connection.ssid);
                                        //this.$con_show.text(TR("general settings screen :: ip_status_connected") + " " + connection.ip_address + " ssid: " + connection.ssid);

                                    break ;
                            }
                        }
                        
                },
                
                on_interface_error: function()
                {
                        if ( this.wififlag == true)
                        {
                        				TR_UPDATE(this.$con_status, "general settings screen :: wrong wifi password");
                        				this.$con_ipaddress.text("");
                                //this.$con_show.text(TR("general settings screen :: wrong wifi password"));        
                                this.wififlag = 0;
                                return;
                        }
                        this.wififlag = 0;
                        
                        //var $con_show = null;
                        //$con_show = $('<div>').addClass('connection status').append(TR("general settings screen :: ip_status_disconnected") ).appendTo(this.$connection_text);            
                        //this.$con_show.remove();
                        //this.$con_show = $con_show;
                        TR_UPDATE(this.$con_status, "general settings screen :: ip_status_disconnected");
                        this.$con_ipaddress.text("");
                        //this.$con_show.text(TR("general settings screen :: ip_status_disconnected"));
                },
                
                set_connection_status_to_connect_to_wifi: function()
                {
                        //var $con_show = null;
                        //$con_show = $('<div>').addClass('connection status').append(TR("general settings screen :: connecting to wifi ...") ).appendTo(this.$connection_text);
                        //this.$con_show.remove();
                        //this.$con_show = $con_show;
                        TR_UPDATE(this.$con_status, "general settings screen :: connecting to wifi ...");
                        this.$con_ipaddress.text("");
                        //this.$con_show.text(TR("general settings screen :: connecting to wifi ..."));
                        this.wififlag = 1;
                        
                },
                
                set_connection_status_to_connect_to_lan: function()
                {
                        //var $con_show = null;
                        //$con_show = $('<div>').addClass('connection status').append(TR("general settings screen :: connecting to lan ...") ).appendTo(this.$connection_text);
                        //this.$con_show.remove();
                        //this.$con_show = $con_show;
                        TR_UPDATE(this.$con_status, "general settings screen :: connecting to lan ...");
                        this.$con_ipaddress.text("");
                        //this.$con_show.text(TR("general settings screen :: connecting to lan ..."));

                },
                
                /*
                update_txt: function( )
                {
        
                    this.$con_show.remove();
                    var $con_show = $('<div>').addClass('connection status').append($TR(this.status_txt) + this.ip ).appendTo(this.$connection_text);
                    this.$con_show = $con_show;
        
                },
                */
        }



) ;

window.connection_status = connection_status;

})( ) ;

		
	
