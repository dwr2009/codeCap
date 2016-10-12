"use strict";
(function( ) {

	var printKeyInformation = function( widget ) {
		var all_keys = _.values( Platform.keymap ) ;
		var existing = _.filter( all_keys, function( key_name ) {
			var handler = "onKey" + key_name ;
			return _.isFunction( widget[handler] ) ;
		} ) ;
		console.log( "Widget '" + widget.name + "' responds to keys:", existing ) ;
	} ;


	var Scene = window.Class(

		// parent
		window.Widget,

		// constructor
		function( name ) {
			Widget.call( this, name ) ;

			this.$root.addClass( 'scene' ) ;

			this._is_active = false ;
		},

		// prototype
		{
			_setActive: function( flag ) {
				this._is_active = flag ;
				if( flag ) {
					this.onActivate && this.onActivate( ) ;
				}
				else {
					this.onDeactivate && this.onDeactivate( ) ;
				}
			},

			enter: function( ) {
				if( this._is_active ) { return ; }
				this._setActive( true ) ;

				this.onEnter && this.onEnter( ) ;
				this.show( ) ;

				printKeyInformation( this ) ;
			},

			exit: function( ) {
				if( !this._is_active ) { return ; }
				this._setActive( false ) ;

				this.onExit && this.onExit( ) ;
				this.hide( ) ;
			},

			leave: function( ) {
				if( !this._is_active ) { return ; }
				this._setActive( false ) ;

				this.onLeave && this.onLeave( ) ;
			},

			return: function( ) {
				if( this._is_active ) { return ; }
				this._setActive( true ) ;

				this.onReturn && this.onReturn( ) ;

				printKeyInformation( this ) ;
			},

			isActive: function( ) {
				return this._is_active ;
			},

			onKeyEXIT: function( ) {
				window.App.goToHome( ) ;
				return true ;
			},

			onKeyMENU: function( ) {
				window.App.goToMainMenu( ) ;
				return true ;
			},

			onKeyBACK: function( ) {
				window.AppManager.back( ) ;
				return true ;
			},
		}

	) ;

	window.Scene = Scene ;

})( ) ;
