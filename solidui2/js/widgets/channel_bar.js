"use strict";
(function( ) {

	var name = "channelbar" ;


	var channelbar = function( ) {

		var parent = new window.Widget( name ) ;
		var self = Object.create( parent ) ;
		var channelLists = Platform.dvb.getLists( );
		var selectedList = 0;

		var $channelbar = $( '<div>' ).attr( 'id', 'channelbar' ).appendTo( self.$root ) ;
		var $left  = $( '<div>' ).addClass( 'arrow_left' ).appendTo( $channelbar ) ;
		var $name  = $( '<div>' ).attr( 'id', 'channelbar_name' ).addClass( 'name' ).text( "Channelbar" ).appendTo( $channelbar ) ;
		var $right = $( '<div>' ).addClass( 'arrow_right' ).appendTo( $channelbar ) ;
	
		self.update = function( currentList ) {
			Platform.dvb.Lists = Platform.dvb.getLists();
			if(window.Template.EnableMaster == false){
				//Update the lists										  
				for(var i = 0; i < Platform.dvb.Lists.length; i++){
					if(Platform.dvb.Lists[i].id == 0 ){
						Platform.dvb.Lists.splice(i, 1);
						break;
					}			
				}		
			}
			
			selectedList = currentList;
			var getCurrentList_index = function(){
				for(var i = 0; i < Platform.dvb.Lists.length; i++){
					if( currentList == Platform.dvb.Lists[i].id){
						return i;
					}
				}
			}
			var CurrentList_index = getCurrentList_index();
			var channelname = Platform.dvb.Lists[CurrentList_index].name;
			$('#channelbar_name').text( channelname );
		}
		
		var currentList = Platform.dvb.getCurrentList();
		self.update( currentList ) ;	

	/*
		self.setCurrent = function( current ) {		
			//scrollbarCurrent = current;
		}
		self.setMax = function( max ) {		
			//scrollbarMax = max;
		}
	*/
	
		self.getSelectedList = function( ) {
			return selectedList;
		}
	
		self.onShow = function( ) {			
			console.log( "channelbar.onShow ");	
			//channelLists = Platform.dvb.getLists( );						
			//start();
		}

		self.onHide = function( ) {		
			console.log( "channelbar.onHide ");			
			//stop();
		}

		self.select = function( flag ) {
				$channelbar.toggleClass( 'selected', flag ) ;
		} ;



		return self ;
	} ;

	window.AppManager.addWidget( name, channelbar ) ;

})( ) ;
