"use strict";
(function( ) {

	var ListView = window.Class(

		// parent
		window.Widget,

		// constructor
		function( Delegate, items ) {
			Widget.call( this, "List View" ) ;

			this.$root.addClass( 'list_view' ) ;
			this.$items = $( '<div>' ).appendTo( this.$root ) ;

			this.Delegate = Delegate ;
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

			select: function( index ) {
				this.selection = Math.max( 0, Math.min( this.length, index ) ) ;
				this._update( ) ;
			},
			getItemByName:function(itemName){
				for(var i = 0; i < this.length; i++){
					if(this.delegates[i].name == itemName){
						return this.delegates[i];
					}
				}
				return null;
			},			
			getItemByIndex:function(index){
					return this.delegates[index];
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

				for( var i in items ) {
					var item = items[i] ;

					if( item == last_item ) {
						this.selection = i ;
					}

					var delegate = new this.Delegate( item ) ;
					delegate.$root.appendTo( this.$items ) ;
					delegate.show( ) ;
					this.delegates.push( delegate ) ;
				}

				this._update( ) ;
			},

			_update: function( ) {
				for( var i in this.delegates ) {
					var delegate = this.delegates[i] ;
					delegate.select( i == this.selection ) ;
				}
			},
		}

	) ;

	window.ListView = ListView ;

})( ) ;
