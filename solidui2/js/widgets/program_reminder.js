"use strict";
(function( ) {

	var name = "Programme Reminder" ;

	var program_reminder = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;
		
		self.$root.attr('id','Programme_Reminder');

		var $table = $( '<div>').addClass( 'reminder_table' ).appendTo(self.$root);
		
		var $table_caption_row = $( '<div>').addClass( 'caption' ).appendTo($table);
		
		var $date = $('<div>').addClass('caption_date').append($TR('reminder screen :: reminder date')).appendTo($table_caption_row);
		var $channel = $('<div>').addClass('caption_channel').append($TR('reminder screen :: reminder channel')).appendTo($table_caption_row);
		var $reminder_name = $('<div>').addClass('caption_name').append($TR('reminder screen :: reminder name')).appendTo($table_caption_row);
		var $duration = $('<div>').addClass('caption_duration').append($TR('reminder screen :: reminder duration')).appendTo($table_caption_row);
		//var $frequency = $('<div>').addClass('reminder_frequency').append($TR('reminder screen :: reminder frequency')).appendTo($table_caption_row);
		
		var $table_rows = $( '<div>').addClass( 'table_rows' ).appendTo($table);
		
		var $pr_button = $( '<div>' ).addClass( 'pr_button' ).appendTo(self.$root);
		
		var $first_button = $( '<div>' ).addClass( 'first_button' ).appendTo($pr_button);
		var $delete_button = $('<div>').addClass('delete_button').appendTo($first_button);
		var $deleteText = $('<div>').addClass('deleteText').append($TR('ok bar screen :: okbardeleteText')).appendTo($first_button);

		
		
		
		// these varibles get used in the
		// refresh function
		// and contain data that might is
		// needed in the rest of the script
		var data;
		var reminder_desc;
		var reminder_list; 
		var first_show = true;
		
/*
		// these a help variabls do not
		// use them exept for the callback delte method
		var singel_remider_handle;
		var singel_remider_name;
*/	
		
		// callback function to delete a reminder
		var delete_reminder = function( data, name )
		{
			var ok_fuction = function()
			{
				
				// delete reminder
				window.Platform.pvr.deleteSchedule( data );
				
				// get old index pos
				var index = reminder_list.get_focus_pos();
				
				// we now need to refresh and set the list to the old index
				// as long that still exists
				refresh();
				
				//if no focus was set reset evething to the top
				if ( index == -1 ) {
					// refresh already sets focus to top so we don't need to do that anymore
					scroll("reset");
				}
				
				// get the size of the new list
				var size = reminder_list.size();
				
				// if size is 0 there will be no item to focus
				if (size <= 0) {
					return;
				}
				
				// if ouer index still exists set it as focused 
				if ( size > index)
				{
					reminder_list.set_pos(index);
				}
				// there is no way that index is not smaler than size and zero because size has to be at least 1
				else
				{
					reminder_list.set_pos(index -1);
				}
				
				return;
			};
			
			var cancel_function = function() {};
			
			window.AppManager.getScene('confirm_box').config_confirm_box(
				TR('confirm box :: do you really want to delete') + ' ' + name,
				TR('confirm box :: delete reminder headline' ),
				ok_fuction, cancel_function);
			
			window.AppManager.enter('confirm_box');

			return;
		
			/*
			// delete reminder
			window.Platform.pvr.deleteSchedule( data );
			
			// get old index pos
			var index = reminder_list.get_focus_pos();
			
			// we now need to refresh and set the list to the old index
			// as long that still exists
			refresh();
			
			//if no focus was set reset evething to the top
			if ( index == -1 ) {
				// refresh already sets focus to top so we don't need to do that anymore
				scroll("reset");
			}
			
			// get the size of the new list
			var size = reminder_list.size();
			
			// if size is 0 there will be no item to focus
			if (size <= 0) {
				return;
			}
			
			// if ouer index still exists set it as focused 
			if ( size > index)
			{
				reminder_list.set_pos(index);
			}
			// there is no way that index is not smaler than size and zero because size has to be at least 1
			else
			{
				reminder_list.set_pos(index -1);
			}
			
			return;
			*/
		}
		
		
		//help variables for the scroll function
		var scroll_tabe = $table_rows[0];
		var x = 0;
		var max_heigt = scroll_tabe.scrollHeight - $table_rows.height();
		
		// hadels the scroll bar pass in up down or reset to
		// go up down or reset(go to top)
		var scroll = function(command)
		{
			// this is the number of pixels that will be done by every scroll down or up
			var steps_size = 40;
			
			// get max_heigt in case it changed
			max_heigt = scroll_tabe.scrollHeight - $table_rows.height();
			
			// if x is higer than max heigt set it back to max
			if (x > max_heigt ) {
				x = max_heigt;
			}
			
			if ("up" === command ) {
				if ( x- steps_size < 0)
					x = 0;
				else
					x -= steps_size;
					
				scroll_tabe.scrollTop = x;
			}
			else
			if ("down" === command ) {
				if ( max_heigt - x- steps_size < 0)
					x = max_heigt;
				else
					x += steps_size;
				
				scroll_tabe.scrollTop = x;
				
			}
			else
			if ("reset" === command ) {
				x = 0;
				scroll_tabe.scrollTop = x;
			}
			
		};
		
		
		// refresh function removes the old reminder rows
		// and creates new ones
		// this will be done after every reminder delete
		// and on each show since the reminder list could have canged
		var refresh = function()
		{
			
			
			//if this is the first show dont remive the old
			//reminder list since there is non
			if ( first_show == true ) 
				first_show = false;
			else
				reminder_list.$root.remove();
			
			// get reminder data from the media engine
			data = window.Platform.pvr.getSchedules();
			
			// data to pass in the mono list 
			reminder_desc =
				{
					name: "reminder_list",
					window_function: window.reminder_row,
					items: [ ],
				};
			
			// fill up the items array with the reminder data
			for( var i in data )
			{
				
				
				reminder_desc.items[i] =
					{
						name: i,
						date_time: window.Format.formatDateLayoutTS( data[i].time, name ) + " " + window.Format.formatTimeLayoutTS( data[i].time, name ),
						callback_function: delete_reminder,
						channel: data[i].serviceName,
						reminder_name: data[i].name,
						duration: "" + Math.round((data[i].duration / 60000)) + " " + TR("duration :: min"),
						frequency: data[i].frequency,
						handel: data[i].handle,
					};
				/*
				console.log( "reminder_row_data date_time: ", data[i].time ) ;
				console.log( "reminder_row_data channel: ", data[i].serviceName ) ;
				console.log( "reminder_row_data reminder_name: ", data[i].name ) ;
				console.log( "reminder_row_data duration: ", data[i].length ) ;
				console.log( "reminder_row_data frequency: ", data[i].frequency ) ;
				
				console.log( "reminder_row_bef name: ", i ) ;
				console.log( "reminder_row_bef date_time: ", reminder_desc.items[i].date_time ) ;
				console.log( "reminder_row_bef channel: ", reminder_desc.items[i].channel ) ;
				console.log( "reminder_row_bef reminder_name: ", reminder_desc.items[i].reminder_name ) ;
				console.log( "reminder_row_bef duration: ", reminder_desc.items[i].duration ) ;
				console.log( "reminder_row_bef frequency: ", reminder_desc.items[i].frequency ) ;
				*/
			}
			
			// give the thata to the mono list aka text_field
			reminder_list = new window.text_field(reminder_desc);
			reminder_list.$root.appendTo( $table_rows ) ;
			reminder_list.show( );
			//reminder_list.enter();
			
			// set focus to top and enter mono list
			reminder_list.set_top();
			reminder_list.enter();
		};

		
		
		self.onKey = function( ) {
			return true ;
		} ;
		
		// delete on reminder on red key
		self.onKeyRED = function() {
			
			
			// set scroll back to top
			// scroll("reset"); // well they sad we shouldn't do that

			
			// makes callback to delete_reminder and 
			// deletes reminder that way
			reminder_list.activate();
			
			// de_activate element so it can be activated agein
			reminder_list.de_activate();
		};

		self.onKeyGREEN = function() {

// 		    var cur_channel = MediaEngineChannels.getCurrentChannel();
// 		    var mrl = cur_channel.mrl;
// 		    var startTime = new Date();
// 		    var name = cur_channel.short_name + "_" + new Date(startTime);
// 		    var now = startTime.getMilliseconds();
// 		    var neu = now + 120000 ;
// 		    var newStartTime =  startTime.setMilliseconds(neu) ;
//
// 		    var reminderMap = {
// 		    "sourceMediaUrl": mrl,
// 		    "startTime": newStartTime,
// 		    "duration": 90000 ,
// 		    "title": name ,
// 		    "offset": 30000
// 		    };
//
// 		    var ret = MediaEnginePVR.createReminder(reminderMap);
//
// 		    switch (ret) {
// 			case 0:
// 			    console.log( "-> MediaEnginePVR.createReminder - ret = PVR_ADD_REMINDER_STATUS_NOK");
// 			    break;
// 			case -1:
// 			    console.log( "-> MediaEnginePVR.createReminder - ret = PVR_ADD_REMINDER_STATUS_INVALID_SERVICE_OR_URL");
// 			    break;
// 			case -2:
// 			    console.log( "-> MediaEnginePVR.createReminder - ret = PVR_ADD_REMINDER_STATUS_NAME_OR_URL_LENGTH_NOK");
// 			    break;
// 			case -3:
// 			    console.log( "-> MediaEnginePVR.createReminder - ret = PVR_ADD_REMINDER_STATUS_PAST_START_TIME");
// 			    break;
// 			case -4:
// 			    console.log( "-> MediaEnginePVR.createReminder - ret = PVR_ADD_REMINDER_STATUS_CONFLICT_START_TIME");
// 			    break;
// 			default:
// 			    console.log( "-> MediaEnginePVR.createReminder - ret = PVR_ADD_REMINDER_STATUS_OK");
// 			    break;
// 		    }
		    refresh();
		};


		// call the refresh function on
		// every time the reminder list gets visible
		self.onShow = function( ) {
			refresh();
			
			
			// reset the scrollbar / go back to the top
			// since the list could have changed
			// and the fist item will get foucused in refresh()
			// automaticly
			scroll("reset");
		} ;
		
		self.onKeyUP = function( ) {
			// move the focus up in the reminder list
			reminder_list.move_up();

			// this is a workaround since javascript somehow
			// dosen't update that and I ge a wrong value back on first call
			console.log("pos: ", reminder_list.get_focus_pos()); 
			console.log("pos: ", reminder_list.size()); 

			
			//scroll up
			if ( reminder_list.get_focus_pos() < reminder_list.size() - 7 )
			{
				scroll("up");
			}
		} ;

		self.onKeyDOWN = function( ) {
			// move the focus down in the reminder list
			reminder_list.move_down();

			//scroll down
			if ( reminder_list.get_focus_pos() > 6 )
			{
				scroll("down");
			}
			
		} ;

		
		return self ;
	} ;

	window.AppManager.addScene( name, program_reminder ) ;

})( ) ;
