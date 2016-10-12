"use strict";
(function( ) {


	var Delegate = new window.Class(

		// parent
		window.ListViewDelegate,

		// constructor
		function( description ) {
			ListViewDelegate.call( this, description.name ) ;

			var select_view = new window.SelectView( description, description.action ) ;
			select_view.$root.appendTo( this.$root ) ;
			select_view.show( ) ;

			this.select_view = select_view ;
		},

		// prototype
		{}
	) ;


	var Settings = window.Class(

		// parent
		window.Widget,

		// constructor
		function( descriptions ) {
			Widget.call( this, "Settings" ) ;

			this.$root.addClass( 'settings' ) ;

			var list_view = new window.ListView( Delegate, descriptions ) ;
			list_view.$root.appendTo( this.$root ) ;
			list_view.show( ) ;

			this.list_view = list_view ;
		},

		// prototype
		{}
	) ;

	window.Settings = Settings ;

})( ) ;
