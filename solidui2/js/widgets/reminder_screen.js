"use strict";
(function( ) {

	var name = "Reminder Screen" ;

	var Reminder = new window.Class(
		
		//parent
		window.ListViewDelegate,
		
		//constructor
		function (reminderinfo){
            ListViewDelegate.call(this,name);
            this.$root.addClass('email_listlable');	
            var date_string = window.Format.formatDateLayoutTS( reminderinfo.time, name ) + " " + window.Format.formatTimeLayoutTS( reminderinfo.time, name );	
            var $date = $('<div>').addClass('caption_date').text(date_string).appendTo(this.$root);
			var $channel = $('<div>').addClass('caption_channel').text(reminderinfo.serviceName).appendTo(this.$root);
			var $remindername = $('<div>').addClass('caption_name').text(reminderinfo.name).appendTo(this.$root);
			var $duration = $('<div>').addClass('caption_duration').text(Math.round((reminderinfo.duration / 60000)) + TR("duration :: min")).appendTo(this.$root);            
    		this.handle = reminderinfo.handle;
    	},
		//prototype
		{
			getHandle: function(){
		  		return this.handle;
		  	},
		}
		);
	var reminder_screen = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;
		self.$root.attr('id','reminderscreen');		
		
		var $table = $( '<div>').addClass( 'reminder_table' ).appendTo(self.$root);
		
		var $table_caption_row = $( '<div>').addClass( 'caption' ).appendTo($table);
		
		var $date_caption = $('<div>').addClass('caption_date').append($TR('reminder screen :: reminder date')).appendTo($table_caption_row);
		var $channel_caption = $('<div>').addClass('caption_channel').append($TR('reminder screen :: reminder channel')).appendTo($table_caption_row);
		var $remindername_caption = $('<div>').addClass('caption_name').append($TR('reminder screen :: reminder name')).appendTo($table_caption_row);
		var $duration_caption = $('<div>').addClass('caption_duration').append($TR('reminder screen :: reminder duration')).appendTo($table_caption_row);
		
		var $list_table = $( '<div>').addClass( 'list_table' ).appendTo($table);
		var list = new window.pageTurningListView( Reminder );
		list.$root.addClass('lable').appendTo($list_table);	

		var $pr_button = $( '<div>' ).addClass( 'pr_button' ).appendTo($table);		
		var $delete_button = $('<div>').addClass('delete_button').appendTo($pr_button);
		var $deleteText = $('<div>').addClass('deleteText').append($TR('ok bar screen :: okbardeleteText')).appendTo($pr_button);
		
		var updateScreen = function(){
      		var reminderInfo = window.Platform.pvr.getSchedules();
      		list.updateItems(reminderInfo) ;
     		list.getPageCountInit();
			list.show( );
		}						
		self.onKey = function( ) {
			return true ;
		} ;
		
		// delete on reminder on red key
		self.onKeyRED = function() {
			if(list.length > 0){
				var callbackOk = function ( ) {
						var item = list.getSelected();
						window.Platform.pvr.deleteSchedule(item.getHandle());
						window.AppManager.back();							
						selectPopDlg.HideSelectPopDlg();																													
						updateScreen();																																					
				}
				var callbackCancel = function ( ) {
					window.AppManager.back() ;					
				}
				var string = "confirm box :: do you really want to delete this reminder";				
				var selectPopDlg = window.AppManager.getWidget("Select PopDlg");
				selectPopDlg.registerCallback( callbackOk,callbackCancel,string );
				window.AppManager.enter( "Select PopDlg" ) ;
	  		}		
			
		};

		self.onShow = function( ) {
			updateScreen();
		} ;
		
		self.onKeyUP = function( ) {
			list.selectPrev( ) ;
		} ;

		self.onKeyDOWN = function( ) {
			list.selectNext( ) ;
		} ;

		self.onKeyCHANDOWN = function( ) {
				list.getPagePrev( ) ;				
		};
		
		self.onKeyCHANUP = function( ) {
				list.getPageNext( ) ;				
		};	
		return self ;
	} ;

	window.AppManager.addScene( name, reminder_screen ) ;

})( ) ;
