"use strict";
(function( ) {

	var name = "scrollbar" ;


	var scrollbar = function( ) {

		var parent = new window.Widget( name ) ;
		var self = Object.create( parent ) ;
		
		var scrollbarMax = 100 ;
		var scrollbarCurrent = 50;		
		
		var $scrollbar = $( '<div>' ).attr( 'id', 'scrollbar' ).appendTo( self.$root ) ;
		
		var $scrollbartrack = $( '<div>' ).addClass( 'scrollbartrack' ).appendTo( $scrollbar ) ;
		
		var $scrollbarthumb = $( '<div>' ).attr( 'id', 'scrollbarthumb' ).addClass( 'scrollbarthumb' ).appendTo( $scrollbar ) ;
	

		self.update = function( ) {
			if(scrollbarMax == 0){
				var scrollbarCurrent_value = 0;
			}
			else{
				var scrollbarCurrent_value = Math.floor(scrollbarCurrent * 100 / scrollbarMax );
			}
			//console.log( "scrollbar.update -> scrollbarCurrent_value = " + scrollbarCurrent_value);
			$('#scrollbarthumb').css("top",scrollbarCurrent_value+"%");
		}
		self.update();	

	
		self.setCurrent = function( current ) {	
			scrollbarCurrent = current;
		}
		self.setMax = function( max ) {		
			scrollbarMax = max;
		}
	
	

		return self ;
	} ;

	window.AppManager.addWidget( name, scrollbar ) ;

})( ) ;
