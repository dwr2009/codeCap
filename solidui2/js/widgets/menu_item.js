"use strict";
(function( ) {

	var MenuItem = window.Class(

		// parent
		window.ListViewDelegate,

		// constructor
		function( item, menu ) {
			var name = item.name ;
			ListViewDelegate.call( this, "-> " + name ) ;

			this.$root.addClass( 'main_menu_item' ) ;
			if( item.back ) {
				this.$root.addClass( 'back' ) ;
			}

			var $arrow = $( '<div>' ).addClass( 'arrow' ).appendTo( this.$root ) ;

			var $text = $( '<div>' ).addClass( 'text' ).append( $TR( name ) ).appendTo( this.$root ) ;


			var action = null ;
			if( item.submenu ) {
				action = new Submenu( item.submenu, menu, name ) ;
			}
			if( item.back ) {
				action = item.back ;
			}
			else if( item.screen ) {
				action = (function( name ) {
					return function( ) {
						window.AppManager.enter( name ) ;
					} ;
				})( item.screen ) ;
			}

			this.action = action ;
		},

		// prototype
		{}

	) ;

	window.MenuItem = MenuItem ;

})( ) ;
