"use strict";
(function( ) {

	var name = "Info Bar" ;


	var InfoBar = function( ) {

		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'info_bar' ) ;

		var top_bar = window.AppManager.getWidget( "Top Bar" ) ;
		
		var $infobar = $( '<div>' ).addClass( 'infobar' ).appendTo( self.$root );

		// Channel info
		var $channelinfo = $( '<div>' ).addClass( 'channelinfo' ).appendTo( $infobar ) ;

		var $currenttitle = $( '<div>' ).addClass( 'currenttitle' ).append( $TR( "infobar :: current" ) ).appendTo( $channelinfo ) ;
		
		var $channeltype = $( '<div>' ).attr( 'id', 'channeltypetv' ).addClass( 'channeltypetv' ).appendTo( $channelinfo ) ;
		var $channelid = $( '<div>' ).attr( 'id', 'channelid' ).addClass( 'channelid' ).appendTo( $channelinfo ) ;
		var $channelname = $( '<div>' ).attr( 'id', 'channelname' ).addClass( 'channelname' ).appendTo( $channelinfo ) ;
		
		
		var $currentinfo = $( '<div>' ).addClass( 'currentinfo' ).appendTo( $channelinfo ) ;
		var $preview = $( '<div>' ).addClass( 'preview' ).text( "" ).appendTo( $currentinfo ) ;

		// Program guide
		var $programguide = $( '<div>' ).addClass( 'programguide' ).appendTo( $infobar ) ;
		
		var $currentnextepg = [] ;		
	        
		var $currentprogram = $( '<div>' ).addClass( 'currentprogram' ).appendTo( $programguide ) ;
		
		var time_string_start = "" ;
		var time_string_stop = "" ;
		
		var $currentprogramtime = $('<div>').attr( 'id', 'currentprogramtime' ).addClass( 'currentprogramtime' ).appendTo( $currentprogram ) ;

		var currentprogramname = $( '<div>' ).attr( 'id', 'currentprogramname' ).appendTo( $currentprogram ) ;
		
		/* //scrolling text
		var begin = 0;
		var end = 0 ;

		function scrolling_text()
		{
			if (end>10) {
				$('#currentprogramname').text( $currentnextepg[0].name.substring(begin,end) + " " + $currentnextepg[0].name.substring(0,begin) ) ;
				begin++;
                
				if (begin >= end) { 
					begin = 0;
				};
			}
			else {
				$('#currentprogramname').text( $currentnextepg[0].name ); 		
			};
			//console.log( "-> name of your trust  -> begin = " + begin ) ;
			//console.log( "-> name of your trust  -> end = " + end ) ;
		}; */
		
		var progressbar = window.AppManager.getWidget( "progressbar" ) ;
		
		var $programinfo = $( '<div>' ).addClass( 'programinfo' ).appendTo( $programguide ) ;
		
//		var $nextinfo = $( '<div>' ).addClass( 'nextinfo' ).appendTo( $programinfo ) ;
//		var $nexttitle = $( '<div>' ).addClass( 'nexttitle' ).append( $TR( "infobar :: next" ) ).appendTo( $nextinfo ) ;
		
		var $currentprograminfo = $( '<div>' ).addClass( 'currentprograminfo' ).appendTo( $programinfo ) ;

//		var $size = $( '<div>' ).addClass( 'size' ).text( "16:9" ).appendTo( $currentprograminfo ) ;
//		var $quality = $( '<div>' ).addClass( 'quality' ).appendTo( $currentprograminfo ) ;
		var $audio = $( '<div>' ).addClass( 'audio' ).appendTo( $currentprograminfo ) ;	
		var $audio_name = $( '<div>' ).addClass( 'audio_name' ).appendTo( $audio ) ;
		var $voice_logo = $( '<div>' ).addClass( 'voice_logo' ).appendTo( $audio ) ;
		
		var $nexttitle = $( '<div>' ).addClass( 'nexttitle' ).appendTo( $currentprograminfo ) ;
		var $nextprogram = $( '<div>' ).addClass( 'nextprogram' ).appendTo( $programinfo ) ;

		var time_string_start_next = "" ;	
		var time_string_stop_next = "" ;			
		
		var $nextprogramtime = $( '<div>' ).attr( 'id', 'nextprogramtime' ).addClass( 'nextprogramtime' ).appendTo( $nextprogram ) ;
		var $nextprogramname = $( '<div>' ).attr( 'id', 'nextprogramname' ).appendTo( $nextprogram ) ;
		
		/* //scrolling text next epg
		var begin_2 = 0;
		var end_2 = 0 ;

		function scrolling_text_2() {
			if( end_2 > 10 ) {
				$('#nextprogramname').text( $currentnextepg[1].name.substring(begin_2,end_2) + " " + $currentnextepg[1].name.substring(0,begin_2) ) ;
				begin_2++;
                    
				if(begin_2 >= end_2) { 
					begin_2 = 0; 
				};
			}
			else {
				$('#nextprogramname').text( $currentnextepg[1].name );        		
			};
			//console.log( "-> name of your trust  -> begin_2 = " + begin_2 ) ;
			//console.log( "-> name of your trust  -> end_2 = " + end_2 ) ;
		}; */

		var should_autohide = true ;
		
		var initChannelInfo = function(){
			var liveTV = window.AppManager.getWidget("Live TV");
			var $currentchannelinfo = liveTV.judgeIfTunedChannel( );
			
			console.log( "-> Info Bar - update");
			var loadStr = Platform.storage.load("AudioTrack",'{\"mrl\":{\"audio\":{\"selected\":\"0\"}}}');
			var defaultAudioTrackStr = JSON.parse(loadStr);
			
			if ( _.isUndefined( defaultAudioTrackStr[$currentchannelinfo.mrl] ) ) {
				var audioIndex = window.Platform.settings.getAudioTrack();
			}
			else {
				var audioIndex = defaultAudioTrackStr[$currentchannelinfo.mrl].audio.selected;
			}			
			$audio_name.text( TR("info bar :: audio" ) + (parseInt(audioIndex) + 1) );
			$nexttitle.text( TR( "infobar :: next" ));
			            
			if (!$.isEmptyObject($currentchannelinfo)) {
				$('#channelid').text( window.Format.padWithZeroes($currentchannelinfo.lcn, 3) );
				$('#channelname' ).text( $currentchannelinfo.name);
				$currentnextepg = window.Platform.epg.loadNowNext($currentchannelinfo.mrl) ;
			}
            
			if ( _.isEmpty($currentnextepg) ) {
				console.log("------> $currentnextepg is empty!!") ;
				$currentnextepg[0] = $currentnextepg[1] = {
					name          : "",
					startDateTime : 0,
					stopDateTime  : 0,
				};
			}
			/* begin = 0 ;
			end = $currentnextepg[0].name.length ;			
			begin_2 = 0 ;
			end_2 = $currentnextepg[1].name.length; */
			time_string_start = window.Format.formatTimeLayoutTS( $currentnextepg[0].startDateTime, name ) ;	
			time_string_stop = window.Format.formatTimeLayoutTS( $currentnextepg[0].stopDateTime, name ) ;			
			$('#currentprogramtime').text( time_string_start + "-" + time_string_stop );
			$('#currentprogramname').text( $currentnextepg[0].name );
			if ( document.getElementById( 'currentprogramname' ).scrollWidth > document.getElementById( 'currentprogramname' ).clientWidth ){
				$('#currentprogramname').html("<marquee scrollamount=5><span style='font-size:22px'>" + $currentnextepg[0].name + "</span></marquee>") ;
			}

			if ($currentnextepg.length > 1) {
				time_string_start_next = window.Format.formatTimeLayoutTS( $currentnextepg[1].startDateTime, name ) ;	
				time_string_stop_next = window.Format.formatTimeLayoutTS( $currentnextepg[1].stopDateTime, name ) ;			
				$('#nextprogramtime').text(time_string_start_next + "-" + time_string_stop_next);	
				$('#nextprogramname').text( $currentnextepg[1].name );
				if ( document.getElementById( 'nextprogramname' ).scrollWidth > document.getElementById( 'nextprogramname' ).clientWidth ){
					$('#nextprogramname').html("<marquee scrollamount=5><span style='font-size:22px'>" + $currentnextepg[1].name + "</span></marquee>") ;
				}
			}
			else {
				$('#nextprogramtime').text( $TR( "infobar :: noinfo" ) ) ;
				$('#nextprogramname').text( $TR( "infobar :: noinfo" ) );
			}; 									
		};

		var autohide = _.debounce( function( ) {
			should_autohide && self.isActive( ) && window.AppManager.back( ) ;
		}, window.settings.info_bar_autohide_delay ) ;

		self.goBack = function( ) {
			self.isActive( ) && window.AppManager.back( ) ;
		} ;

		self.setAutohide = function( flag ) {
			should_autohide = flag ;
		} ;

		self.onShow = function( ) {			
			progressbar.setFormatName( name ) ;
			console.log("------> InfoBar is shown!");
			self.start();
			top_bar.show( ) ;
			progressbar.show( ) ;
			autohide( ) ;
		} ;

		self.onHide = function( ) {
			self.stop();
			// hide child widgets
			top_bar.hide( ) ;
			progressbar.hide( ) ;
		} ;
		
		self.onKeyINFO = 
		self.onKeyOK = function( ) {
			this.setAutohide( true ) ;
			window.AppManager.back( ) ;
			return true ;
		} ;
		
		var timer = null ;

		self.update = function( ) {
					
			var real_timestamp = window.Platform.time.getTime( ) ;           
			if ($currentnextepg[0].startDateTime !== 0 && $currentnextepg[0].stopDateTime !== 0 && real_timestamp >= $currentnextepg[0].startDateTime && real_timestamp <= $currentnextepg[0].stopDateTime) {                                         
				// scrolling_text(); 	
				// scrolling_text_2();
				
				progressbar.$root.appendTo( $currentprogram ) ;
				progressbar.setStarttime($currentnextepg[0].startDateTime);
				progressbar.setStoptime($currentnextepg[0].stopDateTime);
				progressbar.updateStartStop();
				//time_string_start = window.Format.formatTimeLayoutTS( $currentnextepg[0].startDateTime, name ) ;	
				//time_string_stop = window.Format.formatTimeLayoutTS( $currentnextepg[0].stopDateTime, name ) ;			
				
				/*$('#currentprogramtime').text( time_string_start + "-" + time_string_stop );
				$('#currentprogramname').text( $currentnextepg[0].name );
				if ( document.getElementById( 'currentprogramname' ).scrollWidth > document.getElementById( 'currentprogramname' ).clientWidth ){
					$('#currentprogramname').html("<marquee scrollamount=5><span style='font-size:20px'>" + $currentnextepg[0].name + "</span></marquee>") ;
				}
				
				if ($currentnextepg.length > 1) {
					time_string_start_next = window.Format.formatTimeLayoutTS( $currentnextepg[1].startDateTime, name ) ;	
					time_string_stop_next = window.Format.formatTimeLayoutTS( $currentnextepg[1].stopDateTime, name ) ;			
					$('#nextprogramtime').text(time_string_start_next + "-" + time_string_stop_next);	
					$('#nextprogramname').text( $currentnextepg[1].name );
					if ( document.getElementById( 'nextprogramname' ).scrollWidth > document.getElementById( 'nextprogramname' ).clientWidth ){
						$('#nextprogramname').html("<marquee scrollamount=5><span style='font-size:20px'>" + $currentnextepg[1].name + "</span></marquee>") ;
					}
				}
				else {
					$('#nextprogramtime').text( $TR( "infobar :: noinfo" ) ) ;
					$('#nextprogramname').text( $TR( "infobar :: noinfo" ) );
				}; */
					
				
				progressbar.setProgress(real_timestamp);			
				progressbar.update() ;
			};
		} ;

		self.setPreviewText = function ( str ) {
			console.log("InfoBar"+str);
			$preview.html(window.$TR(str)) ; 
		} ;

		self.setAudioTrackText = function ( index ) {
			$audio_name.text( TR("info bar :: audio" ) + (parseInt(index) + 1) );
		} ;

		self.start = function( ) {					
			console.log( "-> Info Bar - start");
			initChannelInfo();
			self.update();
			if ( timer == null ) {
				timer = setInterval( self.update, 1000 ) ;
			}
		} ;

		self.stop = function( ) {
			clearInterval( timer ) ;
			timer = null ;
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, InfoBar ) ;

})( ) ;
