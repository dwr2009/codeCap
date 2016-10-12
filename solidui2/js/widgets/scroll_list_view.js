"use strict";
(function( ) {

	var scrollListView = window.Class(

		// parent
		window.Widget,

		// constructor
		function( Delegate, items ) {
			Widget.call( this, "Scroll List View" ) ;

			this.$root.addClass( 'scroll_list_view' ) ;
			
			this.$items = $( '<div>' ).appendTo( this.$root ) ;
			this.$bigScroll = $( '<div>' ).addClass('bigScroll').appendTo( this.$root ) ;
			this.$smallScroll = $( '<div>' ).addClass('smallScroll').appendTo( this.$bigScroll ) ;
			
			this.Delegate = Delegate ;
			this.updateItems( items || [] ) ;
			
			this.scrollFunction = false ;
			this.defaultItemsNum = 10 ;
			this.showItemsNum = this.defaultItemsNum ;
			
      this.visibleItemFront = 0 ;
			this.visibleItemEnd = this.showItemsNum - 1 ;
			
			this.callbackUp = null;
			this.callbackDown = null;
			
			this.smallScroll_top = 0;
			
			this.getBigScrollHeight = function(  )
			{
				this.big_scrollHeight = this.$bigScroll.css("height");
				this.bigScroll_height_int = parseInt(this.big_scrollHeight);
				
				return this.bigScroll_height_int;
			};
			
			this.getSmallScrollHeight = function(  )
			{
				this.small_scrollHeight = this.$smallScroll.css("height");
				this.smallScroll_height_int = parseInt(this.small_scrollHeight);
				
				return this.smallScroll_height_int;
			};
			
			this.getItemsHeight = function( )
			{
				this.items_Height = this.$items.css("height");
				this.items_height_int = parseInt(this.items_Height);
				
				return this.items_height_int;
			};
		},

		// prototype
		{
				select: function( index ) {
					this.selection = Math.max( 0, Math.min( this.length, index ) ) ;
					this._update( ) ;
				},
				
				pageUpAndDown: function( index ) {
					var defaultItemsNum_init = this.getDefaultItemsNum();
		    	var itemsHeight = this.getItemsHeight( );
					this.select(index);
					
					if( this.length > 0){
	    			var every_itemsHeight = itemsHeight / ( this.visibleItemEnd - this.visibleItemFront + 1);
						var big_height = defaultItemsNum_init * every_itemsHeight;
					
						this.$bigScroll.css("height",big_height);
						this.$bigScroll.css("top", -itemsHeight ) ;
						
						if( this.length > defaultItemsNum_init ){
							var height = (defaultItemsNum_init / this.length * 100) + "%";
							this.$smallScroll.css("height", height );
							
							var _height = parseFloat(height);
							
							if( index > defaultItemsNum_init - 1 ){
								var _base =( 1 / this.length ) * 100 ;
								var smallScroll_top =( index - defaultItemsNum_init + 1) * _base;
								this.smallScroll_top = smallScroll_top + "%";
								this.$smallScroll.css("top", this.smallScroll_top );
							}
							else{
								this.$smallScroll.css("top", 0 );
							}
						}
						else{
							this.$smallScroll.css("top", 0 );
							var BigScrollHeight = this.getBigScrollHeight();
							this.$smallScroll.css("height", BigScrollHeight - 4 );
							var SmallScrollHeight = this.getSmallScrollHeight();
						}
					}
					else{
							this.visibleItemEnd = 0 ;
							var height = 100 + "%";
							this.$bigScroll.css("height", height);
							this.$bigScroll.css("top", 0 ) ;
							this.$smallScroll.css("height", 0 );
							this.$smallScroll.css("top", 0 );
					}
				},
				
				getScrollUp : function ( value ) {
					var itemsHeight = this.getItemsHeight( );
					this.$bigScroll.css("top", -itemsHeight ) ;
					var BigScrollHeight = this.getBigScrollHeight();
					var SmallScrollHeight = this.getSmallScrollHeight();
					
					if( value == "bottom")
					{
						var top = ( BigScrollHeight - SmallScrollHeight ) - 4 ;
						this.$smallScroll.css ("top", top ) ;
						var smallScroll_bottom = ( 1/this.length) * (this.length - this.defaultItemsNum ) * 100;
						this.smallScroll_top = smallScroll_bottom;
					}
					else {
						this.smallScroll_top = parseFloat(this.smallScroll_top);
						this.smallScroll_top -= ( 1/this.length  * 100 );
						var top = this.smallScroll_top + "%";
						this.$smallScroll.css ("top", top ) ;
					}
				},
				
				getScrollDown : function ( value ) {
					var defaultItemsNum_init = this.getDefaultItemsNum();
					var itemsHeight = this.getItemsHeight( );
					this.$bigScroll.css("top", -itemsHeight ) ;
					
					if ( value == "top")
					{
						this.$smallScroll.css ("top", 0 ) ;
						this.smallScroll_top = 0;
					}
					else {
						this.smallScroll_top = parseFloat(this.smallScroll_top);
						this.smallScroll_top +=( 1 / this.length * 100);
						var top = this.smallScroll_top + "%";
						this.$smallScroll.css ("top", top);
					}
				},
				
				setDefaultItemsNum: function (defaultItemsNum) {
					this.defaultItemsNum = defaultItemsNum;
				},
				
			  getDefaultItemsNum : function ( ) {
			  	  return this.defaultItemsNum;
			  },
			  
			  getVisibleItemFront: function(){
					return this.visibleItemFront;
				},
				
				getVisibleItemEnd: function(){
					return this.visibleItemEnd;
				},
				
				getPageCount: function(){
					return this.pageCount;
				},
				
		    length : function ( ) {
		        return this.delegates.length ;
		    },

		    getItems: function ( ) {
		        return this.delegates;
		    },		    
		    
		    getAllItems: function ( ) {
		        return this.items;
		    },
		    
		    getItem: function ( index ) {
		        return this.delegates[index] ;
		    },
		    
		    getItemData: function( index ){
		    	return this.items[index];
		    },
		    
		    setItemData: function( index,data){
		    	this.items[index] = data; 
		    },
		    
				selectNext: function( ) {
					var defaultItemsNum = this.getDefaultItemsNum();
					
					var selection = (this.selection + 1) % this.length ;
					this.select( selection ) ;
					
					this.item_update("select_next");
					if( this.selection == 0 ){
						this.getScrollDown("top");
					}
					else if(this.selection > defaultItemsNum - 1){
						this.getScrollDown();
					}
					else{
						return  ;
					}
				},

				selectPrev: function( ) {
					var defaultItemsNum = this.getDefaultItemsNum();
					var selection = (this.selection + this.length - 1) % this.length ;
					this.select( selection ) ;
					
					this.item_update("select_prev");
					
					if(this.selection == this.length - 1){
						this.getScrollUp("bottom");
					}
					else if(this.selection >= defaultItemsNum - 1 ) {
						this.getScrollUp();
					}
					else{
						return  ;
					}
				},
				
				getPagePrev: function( ) {
						this.item_update("page_prev");
						this.pageUpAndDown(this.selection);
				},
				
				getPageNext: function( ) {
						this.item_update("page_next");
						this.pageUpAndDown(this.selection);
				},				
	
				getSelected: function( ) {
					var defaultItemsNum = this.getDefaultItemsNum();
					var _select = this.selection % defaultItemsNum ;
					return this.delegates[_select] ;
				},
				
				getSelectedIndex: function( ) {
					return this.selection ;
				},
				
				getItemByIndex: function( index ){
					this.selection = index;
				}	,	
				
				updateItems: function( items,index ) {
					var defaultItemsNum = this.getDefaultItemsNum();
					this.items = items ;
					this.length = items.length ;
					this.delegates = [] ;
					this.$items.empty( ) ;
					this.selection = index ;
					
					this.quotient = Math.ceil( this.length / this.defaultItemsNum );
		    	this.pageCount = Math.floor( index / defaultItemsNum ) + 1 ;
		    	
		    	this.visibleItemFront = ( this.pageCount - 1 ) * defaultItemsNum ;
		    	
					if( this.pageCount != this.quotient ){
	    			this.visibleItemEnd = this.visibleItemFront + defaultItemsNum - 1 ;
	    		}
	    		else{
	    			this.visibleItemEnd = this.length - 1 ;
	    		}
					if(items.length < defaultItemsNum){
						for( var i = 0; i < items.length ; i++ ) {
							var item = items[i] ;
							
							var delegate = new this.Delegate( item ) ;
							delegate.$root.appendTo( this.$items ) ;
							delegate.show( ) ;
							this.delegates.push( delegate ) ;
						}
					}
					else{
						for( var i = this.visibleItemFront; i <= this.visibleItemEnd; i++ ) {
						
							var item = this.items[i] ;
							var delegate = new this.Delegate( item ) ;
							delegate.$root.appendTo( this.$items ) ;
							delegate.show( ) ;
							this.delegates.push( delegate ) ;
						}
					}
					this._update( ) ;
					
					var items_Height = this.$items.css("height");
					var itemsHeight = parseInt(items_Height);
					this.select(this.selection);
					if( this.length > 0){
	    			var every_itemsHeight = itemsHeight / ( this.visibleItemEnd - this.visibleItemFront + 1);
						var big_height = defaultItemsNum * every_itemsHeight;
						this.$bigScroll.css("height",big_height);
						this.$bigScroll.css("top", -itemsHeight ) ;
						
						if( this.length > defaultItemsNum ){
							var height = (defaultItemsNum / this.length * 100) + "%";
							this.$smallScroll.css("height", height );
							
							var _height = parseFloat(height);
							
							if( index > defaultItemsNum - 1 ){
								var _base =( 1 / this.length ) * 100 ;
								var smallScroll_top =( index - defaultItemsNum + 1) * _base;
								this.smallScroll_top = smallScroll_top + "%";
								this.$smallScroll.css("top", this.smallScroll_top );
							}
							else{
								this.smallScroll_top = 0;
								this.$smallScroll.css("top", this.smallScroll_top );
							}
						}
						else{
							this.$smallScroll.css("top", 0 );
							var BigScrollHeight = this.getBigScrollHeight();
							this.$smallScroll.css("height", BigScrollHeight - 4 );
							var SmallScrollHeight = this.getSmallScrollHeight();
						}
					}
					else{
							this.visibleItemEnd = 0 ;
							var height = 100 + "%";
							this.$bigScroll.css("height", height);
							this.$bigScroll.css("top", 0 ) ;
							this.$smallScroll.css("height", 0 );
							this.$smallScroll.css("top", 0 );
					}
				},
				
				item_update: function( value ){
					var defaultItemsNum = this.getDefaultItemsNum();
					
					if(0 <= this.length && this.length <= defaultItemsNum){
						if(value == 'page_prev' || value == 'page_next' || value == 'select_prev' || value == 'select_next'){
							return ;
						}
					}
					else{
						if(this.pageCount == this.quotient){
							if(value == 'page_next'){
								return ;
							}
							else if(value == 'page_prev'){
								this.$items.empty( ) ;
								
								this.visibleItemFront -= defaultItemsNum;
								this.visibleItemEnd = this.visibleItemFront + defaultItemsNum - 1;
								
								var selection = this.visibleItemFront;
								this.select( selection ) ;
								
								this.pageCount -= 1;
							}
							else if(value == 'select_prev'){
								if(this.selection == this.visibleItemFront - 1){
									this.$items.empty( ) ;
									
									this.visibleItemFront -= defaultItemsNum;
									this.visibleItemEnd = this.visibleItemFront + defaultItemsNum - 1;
									
									var selection = this.visibleItemEnd;
									this.select( selection ) ;
									
									this.pageCount -= 1;
								}
								else{
									return ;
								}
							}
							else{
								if(this.selection == 0){
									this.$items.empty( ) ;
									
									this.visibleItemFront = 0;
									this.visibleItemEnd = this.visibleItemFront + defaultItemsNum - 1;
									
									this.selection = this.visibleItemFront;
									this.select( this.selection ) ;
									
									this.pageCount = 1;
									
								}
								else{
									return ;
								}
							}
						}
						
						else if(this.pageCount == 1){
							if(value == 'page_prev'){
								return ;
							}
							else if(value == 'page_next'){
								this.$items.empty( ) ;
								if(this.pageCount + 1 == this.quotient){
									
									this.visibleItemFront += defaultItemsNum;
									this.visibleItemEnd = this.length - 1;
									
									var selection = this.visibleItemFront;
									this.select( selection ) ;
								}
								else{
									this.visibleItemFront += defaultItemsNum;
									this.visibleItemEnd += defaultItemsNum;
									
									var selection = this.visibleItemFront;
									this.select( selection ) ;
								}
								this.pageCount += 1;
							}
							else if(value == 'select_prev'){
								if(this.selection == this.length - 1){
									this.$items.empty( ) ;
									
									this.pageCount = this.quotient;
									this.visibleItemFront = ( this.pageCount - 1 ) * defaultItemsNum ;
									this.visibleItemEnd = this.length - 1;
									
									var selection = this.visibleItemEnd;
									this.select( selection ) ;
								}
								else{
									return ;
								}
							}
							else{
								if(this.selection == this.visibleItemEnd + 1){
									this.$items.empty( ) ;
									
									if(this.pageCount + 1 == this.quotient){
										this.visibleItemFront += defaultItemsNum;
										this.visibleItemEnd = this.length - 1;
										
										var selection = this.visibleItemFront;
										this.select( selection ) ;
									}
									else{
										this.visibleItemFront += defaultItemsNum;
										this.visibleItemEnd = this.visibleItemEnd + defaultItemsNum;
										
										var selection = this.visibleItemFront;
										this.select( selection ) ;
									}
									this.pageCount += 1;
								}
								else{
									return ;
								}
							}
						}
						
						else{
							if(value == 'page_prev'){
								this.$items.empty( ) ;
								
								this.visibleItemFront -= defaultItemsNum;
								this.visibleItemEnd -= defaultItemsNum;
								
								
								var selection = this.visibleItemFront;
								this.select( selection ) ;
								
								this.pageCount -= 1;
							}
							else if(value == 'page_next'){
								this.$items.empty( ) ;
								
								if(this.pageCount + 1 == this.quotient){
									this.visibleItemFront += defaultItemsNum;
									this.visibleItemEnd = this.length - 1;
									
									var selection = this.visibleItemFront;
									this.select( selection ) ;
								}
								else{
									this.visibleItemFront += defaultItemsNum;
									this.visibleItemEnd += defaultItemsNum;
									
									var selection = this.visibleItemFront;
									this.select( selection ) ;
								}
								this.pageCount += 1;
							}
							else if(value == 'select_prev'){
								if(this.selection == this.visibleItemFront - 1){
									this.$items.empty( ) ;
									
									this.visibleItemFront -= defaultItemsNum;
									this.visibleItemEnd = this.visibleItemFront + defaultItemsNum - 1;
									
									var selection = this.visibleItemEnd;
									this.select( selection ) ;
									
									this.pageCount -= 1;
								}
								else{
									return ;
								}
							}
							else{
								if(this.selection == this.visibleItemEnd + 1){
									this.$items.empty( ) ;
									
									if(this.pageCount + 1 == this.quotient){
										this.visibleItemFront += defaultItemsNum;
										this.visibleItemEnd = this.length - 1;
										
										var selection = this.visibleItemFront;
										this.select( selection ) ;
									}
									else{
										this.visibleItemFront += defaultItemsNum;
										this.visibleItemEnd = this.visibleItemEnd + defaultItemsNum;
										
										var selection = this.visibleItemFront;
										this.select( selection ) ;
									}
									this.pageCount += 1;
								}
								else{
									return ;
								}
							}
						}
					}
					
					this.delegates = [];
					for( var i = this.visibleItemFront; i <= this.visibleItemEnd; i++ ) {
						
						var item = this.items[i] ;
						var delegate = new this.Delegate( item ) ;
						delegate.$root.appendTo( this.$items ) ;
						delegate.show( ) ;
						this.delegates.push( delegate ) ;
					}
					
					this._update( ) ;
				},
				
				_update: function( ) {
					var defaultItemsNum = this.getDefaultItemsNum();
					for( var i in this.delegates ) {
						var delegate = this.delegates[i] ;
						var temp = this.selection % defaultItemsNum ;
						delegate.select( i == temp ) ;
					}
				},
		}

	) ;

	window.scrollListView = scrollListView ;

})( ) ;
