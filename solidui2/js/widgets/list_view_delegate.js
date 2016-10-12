"use strict";
(function( ) {

	var ListViewDelegate = window.Class(

		// parent
		window.Widget,

		// constructor
		function( name ) {
			Widget.call( this, name ) ;

			this.$root.addClass( 'list_view_delegate' ) ;
			this.selected = false ;
		},

		// prototype
		{
			select: function( flag ) {
				if( flag == this.selected ) {
					return ;
				}

				this.selected = flag ;
				this.$root.toggleClass( 'selected', flag ) ;

				if( flag ) {
					this.onSelect && this.onSelect( ) ;
				}
				else {
					this.onDeselect && this.onDeselect( ) ;
				}
			},
		}

	) ;

	window.ListViewDelegate = ListViewDelegate ;

})( ) ;
