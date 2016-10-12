var SelectView = new window.Class(

	// parent
	window.Widget,

	// constructor
	function( description, callback ) {
		Widget.call( this, description.name ) ;

		this.$root.addClass( 'select_view' ) ;
		this.$panel = $( '<div>' ).addClass( 'panel' ).appendTo( this.$root ) ;
		this.$name  = $( '<div>' ).addClass( 'name' ).append( $TR( description.name ) ).appendTo( this.$panel ) ;
		this.$left  = $( '<div>' ).addClass( 'arrow left' ).appendTo( this.$panel ) ;
		this.$items = $( '<div>' ).addClass( 'items' ).appendTo( this.$panel ) ;
		this.$right = $( '<div>' ).addClass( 'arrow right' ).appendTo( this.$panel ) ;

		var items = description.items ;

		this.callback = callback ;
		this.selection = 0 ;

		this.delegates = [] ;

		this.updateItems(items);
	},

	// prototype
	{
		updateItems: function(items){
			this.items = items;
			this.delegates = [] ;
			this.$items.empty();
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
		getDefaultItemValue: function(items){
			this.items = items;
			var item = items[0];
			return item.value;
			},
			
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

				//this.callback && this.callback( this.getSelected( ) ) ;
			}
		},

		selectNext: function( ) {
			if( !this.active ) { return ; }
			console.log("items.length-1 ="+(this.items.length-1));
			var index = this.selection;
			var selectIndex = parseInt(index) + 1;
			console.log("selectIndex ="+selectIndex);
			if(selectIndex == (this.items.length)){
				selectIndex = 0;
			}
			this.changeSelection(selectIndex) ;
		},
		selectPrev: function( ) {
			if( !this.active ) { return ; }
			var index = this.selection;
			var selectIndex = parseInt(index)-1;
			console.log("selectIndex ="+selectIndex);
			if(selectIndex == -1){
				selectIndex = (this.items.length-1);
			}
			this.changeSelection(selectIndex) ;
		},

		changeSelection: function( selection ) {
			this.selection = selection ;
			this.callback && this.callback( this.getSelected( ) ) ;
			this._update( );
		},

		getSelected: function( ) {
			return this.delegates[this.selection].item ;
		},
		
		getSelectedIndex: function( ) {
			return this.selection ;
		},
		getItemByIndex: function( index ){
			return this.delegates[index].item;
		}	,
		
		selectValue: function( value ) {
			for( var i in this.items ) {
				if( value == this.items[i].value ) {
					this.selection = i ;
					this._update( ) ;
					return true ;
				}
			}
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