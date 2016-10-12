"use strict";
(function( ) {

	var name = "General Settings" ;

	var GeneralSettings = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;

		var switchToLanguage = function( item ) {
			var code = item.value ;
			window.Translation.setLanguage( code ) ;
		} ;
		
		var switchToNetwork = function( item ) {
			
		} ;
		var switchToNetworkSettings = function( item ) {
		};
				
		var switchToAspectRatio = function( item ) {
			var ratioType = item.value;
			setType(ratioType);
			updateConversionMode(); 
			window.Platform.settings.setAspectRatio(item.value);
		} ;
				
		var switchToVideoFormat = function( item ) {
			window.Platform.settings.setConversionMode(item.value);
		} ;
						
		var switchToAudioOutput = function(item){
			console.log("item.value="+item.value);
			window.Platform.settings.setAudioOutput(item.value);
		} ;

		var switchToOpenSubtitle = function(item){
		};
			
		var switchToVideoResolution = function( item ) {
			window.Platform.settings.setResolution(item.value);
		} ;
		
		var switchToAudioPreference = function( item ) {
			window.Platform.settings.setAudioTrack(item.value);
			
			var defaultAudioTrackStr={
				default:{	
					audio : {
	  					 defaulttrack:item.value, 
					}
				}
			}; 
			var audioTrackData = JSON.stringify(defaultAudioTrackStr);							
			Platform.storage.store("defaultsettings",audioTrackData);			
		} ;
		
		var switchTimezone = function( item ) {
			var timezone = item.value ;
			if (window.Platform.time.getTimezone() !== timezone) {
				window.Platform.time.setTimezone(timezone);
			}
		} ;

		var description = [
			{
				name: "general settings screen :: language",
				items: [
					{ name: 'general settings screen :: english', value: 'en', },
					//{ name: 'general settings screen :: german',  value: 'de', },
					{ name: 'general settings screen :: chinese', value: 'ch', },
				],
				action: switchToLanguage,
			},
			{
				name: "general settings screen :: network",
				items: [
					{ name: 'general settings screen :: wifi', value: 'wifi', },
					{ name: 'general settings screen :: lan',  value: 'lan', },
					{ name: 'general settings screen :: none', value: 'none', },
				],
				action: switchToNetwork,
			},
			
			{
				name: "general settings screen :: network settings",
				items: [
					{ name: 'general settings screen :: dhcp', value: 'dhcp', },
					{ name: 'general settings screen :: manuel',  value: 'manuel', },
				],
				action: switchToNetworkSettings,
			},
			
			{
				name: "general settings screen :: aspectratio",
				items: [
					{ name: 'general settings screen :: AUTO', value: 'ASPECT_RATIO_AUTO', },
					{ name: 'general settings screen :: 16:9', value: 'ASPECT_RATIO_16_9', },
					{ name: 'general settings screen :: 4:3',  value: 'ASPECT_RATIO_4_3', },
				],
				action: switchToAspectRatio,
			},			
			{
				name: "general settings screen :: videoformat",
				items: [],
				action: switchToVideoFormat,
			},
			{
				name: "general settings screen :: audiooutput",
				items: [
					{ name: 'general settings screen :: stereo', value: 'AUDIO_STEREO', },
					{ name: 'general settings screen :: hdmi_spdif',  value: 'AUDIO_HDMI_SPDIF', },
					{ name: 'general settings screen :: spdif',  value: 'AUDIO_SPDIF', },
				],
				action: switchToAudioOutput,
			},
			{
				name: "general settings screen :: subtitle",
				items: [
					{ name: 'general settings screen :: off', value: 'off', },
					{ name: 'general settings screen :: on',  value: 'on', },
				],
				action: switchToOpenSubtitle,
			},		
			{
				name: "general settings screen :: videoresolution",
				items: [
					{ name: 'general settings screen :: 1080i', value: 'VIDEO_MODE_1080I50', },
					{ name: 'general settings screen :: 720p', value: 'VIDEO_MODE_720P50', },
					{ name: 'general settings screen :: 576i', value: 'VIDEO_MODE_576I50', },				
				],
				action: switchToVideoResolution,
			},
			{
				name: "general settings screen :: audiopreference",
				items: [
					{ name: 'general settings screen :: Audio1', value: '0', },
					{ name: 'general settings screen :: Audio2', value: '1', },
				],
				action: switchToAudioPreference,
			},			
			/*{
				name: "general settings screen :: timezone",
				items: [
					{ name: 'general settings screen :: Hongkong', value: media_engine_time.TimeZone_China, },
					{ name: 'general settings screen :: Berlin',  value: media_engine_time.TimeZone_Central_European, },
					{ name: 'general settings screen :: Greece', value: media_engine_time.TimeZone_Greece, },
					{ name: 'general settings screen :: UK', value: media_engine_time.TimeZone_United_Kingdom, },
				],
				action: switchTimezone,
			},*/

		] ;
		var getNewArray = function(array){
			var indexArray = window.Template.generalSetting;
			var newArray = [];
			for(var i = 0; i < array.length ;i++){
				if(indexArray.indexOf(array[i].name) >= 0){
					newArray.push(array[i]);
				}
			}
			return newArray;
		}

		var settings = new window.Settings( getNewArray(description) ) ;
		self.$root.attr('id','general_settings');
		settings.$root.appendTo( self.$root ) ;
		settings.show( ) ;
		var typeName = null;
		var initVideoFormat = function(){
			var RatioType = window.Platform.settings.getAspectRatio();
			if(RatioType == 1){
				typeName = 'ASPECT_RATIO_16_9';
			}
			else if(RatioType == 2){
				typeName = 'ASPECT_RATIO_4_3';
			}
			else{
				typeName = 'ASPECT_RATIO_AUTO';
			} 
		}
		var mode_auto = [{name: 'general settings screen :: auto',      value:'VIDEO_ZOOM_NORMAL',}];	
		var mode_16_9 =[{ name: 'general settings screen :: stretched',  value: 'VIDEO_ZOOM_FULL_SCREEN', },
						{ name: 'general settings screen :: pillarbox',  value: 'VIDEO_ZOOM_MAKE_WIDER', },] ;
		var mode_4_3 = [{ name: 'general settings screen :: stretched',  value:'VIDEO_ZOOM_FULL_SCREEN', },
						{ name: 'general settings screen :: pan_scan',  value: 'VIDEO_ZOOM_NON_LINEAR_STRETCH_TO_FULL_SCREEN', },
						{ name: 'general settings screen :: letterbox',  value: 'VIDEO_ZOOM_ENLARGE', },						];		
        
        var getType = function() {
            return typeName;
        }
        var setType = function(name) {
            typeName = name;
        }		
		
		var updateConversionMode = function(){
			if(null != settings.list_view.getItemByName("general settings screen :: videoformat")){			
				if(getType() == 'ASPECT_RATIO_16_9'){
					settings.list_view.getItemByName("general settings screen :: videoformat").select_view.updateItems(mode_16_9);
					settings.list_view.getItemByName("general settings screen :: videoformat").select_view.changeSelection(0);
				}
				else if(getType() == 'ASPECT_RATIO_4_3'){
					settings.list_view.getItemByName("general settings screen :: videoformat").select_view.updateItems(mode_4_3);
					settings.list_view.getItemByName("general settings screen :: videoformat").select_view.changeSelection(0);
				}
				else{
					settings.list_view.getItemByName("general settings screen :: videoformat").select_view.updateItems(mode_auto);
					settings.list_view.getItemByName("general settings screen :: videoformat").select_view.changeSelection(0);									
				}
				settings.list_view.getItemByName("general settings screen :: videoformat").select_view.show();
			}
		}
		var initConversionMode = function(){
			if(null != settings.list_view.getItemByName("general settings screen :: videoformat")){			
				if(getType() == 'ASPECT_RATIO_16_9'){
					settings.list_view.getItemByName("general settings screen :: videoformat").select_view.updateItems(mode_16_9);
				}
				else if(getType() == 'ASPECT_RATIO_4_3'){
					settings.list_view.getItemByName("general settings screen :: videoformat").select_view.updateItems(mode_4_3);					
				}
				else{
					settings.list_view.getItemByName("general settings screen :: videoformat").select_view.updateItems(mode_auto);
				}
				settings.list_view.getItemByName("general settings screen :: videoformat").select_view.show();
			}
		}
		var $password_lable = $('<div>').addClass('setting_lable').appendTo(self.$root);
		var $changepassword = $('<div>').addClass('panel').append($TR('general settings screen :: changepassword')).appendTo($password_lable);
    
		var $setting_connectingwifi = $('<div>').addClass('setting_lable').appendTo(self.$root);
		var $connectingwifi = $('<div>').addClass('panel').append($TR('general settings screen :: connectingwifi')).appendTo($setting_connectingwifi);


		// this should be a status showed in the genaral settings abaut the connection
		var connection_status = new window.connection_status();
		connection_status.$root.appendTo( self.$root );
		connection_status.show();

		var listData = [];
		(function(){
			for(var i = 0; i < settings.list_view.length; i++){
				var json = {
					name:settings.list_view.getItemByIndex(i).name,
					$root : settings.list_view.getItemByIndex(i).$root,
					callback: null,
				};
				listData.push(json);	
			}
			var json1 = {
				name:"general settings screen :: password",
				$root :$password_lable,
				callback:null,  	
			};
			var json2 = {
				name:"general settings screen :: connectwifi",
				$root :$setting_connectingwifi,
				callback:null,
			};
			listData.push(json1);
			listData.push(json2);
		})();
		
		var focusList = new window.FocusList(listData);
		console.log("focusList.length="+focusList.length);	
		//var list_view = settings.list_view ;
		var select_view = null ;



		var caPIN = [6,8,6,8,6,8];
		var pin = new Array();

		//Init Network
		window.Platform.network.init();
		
		var reload_nettwork_settings = function()
		{
			//Load	Network		
			if(null != settings.list_view.getItemByName("general settings screen :: network")){
				var networkWidget = settings.list_view.getItemByName("general settings screen :: network").select_view ;			
				var interface_info = window.Platform.network.getActiveInterface();
				
				//console.log("interface_info  type= "+ interface_info.type);
				//console.log("interface_info  ip_address= "+ interface_info.ip_address);
				//console.log("interface_info  linkState= "+ interface_info.linkState);
				
				// if we have no ip always display none as internet connection
				if (interface_info.ip_address == "") {
					settings.list_view.getItemByName("general settings screen :: network").select_view.changeSelection(2);
					// this is just a displayed none so we don't set it to the settings
					//window.Platform.network.setConnectionDetails_type(window.Platform.network.NONE);

				}
				else
				{
					switch(interface_info.type)
					{
						case window.Platform.network.ETHERNET:
							if (interface_info.linkState == window.Platform.network.UNPLUGGED)
							{
								settings.list_view.getItemByName("general settings screen :: network").select_view.changeSelection(2);
							}
							else if (interface_info.linkState == window.Platform.network.PLUGGED)
							{
								settings.list_view.getItemByName("general settings screen :: network").select_view.changeSelection(1);						
								window.Platform.network.setConnectionDetails_type(window.Platform.network.ETHERNET);
							}
							else
							{
								settings.list_view.getItemByName("general settings screen :: network").select_view.changeSelection(2);
							}
							
							break;
					
						case window.Platform.network.WIRELESS:
							settings.list_view.getItemByName("general settings screen :: network").select_view.changeSelection(0);					
							window.Platform.network.setConnectionDetails_type(window.Platform.network.WIRELESS);
							break;
						
						case window.Platform.network.NONE:
							settings.list_view.getItemByName("general settings screen :: network").select_view.changeSelection(2);
							// this is just a displayed none so we don't set it to the settings
							//window.Platform.network.setConnectionDetails_type(window.Platform.network.NONE);
							break;
						
						default:			
							settings.list_view.getItemByName("general settings screen :: network").select_view.changeSelection(2);
							// this is just a displayed none so we don't set it to the settings
							//window.Platform.network.setConnectionDetails_type(window.Platform.network.NONE);
							break;
					};				
				}
			}
			
		};
		
		//Load Network Settings
		if(null != settings.list_view.getItemByName("general settings screen :: network settings")){
			var networkSettingWidget = settings.list_view.getItemByName("general settings screen :: network settings").select_view ;	
			var interface_info = window.Platform.network.getActiveInterface();				
			
			if (interface_info.dhcp == false )
			{
				settings.list_view.getItemByName("general settings screen :: network settings").select_view.changeSelection(1);
				window.Platform.network.setConnectionDetails_settings( window.Platform.network.MANUAL );
			}
			else
			{
				settings.list_view.getItemByName("general settings screen :: network settings").select_view.changeSelection(0);
				window.Platform.network.setConnectionDetails_settings( window.Platform.network.DHCP );
			}			
		}

		self.onShow = function( ) {
			initVideoFormat();
			initConversionMode();
			settings.show( ) ;
			
						
			//Load Language
			if(null != settings.list_view.getItemByName("general settings screen :: language")){        	
				var languageWidget = settings.list_view.getItemByName("general settings screen :: language").select_view ;
				var lang = window.Platform.settings.getLanguage();
				languageWidget.selectValue(lang);
			}
			
			//Load	Network
			//Load Network Settings
			reload_nettwork_settings();
			connection_status.update_status( window.Platform.network.getActiveInterface() );
			
			
			//Load Aspect Ratio
			if(null != settings.list_view.getItemByName("general settings screen :: aspectratio")){
				var aspectRatioWidget = settings.list_view.getItemByName("general settings screen :: aspectratio").select_view ;
				var aspectRatio = window.Platform.settings.getAspectRatio();
				if  (aspectRatio == Platform.settings.RatioStatus["ASPECT_RATIO_16_9"])
				{
					aspectRatioWidget.selectValue("ASPECT_RATIO_16_9");
				}
				else if (aspectRatio == Platform.settings.RatioStatus["ASPECT_RATIO_4_3"])
				{
					aspectRatioWidget.selectValue("ASPECT_RATIO_4_3");
				} 
			}
			
			//Load Video Format
			if(null != settings.list_view.getItemByName("general settings screen :: videoformat")){            
				var conversionModeWidget = settings.list_view.getItemByName("general settings screen :: videoformat").select_view ;
				var format = window.Platform.settings.getConversionMode();
				if(format == Platform.settings.VideoFormatStatus["VIDEO_ZOOM_MAKE_WIDER"])
				{
					conversionModeWidget.selectValue("VIDEO_ZOOM_MAKE_WIDER");
				}
				else if(format ==  Platform.settings.VideoFormatStatus["VIDEO_ZOOM_NON_LINEAR_STRETCH_TO_FULL_SCREEN"])
				{
					conversionModeWidget.selectValue("VIDEO_ZOOM_NON_LINEAR_STRETCH_TO_FULL_SCREEN");
				}
				else if(format ==  Platform.settings.VideoFormatStatus["VIDEO_ZOOM_ENLARGE"])
				{
					conversionModeWidget.selectValue("VIDEO_ZOOM_ENLARGE");
				}
				else if(format ==  Platform.settings.VideoFormatStatus["VIDEO_ZOOM_NORMAL"])
				{
					conversionModeWidget.selectValue("VIDEO_ZOOM_NORMAL");
				}
				else if(format ==  Platform.settings.VideoFormatStatus["VIDEO_ZOOM_FULL_SCREEN"]){
					conversionModeWidget.selectValue("VIDEO_ZOOM_FULL_SCREEN");
				}							
			}
			
			//Load Audio Output
			if(null != settings.list_view.getItemByName("general settings screen :: audiooutput")){
				var audioOutputWidget = settings.list_view.getItemByName("general settings screen :: audiooutput").select_view ;
				var audioOutput = window.Platform.settings.getAudioOutput();
				if (audioOutput == Platform.settings.AudioOutputStatus["AUDIO_STEREO"])
				{
					audioOutputWidget.selectValue("AUDIO_STEREO");
				}
				else if (audioOutput == Platform.settings.AudioOutputStatus["AUDIO_HDMI_SPDIF"])
				{
					audioOutputWidget.selectValue("AUDIO_HDMI_SPDIF");
				}                        
				else if (audioOutput == Platform.settings.AudioOutputStatus["AUDIO_SPDIF"])
				{
					audioOutputWidget.selectValue("AUDIO_SPDIF");
				} 			
			}
			
			//Load Subtitle
			if(null != settings.list_view.getItemByName("general settings screen :: subtitle")){
				var teletextWidget = settings.list_view.getItemByName("general settings screen :: subtitle").select_view ;
			}
			
			//Load Video Resolution
			if(null != settings.list_view.getItemByName("general settings screen :: videoresolution")){
				var videoModeWidget = settings.list_view.getItemByName("general settings screen :: videoresolution").select_view ;            
				var videoMode = window.Platform.settings.getResolution();
				if (videoMode == Platform.settings.VideoModeStatus["VIDEO_MODE_1080I50"])
				{
					videoModeWidget.selectValue("VIDEO_MODE_1080I50");
				}
				else if (videoMode == Platform.settings.VideoModeStatus["VIDEO_MODE_720P50"])
				{
					videoModeWidget.selectValue("VIDEO_MODE_720P50");
				}                        
				else if (videoMode == Platform.settings.VideoModeStatus["VIDEO_MODE_576I50"])
				{
					videoModeWidget.selectValue("VIDEO_MODE_576I50");
				}			
			}
			
			//Load Audio Preference
			if(null != settings.list_view.getItemByName("general settings screen :: audiopreference")){
				var audioPrefWidget = settings.list_view.getItemByName("general settings screen :: audiopreference").select_view ;
				var loadStr = Platform.storage.load("defaultsettings", '{\"default\":{\"audio\":{\"defaulttrack\":\"0\"}}}');								
				var defaultAudioTrackStr = JSON.parse(loadStr);	
				var audioPref = defaultAudioTrackStr.default.audio.defaulttrack;
				if (audioPref == 0)
				{
					audioPrefWidget.selectValue(0);
				}
				else if (audioPref == 1)
				{
					audioPrefWidget.selectValue(1);
				} 			
			}
			
			//Load Timezone
			if(null != settings.list_view.getItemByName("general settings screen :: timezone")){
				var timezoneWidget = settings.list_view.getItemByName("general settings screen :: timezone").select_view ;
            }
        
		} ;

		self.onKey = function( ) {
			return true ;
		} ;
		self.onKeyUP = function( ) {
			if( select_view ) { return ; }

			//list_view.selectPrev( ) ;
			focusList.selectPrev();
		} ;

		self.onKeyDOWN = function( ) {
			if( select_view ) { return ; }

			//list_view.selectNext( ) ;
			focusList.selectNext();
			console.log("focusList.getSeletedIndex() = "+ focusList.getSeletedIndex());
		} ;
		
		self.set_focus = function()
		{
			var selectedName = focusList.getSelectedName();
			switch(selectedName){
				case "general settings screen :: language":
				case "general settings screen :: language":
				case "general settings screen :: network":
				case "general settings screen :: network settings":
				case "general settings screen :: aspectratio":
				case "general settings screen :: videoformat":
				case "general settings screen :: audiooutput":
				case "general settings screen :: subtitle":
				case "general settings screen :: videoresolution":
				case "general settings screen :: audiopreference":
				case "general settings screen :: timezone":{
					if( !select_view ) {
						select_view = settings.list_view.getItemByName(selectedName).select_view ;
						select_view.activate( true ) ;
					}
					break;
				}
			}
		} ;
		
		self.change_item_to = function( name, nummber_to_change_to )
		{
			settings.list_view.getItemByName( name ).select_view.changeSelection( nummber_to_change_to );
		};
		
		self.reload_nettwork_settings = function()
		{
			reload_nettwork_settings();
		};
		
		
		self.onKeyOK = function( ) {
			var selectedName = focusList.getSelectedName();
			console.log("selectedName = "+selectedName);
			switch(selectedName){
				case "general settings screen :: language":
				case "general settings screen :: language":
				case "general settings screen :: network":
				case "general settings screen :: network settings":
				case "general settings screen :: aspectratio":
				case "general settings screen :: videoformat":
				case "general settings screen :: audiooutput":
				case "general settings screen :: subtitle":
				case "general settings screen :: videoresolution":
				case "general settings screen :: audiopreference":
				case "general settings screen :: timezone":{
					if( !select_view ) {
						select_view = settings.list_view.getItemByName(selectedName).select_view ;
						select_view.activate( true ) ;
					}
					else {
						console.log( "selected :", select_view.getSelected().name );    
                        
						if( select_view.getSelected().name == 'general settings screen :: manuel' )
						{
							console.log("enter ip_config");
							window.AppManager.back( );
							window.AppManager.enter( "ip_config" );
						}
	
						
						if ( select_view.getSelected().name == 'general settings screen :: dhcp')
						{
							window.Platform.network.setConnectionDetails_settings( window.Platform.network.DHCP );
							var ret = window.Platform.network.auto_apply_settings();
							window.Platform.network.standard_error_reaction( ret );
							if ( ret < 0) {
								//reset settings
								reload_nettwork_settings();
							}
						}
						
						if ( select_view.getSelected().name == 'general settings screen :: wifi' )
						{
							//window.AppManager.back( ) ;
							//window.AppManager.enter( 'wifi_config') ;
		 
							window.Platform.network.setConnectionDetails_type( window.Platform.network.WIRELESS );
							var ret = window.Platform.network.auto_apply_settings();
							window.Platform.network.standard_error_reaction( ret );
							if ( ret < 0) {
								//reset settings
								reload_nettwork_settings();
							}
						}
						
						if ( select_view.getSelected().name == 'general settings screen :: lan' )
						{
				            window.Platform.network.setConnectionDetails_type( window.Platform.network.ETHERNET );
							var ret = window.Platform.network.auto_apply_settings();
							window.Platform.network.standard_error_reaction( ret );
							if ( ret < 0) {
								//reset settings
								reload_nettwork_settings();
							}
						}
						
						if ( select_view.getSelected().name == 'general settings screen :: none' )
						{
				            window.Platform.network.setConnectionDetails_type( window.Platform.network.NONE );
							var ret = window.Platform.network.auto_apply_settings();
							window.Platform.network.standard_error_reaction( ret );
							if ( ret < 0) {
								//reset settings
								reload_nettwork_settings();
							}
						}
						
						select_view.activate( false ) ;
						select_view = null ;
					}
					break;	
				}	
				case "general settings screen :: password":{
						window.AppManager.back( ) ;
						window.AppManager.enter( 'PIN Code') ;
						break;
				}
				case "general settings screen :: connectwifi":{
						window.AppManager.back( ) ;
						window.AppManager.enter( 'wifi_config') ;
						break;
				}

				default:{
					if( !select_view ) {
						select_view = settings.list_view.getItemByName(selectedName).select_view ;
						select_view.activate( true ) ;
					}
					else {
						select_view.activate( false ) ;
						select_view = null ;
					}
					break;
				}
			}
		} ;

		self.onKeyLEFT = function( ) {
			if( !select_view ) {
				window.AppManager.back( ) ;
				return ;
			}

			select_view.selectPrev( ) ;
		} ;

		self.onKeyRIGHT = function( ) {
			if( !select_view ) { return ; }

			select_view.selectNext( ) ;
		} ;
		
		self.onKeyNUMBER = function(num){
			if(pin.length >= 6){
				pin.splice(0,1);
			}
			pin.push(num);
		}
		self.onKeyRED = function(){
			if(pin.toString() == caPIN.toString()){	
					pin.splice(0,pin.length);													
					window.App.goToCAMenu( ) ;
			}				
		}
		return self ;
	} ;

	window.AppManager.addScene( name, GeneralSettings ) ;

})( ) ;
