"use strict";
(function( ) {

	var name = "Timeshift Icon" ;


	var TimeshiftIcon = function( ) {

		var parent = new window.Widget( name ) ;
		var self = Object.create( parent ) ;
		var autohide_intervall = 10000;

		self.$root.attr( 'id', 'timeshift_icon' ).appendTo( '#interface' ) ;

		var $timeshift_icon = $( '<div>' ).addClass( 'timeshift_icon' ).appendTo( self.$root ) ;

		self.onShow = function( ) {
			console.log( "-> onShow()") ;
		} ;

		self.onHide = function( ) {
			console.log( "-> onHide()") ;
		} ;

		return self ;
	} ;

	window.AppManager.addWidget( name, TimeshiftIcon ) ;

})( ) ;