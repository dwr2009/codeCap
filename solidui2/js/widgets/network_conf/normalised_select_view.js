"use strict";
(function( ) {
// this is a select view whit some changes so that mono_list can and list manager can handel him
// this was needed for ipv6 since ipv6 isn't needed this also isn't but i let it stay in case ipv6
// will be implemented one day

	var normalised_select_view = new window.Class(

	// parent
	window.Widget,

	/*
	var sample_description =
	{
		name: "sample_name",
		action: some_callback_function,
		items: [
			// inner items
			{ name: 'nettwork settings screen :: ipv4 and ipv6', value: 'ipv4 and ipv6', },
                        { name: 'nettwork settings screen :: ipv4', value: 'ipv4', },
      	                { name: 'nettwork settings screen :: ipv6', value: 'ipv6', },
			],
	}
	*/

	// constructor
	function( description ) {
		Widget.call( this, description.name ) ;

		this.$root.addClass( 'select_view' ) ;
		this.$panel = $( '<div>' ).addClass( 'panel' ).appendTo( this.$root ) ;
		this.$name  = $( '<div>' ).addClass( 'name' ).append( $TR( description.name ) ).appendTo( this.$panel ) ;
		this.$left  = $( '<div>' ).addClass( 'arrow left' ).appendTo( this.$panel ) ;
		this.$items = $( '<div>' ).addClass( 'items' ).appendTo( this.$panel ) ;
		this.$right = $( '<div>' ).addClass( 'arrow right' ).appendTo( this.$panel ) ;

		var items = description.items ;
		this.items = items ;
		this.callback = description.action ;
		this.selection = 0 ;

		this.delegates = [] ;

		for( var i in items ) {
			var item = items[i] ;

			var delegate = {
				item: item,
				$root: $( '<span>' ).addClass( 'item' ).append( $TR( item.name ) ).appendTo( this.$items ),
			} ;

			this.delegates.push( delegate ) ;
		}

		this._update( ) ;
	},

	// prototype
	{
		activate: function( flag ) {
			if( flag == this.active ) { return ; }

			this.active = flag ;
			this.$root.toggleClass( 'active', flag ) ;

			if( flag ) {
				this.last_selection = this.selection ;
			}
			else {
				if( this.selection == this.last_selection ) { return ; }
				this.last_selection = undefined ;

				this.callback && this.callback( this.getSelected( ) ) ;
			}
		},

		selectNext: function( ) {
			if( !this.active ) { return ; }
			this.changeSelection( Math.min( this.selection + 1, this.items.length-1 ) ) ;
		},
		selectPrev: function( ) {
			if( !this.active ) { return ; }

			this.changeSelection( Math.max( this.selection - 1, 0 ) ) ;
		},

		changeSelection: function( selection ) {
			if( selection != this.selection ) {
				this.selection = selection ;
				this._update( ) ;
			}
		},

		getSelected: function( ) {
			return this.delegates[this.selection].item ;
		},

		selectItem: function( item ) {
			for( var i in this.items ) {
				if( item == this.items[i] ) {
					this.selection = i ;
					this._update( ) ;
					return true ;
				}
			}
		},

		_update: function( ) {
			for( var i in this.delegates ) {
				var delegate = this.delegates[i] ;

				delegate.$root.toggle( i == this.selection ) ;
			}
		},
	}
) ;

window.normalised_select_view = normalised_select_view;

})( ) ;

