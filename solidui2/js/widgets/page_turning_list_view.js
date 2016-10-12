"use strict";
(function( ) {

	var pageTurningListView = window.Class(

		// parent
		window.Widget,

		// constructor
		function( Delegate, items ) {
			Widget.call( this, "Page Turning List View" ) ;

			this.$root.addClass( 'page_turning_list_view' ) ;
			this.$items = $( '<div>' ).appendTo( this.$root ) ;
			
			this.Delegate = Delegate ;
			this.updateItems( items || [] ) ;
			
			this.scrollFunction = false ;
			this.defaultItemsNum = 8 ;
			this.showItemsNum = this.defaultItemsNum ;
      this.visibleItemFront = 0 ;
			this.visibleItemEnd = this.showItemsNum - 1 ;
			
			this.pageCount = 1 ;
			this.pageChangeCount = this.defaultItemsNum ;
			
			this.$count = $('<div>').addClass('count').appendTo(this.$root);
			this.$current_total_page = $('<div>').addClass('column_three').appendTo(this.$count);
			
			this.$page_remind = $('<div>').addClass('remind').appendTo(this.$root);
			this.$big_left = $('<div>').addClass('big_left').appendTo(this.$page_remind);
			this.$remind_left = $('<div>').addClass('remind_left').append("PAGE-").appendTo(this.$big_left);
			this.$pagePrev = $('<div>').addClass('pageNext').append($TR('Page :: Previous page')).appendTo(this.$big_left);
			this.$big_right = $('<div>').addClass('big_left').appendTo(this.$page_remind);
			this.$remind_right = $('<div>').addClass('remind_left').append("PAGE+").appendTo(this.$big_right);
			this.$pageNext = $('<div>').addClass('pagePrev').append($TR('Page :: Next page')).appendTo(this.$big_right);
		},

		// prototype
		{
	    length : function ( ) {
	        return this.delegates.length ;
	    },
	    
	    getItem: function ( index ) {
	        return this.delegates[index] ;
	    },
		    
			selectNext: function( ) {
				var visible_front = this.getVisibleItemFront();
				var visible_end = this.getVisibleItemEnd();
				
				var selection = this.selection + 1;
				this.select( selection ) ;
				
				if ( this.scrollFunction == true ) {
						
    				if ( selection > visible_end ) {
    					var selection = visible_end;
							this.select( selection ) ;
    				}
    				else {
    				    var selection = this.selection;
								this.select( selection ) ;  
    				}
			  }else {
		       if ( selection > visible_end ) {
		         var selection = visible_end;
		       this.select( selection ) ;
		        }
		        else {
		            var selection = this.selection;
		        this.select( selection ) ;  
		        }			  	
			 	}
			},
			
			selectPrev: function( ) {
				var visible_front = this.getVisibleItemFront();
				var visible_end = this.getVisibleItemEnd();
				
				var selection = this.selection - 1;
				this.select( selection ) ;
				
				if ( this.scrollFunction == true ) {
					if( selection < visible_front){
						var selection = visible_front;
						this.select( selection ) ;
					}
					else{
						var selection = this.selection;
						this.select( selection ) ; 
					}
				}
				else{									
					if( selection < visible_front){
						var selection = visible_front;
						this.select( selection ) ;
					}
					else{
						var selection = this.selection;
						this.select( selection ) ; 
					}
				}
			},

			select: function( index ) {
				this.selection = Math.max( 0, Math.min( this.length, index ) ) ;
				this._update( ) ;
			},

			getSelected: function( ) {
				return this.delegates[this.selection] ;
			},
			
			getSelectedIndex: function( ) {
				return this.selection ;
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
					if ( i < this.showItemsNum ) {
					    delegate.show( ) ;
					}

          if (items.length <= this.defaultItemsNum ) {
              this.scrollFunction = false ;
          }
          else {
              this.showItemsNum = this.defaultItemsNum ;
              this.scrollFunction = true ;
          }
                    
					this.delegates.push( delegate ) ;
				}

				this._update( ) ;
				//console.table(this.delegates);
			},

			_update: function( ) {
				for( var i in this.delegates ) {
					var delegate = this.delegates[i] ;
					delegate.select( i == this.selection ) ;
				}
			},
			
			getDefaultItemsNum: function(){
				return this.defaultItemsNum;
			},
			
			setDefaultItemsNum: function(items_num){
				this.defaultItemsNum = items_num;
				this.showItemsNum = this.defaultItemsNum ;
				this.visibleItemEnd = this.showItemsNum - 1 ;
				this.pageChangeCount = this.defaultItemsNum ;
			},
			
			getPageCountInit: function( ) {
		    	this.pageCount = 1;
		    	if( this.length > this.defaultItemsNum ){
		    		this.quotient = Math.ceil( this.length / this.defaultItemsNum );
					
						this.visibleItemFront = 0;
						this.visibleItemEnd = this.defaultItemsNum - 1 ;
					
						for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
	    				this.delegates[i].show( ) ;
	    			}
	    			
	    			this.$current_total_page.text(this.pageCount + "/" + this.quotient ) ;
		    	}
		    	else{
		    		this.visibleItemFront = 0;
						this.visibleItemEnd = this.length - 1 ;
						
						for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
	    				this.delegates[i].show( ) ;
	    			}
	    			console.log();	    			
	    			this.$current_total_page.text( this.pageCount + "/" + this.pageCount ) ;
		    	}
		    	var visible_front = this.getVisibleItemFront();
					var selection = visible_front;
					this.select( selection ) ;	
		  },
			
			getPageNext: function( ) {
				if( this.length > this.defaultItemsNum ){
					var quotient = Math.ceil( this.length / this.defaultItemsNum );
					if( this.pageCount == quotient){
						return ;
					}
						if( quotient == 1 ){
							this.pageCount = 1 ;
							this.visibleItemFront = 0;
							this.visibleItemEnd = this.length - 1 ;
							
							this.$current_total_page.text(this.pageCount + "/" + quotient ) ;
							
							for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
    						this.delegates[i].show( ) ;
    					}
						}
						else {
							for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
    							this.delegates[i].hide( ) ;
    					}
							this.pageCount += 1 ;
							this.$current_total_page.text(this.pageCount + "/" + quotient ) ;
							
							this.visibleItemFront = this.visibleItemEnd + 1 ;
							
								if( this.pageCount == quotient){
									this.visibleItemEnd = this.length - 1 ;
									
									for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
    								this.delegates[i].show( ) ;
    							}
								}
								else {
									this.visibleItemEnd = (this.visibleItemEnd + this.pageChangeCount) ;
									
									for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
    								this.delegates[i].show( ) ;
    							}
								}
						}
				}
				else {
					this.pageCount = 1 ;
					this.visibleItemFront = 0;
					this.visibleItemEnd = this.length - 1 ;
					
					this.$current_total_page.text(this.pageCount + "/" + quotient ) ;
					
					for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
						this.delegates[i].show( ) ;
					}
					return ;
				}
				var visible_front = this.getVisibleItemFront();
				var selection = visible_front;
				this.select( selection ) ;
			},
			
			
			getPagePrev: function( ) {
				if( this.pageCount == 1 ){
					return ;
				}
				if( this.length > this.defaultItemsNum ){
					var quotient = Math.ceil( this.length / this.defaultItemsNum );
						if( quotient == 1 ){
							this.pageCount = 1 ;
							this.visibleItemFront = 0;
							this.visibleItemEnd = this.length - 1 ;
							this.$current_total_page.text(this.pageCount + "/" + quotient ) ;
							
							for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
    						this.delegates[i].show( ) ;
    					}
						}
						else {
							for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
    							this.delegates[i].hide( ) ;
    					}
    					if( this.pageCount == 2 ){
    							this.pageCount -= 1 ;
    							this.$current_total_page.text(this.pageCount + "/" + quotient ) ;
    							
									this.visibleItemEnd = this.visibleItemFront - 1;
									this.visibleItemFront = 0 ;
    							
    							for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
	    								this.delegates[i].show( ) ;
	    						}
    					}
    					else{
								this.pageCount -= 1 ;
								this.$current_total_page.text(this.pageCount + "/" + quotient ) ;
								
									if( this.pageCount == 1){
										
										this.visibleItemFront = 0 ;
										this.visibleItemEnd = (this.visibleItemEnd - this.pageChangeCount) ;
										
										for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
	    								this.delegates[i].show( ) ;
	    							}
	    							return ;
									}
									else{
										this.visibleItemEnd = this.visibleItemFront - 1 ;
										this.visibleItemFront = this.visibleItemFront - this.pageChangeCount ;
										
										for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
	    								this.delegates[i].show( ) ;
	    							}
									}
							}
						}
				}
				else {
					this.pageCount = 1 ;
					this.visibleItemFront = 0;
					this.visibleItemEnd = this.length - 1 ;
					
					this.$current_total_page.text(this.pageCount + "/" + quotient ) ;
					
					for (var i = this.visibleItemFront; i <= this.visibleItemEnd; i++) {
						this.delegates[i].show( ) ;
					}
					return ;
				}
				var visible_front = this.getVisibleItemFront();
				var selection = visible_front;
				this.select( selection ) ;
			},
			
			getVisibleItemFront: function(){
				return this.visibleItemFront;
			},
			
			getVisibleItemEnd: function(){
					if( this.length < this.defaultItemsNum ){
							this.visibleItemEnd = this.length - 1;
					}
				return this.visibleItemEnd;
			},
			
			getPageCount: function(){
				return this.pageCount;
			},
			
			hidePageCount: function(){
				this.$count.hide();
			},

			hideHelpInfo: function(){
				this.$page_remind.hide();
			},
		}

	) ;

	window.pageTurningListView = pageTurningListView ;

})( ) ;
