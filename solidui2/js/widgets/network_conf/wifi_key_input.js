"use strict"; 
(function() {

    var name = "Wifi Key Input";

    var  KeyInputPopDlg= function() {

        var parent = new window.Scene(name);
        var self = Object.create(parent);
        self.$root.attr('id', 'wifiKeyInput');

        var wifiKeyInputDesc = {
            accurationLength: 20, 
            type: 'password',
        };
        
        var $keyinput = $('<div>').addClass('keyinput').appendTo(self.$root);
        var keyName = $('<div>').addClass('column_forty').append($TR('wifi settings screen :: wifi_password')).appendTo($keyinput);
        var $inputField = $('<div>').addClass('column_sixty').appendTo($keyinput); 
        var input = new window.InputItemVK(wifiKeyInputDesc);
        input.$root.appendTo($inputField);
        input.show();

        var $keyinput_message = $('<div>').addClass('message').appendTo(self.$root);

        var $keyinput_confirm_btn = $('<div>').addClass('confirmbtnlable').appendTo(self.$root);
        var $confirm_btn_area = $('<div>').addClass('confirmbtn').appendTo($keyinput_confirm_btn);
        var $keyinput_confirm_panel = $('<div>').addClass('panel').append($TR('pincode_btn_confirm :: confirm')).appendTo($confirm_btn_area);

		this.CallbackOK = null;
		
        var listData = [{
            name: 'keyinput',
            $root: $inputField,
            callback: null,
        },
        {
            name: 'confirm_btn',
            $root: $keyinput_confirm_panel,
            callback:null,
        }];
        var focusList = new window.FocusList(listData);
       
        var updateScreen = function() {
            focusList.select(0);
            input.clear();
            input.setFocus(true);
            $keyinput_message.text("");
        };
     
        //var wifiInterface =  _StbNetworkObj.interfaceId(tv2_stb_network.Interface_Type_WIFI);
        
        var connectWifi = function (password) {
            var selected_hotspot = Platform.network.Selected_hotspot;
            console.log("selected Hotspots: " + JSON.stringify(selected_hotspot));
            console.log("got the wifi password: " + password);
            
            if (selected_hotspot !== undefined) {
                // sadly we can not do this whit the media engine
                // since we have 2 signals and they are also connected
                // to the Internet icon and if the connection where to fail it would
                // create a popup this would be bad if we give the message already
                // out here.
                // we could avoid that by disconnecting the other functions temporary
                // but i think it's better to have the same behavior as with all the other
                // connections so we just in the end we just return to the general settings menu
                // and inform the user with a popup and a text line
                // so i uncommented the following line
                //Platform.network.set_networkStateChanged_callback(updateNetworkState);
                
                /*
                var networkConfig = {
                    "interface" : wifiInterface,
                    "dhcp" : true,
                    "wps" : false,
                    "ssid" : selected_hotspot.ssid,
                    "wifiKey" : password,
                    "security": selected_hotspot.security
                };

                Platform.network.activateWifiInterface(networkConfig, debugCallback);
                Platform.network.activateWifiInterface(networkConfig, messageCallback);
                */
                
                window.Platform.network.setConnectionDetails_type( window.Platform.network.WIRELESS );
                window.Platform.network.setConnectionDetails_wps( false );
                window.Platform.network.setConnectionDetails_ssid( selected_hotspot.ssid );
                window.Platform.network.setConnectionDetails_wifiKey( password );
                window.Platform.network.setConnectionDetails_security( selected_hotspot.security );
    
                window.AppManager.back();
                window.AppManager.back();
                window.AppManager.enter("General Settings");
                
                var ret = window.Platform.network.auto_apply_settings();
                window.Platform.network.standard_error_reaction( ret );
                if ( ret < 0) {
                    //reset settings
                    window.AppManager.getScene("General Settings").reload_nettwork_settings();
                }
            }
        } ;
        
        // this function is not used see comment in line  //Platform.network.set_networkStateChanged_callback(updateNetworkState);
        var updateNetworkState = function() {
            var networkState = 0;
            //networkState = _StbNetworkObj.interfaceStatus(wifiInterface);
            
            switch (networkState) {
             case 0:
                 console.log("WiFi network state: Interface_Init");
                 $keyinput_message.text(TR('wifi settings screen :: Connecting'));
                 break;
             case 1:
                 console.log("WiFi network state: Interface_Up");
                 window.AppManager.back();
                 break;
             case 2:
                 console.log("WiFi network state: Interface_Down");
                 break;
             case 3:
                 console.log("WiFi network state: Interface_Error");
                 $keyinput_message.text(TR('wifi settings screen :: Invalid Password'));
                 break;
            }
        } ;
     
        var debugCallback = function(error, return_val) {
            if(return_val == false) {
                console.log("fault applying to wifi config!");
            }
            else {
                console.log("successful applying to wifi config!");
            }
        } ;
	
        self.onKey = function() {
            return true;
        };
        
        self.onShow = function() {
            updateScreen();
        };

        self.onKeyNUMBER = function(num) {
            if (focusList.getSelectedName() == 'keyinput') {
                input.appendChar(num.toString());
            }
        };

        self.onKeyUP = function() {
            if (focusList.getSelectedName() == 'keyinput') {
                input.setFocus(false);
//                input.clear();
            }
            else if (focusList.getSelectedName() == 'confirm_btn') {
                input.setFocus(true);
                input.clear();
            }
            $keyinput_message.text("");
            focusList.selectPrev();
        };

        self.onKeyDOWN = function() {
            if (focusList.getSelectedName() == 'keyinput') {
                input.setFocus(false);
//                input.clear();
            }
            else if (focusList.getSelectedName() == 'confirm_btn') {
                input.setFocus(true);
                input.clear();
            }
            $keyinput_message.text("");
            focusList.selectNext();
        };

        self.onKeyOK = function() {
            var focusListname = focusList.getSelectedName();       
            
            if (focusListname == 'keyinput') {
                input.setFocus(true);
            }
            else if(focusListname == 'confirm_btn') {
                input.setFocus(false);
                var wifi_ps = input.getText();
                connectWifi(wifi_ps);		           	
            }
        };
        
        self.onKeyRED = function() {
            input.removeChar();
        };
        
        self.onKeyLEFT =
        self.onKeyBACK = 
        self.onKeyEXIT = function () {
            input.setFocus(false);
            window.AppManager.back();
        };
        
        return self;
    };

    window.AppManager.addScene(name, KeyInputPopDlg);
})();