"use strict";
(function( ) {

	var name = "Live TV" ;


	var LiveTV = function( ) {

		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'live_tv' ) ;

		var info_bar       = window.AppManager.getWidget( "Info Bar" ) ;
		var nstv_ca        = window.AppManager.getWidget( "NSTV CA" ) ;
		var ippv           = window.AppManager.getWidget( "ippv Bar" ) ;
		var volume_bar     = window.AppManager.getWidget( 'Volume Bar' ) ;
		var channel_number = window.AppManager.getWidget( 'Channel Number' ) ;
		var ok_bar         = window.AppManager.getWidget( 'Ok Bar') ;
		var timeshift_bar  = window.AppManager.getWidget( "Timeshift Bar" ) ;
		var timeshift_icon = window.AppManager.getWidget( "Timeshift Icon" ) ;

		var tuner_status = new window.tunerStatus() ;
		tuner_status.$root.appendTo( self.$root ) ;
		tuner_status.show();
		
		var audioTrackDlg = new window.AudioTrack() ;
		audioTrackDlg.$root.appendTo('#interface') ;

		var showInfoBar = function( autohide ) {
			info_bar.setAutohide( autohide ) ;
			if(info_bar.isActive( )){
				info_bar.start();
			}
			else{
				if( !self.isActive( ) ) { return ; }
				nstv_ca.setKeepFlag(true);
				window.AppManager.enter( info_bar.name ) ;
			}
		} ;

		var showTimeshiftBar = function( autohide ) {
			if(info_bar.isActive( )){
				info_bar.hide();
				window.AppManager.back();
			}else{
				if( !self.isActive( )) {return ;}

			}
			timeshift_bar.setAutohide( autohide ) ;
			window.AppManager.enter( timeshift_bar.name ) ;
		} ;

		var startTimeshift = function (){
			var ret = MediaEngineTimeshift.startTimeshift() ;
			if(0 === ret){
			    timeshift_icon.show();
			    showTimeshiftBar( false ) ;
			    console.log( "-> startTimeshift(): success") ;
			}else if(1 === ret){
				nstv_ca.showPopMsg("reminderPopDig :: No disc found");
			} else{
			    switch (ret) {
			    case 2:
				console.log("-> ERROR: Not defined") ;
				break;
				console.log( "-> startTimeshift(): fail")
			    }
			}
		}

		var toggleTimeshift = function (){
			if(timeshift_bar.isActive()){
				timeshift_bar.hide();
				window.AppManager.back( ) ;
			}
			showTimeshiftBar( true ) ;
			MediaEngineTimeshift.resumePauseTimeshift() ;
			console.log( "-> toggleTimeshift()") ;
		}

		var stopTimeshift = function (){
			if(MediaEngineTimeshift.stopTimeshift()){
				if(timeshift_bar.isActive()){
					timeshift_bar.hide();
					window.AppManager.back( ) ;
				}
			    timeshift_icon.hide();
			    console.log( "-> stopTimeshift() = true") ;
			    showInfoBar( true ) ;
			} else {
			    console.log( "-> stopTimeshift() = false") ;
			}
		}


		var showOkBar = function( autohide ) {
			nstv_ca.setKeepFlag(true);
			window.AppManager.enter( ok_bar.name ) ;
		};

		var setCurrent = function( mrl ) {
			if ( _.isUndefined( mrl ) ) {
				console.log ("[setCurrent]: mrl == Underfined!!!");
				return ;
			}

			var index = Platform.dvb.ChannelsMrl.indexOf( mrl ) ;
			Platform.dvb.ChannelCurrent = {
				mrl: mrl,
				index: index,
			} ;

			showInfoBar( true ) ;
		} ;

		var showCurrent = function( ) {
			channel_number.showNumber( ) ;
		} ;

		var currentChannelLockStatus = function(){
			var currentChannelInfo = Platform.dvb.getCurrentChannel();
			return currentChannelInfo.locked;
		};

		var nextChannelLockStatus = function(mrl){
			var nextChannelInfo = window.Platform.dvb.getChannelInfo(mrl) ;
			return nextChannelInfo.locked;
		};

		var isLockedChannel = function(mrl){
			if ( _.isUndefined( mrl ) ) {
				console.log ("[isLockedChannel]: mrl == Underfined!!!");
				return false;
			}		
		
			if(!nextChannelLockStatus(mrl)){
				return false;//not locked channel,unlock
			}
			else{
				if(!currentChannelLockStatus()){
					return true;//is lock channel,and last channel is not locked channel,lock
				}else{
					var state = Platform.media.getPlaybackState( );
					if( (!state) || (state == -4)){
						return true;
					}
					else{
						return false; //is lock channel,but last channel is locked channel,unlock
					}
				}
			}
		};

		var currentChannelIsAdult = function(){
			var currentChannelInfo = Platform.dvb.getCurrentChannel();
			if(currentChannelInfo.sid >= 80 && currentChannelInfo.sid <= 92){
				return true;
			}else{
				return false;
			}
		};

		var nextChannelIsAdult = function(mrl){
			var nextChannelInfo = window.Platform.dvb.getChannelInfo(mrl);
			if(nextChannelInfo.sid >= 80 && nextChannelInfo.sid <= 92){
				return true;
			}else{
				return false;
			}
		};		

		var isAdultChannel = function(mrl){
			if(window.Template.EnableAdultChannels == true){
				if ( _.isUndefined( mrl ) ) {
					console.log ("[isAdultChannel]: mrl == Underfined!!!");
					return false;
				}			
			
				if(!nextChannelIsAdult(mrl)){
					return false;//not adult channel,unlock
				}
				else{
					if(!currentChannelIsAdult()){
						return true;//is adult channel and last channel is not adult channel,lock
					}
					else{
						var state = Platform.media.getPlaybackState( );
						if( (!state) || (state == -4)){
							return true;
						}
						else{
							return false;//is adult channel and last channel is  adult channel,unlock
						}
					}
				}
			}else{
				return false;
			}
		};
		
		var vod_pop_Dlg = new window.popDlg( );
		vod_pop_Dlg.$root.appendTo( self.$root ) ;
		vod_pop_Dlg.hide();
		
		var showKeyInputDlg = function(mrl , flag){
			console.log("mrl: = "+mrl);
			
			window.Platform.media.stop( ) ;
			self.enableNotTunedChannel( mrl ) ;
			var playCurrentChannel = function() {
				playChannelWithoutJudgement( mrl );
			}
			var saveCurrentChannel = function() {
				setCurrent( mrl );
			}
			
			var callbackCancel = function(){
				var state = Platform.media.getPlaybackState( );
				if( state == 0 && flag == 1){
					var str = "pincode :: vod warning" ;
					vod_pop_Dlg.show();
					vod_pop_Dlg.showDlgMsg( str );
					setTimeout(function( ) { vod_pop_Dlg.hide(); },4000);
				}
			}
			
			info_bar.goBack();		
			channel_number.hide( ) ;
			
			var index = Platform.dvb.ChannelsMrl.indexOf( mrl ) ;
			var keyinputLcn = Platform.dvb.ChannelsLcn[index];
			
			var callback = {
				callbackOK: playCurrentChannel,
				callbackCancel: callbackCancel,
				saveCurrentChannel: saveCurrentChannel,
				flag: flag,
				keyinputLcn: keyinputLcn,
			};
			
			var keyinputDlg = window.AppManager.getWidget("KeyInput PopDlg");
			keyinputDlg.registerCallback(callback);
			window.AppManager.enter( "KeyInput PopDlg" ) ;
		}

		var selectAudioTrack = function ( mrl ) {
			//get current Audio Track
			var loadSetting = Platform.storage.load("defaultsettings",'{\"default\":{\"audio\":{\"defaulttrack\":\"0\"}}}');
			var defaultAudioSetting = JSON.parse(loadSetting);
			var currentAudioTrack = defaultAudioSetting.default.audio.defaulttrack;

			var loadStr = Platform.storage.load("AudioTrack",'{\"mrl\":{\"audio\":{\"selected\":\"0\"}}}');
			var defaultAudioTrackStr = JSON.parse(loadStr);
			
			if ( _.isUndefined( defaultAudioTrackStr[mrl] ) ) {
				var audioTrackIndex = currentAudioTrack;
				/*defaultAudioTrackStr[mrl] = {
					audio : {
      					 selected:audioTrackIndex,
					}
				}
				var audioTrackData = JSON.stringify(defaultAudioTrackStr);
				Platform.storage.store("AudioTrack",audioTrackData);*/
			}
			else {
				var audioTrackIndex = defaultAudioTrackStr[mrl].audio.selected;
			}
			console.log("audioTrackIndex = "+audioTrackIndex);
			if ( window.Platform.settings.getAudioTrack() == -1 ) {
				setTimeout(function(){
					window.Platform.settings.setAudioTrack(audioTrackIndex);
					info_bar.setAudioTrackText(audioTrackIndex);
				}, 2000);
			}
			else {
				window.Platform.settings.setAudioTrack(audioTrackIndex);
				info_bar.setAudioTrackText(audioTrackIndex);						
			}

		}
		var playChannelWithoutJudgement = function( mrl ){
			self.disableNotTunedChannel();
			Platform.media.play( mrl, function( error ) {
				if( error ) { throw new Error( error.message ) ; }
				vod_pop_Dlg.hide();
				showCurrent( ) ;
				setCurrent( mrl ) ;
				selectAudioTrack( mrl ) ;
				console.log("current scene = "+ window.AppManager.current( ).name);
				if(window.AppManager.current( ).name == "Programme Guide"){
					var epg = window.AppManager.getWidget( "Programme Guide" );
					epg.updateScreenPosition(); 
				}
			} ) ;
		}
		
		var playChannel = function( mrl ) {
			console.log ("[playChannel]: mrl=" + mrl);
			info_bar.setPreviewText (" ") ;
			var epg = window.AppManager.getWidget( "Programme Guide" ) ; 
			epg.setPreviewText (" ") ;
			if(isLockedChannel(mrl)|| isAdultChannel(mrl)){
				var flag = 0;
				if( window.AppManager.current( ).name == "Ok Bar" ){
					flag = 3;
				}
				else if(window.AppManager.current( ).name == "Programme Guide" ){
					flag = 0;
				}
				else{
					flag = 1;
				}
				showKeyInputDlg(mrl, flag);
			}
			else{
				playChannelWithoutJudgement( mrl );
			}
		} ;

		var start = function( ) {
			if(window.Template.enhance["Enable"] == true){			
				Platform.settings.setVideoEnhance(Platform.settings.VideoEnhance["VIDEO_ENHANCE_BRIGHTNESS"], window.Template.enhance["Settings"][0]);
				Platform.settings.setVideoEnhance(Platform.settings.VideoEnhance["VIDEO_ENHANCE_CONTRAST"],   window.Template.enhance["Settings"][1]);
				Platform.settings.setVideoEnhance(Platform.settings.VideoEnhance["VIDEO_ENHANCE_SATURATION"], window.Template.enhance["Settings"][2]);
				Platform.settings.setVideoEnhance(Platform.settings.VideoEnhance["VIDEO_ENHANCE_SHARPNESS"],  window.Template.enhance["Settings"][3]);
				Platform.settings.setVideoEnhance(Platform.settings.VideoEnhance["VIDEO_HUE"],                window.Template.enhance["Settings"][4]);
			}			
			
			window.Platform.time.syncDVBTime();
			Platform.dvb.Lists = Platform.dvb.getLists();
			if(window.Template.EnableMaster == false){
				//Search the mainList
				for(var i = 0; i < Platform.dvb.Lists.length; i++){
					if( Platform.dvb.Lists[i].name == window.Template.mainListName[0] || Platform.dvb.Lists[i].name == window.Template.mainListName[1]) {
						Platform.okList.mainListId = Platform.dvb.Lists[i].id;
						break;
					}
				}

				//Set the currentList to mainList
				//if ( Platform.dvb.getCurrentList() == 0 ) {
					Platform.dvb.setCurrentList( Platform.okList.getMainListID() );
				//}

				//Update the lists
				for(var i = 0; i < Platform.dvb.Lists.length; i++){
					if(Platform.dvb.Lists[i].id == 0 ){
						Platform.dvb.Lists.splice(i, 1);
						break;
					}
				}

			}

			Platform.dvb.CurrentList = Platform.dvb.getCurrentList();
			Platform.dvb.getChannelsMrl(Platform.dvb.CurrentList, 0, window.Template.ChannelMaxLimit);

			Platform.dvb.getCurrentChannel( function( error, channel ) {
				if( error ) { throw new Error( error.message ) ; }
				playChannel( channel.mrl ) ;
			} ) ;
		} ;

		var stop = function( ) {
			Platform.media.stop( function( error ) {
				if( error ) { throw new Error( error.message ) ; }
			} ) ;
		} ;

		var selectChannelByLcn = function( lcn ) {
			var index = Platform.dvb.ChannelsLcn.indexOf( lcn ) ;
			if (index == -1) {
				if( Platform.dvb.CurrentList >= 8000 && Platform.dvb.CurrentList != Platform.okList.getMainListID()){
					var mainListID = 0;
					if(window.Template.EnableMaster == false){
						mainListID = Platform.okList.getMainListID();
					}
					Platform.dvb.setCurrentList( mainListID );
					Platform.dvb.getChannelsMrl(mainListID, 0, window.Template.ChannelMaxLimit);
					var index = Platform.dvb.ChannelsLcn.indexOf( lcn ) ;
					var mrl = Platform.dvb.ChannelsMrl[index] ;
					playChannel( mrl ) ;
				}
			} 
			else {
				var mrl = Platform.dvb.ChannelsMrl[index] ;
				playChannel( mrl ) ;

				if(window.AppManager.isInStack("Programme Guide")) {
					console.log("-> epg window is already opened!");
					var epg = window.AppManager.getWidget("Programme Guide");
					epg.updateCurrentChannel(index);
				}
			}
		} ;

		var selectChannelByIndex = function( index ) {
			var mrl = Platform.dvb.ChannelsMrl[index] ;
			playChannel( mrl ) ;
		} ;

		var nextChannel = function( ) {
			if( !Platform.dvb.ChannelCurrent ) { return ; }
			var index = Platform.dvb.ChannelCurrent.index + 1;
			if (index >= Platform.dvb.ChannelsMrl.length ) {
				index = 0;
			};
			selectChannelByIndex( index ) ;
		} ;

		var prevChannel = function( ) {
			if( !Platform.dvb.ChannelCurrent ) { return ; }

			var index = Platform.dvb.ChannelCurrent.index - 1 ;
			if (index < 0) {
				index = Platform.dvb.ChannelsMrl.length - 1 ;
			};
			selectChannelByIndex( index ) ;
		} ;
		
		var timeshiftWarning_pop_Dlg = new window.popDlg( );
				timeshiftWarning_pop_Dlg.$root.appendTo( self.$root ) ;
				timeshiftWarning_pop_Dlg.hide();

		channel_number.onChange( selectChannelByLcn ) ;
		
		var showNotTunedChannel = false;
		var notTunedChannelMrl = 0;

		self.onExit =
		self.onLeave = function( ) {
			if (!nstv_ca.getKeepFlag()) {
				nstv_ca.hide();

				//Check the ippv status
				ippv.ippvControl(0);
			} else {
				nstv_ca.setKeepFlag(false);
			}
		} ;

		self.onReturn =
		self.onEnter = function( ) {
			if (!nstv_ca.isVisible()) {
				nstv_ca.show();

				//Check the ippv status
				nstv_ca.setKeepFlag(true);
				ippv.ippvControl(1);
				nstv_ca.setKeepFlag(false);
			}
		} ;

		self.onShow = function( ) {
			Platform.settings.firstTimeRun(start);
		} ;

		self.activate = function( ) {
			if(window.Template.EnableMaster == false){
				//Need to update BAT lists data
				Platform.dvb.updateServices();
			}

			var state = Platform.media.getPlaybackState( );
			if( !state || (state == -4)){
				//Platform.dvb.getCurrentChannel( function( error, channel ) {
				//if( error ) { throw new Error( error.message ) ; }
				var channel = self.judgeIfTunedChannel();
				playChannel( channel.mrl ) ;
				//} ) ;
			}
		};

		self.deactivate = function( ) {
			// stop( ) ;
		} ;

		self.onKeyUP = function( ) {
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			} else {
			      nextChannel( ) ;
			}
		} ;

		self.onKeyDOWN = function( ) {
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			} else {
			      prevChannel( ) ;
			}
		} ;

		self.onKeyCHANUP = function( ) {
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			} else {
			      nextChannel( ) ;
			}
		} ;

		self.onKeyCHANDOWN = function( ) {
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			} else {
			      prevChannel( ) ;
			}
		} ;

		self.onKeyVOLUP = function( ) {
			volume_bar.up( ) ;
		} ;

		self.onKeyVOLDOWN = function( ) {
			volume_bar.down( ) ;
		} ;
		self.onKeyMAIL = function(){
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			      return true ;
			} else {
			      window.App.goToMainMenu();
			      var menu = window.AppManager.getWidget( 'Main Menu') ;
			      menu.focusMail();
			      window.AppManager.enter( "Email" );
			      return true ;
			}
		} ;
		self.onKeyVOD = function(){
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			      return true ;
			} else {
			      window.App.goToVod();
			      return true ;
			}
		};
		self.onKeyMUTE = function( ) {
			volume_bar.toggleMute( ) ;
		} ;

		self.onKeyNUMBER = function( number ) {
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			} else {
			      channel_number.pushDigit( number ) ;
			}
		} ;

		self.onKeyINFO = function( ) {
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			      return true ;
			}
			if( channel_number.isWriting( ) ) {
				channel_number.flush( ) ;
			}
			else{
				showCurrent( ) ;
				showInfoBar( true ) ;
			}
			return true ;
		} ;

		self.onKeyOK = function( ) {
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			} else {
			      if( channel_number.isWriting( ) ) {
				      channel_number.flush( ) ;
			      }
			      else{
				      showOkBar( false );
			      }
			}
			return true ;
		} ;

		self.onKeyPAUSE = function( ) {
			console.log( "-> live_tv - onKeyPAUSE");
			if(MediaEngineTimeshift.doingTimeshift){
			  toggleTimeshift();
			}
			else {
			  startTimeshift();
			}
			return true ;
		} ;

		self.onKeyPLAY = function( ) {
			console.log( "-> live_tv - onKeyPLAY");
			if(MediaEngineTimeshift.doingTimeshift){
			  toggleTimeshift();
			}
			return true ;
		} ;

		self.onKeySTOP = function( ) {
			console.log( "-> live_tv - onKeySTOP");
			if(MediaEngineTimeshift.doingTimeshift){
			  stopTimeshift();
			}
			else {
			  console.log( "-> No timeshift ongoing");
			}
			return true ;
		} ;

		self.onKeyFASTREWIND = function( ) {
			console.log( "-> live_tv - onKeyFASTREWIND");
			if(Math.abs(MediaEngineTimeshift.timeshiftSpeed/100) < 8){
				MediaEngineTimeshift.fastRewindTimeshift();
				showTimeshiftBar( false ) ;
				return true ;
			}
		} ;

		self.onKeyFASTFORWARD = function( ) {
			console.log( "-> live_tv - onKeyFASTFORWARD");
			if(Math.abs(MediaEngineTimeshift.timeshiftSpeed/100) < 8){
				MediaEngineTimeshift.fastForwardTimeshift();
				showTimeshiftBar( false ) ;
				return true ;
			}
		} ;

		self.onKeyRETURN = function( ) {
			console.log( "-> live_tv - onKeyRETURN");
			location.reload(true);
			return true ;
		} ;

		/*self.onKeyREC = function( ) {
			console.log( "-> live_tv - onKeyREC");
			showTimeshiftBar( false ) ;
			return true ;
		} ;*/

		self.onKeyEPG = function( ) {
			window.App.goToEPG( ) ;
			return true ;
		} ;

		self.onKeyRIGHT = function( ) {
			window.App.goToMainMenu();
			console.log("window.AppManager.current( ).name = "+ window.AppManager.current( ).name);
			return true ;
		} ;
		self.onKeyLEFT = function( ) {
			console.log("window.AppManager.current( ).name = "+ window.AppManager.current( ).name);
			window.App.goToHome( ) ;
			return true ;
		} ;
		self.onKeyAUDIO = function(){
			if( !Platform.dvb.ChannelCurrent ) { return ; }

			// get current channel mrl
			var mrl = Platform.dvb.ChannelsMrl[Platform.dvb.ChannelCurrent.index] ;

			var loadStr = Platform.storage.load("AudioTrack",'{\"mrl\":{\"audio\":{\"selected\":\"0\"}}}');
			var defaultAudioTrackStr = JSON.parse(loadStr);
			if(_.isUndefined(defaultAudioTrackStr[mrl])){
				var defaultAudioTrackIndex = 0;
			}else{
				var defaultAudioTrackIndex = defaultAudioTrackStr[mrl].audio.selected;
			}
			var audioTracksInfo = Platform.settings.getAudioTracksInfo();
			audioTrackDlg.updateAudioTrack(audioTracksInfo);
			audioTrackDlg.showCurrentAudioTrack(defaultAudioTrackIndex);
			audioTrackDlg.changeAudioTrack(mrl);
			audioTrackDlg.show();
			setTimeout(function(){audioTrackDlg.hide()},5000);
		};

		self.playChannel = function( mrl ) {
			playChannel ( mrl ) ;
		} ;

		self.pushChannelNummber = function ( number ) {
		    channel_number.pushDigit( number ) ;
		} ;

		self.showOkBar = function( ) {
			showOkBar( ) ;
		} ;
		self.onKeyREMINDER = function(){
			if(MediaEngineTimeshift.doingTimeshift){
			      showTimeshiftBar( true ) ;
			      return true ;
			} else {
			      window.App.goToMainMenu();
			      var menu = window.AppManager.getWidget( 'Main Menu') ;
			      menu.focusReminder();
			      window.AppManager.enter( "Reminder Screen" );
			      return true ;
			}
		};
		self.onKeyFAV = function(){
			var favFlag = false;
			var getFavListId = function(){
				for(var i = 0; i < Platform.dvb.Lists.length; i++ ){
					if(Platform.dvb.Lists[i].id > 0 && Platform.dvb.Lists[i].id < 8000){
						favFlag = true;
						var id = Platform.dvb.Lists[i].id;
						return id ;
					}
				}
			}

			var get_FavListId = getFavListId( );
			if( favFlag == true ){
				Platform.dvb.setCurrentList( get_FavListId );
				showOkBar();
			}
			else{
				showOkBar();
				ok_bar.onKeyGREEN(  );
			}
		};

		self.judgeAdultChannel = function(){
			var currentChannelInfo = self.judgeIfTunedChannel();
			console.log("-> live_tv - currentChannelInfo.mrl="+currentChannelInfo.mrl);
			if(currentChannelInfo.sid <= 92 && currentChannelInfo.sid >= 80 || currentChannelInfo.locked == true ){
				Platform.media.stop( function( error ) {
					if( error ) { throw new Error( error.message ) ; }
				} ) ;
			}
		};
		
		self.judgeIsTimeShift = function(){
			if(MediaEngineTimeshift.doingTimeshift){
				var str = "popDlg :: is timeshift" ;
						timeshiftWarning_pop_Dlg.show();
						timeshiftWarning_pop_Dlg.showDlgMsg( str );
						
					setTimeout(function( ) { timeshiftWarning_pop_Dlg.hide(); },4000);
			}
		};
		
		self.enableNotTunedChannel = function ( mrl ) {
			showNotTunedChannel = true ;
			notTunedChannelMrl = mrl ;
		} ;

		self.disableNotTunedChannel = function ( ) {
			showNotTunedChannel = false ;
		} ;
		
		self.judgeIfTunedChannel = function( ){
			if ( showNotTunedChannel == false ) {
				return window.Platform.dvb.getCurrentChannel(  ) ;
			}
			else {
				return window.Platform.dvb.getChannelInfo( notTunedChannelMrl ) ;
			}
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, LiveTV ) ;

})( ) ;
