"use strict";
(function( ) {

	var App = new function( ) {

		var getParameters = function( ) {
			var query = location.search.slice( 1 ) ;
			var pairs = query.split( '&' ) ;

			var params = {} ;

			for( var i in pairs ) {
				var pair = pairs[i].split( '=' ) ;
				var key = pair[0] ;
				var value = pair[1] ;
				params[key] = value ;
			}

			return params ;
		}

		var getPlatformScript = function( ) {
			var platform = window.settings.platform ;
			var path = 'js/platforms/' + platform + '/init.js' ;

			console.log( "platform is", platform ) ;

			return path ;
		} ;

		var getTemplateScript = function( ) {
			var template = window.settings.template ;
			var path = 'templates/' + template + '/init.js' ;

			console.log( "template is", template ) ;

			return path ;
		} ;

		var adjustMonitor = function( ) {
			var monitor_adjust_top = window.settings.monitor_adjust_top -10 ;
			var monitor_adjust_left = window.settings.monitor_adjust_left -10 ;
			var monitor_adjust_bottom = window.settings.monitor_adjust_bottom ;
			var monitor_adjust_right = window.settings.monitor_adjust_right ;
			var monitor_width = 1280 - monitor_adjust_left - monitor_adjust_right -10 ; //need to get the Resolution Information from Platform.Display
			var monitor_height = 720 - monitor_adjust_top - monitor_adjust_bottom -10  ; //need to get the Resolution Information from Platform.Display
			
			$( "#interface" ).css({
      		"margin-top": monitor_adjust_top,
      		"margin-left": monitor_adjust_left,
      		"margin-bottom": monitor_adjust_bottom,
      		"margin-right": monitor_adjust_right,
    			"width": monitor_width,
    			"height": monitor_height,
    		});
		} ;


		var init = function( ) {
			var params = getParameters( ) ;
			_.extend( window.settings, params ) ;

			loadScripts( ) ;
			adjustMonitor();
		} ;


		var loadScripts = function( ) {
			console.log( "loading scripts" ) ;

			var callback = function( ) {
				console.log( "scripts loaded" ) ;

				initTranslation( ) ;
			}

			var scripts = [
				"js/class.js",
				"js/format.js",
				getPlatformScript( ),				
				"js/translation.js",
				getTemplateScript( ),
				"js/app_manager.js",
			] ;

			var loader = window.Loader.loadScripts( scripts ) ;
			loader.onDone( callback ) ;
		} ;


		var initTranslation = function( ) {
			console.log( "initializing translation" ) ;

			var callback = function( ) {
				console.log( "translation initialized" ) ;

				initPlatform( ) ;
			} ;

			window.Translation.init( callback ) ;
		} ;


		var initPlatform = function( ) {
			console.log( "initializing platform" ) ;

			var callback = function( ) {
				console.log( "platform initialized" ) ;

				initTemplate( ) ;
			} ;

			window.Platform.init( callback ) ;
		} ;


		var initTemplate = function( ) {
			console.log( "initializing template" ) ;

			var callback = function( ) {
				console.log( "template initialized" ) ;

				createViews( ) ;
			} ;

			window.Template.init( callback ) ;
		} ;


		var createViews = function( ) {
			console.log( "creating views" ) ;

			var callback = function( ) {
				console.log( "views created" ) ;

				loadLiveTV( ) ;
			} ;

			window.AppManager.init( callback ) ;
		} ;


		var loadLiveTV = function( ) {
			console.log( "loading live tv" ) ;

			window.AppManager.enter( "Live TV" ) ;
		} ;


		$( init ) ;


		this.goToHome = function( ) {
			while( window.AppManager.current( ).name !== "Live TV" ) {
				window.AppManager.back( ) ;
			}
		} ;

		this.goToMainMenu = function( ) {
			var target = "Live TV" ;
			if( window.AppManager.isInStack( "Main Menu" ) ) {
				target = "Main Menu" ;
			}

			while( window.AppManager.current( ).name !== target ) {
				window.AppManager.back( ) ;
			}

			if( window.AppManager.current( ).name === "Live TV" ) {
				window.AppManager.enter( "Main Menu" ) ;
			}
		} ;

		this.goToCAMenu = function( ) {
			var target = "General Settings" ;
			if( window.AppManager.isInStack( "CA Menu" ) ) {
				target = "CA Menu" ;
			}

			while( window.AppManager.current( ).name !== target ) {
				window.AppManager.back( ) ;
			}
						
			if( window.AppManager.current( ).name === "General Settings" ) {
				window.AppManager.back( ) ;
				window.AppManager.enter( "CA Menu" ) ;
			}
			
		};

		this.goToEPG = function( ) {
			if( window.AppManager.current( ).name === "Programme Guide" ) {
				return ;
			}

			while( window.AppManager.current( ).name !== "Live TV" ) {
				window.AppManager.back( ) ;
			}

			window.AppManager.enter( "Programme Guide" ) ;
		} ;
		
		this.goToVod = function( ) {
			if( window.AppManager.current( ).name === "on demand" ) {
				return ;
			}

			while( window.AppManager.current( ).name !== "Live TV" ) {
				window.AppManager.back( ) ;
			}

			window.AppManager.enter( "on demand" ) ;
		} ;
	} ;

	window.App = App ;

})( ) ;
