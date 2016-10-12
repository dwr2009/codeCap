"use strict";
(function( ) {

	var name = "Main Menu" ;


	var MainMenu = function( ) {

		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'main_menu' ) ;

		var $overlay = $( '<div>' ).addClass( 'overlay' ).appendTo( self.$root ) ;

		var top_bar = window.AppManager.getWidget( "Top Bar" ) ;

		var $side_pane = $( '<div>' ).addClass( 'side_pane' ).appendTo( self.$root ) ;
		var $logo = $( '<div>' ).attr( 'id', 'logo' ).appendTo( $side_pane ) ;


		// Menu Structure
		var items = [
			{ name: "menu :: channels", submenu: [
					{ name: "menu :: channels :: programme guide",    screen: "Programme Guide", },
					{ name: "menu :: channels :: favorite channels", screen: "Favourite Channels", },
					//{ name: "menu :: channels :: programme reminder", screen: "Programme Reminder", },
					{ name: "menu :: channels :: programme reminder", screen: "Reminder Screen", },
				],
			},
			{ name: "menu :: on demand", screen:"on demand",
			},
			{ name: "menu :: account", submenu: [
					/*{name: "menu :: account :: my account", submenu:[
							{name:"menu :: account :: my account :: service info" , screen: "Service Info",},
							{name:"menu :: account :: my account :: statement" , screen: "Statement",},
					],
				 	},*/
				 	{name: "menu :: account :: my account",            screen: "TemAccount", },
					{name: "menu :: account :: message",            screen: "Email", },
					/*{name: "menu :: account :: ca system", submenu:[
							{name:"menu :: account :: ca system :: stb information" , screen:"Stb Information",},
							{name:"menu :: account :: ca system :: smart card" , screen:"Smart Card",},
							{name:"menu :: account :: ca system :: pin code" , screen:"PIN Code",},
							{name:"menu :: account :: ca system :: parental rating" , screen:"Parental Rating",},
							{name:"menu :: account :: ca system :: work time" , screen:"Work Time",},
							{name:"menu :: account :: ca system :: ca information" , screen:"CA Information",},
							//{name:"menu :: account :: ca system :: email" , screen:"Email",},
						],   
					},*/
				],
			},
			{ name: "menu :: app", screen:"APP",
			},
			{ name: "menu :: settings", submenu: [
					//{ name: "menu :: settings :: manual scan",      screen: "Manual Scan C", },
					{ name: "menu :: settings :: general settings", screen: "General Settings", },
					{ name: "menu :: settings :: parental lock",    screen: "Parental Lock", },
					{ name: "menu :: settings :: system info",      screen: "System Info", },
					{ name: "menu :: settings :: drives",           screen: "Drives", },
				],
			},
		] ;


		var $menu = $( '<div>' ).appendTo( $side_pane ) ;
		var menu = null ;

		var switchToMenu = function( submenu ) {
			menu && menu.$root.detach( ) ;
			menu = submenu ;
			submenu.$root.appendTo( $menu ) ;
			if(submenu.parent){
			submenu.select(1);
			}else{
			submenu.select(0);
			}
			
			submenu.show( ) ;
		}

		var main_menu = new window.Submenu( items ) ;

		self.onEnter = function( ) {
			switchToMenu( main_menu ) ;
		} ;

		self.onShow = function( ) {
			// show child widgets
			top_bar.show( ) ;
		} ;

		self.onHide = function( ) {
			// hide child widgets
			top_bar.hide( ) ;
		} ;

		self.onKey = function( ) {
			return true ;
		} ;

		self.onKeyUP = function( ) {
			menu.selectPrev( ) ;
		} ;

		self.onKeyDOWN = function( ) {
			menu.selectNext( ) ;
		} ;

		self.onKeyRIGHT =
		self.onKeyOK = function( ) {
			var action = menu.getAction( ) ;

			if( _.isFunction( action ) ) {
				action( ) ;
			}
			else {
				switchToMenu( action ) ;
			}
		} ;
		self.focusMail = function(){
			menu.select(2);
			var action = menu.getAction( ) ;
			if( _.isFunction( action ) ) {
				action( ) ;
			}
			else {
				switchToMenu( action ) ;
			}
			menu.selectNext();
		};
		self.focusReminder = function(){
			var action = menu.getAction( ) ;
			if( _.isFunction( action ) ) {
				action( ) ;
			}
			else {
				switchToMenu( action ) ;
			}
			menu.select(3);		
		}

		self.onKeyLEFT =
		self.onKeyBACK = function( ) {
			var parent_menu = menu.parent ;
			if( parent_menu ) {
				switchToMenu( parent_menu ) ;
			}
			else {
				window.AppManager.back( ) ;
			}
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, MainMenu ) ;

})( ) ;
