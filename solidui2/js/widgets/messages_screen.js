"use strict";
(function( ) {

	var name = "Messages" ;


	var Message = new window.Class(

		// parent
		window.ListViewDelegate,

		// constructor
		function( message ) {
			ListViewDelegate.call( this, message.name ) ;

			this.$root.addClass( 'message' ) ;

			var date = new Date( message.date ) ;
			var date_string = window.Format.formatDate( date ) + ' ' + window.Format.formatTime( date ) ;

			var $is_read = $( '<div>' ).addClass( 'column is_read' ).appendTo( this.$root ) ;
			
			if (message.is_read == false){$is_read.removeClass('column is_read').addClass('column is_not_read');};		
			
			var $title   = $( '<div>' ).addClass( 'column title'   ).text( message.title   ).appendTo( this.$root ) ;
			var $date    = $( '<div>' ).addClass( 'column date'    ).text( date_string     ).appendTo( this.$root ) ;
		},

		// prototype
		{}
	) ;


	var Messages = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'messages' ) ;

		var $message_table = $( '<div>' ).addClass( 'message_table' ).appendTo( self.$root ) ;		
		
		var $caption = $( '<div>' ).addClass( 'caption' ).appendTo( $message_table ) ;	
		
		var $is_read = $( '<div>' ).addClass( 'column is_read_caption' ).append( $TR( 'messages :: is_read' ) ).appendTo( $caption ) ;
		var $title   = $( '<div>' ).addClass( 'column title'   ).append( $TR( 'messages :: title'   ) ).appendTo( $caption ) ;
		var $date    = $( '<div>' ).addClass( 'column date'    ).append( $TR( 'messages :: date'    ) ).appendTo( $caption ) ;
		
		var $message_rows = $( '<div>' ).addClass( 'message_rows' ).appendTo( $message_table ) ;	


		var list = new window.ListView( Message ) ;

		var updateList = function( ) {

			Platform.hkc.getMessages( function( error, messages ) {
				if( error ) { throw new Error( error.message ) ; }

				list.updateItems( messages ) ;

			} ) ;

		} ;


		self.onKey = function( ) {
			return true ;
		} ;

		self.onShow = function( ) {
			list.$root.appendTo( $message_rows ) ;
			list.show( ) ;
			updateList( ) ;
		} ;

		self.onKeyUP = function( ) {
			list.selectPrev( ) ;
		} ;

		self.onKeyDOWN = function( ) {
			list.selectNext( ) ;
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, Messages ) ;

})( ) ;
