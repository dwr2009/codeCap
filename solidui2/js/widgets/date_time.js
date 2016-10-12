"use strict";
(function( ) {

	var name = "Date and Time" ;

	var formatDateTime = function( timestamp ) {
		var date = new Date( timestamp ) ;
		return window.Format.formatDate( date ) + "   " + window.Format.formatTime( date ) ;
	} ;

	var DateTime = function( ) {

		var parent = new window.Widget( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'date_time' ) ;

		var $date = $( '<div>' ).appendTo( self.$root ) ;

		//var real_timestamp = null ;
		//var start_date = null ;
		var timer = null ;

		var start = function( ) {
			//real_timestamp = window.Platform.time.getTime( ) ;
			//start_date = Date.now( ) ;
			//start_date = window.Platform.time.getTime( ) ;

			timer = setInterval( update, 500 ) ;
			update( ) ;
		} ;

		var stop = function( ) {
			clearInterval( timer ) ;
		} ;

		var update = function( ) {
			//var now_date = Date.now( ) ;
			//var now_date = window.Platform.time.getTime( ) ;
//			var diff = now_date - start_date ;

//			var timestamp = real_timestamp + diff ;
//			var date_string = formatDateTime( timestamp ) ;


			var real_timestamp = window.Platform.time.getTime( ) ;

			var date_string = window.Format.formatDateTimeLayoutTS( real_timestamp, name ) ;

			$date.text( date_string ) ;
		} ;


		self.onShow = function( ) {
			start( ) ;
		} ;

		self.onHide = function( ) {
			stop( ) ;
		} ;

		return self ;
	} ;

	window.AppManager.addWidget( name, DateTime ) ;

})( ) ;
