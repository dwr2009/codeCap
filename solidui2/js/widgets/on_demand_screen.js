"use strict";
(function( ) {

	var name = "on demand" ;

	var OnDemand = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'on_demand' ) ;

		var networkStatus = new window.popDlg();
		networkStatus.$root.appendTo(self.$root);
		networkStatus.hide();

		var HOME_CODE    = window.Platform.keycodes['HOME'] ;
		var VOLUP_CODE   = window.Platform.keycodes['VOLUP'] ;
		var VOLDOWN_CODE = window.Platform.keycodes['VOLDOWN'] ;	
				
		var createWebview = function( ) {
			var exists = MediaEngineBrowser.exists( window.Platform.browser.onDemandId );
			if( !exists ) {
				window.Platform.browser.onDemandId = MediaEngineBrowser.createWebView( ) ;
				// ESCAPE KEY is used to return to the main menu. No other page should handle it.
				MediaEngineBrowser.ignoreKeys( window.Platform.browser.onDemandId, [ HOME_CODE, VOLUP_CODE, VOLDOWN_CODE ] ) ;
			}
			return window.Platform.browser.onDemandId ;
		} ;

		var ORIGINAL_WEBVIEW = 0 ;
		var URL = "";
	
		var updateURL = function( ) {
			var userId = "";
			var backurl = "";
			var language = "";
			
			var sn = window.Platform.ca.GetCardSN();
			if (sn.retVal == 0) {      
				userId = sn.cardSN;
			}
			
			if (window.Platform.settings.getLanguage() == "en") {
				language = "en-US";
			}
			else {
				language = "zh-HK";
			}
			
			URL = 'http://portal.ott.cas.hkcable.com.hk:6601/epghkc/' + "?userId=" + userId + "&language=" + language;		
			//URL = 'http://lab.portal.ott.cas.hkcable.com.hk:6600/epghkc/' + "?userId=" + userId + "&language=" + language;		
			//URL = 'http://219.134.89.204:6600/epghkc/templates/default/' + "?userId=" + userId + "&language=" + language;			
		} ;
		
		var EXTERNAL_WEBVIEW = createWebview( ) ;

		var switchToWebview = function( id ) {

			// show and focus the webview we are switching to
			if( id == ORIGINAL_WEBVIEW ) {
				MediaEngineBrowser.hideView( EXTERNAL_WEBVIEW ) ;
				MediaEngineBrowser.showView( ORIGINAL_WEBVIEW ) ;
                
			        Platform.media.stop( function( error ) {
			            if( error ) { throw new Error( error.message ) ; }
			        } ) ;	                
				MediaEngineBrowser.setUrl( EXTERNAL_WEBVIEW, "about:blank" ) ;
			}
			else {         
				MediaEngineBrowser.setUrl( EXTERNAL_WEBVIEW, URL ) ;
                
				MediaEngineBrowser.showView( EXTERNAL_WEBVIEW ) ;
				MediaEngineBrowser.hideView( ORIGINAL_WEBVIEW ) ;
			}

			MediaEngineBrowser.focusView( id ) ;

			// the webview we are switching to should be the first to recieve key events
			MediaEngineBrowser.inputFirst( id ) ;
		} ;

		var gotoExternal = function( ) {
			updateURL( ) ;			
			switchToWebview( EXTERNAL_WEBVIEW ) ;
		} ;

		var comeBack = function( ) {
			switchToWebview( ORIGINAL_WEBVIEW ) ;
		} ;

		var reminderPOPMsg = function(){
			var reminderInfo = window.Platform.pvr.getUpcommingReminderData();
			var currentTime = window.Platform.time.getTime();
			if(reminderInfo.time - currentTime < 30*60*1000){
				console.log("aaaaaaaaaaaaaaaaaaaaaaaaa");
				networkStatus.show();
				networkStatus.showDlgMsg(TR("reminder screen :: reminder message"));
				setTimeout( function( ) {
					networkStatus.hide() ;
					gotoExternal( ) ; 
				}, 4000);
			}else{
				gotoExternal( ) ;
			}
		};


		self.onKey = function( key ) {
			return true ;
		} ;
 
		self.onKeyLEFT = 
		self.onKeyBACK = 
		self.onKeyEXIT = function( ) {
			return true ;
		} ;

		self.onKeyHOME = function( ) {
			window.AppManager.back( ) ;
			return true ;
		} ;

		self.onShow = function( ) {	      
	        var net = Platform.network.getActiveInterface();
			if ( window.Platform.network.CONNECTED == net.status ) {
		        Platform.media.stop( function( error ) {
		            if( error ) { throw new Error( error.message ) ; }
		        } ) ;	
	        	reminderPOPMsg();
	        }
	        else {
	        	networkStatus.show();
				networkStatus.showDlgMsg(TR('general settings screen :: ip_status_disconnected'));
				setTimeout( function( ) {
					networkStatus.hide() ; 
					window.AppManager.back() ;
				}, 2000);
	        }
		} ;

		self.onHide = function( ) {
			comeBack( ) ;
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, OnDemand ) ;

})( ) ;
