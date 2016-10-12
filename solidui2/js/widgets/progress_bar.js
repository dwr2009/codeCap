"use strict";
(function( ) {

	var name = "progressbar" ;


	var progressbar = function( ) {

		var parent = new window.Widget( name ) ;
		var self = Object.create( parent ) ;
		
		var ts_starttime = 0;  
		var ts_stoptime = 0;
		var ts_progress = 0; 
		var formatName = name; // to use a other name for date and time formating
		
		var time_string_start = window.Format.formatTimeLayoutTS( ts_starttime,formatName ) ;	
		var time_string_stop = window.Format.formatTimeLayoutTS( ts_stoptime, formatName ) ;	
		
		var $progressbar = $( '<div>' ).attr( 'id', 'progressbar' ).appendTo( self.$root ) ;
		
		var $starttime = $( '<div>' ).attr( 'id', 'starttime' ).addClass( 'starttime' ).text( time_string_start ).appendTo( $progressbar ) ;
		
		var $bar = $( '<div>' ).addClass( 'bar' ).appendTo( $progressbar ) ;
		var $barborder = $( '<div>' ).addClass( 'barborder' ).appendTo( $bar );
		var $progress = $( '<div>' ).attr( 'id', 'progress' ).addClass( 'progress' ).appendTo( $bar ) ;
		var $progressborder = $( '<div>' ).addClass( 'progressborder' ).appendTo( $progress ) ;
		var $progressstream = $( '<div>' ).addClass( 'progressstream' ).appendTo( $progress ) ;
		var $progressdot = $( '<div>' ).addClass( 'progressdot' ).appendTo( $progress ) ;
		var $line = $( '<div>' ).addClass( 'line' ).appendTo( $progress ) ;
		var $current = $( '<div>' ).addClass( 'current' ).appendTo( $progressdot ) ;
		var $currentbox = $( '<div>' ).addClass( 'currentbox' ).appendTo( $current ) ;
		
		var $currenttime = $( '<div>' ).attr( 'id', 'currenttime' ).addClass( 'currenttime' ).text( "" ).appendTo( $currentbox ) ;
		
		var $endtime = $( '<div>' ).attr( 'id', 'endtime' ).addClass( 'endtime' ).text( time_string_stop ).appendTo( $progressbar ) ;	
		
		self.update = function( ) {
			
			var progress_value_100 = ts_stoptime - ts_starttime;
			var progress_value_x = ts_progress - ts_starttime;
			var progress_value = Math.floor(progress_value_x * 100 / progress_value_100 );
			// dot starts only at 3% 
			if (progress_value < 3) {
				progress_value = 3;
			} ;
			$('#currenttime').text(window.Format.formatTimeLayoutTS(ts_progress,formatName ));
			$('#progress').css("width",progress_value+"%")
		}
		self.update();	

		self.updateStartStop = function( ) {
			$('#starttime').text(window.Format.formatTimeLayoutTS(ts_starttime,formatName ));
			$('#endtime').text(window.Format.formatTimeLayoutTS(ts_stoptime,formatName ));
		}
		self.updateStartStop();	

	
		self.onShow = function( ) {											
			//start();
		}

		self.onHide = function( ) {		
			//stop();
		}
		
		self.setStarttime = function( start ) {	
			ts_starttime = start;
		}
		self.setStoptime = function( stop ) {
			ts_stoptime = stop;
		}
		self.setProgress = function( progress ) {		
			ts_progress = progress;
		}
		self.setFormatName = function( formatname ) {		
			formatName = formatname;
		}


		return self ;
	} ;


	window.AppManager.addWidget( name, progressbar ) ;

})( ) ;
