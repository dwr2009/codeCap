"use strict";
(function () {

    var name = "wifi_config";

    //Hotspots Delegate
    var Hotspot = new window.Class(
        //parent
        window.ListViewDelegate,

        //constructor
        function (hotspot) {
            ListViewDelegate.call(this, name);

            this.$root.addClass('hotspot_delegate');

            this.$hotspot_signal = $('<div>').addClass('column_one').appendTo(this.$root);
            this.getWifiSignal(hotspot.signal);
            
            var $hotspot_ssid = $('<div>').addClass('column_seventy').text(hotspot.ssid).appendTo(this.$root);
            
            var $hotspot_security = $('<div>').addClass('column_two').appendTo(this.$root);
            var security_icon = $( '<div>' ).addClass( 'security_icon' ).text(this.getWifiSecurityType(hotspot.security)).appendTo( $hotspot_security);

            this.hotspot = hotspot;
        },

        //prototype
        {
            getWifiSecurityType: function (wifiSec) {
                switch (wifiSec) {
                case 1:
                    return "WEP";
                    break;
                case 2:
                    return "WPA";
                    break;
                case 3:
                    return "WPA2";
                    break;
                case 4:
                    return "WPA_AES";
                    break;
                case 5:
                    return "WPA2_TKIP";
                    break;
                case 0:
                default:
                    return "";
                    //break;
                };
            },

            getWifiSignal: function (wifiSig) {
                if (wifiSig <= 25)
                    $('<div>').addClass('signal_icon_25').appendTo(this.$hotspot_signal);
                else if (wifiSig <= 50)
                    $('<div>').addClass('signal_icon_50').appendTo(this.$hotspot_signal);
                else if (wifiSig <= 75)
                    $('<div>').addClass('signal_icon_75').appendTo(this.$hotspot_signal);
                else if (wifiSig <= 100)
                    $('<div>').addClass('signal_icon_100').appendTo(this.$hotspot_signal);
            },

            getHotspot: function () {
                return this.hotspot;
            },
        }
    );

    var wifi_config = function () {

        var parent = new window.MainMenuScreen(name);
        var self = Object.create(parent);
        self.$root.attr('id', 'wifi_config');

        var $headline_text = $('<div>').addClass('headline_text').append($TR('wifi settings screen :: headline text')).appendTo(self.$root);

        var $wifi_list = $('<div>').addClass('wifi_list').appendTo(self.$root);

        var hotspots_list = new window.scrollListView(Hotspot);
        hotspots_list.$root.appendTo($wifi_list);

        var key_input_pop = window.AppManager.getScene("Wifi Key Input");

        var start = function () {
            Platform.network.set_onScanStateChanged_callback(updateHotspots);
            Platform.network.scanWifiStart();
        };

        var updateHotspots = function (state, hotspots) {
            switch (state) {
            case 0: // WIFI_SCAN_INIT
                //                 console.log("Wifi scan initialized....");
                break;
            case 1: // WIFI_SCAN_IN_PROGRESS
                //                 console.log("Wifi scan in progress....");
                break;
            case 2: // WIFI_SCAN_FINISHED
                console.log("Wifi scan finished!!");
                updateWifiList(hotspots);
                Platform.network.scanWifiStop();
                hotspots_list.show();
                break;
            case 3: // WIFI_SCAN_ERROR
            default:
                console.log("Wifi scan error!!");
                break;
            }
        };

        var updateWifiList = function (hotspots) {
            console.log("available Hotspots: \n" + JSON.stringify(hotspots));

            var getActivatedWifi = function (error, interfaceInfo) {
            	 var index = 0;
                if (hotspots.length > 0 && interfaceInfo.type == 2) {
                    var ssid_list = _.pluck(hotspots, 'ssid');
                    index = ssid_list.indexOf(interfaceInfo.ssid);
                    if ( index == -1 ) {
                    	index = 0;
                    }
                }
                else {
                    index = 0;
                }
                hotspots_list.updateItems(hotspots, index);
//                hotspots_list.pageUpAndDown(index);
            };

            window.Platform.network.getConnectionDetails(getActivatedWifi);
        };

        var stop = function () {
            hotspots_list.updateItems([], 0);
            Platform.network.unset_onScanStateChanged_callback(updateHotspots);
            //hotspots_list.init(0);
        };

/*
        var connectWifi = function () {
            window.Platform.network.setConnectionDetails_type( window.Platform.network.WIRELESS );
            window.Platform.network.setConnectionDetails_settings( window.Platform.network.DHCP );
            window.Platform.network.setConnectionDetails_wps( false );
            window.Platform.network.setConnectionDetails_ssid( hard_coded_ssid );
            window.Platform.network.setConnectionDetails_wifiKey( hard_coded_password );
            window.Platform.network.setConnectionDetails_security( hard_coded_security );

            window.Platform.network.auto_apply_settings();                

            //Platform.network.activateInterface(networkConfig);
        } ;
*/
        
        self.onShow = function () {
            start();
        };

        self.onHide = function () {
            stop();
        };

        self.onKey = function () {
            return true;
        };

        self.onKeyOK = function () {
            var selected_delegate = hotspots_list.getSelected();
            Platform.network.Selected_hotspot = selected_delegate.getHotspot();
            window.AppManager.enter(key_input_pop.name);
        };

        self.onKeyUP = function () {
            hotspots_list.selectPrev();
        };

        self.onKeyDOWN = function () {
            hotspots_list.selectNext();
        };

        self.onKeyBACK =
        self.onKeyLEFT = function () {
            window.AppManager.back();
            window.AppManager.enter("General Settings");
        };

        return self;
    };

    window.AppManager.addScene(name, wifi_config);
})();