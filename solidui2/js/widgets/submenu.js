"use strict";
(function( ) {

	var Submenu = window.Class(

		// parent
		window.Widget,

		// constructor
		function( items, parent, name ) {
			Widget.call( this, name || "Main" ) ;

			this.parent = parent ;

			var self = this;
			var Delegate = function( item ) {
				return new window.MenuItem( item, self ) ;
			} ;

			if( parent ) {
				var parent_item = {
					name: name + " :: back", 
					back: parent,
				} ;

				var _items = items.slice( 0 ) ;
				_items.unshift( parent_item ) ;
				items = _items ;
			}

			var list_view = new window.ListView( Delegate, items ) ;
			list_view.$root.appendTo( this.$root ) ;
			list_view.show( ) ;
			this.list_view = list_view ;
		},

		// prototype
		{
			selectNext: function( ) {
				this.list_view.selectNext( ) ;
			},

			selectPrev: function( ) {
				this.list_view.selectPrev( ) ;
			},
			getSeletedIndex:function(){
				return this.list_view.getSeletedIndex();
			},

			select: function( index ) {
				this.list_view.select( index ) ;
			},

			getAction: function( ) {
				return this.list_view.getSelected( ).action ;
			},

		}

	) ;

	window.Submenu = Submenu ;

})( ) ;
