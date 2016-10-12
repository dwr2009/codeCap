"use strict";
(function( ) {

	 // temp
	var makePlaceholder = function( text ) {
		var $root = $( '<div>' ).addClass( 'widget' ) ;
		var $text = window.$TR( text ).addClass( 'widget_name' ).appendTo( $root ) ;
		$root.hide( ).appendTo( '#interface' ) ;
		return $root ;
	} ;


	var Widget = window.Class(

		// parent
		Object,

		// constructor
		function( name ) {
			this.name = name ;

			this._is_visible = false ;

			this.$root = makePlaceholder( name ) ; // temp
		},

		// prototype
		{
			show: function( ) {
				if( this._is_visible ) { return ; }
				this._is_visible = true ;

				this.$root.show( ) ;
				this.onShow && this.onShow( ) ;
			},

			hide: function( ) {
				if( !this._is_visible ) { return ; }
				this._is_visible = false ;

				this.$root.hide( ) ;
				this.onHide && this.onHide( ) ;
			},

			isVisible: function( ) {
				return this._is_visible ;
			},
		}

	) ;

	window.Widget = Widget ;

})( ) ;
