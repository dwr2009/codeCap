"use strict";
(function( ) {

	var MainMenuScreen = window.Class(

		// parent
		window.Scene,

		// constructor
		function( name ) {
			Scene.call( this, name ) ;

			this.$root.addClass( 'menu_screen' ) ;
		},

		// prototype
		{
			onKeyLEFT: function( ) {
				window.AppManager.back( ) ;
				return true ;
			},

			show: function( ) {
				console.log( "HIDE LIVE TV" ) ;
				Scene.prototype.show.call( this ) ;
			},

			hide: function( ) {
				console.log( "SHOW LIVE TV" ) ;
				Scene.prototype.hide.call( this ) ;
				setTimeout(function( ){
					window.AppManager.getWidget( 'Live TV' ).activate( ) ;
				},0);// add the setTimeout was used to fix the issue which exit to liveTV from vod,it will not work if you press any button
			},
		}

	) ;

	window.MainMenuScreen = MainMenuScreen ;

})( ) ;
