"use strict";
(function( ) {

	var name = "Recording List" ;


	// Recording Delegate
	var Recording = new window.Class(

		// parent
		window.ListViewDelegate,

		// constructor
		function( recording ) {
			ListViewDelegate.call( this, recording.name ) ;

			this.$root.addClass( 'recording' ) ;

			var date = new Date( recording.date ) ;
			var date_string = window.Format.formatDate( date ) + ' ' + window.Format.formatTime( date ) ;

			var duration = recording.range.stop - recording.range.start ;
			var duration_string = window.Format.formatDuration( duration, 2 ).join( ', ' ) ;

			var $date     = $( '<div>' ).addClass( 'column date'     ).text( date_string       ).appendTo( this.$root ) ;
			var $channel  = $( '<div>' ).addClass( 'column channel'  ).text( recording.channel ).appendTo( this.$root ) ;
			var $name     = $( '<div>' ).addClass( 'column name'     ).text( recording.name    ).appendTo( this.$root ) ;
			var $duration = $( '<div>' ).addClass( 'column duration' ).text( duration_string   ).appendTo( this.$root ) ;
		},

		// prototype
		{}
	) ;


	var RecordingList = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'recordings' ) ;

		var $caption = $( '<div>' ).addClass( 'caption' ).appendTo( self.$root ) ;
		var $date     = $( '<div>' ).addClass( 'column date'     ).append( $TR( 'recordings :: date'     ) ).appendTo( $caption ) ;
		var $channel  = $( '<div>' ).addClass( 'column channel'  ).append( $TR( 'recordings :: channel'  ) ).appendTo( $caption ) ;
		var $name     = $( '<div>' ).addClass( 'column name'     ).append( $TR( 'recordings :: name'     ) ).appendTo( $caption ) ;
		var $duration = $( '<div>' ).addClass( 'column duration' ).append( $TR( 'recordings :: duration' ) ).appendTo( $caption ) ;


		var list = new window.ListView( Recording ) ;

		var updateList = function( ) {

			Platform.storage.getRecordingsList( function( error, recordings_list ) {
				if( error ) { throw new Error( error.message ) ; }

				list.updateItems( recordings_list ) ;

			} ) ;

		} ;


		self.onKey = function( ) {
			return true ;
		} ;

		self.onShow = function( ) {
			list.$root.appendTo( self.$root ) ;
			list.show( ) ;
			updateList( ) ;
		} ;

		self.onKeyUP = function( ) {
			list.selectPrev( ) ;
		} ;

		self.onKeyDOWN = function( ) {
			list.selectNext( ) ;
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, RecordingList ) ;

})( ) ;
