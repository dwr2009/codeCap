"use strict";
(function( ) {

	var name = "Top Bar" ;


	var TopBar = function( ) {

		var parent = new window.Widget( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'top_bar' ).appendTo( '#interface' ) ;

		var net_icon  = window.AppManager.getWidget( "Internet Icon" ) ;
		var date_time = window.AppManager.getWidget( "Date and Time" ) ;

		self.onShow = function( ) {
			// show child widgets
			net_icon.$root.appendTo( self.$root ) ;
			date_time.$root.appendTo( self.$root ) ;
			net_icon.show( ) ;
			date_time.show( ) ;
		} ;

		self.onHide = function( ) {
			// hide child widgets
			net_icon.hide( ) ;
			date_time.hide( ) ;
		} ;

		return self ;
	} ;

	window.AppManager.addWidget( name, TopBar ) ;

})( ) ;
