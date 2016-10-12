"use strict"; (function() {

    var name = "Stb Information";

    var Stbinformations = function() {
        var parent = new window.MainMenuScreen(name);
        var self = Object.create(parent);
        self.$root.attr('id', 'stbinformation');
        
			var $stbinfo_caption = $('<div>').addClass('caption').append($TR('stbinfo_caption :: caption')).appendTo(self.$root);	
			
			var $stbinfo_first_lable = $('<div>').addClass('lable').appendTo(self.$root);
			var $stbinfo_stbidname = $('<div>').addClass('column_left').append($TR('stbinfo_stbidname :: stbidname')).appendTo($stbinfo_first_lable);
			var $stbinfo_stbid = $('<div>').addClass('column_right').appendTo($stbinfo_first_lable);
			
			var $stbinfo_second_lable = $('<div>').addClass('lable').appendTo(self.$root);
			var $stbinfo_hwversionname = $('<div>').addClass('column_left').append($TR('stbinfo_hwversionname :: hwversionname')).appendTo($stbinfo_second_lable);
			var $stbinfo_hwversion = $('<div>').addClass('column_right').appendTo($stbinfo_second_lable);
			
			var $stbinfo_third_lable = $('<div>').addClass('lable').appendTo(self.$root);
			var $stbinfo_swversionname = $('<div>').addClass('column_left').append($TR('stbinfo_swversionname :: swversionname')).appendTo($stbinfo_third_lable);
			var $stbinfo_swversion = $('<div>').addClass('column_right').appendTo($stbinfo_third_lable);
			
			var $stbinfo_forth_lable = $('<div>').addClass('lable').appendTo(self.$root);
			var $stbinfo_loaderversionname = $('<div>').addClass('column_left ').append($TR('stbinfo_loaderversionname :: loaderversionname')).appendTo($stbinfo_forth_lable);
			var $stbinfo_loaderversion = $('<div>').addClass('column_right ').appendTo($stbinfo_forth_lable);
			
			var $stbinfo_fifth_lable = $('<div>').addClass('lable').appendTo(self.$root);
			var $stbinfo_caversionname = $('<div>').addClass('column_left ').append($TR('stbinfo_caversionname :: caversionname')).appendTo($stbinfo_fifth_lable);
			var $stbinfo_caversion = $('<div>').addClass('column_right ').appendTo($stbinfo_fifth_lable);
			
			var $stbinfo_sixth_lable = $('<div>').addClass('lable').appendTo(self.$root);
			var $stbinfo_securebootname = $('<div>').addClass('column_left').append($TR('stbinfo_securebootname :: securebootname')).appendTo($stbinfo_sixth_lable);
			var $stbinfo_secureboot = $('<div>').addClass('column_right ').appendTo($stbinfo_sixth_lable);

			var $stbinfo_seventh_lable = $('<div>').addClass('lable').appendTo(self.$root);
			var $stbinfo_jtagprotectionname = $('<div>').addClass('column_left').append($TR('stbinfo_jtagprotectionname :: jtagprotectionname')).appendTo($stbinfo_seventh_lable);
			var $stbinfo_jtagprotection = $('<div>').addClass('column_right').appendTo($stbinfo_seventh_lable);
			
			var $stbinfo_eighth_lable = $('<div>').addClass('lable').appendTo(self.$root);
			var $stbinfo_marketidname = $('<div>').addClass('column_left').append($TR('stbinfo_marketidname :: marketidname')).appendTo($stbinfo_eighth_lable);
			var $stbinfo_marketid = $('<div>').addClass('column_right ').appendTo($stbinfo_eighth_lable);	
        
        	var stbInfoList = function() {

            var stbid = Platform.ca.GetSTBID();
            var stbver = Platform.ca.GetSTBVer();
            var caver = Platform.ca.GetVer();
            var security = Platform.ca.GetSTBSecurity();

            $stbinfo_stbid.text(stbid.retVal ? "": stbid.stbID);
            $stbinfo_hwversion.text(stbver.retVal ? "": stbver.hwVer);
            $stbinfo_swversion.text(stbver.retVal ? "": stbver.swVer);
            $stbinfo_loaderversion.text(stbver.retVal ? "": stbver.ldrVer/10);
            $stbinfo_caversion.text(caver.retVal ? "": ("CA:0x" + caver.caVer.toString(16).toUpperCase()));
            $stbinfo_secureboot.text(security.retVal ? "": (security.secureBoot ? TR('smart :: Enable') : TR('smart :: Disable')));
            $stbinfo_jtagprotection.text(security.retVal ? "": (security.jtagProtection ? TR('smart :: Enable') : TR('smart :: Disable')));
            $stbinfo_marketid.text(security.retVal ? "": security.marketID);
        };
        
        var nvmPIN = new Array();

        self.onKey = function() {
            return true;
        };

        self.onShow = function() {
            stbInfoList();
        }
        
        self.onKeyNUMBER = function(num){
						if(nvmPIN.length >= 6){
								nvmPIN.splice(0,1);
						}
						nvmPIN.push(num);
				}
				self.onKeyRED = function(){
						if(nvmPIN.toString() == "6,8,6,8,6,8"){	
								nvmPIN.splice(0,nvmPIN.length);													
	
								var callbackOk = function ( ) {
									Platform.ca.FormatBuffer();												
									window.AppManager.back() ;										
								}
								var callbackCancel = function ( ) {
									window.AppManager.back() ;
								}
								var string = "Format CA NVM?";
								var selectPopDlg = window.AppManager.getWidget("Select PopDlg");
								selectPopDlg.registerCallback( callbackOk, callbackCancel,string );
								window.AppManager.enter( "Select PopDlg" ) ;
						}				
				}

        return self;
    };

    window.AppManager.addScene(name, Stbinformations);
})();