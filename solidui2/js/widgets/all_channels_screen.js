"use strict";
(function( ) {

	var name = "All Channels" ;


	// Channel Delegate
	var Channel = new window.Class(

		// parent
		window.ListViewDelegate,

		// constructor
		function( channel_id ) {
			ListViewDelegate.call( this, "channel" + channel_id ) ;

			var $root = this.$root ;
			$root.addClass( 'channel' ) ;

			Platform.dvb.getChannelInfo( channel_id, function( error, channel ) {
				if( error ) { throw new Error( error.message ) ; }

				var $number = $( '<div>' ).addClass( 'column number' ).text( channel.number ).appendTo( $root ) ;
				var $name   = $( '<div>' ).addClass( 'column name'   ).text( channel.name ).appendTo( $root ) ;
				var $fav    = $( '<div>' ).addClass( 'column fav'    ).toggleClass( 'visible', channel.isFavorite ).appendTo( $root ) ;
				var $lock   = $( '<div>' ).addClass( 'column lock'   ).toggleClass( 'visible', channel.isLocked   ).appendTo( $root ) ;
			} ) ;
		},

		// prototype
		{}
	) ;


	var AllChannels = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;

		self.$root.addClass( 'channels' ) ;

		var $caption = $( '<div>' ).addClass( 'caption' ).append( $TR( 'channels :: all_channels' ) ).appendTo( self.$root ) ;


		var list = new window.ListView( Channel ) ;

		var updateList = function( ) {
			Platform.dvb.getChannelsNumber( function( error, number ) {
				if( error ) { throw new Error( error.message ) ; }

				Platform.dvb.getChannels( 0, number, function( error, channels ) {
					if( error ) { throw new Error( error.message ) ; }

					list.updateItems( channels ) ;
				} ) ;
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

	window.AppManager.addScene( name, AllChannels ) ;

})( ) ;
