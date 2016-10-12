"use strict";
(function( ){

		var name = "System Info";
		
		
		
		var SystemInfos = function(){
			var parent = new window.MainMenuScreen( name );
			var self = Object.create(parent);
			self.$root.attr('id','system_info');
			
			var $system_info_table = $('<div>').addClass('system_info_table').appendTo(self.$root);
			
			var $system_info_caption = $('<div>').addClass('caption').append($TR('system_info_caption :: caption')).appendTo($system_info_table);
			
			var $system_info_rows = $('<div>').addClass('system_info_rows').appendTo($system_info_table);					
			
			var $system_info_second_lable = $('<div>').addClass('second_lable').appendTo($system_info_rows);
			var $system_info_smartcardnumbername = $('<div>').addClass('column_left').append($TR('system_info_smartcardnumbername :: smartcardnumbername')).appendTo($system_info_second_lable);
			var $system_info_smartcardnumber = $('<div>').addClass('column_right').appendTo($system_info_second_lable);
			
			var $system_info_third_lable = $('<div>').addClass('lable').appendTo($system_info_rows);
			var $system_info_macaddressname = $('<div>').addClass('column_left').append($TR('system_info_macaddressname :: macaddressname')).appendTo($system_info_third_lable);
			var $system_info_macaddress =$('<div>').addClass('column_right').appendTo($system_info_third_lable);
			
			var $system_info_fifth_lable = $('<div>').addClass('lable').appendTo($system_info_rows);
			var $system_info_stbchipidname = $('<div>').addClass('column_left').append($TR('system_info_stbchipidname :: stbchipidname')).appendTo($system_info_fifth_lable);
			var $system_info_stbchipid = $('<div>').addClass('column_right').appendTo($system_info_fifth_lable);

			var $system_info_sixth_lable = $('<div>').addClass('lable').appendTo($system_info_rows);
			var $system_info_hardwareversionname = $('<div>').addClass('column_left').append($TR('system_info_hardwareversionname :: hardwareversionname')).appendTo($system_info_sixth_lable);
			var $system_info_hardwareversion = $('<div>').addClass('column_right').appendTo($system_info_sixth_lable);
			
			var $system_info_seventh_lable = $('<div>').addClass('lable').appendTo($system_info_rows);
			var $system_info_softwareversionname = $('<div>').addClass('column_left').append($TR('system_info_softwareversionname :: softwareversionname')).appendTo($system_info_seventh_lable);
			var $system_info_softwareversion = $('<div>').addClass('column_right').appendTo($system_info_seventh_lable);
			
			var $system_info_eighth_lable = $('<div>').addClass('lable').appendTo($system_info_rows);
			var $system_info_bootloaderversionname = $('<div>').addClass('column_left').append($TR('system_info_bootloaderversionname :: bootloaderversionname')).appendTo($system_info_eighth_lable);
			var $system_info_bootloaderversion = $('<div>').addClass('column_right').appendTo($system_info_eighth_lable);
			
			var $system_info_ninth_lable = $('<div>').addClass('lable').appendTo($system_info_rows);
			var $system_info_ipaddressname = $('<div>').addClass('column_left').append($TR('system_info_ipaddressname :: ipaddressname')).appendTo($system_info_ninth_lable);
			var $system_info_ipaddress =$('<div>').addClass('column_right').appendTo($system_info_ninth_lable);
			
			var $system_info_tenth_lable = $('<div>').addClass('lastlable').appendTo($system_info_rows);
			var $system_info_ca_versionname = $('<div>').addClass('column_left').append($TR('system_info_ca_versionname :: ca_versionname')).appendTo($system_info_tenth_lable);
			var $system_info_ca_version = $('<div>').addClass('column_right').appendTo($system_info_tenth_lable);
			
			var updateScreen = function( ){
				var smcSN= Platform.ca.GetCardSN();	
				var stbVer = Platform.ca.GetSTBVer();
				var stbid = Platform.ca.GetSTBID();
				var caver = Platform.ca.GetVer();
				var net = Platform.network.getActiveInterface();
				
				$system_info_smartcardnumber.text(smcSN.retVal ? "":smcSN.cardSN);											
				$system_info_macaddress.text(net.mac);
				$system_info_ipaddress.text(net.ip_address);					
				$system_info_stbchipid.text(stbid.retVal ? "" :stbid.stbID);
				$system_info_hardwareversion.text(stbVer.retVal ? "" :stbVer.hwVer);
				$system_info_softwareversion.text(stbVer.retVal ? "" :stbVer.swVer);
				$system_info_bootloaderversion.text(stbVer.retVal ? "" :stbVer.ldrVer);
				$system_info_ca_version.text(caver.retVal? "" :"CA:0x" + caver.caVer.toString(16).toUpperCase());
																												
			};
			
			/*
			var $btFactoryReset = $('<div>').addClass('btFactoryReset').appendTo(self.$root);
			var btFactoryReset_description = {
				name: 'system_info :: factoryreset',
				callback: null
			};        
			var btFactoryReset_item = new window.ButtonView(btFactoryReset_description);
			btFactoryReset_item.$root.appendTo($btFactoryReset);
			btFactoryReset_item.show();
			btFactoryReset_item.$root.toggleClass( 'selected', true ) ;
			*/
	
			self.onKey = function( ) {
				return true ;
			} ;
				
			self.onShow = function( ) {
				updateScreen();
			} ;

			var syncFactoryResetStatus = null;
			self.onKeyOK = function( ) {
				/*
				var callbackOk = function ( ) {
					window.Platform.system.factoryResetStart(window.Platform.system.MediaEngineResetType["MEDIA_ENGINE_RESET_TYPE_NAND"]);	
					function intervalFun() {
						var status = window.Platform.system.getFactoryResetStatus();
						if ( status == window.Platform.system.MediaEngineResetStatus["MEDIA_ENGINE_RESET_STATUS_FINISHED"] )
						{
							window.Platform.system.rebootSTB();
							clearInterval(syncFactoryResetStatus);
							syncFactoryResetStatus = null;
						}
					}
					if ( syncFactoryResetStatus == null ) {
						syncFactoryResetStatus = setInterval(intervalFun, 1000);     
					}  																																			
				}
				var callbackCancel = function ( ) {
					window.AppManager.back() ;					
				}
				var string = "Are you sure to do Factory Reset?";
				var selectPopDlg = window.AppManager.getWidget("Select PopDlg");
				selectPopDlg.registerCallback( callbackOk,callbackCancel,string );
				window.AppManager.enter( "Select PopDlg" ) ;
				*/
			} ;
			
			self.judgeLANNotConnect = function( ) {
				$system_info_ipaddress.text("");	
			};
			self.judgeLANConnect = function( ) {
				var net = Platform.network.getActiveInterface();
				$system_info_ipaddress.text(net.ip_address);	
			};
							
			return self ;
		};
		
			window.AppManager.addScene( name,SystemInfos );

})();