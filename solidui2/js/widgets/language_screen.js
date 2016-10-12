"use strict";
(function( ) {

	var name = "Language" ;


	var Language = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;


		var switchToLanguage = function( item ) {
			console.log( "Selected language:", item.name ) ;
			var code = item.value ;
			window.Translation.setLanguage( code ) ;
		} ;

		var description = [
			{
				name: "language screen :: language",
				items: [
					{ name: 'language screen :: english', value: 'en', },
					{ name: 'language screen :: german',  value: 'de', },
					{ name: 'language screen :: chinese', value: 'ch', },
				],
				action: switchToLanguage,
			},
		] ;

		var settings = new window.Settings( description ) ;
		settings.$root.appendTo( self.$root ) ;
		settings.show( ) ;

		var list_view = settings.list_view ;
		var select_view = null ;


		self.onKey = function( ) {
			return true ;
		} ;

		self.onKeyUP = function( ) {
			if( select_view ) { return ; }

			list_view.selectPrev( ) ;
		} ;

		self.onKeyDOWN = function( ) {
			if( select_view ) { return ; }

			list_view.selectNext( ) ;
		} ;

		self.onKeyOK = function( ) {
			if( !select_view ) {
				select_view = list_view.getSelected( ).select_view ;
				select_view.activate( true ) ;
			}
			else {
				select_view.activate( false ) ;
				select_view = null ;
			}
		} ;

		self.onKeyLEFT = function( ) {
			if( !select_view ) {
				window.AppManager.back( ) ;
				return ;
			}

			select_view.selectPrev( ) ;
		} ;

		self.onKeyRIGHT = function( ) {
			if( !select_view ) { return ; }

			select_view.selectNext( ) ;
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, Language ) ;

})( ) ;
