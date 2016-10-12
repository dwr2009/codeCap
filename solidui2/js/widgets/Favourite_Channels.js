"use strict";
(function(){
			var name = "Favourite Channels" ;
			var FavouriteChannels = function(){
						var parent = new window.Scene( name ) ;
						var self = Object.create( parent ) ;

						self.$root.attr( 'id', 'FavouriteChannels' ) ;
						
						var updateScreen = function( ) {
								
								window.App.goToHome( ) ;
								var live = window.AppManager.getWidget("Live TV");
								live.onKeyFAV();
						};
						
						self.onKey = function( ) {
								return true ;
						};
						
						self.onShow = function( ) {
								updateScreen();	
						};
						
						self.onKeyEXIT = function(){
							window.AppManager.back( ) ;
						};
						return self ;
			};
			
			window.AppManager.addScene( name, FavouriteChannels ) ;				
	
})( );