"use strict";
(function( ) {

	var name = "App" ;

	var HbbtvApp = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'hbbtv_app' ) ;

		var $debug_text = $( '<div>' ).addClass( 'debug_text' ).text( "hbbtv app" ).appendTo( self.$root ) ;


		var ESCAPE_KEY = 'EXIT' ;
		var ESCAPE_CODE = window.Platform.keycodes[ESCAPE_KEY] ;

		var createWebview = function( url, hbbtv ) {
			var id = MediaEngineBrowser.createWebView( ) ;

			if( hbbtv ) {
				MediaEngineBrowser.setAsHbbtvView( id ) ;
			}

			MediaEngineBrowser.setUrl( id, url ) ;

			// ESCAPE KEY is used to return to the main menu. No other page should handle it.
			MediaEngineBrowser.ignoreKeys( id, [ ESCAPE_CODE ] ) ;

			return id ;
		} ;

		var ORIGINAL_WEBVIEW = 0 ;
		var HBBTV_WEBVIEW = createWebview( "http://itv.mit-xperts.com/hbbtvtest/", true ) ;

		var switchToWebview = function( id ) {

			// show and focus the webview we are switching to
			if( id == ORIGINAL_WEBVIEW ) {
				MediaEngineBrowser.hideView( HBBTV_WEBVIEW ) ;
				MediaEngineBrowser.showView( ORIGINAL_WEBVIEW ) ;
			}
			else {
				MediaEngineBrowser.showView( HBBTV_WEBVIEW ) ;
				MediaEngineBrowser.hideView( ORIGINAL_WEBVIEW ) ;
			}

			MediaEngineBrowser.focusView( id ) ;

			// the webview we are switching to should be the first to recieve key events
			MediaEngineBrowser.inputFirst( id ) ;
		} ;

		var gotoHbbtv = function( ) {
			switchToWebview( HBBTV_WEBVIEW ) ;
		} ;

		var comeBack = function( ) {
			switchToWebview( ORIGINAL_WEBVIEW ) ;
		} ;


		self.onKey = function( key ) {
			return true ;
		} ;

		self['onKey' +  ESCAPE_KEY ] = function( ) {
			window.AppManager.back( ) ;
			return true ;
		} ;

		self.onShow = function( ) {
			gotoHbbtv( ) ;
		} ;

		self.onHide = function( ) {
			comeBack( ) ;
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, HbbtvApp ) ;

})( ) ;
