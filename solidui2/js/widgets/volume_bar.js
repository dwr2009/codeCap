"use strict";
(function( ) {

	var name = "Volume Bar" ;


	var VolumeBar = function( ) {

		var parent = new window.Widget( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'volume_bar' ).appendTo( '#interface' ) ;

		var $content = $( '<div>' ).addClass( 'content' ).appendTo( self.$root ) ;
		var $bar = $( '<div>' ).addClass( 'bar' ).appendTo( $content ) ;
		var $progress = $( '<div>' ).addClass( 'progress' ).appendTo( $bar ) ;
		var $text = $( '<div>' ).addClass( 'text' ).appendTo( $content ) ;


		var setBar = function( percent, text ) {
			$progress.css( 'width', percent + '%' ) ;
			$text.text( text ) ;
			self.show( ) ;
		} ;

		var modifyVolume = function( delta ) {
			window.Platform.media.getVolume( function( error, volume ) {
				if( error ) { throw new Error( error.message ) ; }

				volume = Math.floor( Math.max( window.Template.VolumeMinLimit, Math.min( 100, volume + delta ) ) ) ;

				window.Platform.media.setVolume( volume, function( error ) {
					if( error ) { throw new Error( error.message ) ; }

					console.log( "volume is", volume ) ;

					setBar( volume, volume ) ;
				} ) ;
			} ) ;
		} ;

		var toggleMute = function( ) {
			window.Platform.media.getMute( function( error, mute ) {
				if( error ) { throw new Error( error.message ) ; }

				mute = !mute ;

				window.Platform.media.setMute( mute, function( error ) {
					if( error ) { throw new Error( error.message ) ; }

					console.log( "mute is", mute ) ;

					if( mute ) {
						setBar( 0, TR( "volume :: mute" ) ) ;
					}
					else {
						modifyVolume( 0 ) ;
					}
				} ) ;
			} ) ;
		} ;


		var autohide = _.debounce( function( ) {
			self.hide( ) ;
		}, window.settings.volume_bar_autohide_delay ) ;


		self.onShow = function( ) {
			autohide( ) ;
		} ;

		self.up = function( ) {
			modifyVolume( +5 ) ;
		} ;

		self.down = function( ) {
			modifyVolume( -5 ) ;
		} ;

		self.toggleMute = toggleMute ;

		return self ;
	} ;

	window.AppManager.addWidget( name, VolumeBar ) ;

})( ) ;
