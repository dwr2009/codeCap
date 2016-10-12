"use strict";
(function( ) {

	var name = "Timeshift Bar" ;


	var TimeshiftBar = function( ) {

		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;
		var run_update = false ;
		var timer = null ;
		var now = 0;
		var start = 0;
		var position = 0;
		var nowNext = 0;


		self.$root.attr( 'id', 'timeshift_bar' ) ;

		var $timeline = $( '<div>' ).addClass( 'timeline' ).appendTo( self.$root ) ;
		var progressbar = window.AppManager.getWidget( "progressbar" ) ;
		var $time_duration = $( '<div>' ).addClass( 'time_duration' ).append( "Time follows" ).appendTo( $timeline ) ;

		var $button_box = $( '<div>' ).addClass( 'button_box' ).appendTo( self.$root ) ;
		var $timeline_text = $( '<div>' ).addClass( 'timeline_text' ).append( "Text follows" ).appendTo( $button_box ) ;

		/*var $button_stop = $( '<div>' ).addClass( 'button_1' ).appendTo( $button_box ) ;
		var $stop_btn = $( '<div>' ).addClass( 'stop_btn' ).appendTo( $button_stop ) ;

		var $button_skip_left = $( '<div>' ).addClass( 'button' ).appendTo( $button_box ) ;
		var $skip_left_btn = $( '<div>' ).addClass( 'skip_left_btn' ).appendTo( $button_skip_left ) ;
		var $skip_left_btn_2 = $( '<div>' ).addClass( 'skip_left_btn_2' ).appendTo( $button_skip_left ) ;

		var $button_play_pause = $( '<div>' ).addClass( 'button' ).appendTo( $button_box ) ;
		var $play_btn = $( '<div>' ).addClass( 'play_btn' ).appendTo( $button_play_pause ) ;
		var $pause_btn = $( '<div>' ).addClass( 'pause_btn' ).appendTo( $button_play_pause ) ;

		var $button_skip_right = $( '<div>' ).addClass( 'button' ).appendTo( $button_box ) ;
		var $skip_right_btn = $( '<div>' ).addClass( 'skip_right_btn' ).appendTo( $button_skip_right ) ;
		var $skip_right_btn_2 = $( '<div>' ).addClass( 'skip_right_btn_2' ).appendTo( $button_skip_right ) ;*/


		var getStatusSpeed = function (){
			var status = MediaEngineTimeshift.timeshiftStatus;
			console.log( "-> getStatus()") ;
			switch (status) {
			case 0:
			    if(MediaEngineTimeshift.doingTimeshift){
				console.log( "-> ERROR: Timeshift status: NULL") ;
			    }
			    return "" ;
			case 1:
			    return TR("Timeshift :: STOP") ;
			case 2:
			    return TR("Timeshift :: PAUSE") ;
			case 3:
			    return TR("Timeshift :: SLOW_FORWARD")  + getSpeed() + "x";
			case 4:
			    return TR("Timeshift :: SLOW_REWIND")  + getSpeed() + "x";
			case 5:
			    return TR("Timeshift :: PLAY") ;
			case 6:
			    return TR("Timeshift :: FAST_FORWARD") + getSpeed() + "x";
			case 7:
			    return TR("Timeshift :: FAST_REWIND")  + getSpeed() + "x";
			case 8:
			    return TR("Timeshift :: QUICK_REPEAT") ;
			}

			return "ERROR" ;
		}

		var getSpeed = function (){
			var speed = MediaEngineTimeshift.timeshiftSpeed;
			speed = Math.abs(speed/100);

			return speed ;
		}

		var should_autohide = false ;

		var autohide = _.debounce( function( ) {
			should_autohide && self.isActive( ) && window.AppManager.back( ) ;
		}, window.settings.info_bar_autohide_delay ) ;


		self.setAutohide = function( flag ) {
			should_autohide = flag ;
		} ;

		self.onShow = function( ) {
			autohide( ) ;
			timer = setInterval( self.update, 1000 ) ;
			$('.time_duration').text( getStatusSpeed() ) ;
			nowNext = window.Platform.epg.loadNowNext(window.Platform.dvb.ChannelCurrent.mrl) ;
			if (nowNext[0] === undefined) {
			  $('.timeline_text').text( name ) ;
			} else {
			  $('.timeline_text').text( nowNext[0].name ) ;
			}
			now = window.Platform.time.getTime( ) ;
			start = now - (MediaEngineTimeshift.timeshiftElapsedSeconds + MediaEngineTimeshift.playbackElapsedSeconds) * 1000;
			position = start + MediaEngineTimeshift.playbackElapsedSeconds * 1000 ;
			progressbar.setFormatName( name ) ;
			progressbar.setStarttime( start ) ;
			progressbar.setProgress( position ) ;
			progressbar.setStoptime( now ) ;
			progressbar.updateStartStop() ;
			progressbar.update() ;
			progressbar.$root.appendTo( $timeline ) ;
			progressbar.show( ) ;
		} ;

		self.onHide = function( ) {
			clearInterval( timer ) ;
		} ;

		self.update = function( ) {
			$('.time_duration').text( getStatusSpeed() ) ;
			now = window.Platform.time.getTime( ) ;
			start = now - (MediaEngineTimeshift.timeshiftElapsedSeconds + MediaEngineTimeshift.playbackElapsedSeconds) * 1000;
			position = start + MediaEngineTimeshift.playbackElapsedSeconds * 1000 ;
			progressbar.setFormatName( name ) ;
			progressbar.setStarttime( start ) ;
			progressbar.setProgress( position ) ;
			progressbar.setStoptime( now ) ;
			progressbar.updateStartStop() ;
			progressbar.update() ;
			progressbar.$root.appendTo( $timeline ) ;
			progressbar.show( ) ;


		} ;

		self.onKeyOK = function( ) {
			return true ;
		} ;


		return self ;
	} ;

	window.AppManager.addScene( name, TimeshiftBar ) ;

})( ) ;
