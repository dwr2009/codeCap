"use strict";
(function( ) {
	var name = "NSTV CA" ;

	var NstvCA = function( ) {
		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;
		self.$root.attr( 'id', 'NstvCa' ).appendTo( '#interface' ) ;

		var ota_sw_ver = 0;
		var keep_display = false;
		
		var fingerPrinter = new window.fingerPrinter();
		fingerPrinter.$root.appendTo('#interface');
		fingerPrinter.show();
		
		var curtainNotify = new window.curtainNotify();
		curtainNotify.$root.appendTo(self.$root);
		curtainNotify.show();
		
		var emailnotify = new window.emailNotifyIcon();
		emailnotify.$root.appendTo(self.$root);
		emailnotify.show();
		
		var showOSDMsg   = new window.showOSDMsg();
		showOSDMsg.$root.appendTo(self.$root);
		showOSDMsg.show();
				
		var actionMessage = new window.popDlg();
		actionMessage.$root.appendTo(self.$root);
		actionMessage.hide();
		
		var popMessage = new window.popDlg();
		popMessage.$root.appendTo(self.$root);
		popMessage.hide();
								
		var feedingMessage = new window.popDlg();
		feedingMessage.$root.addClass('feedingMsg').appendTo(self.$root);
		feedingMessage.hide();

		var buyMessage = new window.buyMsgpopDlg();
		buyMessage.$root.appendTo(self.$root);
		buyMessage.hide();
					
		var progressStrip = new window.progressStrip();
		progressStrip.$root.appendTo(self.$root);
		progressStrip.show();	
		
		var ippvBar = window.AppManager.getWidget("ippv Bar");
		var selectPopDlg = window.AppManager.getWidget("Select PopDlg");
					
		function InitCallback() {
    			window.Platform.ca.setNotifySmcStatusCallback(function(st){});
			window.Platform.ca.setShowProgressStripCallback(_.bind(progressStrip.ShowProgressStrip, progressStrip));
			window.Platform.ca.setEmailNotifyIconCallback(_.bind(emailnotify.EmailNotifyIcon,emailnotify));				
			window.Platform.ca.setShowOSDMessageCallback(showOSDMsg.ShowOSDMessage);
			window.Platform.ca.setHideOSDMessageCallback(showOSDMsg.HideOSDMessage);					
			window.Platform.ca.setShowFingerMessageExtCallback(_.bind(fingerPrinter.ShowFingerMessageExt,fingerPrinter));
			//window.Platform.ca.setShowCurtainNotifyCallback(_.bind(curtainNotify.ShowCurtainNotify,curtainNotify));	
			window.Platform.ca.setRequestFeedingCallback(function (param){
				console.log("RequestFeeding is function");			
				if (param.readStatus == 0) {
					feedingMessage.show();
					feedingMessage.RequestFeeding(TR('NSTV CA screen :: Invalid parent-card'));
					setTimeout(function(){feedingMessage.hide()},5000);
				}
				else {
					feedingMessage.show();
					feedingMessage.RequestFeeding(TR('NSTV CA screen :: Please insert the child-card'));
					setTimeout(function(){feedingMessage.hide()},5000);
				}      	
			});
			window.Platform.ca.setShowBuyMessageCallback(function (param) {
				console.log("param.messageType = "+ param.messageType);
				
				//Comment out DRM error
				if (param.messageType == 0x23 || param.messageType == 0x24 || param.messageType == 0x25) {
					return ;
				}
				
				if (param.messageType == 0) { 
					if (param.messageType == 0x15) { //CDCA_MESSAGE_CURTAIN_TYPE						    
						var epg = window.AppManager.getWidget( "Programme Guide" ) ;
						epg.setPreviewText ( "" ) ;
						var info_bar = window.AppManager.getWidget( "Info Bar" ) ;
						info_bar.setPreviewText ( "" ) ;				    		    
					}
				    								
					buyMessage.ShowBuyMessage(" ");  
					buyMessage.hide();
				}
				else {
					buyMessage.show();
					console.log("param.messageType = "+param.messageType );
					if (param.messageType == 0x30) { //CDCA_MESSAGE_UNSUPPORTDEVICE_TYPE									
						var ret = window.Platform.ca.getTerminalTypeID();
						var Msg = "NSTV CA screen :: buy_message_index_48" + ret.terminalTypeID; 
						console.log("ret =",+ ret);
						buyMessage.ShowBuyMessage(Msg);
					}
					else if (param.messageType == 0x15) { //CDCA_MESSAGE_CURTAIN_TYPE
						var Msg = "NSTV CA screen :: buy_message_index_"+param.messageType;
						console.log("NSTV CA screen :: buy_message_index_"+param.messageType);						    
						var epg = window.AppManager.getWidget( "Programme Guide" ) ;
						epg.setPreviewText (Msg) ;
						var info_bar = window.AppManager.getWidget( "Info Bar" ) ;
						info_bar.setPreviewText ( Msg ) ;				    
					    								
						buyMessage.ShowBuyMessage(Msg);				    
					}
					else {
						var Msg = "NSTV CA screen :: buy_message_index_"+param.messageType;									
						buyMessage.ShowBuyMessage(Msg);
					}
				}       		
			});
       	
			window.Platform.ca.setOTADetectedCallback(function(param) {		
				var dlgFlag = 0;
				var startOTA = function() {
					var ota_param = {
						upgradeType: 1,
						imageType: param.imageType,
						dsmccPID: param.dsmccPID,
						frequency: param.frequency,
						symbolRate: param.symbolRate,
						modulation: param.modulation
					}
					var ret = window.Platform.ca.SetLoaderParam(ota_param);
					if (ret.retVal == 0) {
						window.Platform.system.rebootSTB();
					}
					if (dlgFlag) {
						window.AppManager.back();
					}
				}
				var hideOTA = function() {
					window.AppManager.back();
				}
				var otaMsg = TR('NSTV CA screen :: New software version') + ' v' + param.swVer + ' ' + TR('NSTV CA screen :: is detected, Do you want to upgrade');
        		
				if (param.downloadType == 2) {
					startOTA();
				}
				else {
					if (ota_sw_ver != param.swVer) {
						dlgFlag = 1;
						ota_sw_ver = param.swVer;
						selectPopDlg.registerCallback(startOTA, hideOTA, otaMsg);
						self.setKeepFlag(true);
						window.AppManager.enter( "Select PopDlg" ) ;
						self.setKeepFlag(false);
					}
				}
			});	

       			window.Platform.ca.setStartIppvBuyDlgCallback(function(param) {
				self.setKeepFlag(true);
				ippvBar.StartIppvBuyDlg(param);
				self.setKeepFlag(false);
			});	
			window.Platform.ca.setHideIppvDlgCallback(function(param) {
				ippvBar.HideIPPVDlg();
			});
			window.Platform.ca.setLockServiceCallback(function ( param ) {
				console.log ("NSTV: Lock service!!!");
				if (param.onID && param.tsID && param.serviceID) {
					var mrl = "dvb://" + param.onID + "." + param.tsID + "." + param.serviceID;
					var loadStr = Platform.storage.load("AudioTrack",'{\"mrl\":{\"audio\":{\"selected\":\"0\"}}}');
					var defaultAudioTrackStr = JSON.parse(loadStr);
					defaultAudioTrackStr[mrl] = {
						audio : {
							selected:param.audioIndex,
						}
					}
					var audioTrackData = JSON.stringify(defaultAudioTrackStr);
					Platform.storage.store("AudioTrack",audioTrackData);
					window.AppManager.getWidget("Live TV").playChannel(mrl);
				}
				else {
					window.Platform.ca.PlayLockService();
				}   
				window.Platform.system.setUIFreezed();       
			});
        
			window.Platform.ca.setUNLockServiceCallback(function ( param ) {
				console.log ("NSTV: Unlock service!!!");
				window.Platform.system.setUIUnFreezed();       
			});  
             
			window.Platform.ca.setActionRequestExtCallback(function ( param ) {
				switch (param.actionType) {
				case 0:    //REBOOT
					console.log ("NSTV: Reboot STB!!!");
					actionMessage.show(); 				    
					actionMessage.showActionMessage(TR('NSTV CA screen :: reboot_message'));
					setTimeout(function(){
						window.Platform.ca.CloseCA();
						window.Platform.system.rebootSTB();
					}, 5000);
					break;
				case 1:    //FREEZE STB
					console.log ("NSTV: Freeze STB!!!");
					actionMessage.show(); 
					actionMessage.showActionMessage(TR('NSTV CA screen :: freeze_message'));
					break;
				case 2:    //SEARCH CHANNEL
					break;
				case 3:    //STB UPGRADE
					break;
				case 4:    //UNFREEZE STB
					console.log ("NSTV: Unfreeze STB!!!");
					actionMessage.hide();
					break;
				case 5:    //INITIALIZE STB
					break;
				case 6:    //SHOW SYSTEM INFO
					break;
				}                       
			});
        
			var param = {
				"drmHost": "61.18.0.102", //HKC productional server
				//"drmHost": "61.10.4.84", //HKC lab server
				//"drmHost": "218.240.129.205", //BeiJing server
				"drmURL" : "/catserver",
				"drmPort": 7070,
			};        
			window.Platform.ca.SetDRMHost(param);
			window.Platform.ca.InitCA();              
		}
		self.getKeepFlag = function() {
			return keep_display;
		}
    
		self.setKeepFlag = function(flag) {
			keep_display = flag;
		}
		
		self.onShow = function(){
			InitCallback();
		} ;
		
		self.showPopMsg = function(str){
			popMessage.show();
			popMessage.showDlgMsg(str);
			setTimeout(function(){popMessage.hide()},5000);
		};
		
        
		return self ;
	} ;

	window.AppManager.addScene( name, NstvCA ) ;

})( ) ;
