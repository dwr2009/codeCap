"use strict";
(function( ) {

	var FocusList = window.Class(

		// parent
		window.Widget,

		// constructor
		function( items ) {
			Widget.call( this, "Focus List" ) ;

			this.$root.addClass( 'focus_list' ) ;
			this.$items = $( '<div>' ).appendTo( this.$root ) ;
			this.updateItems( items || [] ) ;
		},

		// prototype
		{
			selectNext: function( ) {
				var selection = (this.selection + 1) % this.length ;
				this.select( selection ) ;
			},

			selectPrev: function( ) {
				var selection = (this.selection + this.length - 1) % this.length ;
				this.select( selection ) ;
			},
			getSelectedName: function( ) {
				return this.delegates[this.selection].name ;
			},
			select: function( index ) {
				this.selection = Math.max( 0, Math.min( this.length, index ) ) ;
				this._update( ) ;
			},

			getSelected: function( ) {
				return this.delegates[this.selection] ;
			},
			
			getSeletedIndex: function( ) {
				return this.selection;
			},

			updateItems: function( items ) {
				var last_item = null ;
				if( this.items ) {
					last_item = this.items[this.selection] ;
				}

				this.items = items ;
				this.length = items.length ;
				this.delegates = [] ;
				this.$items.empty( ) ;
				this.selection = 0 ;

				for( var i = 0; i < items.length; i++ ) {
					var item = items[i] ; 
					if( item == last_item ) {
						this.selection = i ;
					}

					this.delegates.push( item ) ;
				}

				this._update( ) ;
			},

			_update: function( ) {
				for( var i in this.delegates ) {
					var delegate = this.delegates[i] ;
					delegate.$root.toggleClass( 'selected', i == this.selection ) ;
					delegate.callback && delegate.callback( (i == this.selection) ) ;
				}
			}
		}

	) ;

	window.FocusList = FocusList ;

})( ) ;
