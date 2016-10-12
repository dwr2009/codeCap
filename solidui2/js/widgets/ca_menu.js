"use strict";
(function( ) {

	var name = "CA Menu" ;


	var CAMenu = function( ) {

		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'ca_menu' ) ;

		//var $overlay = $( '<div>' ).addClass( 'overlay' ).appendTo( self.$root ) ;

		var top_bar = window.AppManager.getWidget( "Top Bar" ) ;

		var $side_pane = $( '<div>' ).addClass( 'side_pane' ).appendTo( self.$root ) ;
		var $logo = $( '<div>' ).attr( 'id', 'logo' ).appendTo( $side_pane ) ;


		// Menu Structure
		var items = [
				{ name: "menu :: settings :: manual scan",      screen: "Manual Scan C", },			
				{name:"menu :: account :: ca system :: stb information" , screen:"Stb Information",},
				{name:"menu :: account :: ca system :: smart card" , screen:"Smart Card",},
				{name:"menu :: account :: ca system :: pin code" , screen:"PIN Code",},
				{name:"menu :: account :: ca system :: parental rating" , screen:"Parental Rating",},
				{name:"menu :: account :: ca system :: work time" , screen:"Work Time",},
				{name:"menu :: account :: ca system :: ca information" , screen:"CA Information",},
				//{name:"menu :: account :: ca system :: email" , screen:"Email",},
		] ;
			//var smcStatus = new window.popDlg();
			//smcStatus.$root.appendTo(self.$root);
			//smcStatus.show();	

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

		var ca_menu = new window.Submenu( items ) ;
		
		var checkSmcStatus = function(){
				var smc = Platform.ca.GetCardStatus();
				console.log("menu.getSeletedIndex() = "+ menu.getSeletedIndex());
				console.log("smc.cardStatus = "+ smc.cardStatus);
				if(0 < menu.getSeletedIndex() && menu.getSeletedIndex() < 7 && smc.cardStatus == 0){
					//smcStatus.showSmcStatus();
					console.log("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
					return 0;
					}
					else {
					console.log("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
					return 1;	
					}	
			}
		self.onEnter = function( ) {
			switchToMenu( ca_menu ) ;
		} ;
		
		self.onActivate = function( ) {
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
			if(checkSmcStatus()){
				var action = menu.getAction( ) ;
				if( _.isFunction( action ) ) {
					action( ) ;
				}
				else {
					switchToMenu( action ) ;
				}
			}
		} ;

		self.onKeyLEFT =
		self.onKeyBACK = function( ) {
				window.AppManager.back( ) ;
				window.App.goToMainMenu( ) ;		
		} ;
		
		return self ;
	} ;

	window.AppManager.addScene( name, CAMenu ) ;

})( ) ;
