"use strict";
(function( ) {

	var name = "Programme Guide" ;

	// parameters
	var s = 1000 ;
	var m = 60*s ;
	var h = 60*m ;

	var ROWS_IN_SCREEN = 5 ;
	var rows_offset = 0 ;
	var rows_selection = 0 ;
	var favoriteToggleList = false ; // if list change is selected for left right button
	var channelLists = [];
	var currentEvent = [];		// current event data for Reminder 
	var nextUpdateEvents = 0;  // next stoptime for Update grid
	var nextUpdateCurrentEvents = 0;  // next stoptime for Update Current grid
	var reminder = []; 

	var EPG = function( ) {



var print_r = function (arr,level) {
var dumped_text = "";
if(!level) level = 0;

//The padding given at the beginning of the line.
var level_padding = "";
for(var j=0;j<level+1;j++) level_padding += "    ";

if(typeof(arr) == 'object') { //Array/Hashes/Objects 
    for(var item in arr) {
        var value = arr[item];

        if(typeof(value) == 'object') { //If it is an array,
            dumped_text += level_padding + "'" + item + "' ...\n";
            dumped_text += print_r(value,level+1);
        } else {
            dumped_text += level_padding + "'" + item + "' => \"" + value + "\"\n";
        }
        if (item > 10) {
        		break;
        };
    }
} else { //Stings/Chars/Numbers etc.
    dumped_text = "===>"+arr+"<===("+typeof(arr)+")";
}
return dumped_text;
}





		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'epg' ) ;
		
		var info_bar = window.AppManager.getWidget( "Info Bar" ) ;
		//var channel_number = window.AppManager.getWidget( 'Channel Number' ) ;
		
		var $bg_epg_1 = $( '<div>' ).addClass( 'bg_epg_1' ).appendTo( self.$root ) ;
		var $overlay_bg_epg_1 = $( '<div>' ).addClass( 'overlay_bg_epg_1' ).appendTo( self.$root ) ;
		var $bg_epg_2 = $( '<div>' ).addClass( 'bg_epg_2' ).appendTo( self.$root ) ;
		var $overlay_bg_epg_2 = $( '<div>' ).addClass( 'overlay_bg_epg_2' ).appendTo( self.$root ) ;

		var top_bar = window.AppManager.getWidget( "Top Bar" ) ;
		var liveTV = window.AppManager.getWidget("Live TV");

		var $overlay = $( '<div>' ).addClass( 'overlay' ).appendTo( self.$root ) ;
		var $grid = $( '<div>' ).addClass( 'grid' ).appendTo( self.$root ) ;
		
		
		
		var Timeline = new function( ) {

			var duration = 2 * h ;
			var num = 4 ;
			//var real_timestamp = window.Platform.time.getTime( ) ;
		
			var real_timestamp = new Date();
			
			if (real_timestamp.getMinutes()>=0 && real_timestamp.getMinutes()<=30 ) {
				//var offset = new Date(real_timestamp.getFullYear(),real_timestamp.getMonth(),real_timestamp.getDate(),real_timestamp.getHours(),0,0,0).getTime() - Math.floor(duration / num  );
				var offset = new Date(real_timestamp.getFullYear(),real_timestamp.getMonth(),real_timestamp.getDate(),real_timestamp.getHours(),0,0,0).getTime();
			} else {
				//var offset = new Date(real_timestamp.getFullYear(),real_timestamp.getMonth(),real_timestamp.getDate(),real_timestamp.getHours(),30,0,0).getTime() - Math.floor(duration / num );
				var offset = new Date(real_timestamp.getFullYear(),real_timestamp.getMonth(),real_timestamp.getDate(),real_timestamp.getHours(),30,0,0).getTime();
			};
	
			//var offset = real_timestamp - Math.floor(duration / 2 ) ;
			
			var $root = $( '<div>' ).addClass( 'timeline' ) ;
	
			var update = function( ) {
				$root.empty( ) ;
	
				
				var interval = duration / num ;
	
				for( var i = 0 ; i < num ; i++ ) {
					var time = offset + i * interval ;
					var time_string = window.Format.formatTimeLayoutTS( time, name ) ;
					var $time = $( '<div>' ).addClass( 'time' ).text( time_string ).appendTo( $root ) ;
				}
			} ;
	
			update( ) ;
	
	
			this.$root = $root ;
	
			this.start = function( ) {
				return offset ;
			} ;
	
			this.stop = function( ) {
				return offset + duration ;
			} ;
	
			this.length = function( ) {
				return duration ;
			} ;

			this.shift = function( delta ) {
				offset += delta ;
				update( ) ;
			} ;
			this.update = function(){
				update( );
			};

			this.focustime = function( ) {

				var offset_old = offset;
				var temp_date=new Date();
				temp_date.setTime(Focustime() - 1*s);
				if (temp_date.getMinutes()>=0 && temp_date.getMinutes()<=30 ) {
					//offset = new Date(temp_date.getFullYear(),temp_date.getMonth(),temp_date.getDate(),temp_date.getHours(),0,0,0).getTime() - Math.floor(duration / num );
					offset = new Date(temp_date.getFullYear(),temp_date.getMonth(),temp_date.getDate(),temp_date.getHours(),0,0,0).getTime();
				} else {
					//offset = new Date(temp_date.getFullYear(),temp_date.getMonth(),temp_date.getDate(),temp_date.getHours(),30,0,0).getTime() - Math.floor(duration / num );
					offset = new Date(temp_date.getFullYear(),temp_date.getMonth(),temp_date.getDate(),temp_date.getHours(),30,0,0).getTime();
				};
				if ( offset !== offset_old) {
					update( ) ;
					return true;
				}
				else {
					return false;
				};
			} ;


	    } ;

		Timeline.$root.appendTo( $grid ) ;

		var focustime_next = 0;
		var focustime_current = 0;
		var focustime_prev = 0;
		var focustime_auto = true;
		var focustime_direction = "current"; // possible values current,next,prev 

		var Focustime = function( ) {
			var return_value = 0;
			if ( !focustime_auto && focustime_next != 0 && focustime_current != 0 && focustime_prev != 0   ) {
				if (focustime_direction === "current" ) {
					//console.log( "-> Focustime  -> focustime_current = " + focustime_current ) ;
					return_value = focustime_current;
				} else if (focustime_direction === "prev" ) { 
					//console.log( "-> Focustime  -> focustime_prev = " + focustime_prev ) ;
					return_value = focustime_prev;
				} else if (focustime_direction === "next" ) {
					//console.log( "-> Focustime  -> focustime_next = " + focustime_next ) ;
					return_value = focustime_next;
				} else{
					//console.log( "-> Focustime  -> window.Platform.time.getTime( ) = " + window.Platform.time.getTime( ) ) ;
			      return_value = window.Platform.time.getTime( );
				}
			} else {
				//console.log( "-> Focustime  -> window.Platform.time.getTime( ) = " + window.Platform.time.getTime( ) ) ;
				return_value = window.Platform.time.getTime( );
			}

			if (return_value < window.Platform.time.getTime( ) ) {
				return_value = window.Platform.time.getTime( )
			};

			//console.log( "-> EPG - Focustimet.return_value = " + return_value + " return_value tranlated = " + window.Format.formatDateTimeLayoutTS( return_value, name) );
			//console.log( "-> EPG - Event event.stopDateTime = " + event.stopDateTime + " event.stopDateTime tranlated = " + window.Format.formatDateTimeLayoutTS( event.stopDateTime, name) );
			return return_value;

		};
		var getCurrentFocustime = function( ) {
			var return_value = 0;
			if ( !focustime_auto && focustime_current != 0  ) {
				return_value = focustime_current;
			} else {
				return_value = window.Platform.time.getTime( );
			}

			if (return_value < window.Platform.time.getTime( ) ) {
				return_value = window.Platform.time.getTime( );
			};

			if (return_value <= Timeline.start() || return_value >= Timeline.stop() ) {
				return_value = Timeline.start() + 1*s;
			};


			return return_value;

		};

		var $rows = $( '<div>' ).addClass( 'rows' ).appendTo( $grid ) ;
		var scrollbar = window.AppManager.getWidget( "scrollbar" ) ;
		var progressbar = window.AppManager.getWidget( "progressbar" ) ;
		var $channelbar = window.AppManager.getWidget( "channelbar" ) ;

		//var $currentchannel = Platform.dvb.getCurrentChannel() ;
		//var $currentchannelinfo = Platform.dvb.getChannelInfo( $currentchannel ) ;
		var $currentchannelinfo = liveTV.judgeIfTunedChannel( ) ;
		var $currentnextepg = Platform.epg.loadNowNext($currentchannelinfo.mrl) ;
		if ( _.isEmpty($currentnextepg) ) {
			console.log("------> $currentnextepg is empty!!") ;
			$currentnextepg[0] = $currentnextepg[1] = {
				name          : "",
				startDateTime : 0,
				stopDateTime  : 0,
			};
		};

		var date_string_start = window.Format.formatDateLayoutTS( $currentnextepg[0].startDateTime, name ) ;
		var time_string_start = window.Format.formatTimeLayoutTS( $currentnextepg[0].startDateTime, name ) ;	
		var time_string_stop = window.Format.formatTimeLayoutTS( $currentnextepg[0].stopDateTime, name ) ;	
		
		var $info_grid = $( '<div>' ).addClass( 'info_grid' ).appendTo( self.$root ) ;
		var $programname = $( '<div>' ).attr( 'id', 'programname' ).addClass( 'programname' ).appendTo( $info_grid ) ;
		
		/*//scrolling text
		var begin = 0;
		var end = $currentnextepg[0].name.length ;
		
		function scrolling_text() {
			if(end>15) {
                $('#programname').text( $currentnextepg[0].name.substring(begin,end) + " " + $currentnextepg[0].name.substring(0,begin) ) ;
				begin++;

				if(begin >= end) {
					begin = 0;
				};
			}
			else {
				$('#programname').text( $currentnextepg[0].name );
     		};
			//console.log( "-> name of your trust  -> begin = " + begin ) ;
			//console.log( "-> name of your trust  -> end = " + end ) ;
		};*/
		
		//var $live = $( '<div>' ).addClass( 'live' ).text( "Live" ).appendTo( $info_grid ) ;
		
		var $preview = $( '<div>' ).addClass( 'preview' ).text( "" ).appendTo( $info_grid ) ;
		
		var $canalname = $( '<div>' ).addClass( 'canalname' ).text( $currentchannelinfo.name ).appendTo( $info_grid ) ;
		//var $rated = $( '<div>' ).addClass( 'rated' ).text( "rated follows" ).appendTo( $info_grid ) ;
		
		var $date = $( '<div>' ).addClass( 'date' ).text( date_string_start ).appendTo( $info_grid ) ;
		var $length = $( '<div>' ).addClass( 'length' ).text( time_string_start + " " + "-" + " " + time_string_stop ).appendTo( $info_grid ) ;
		
		//var $duration = $( '<div>' ).addClass( 'duration' ).text( window.Format.formatDuration( $currentnextepg[0].duration,2 ).join( ', ' ) ).appendTo( $info_grid ) ;
		
		var $info = $( '<div>' ).addClass( 'info' ).appendTo( self.$root ) ;

		//var $detail = $( '<div>' ).addClass( 'detail' ).appendTo( $info ) ;		
		var $record_btn = $( '<div>' ).addClass( 'button' ).appendTo( $info ) ;
		var $record_btn_icon = $( '<div>' ).addClass( 'record_button_icon' ).appendTo( $record_btn ) ;
		var $record_btn_text = $( '<div>' ).addClass( 'button_text' ).append($TR('epg :: Favorite')).appendTo( $record_btn ) ;
		
		var $reminder_btn = $( '<div>' ).addClass( 'button' ).appendTo( $info ) ;
		var $reminder_btn_icon = $( '<div>' ).addClass( 'reminder_button_icon' ).appendTo( $reminder_btn ) ;
		var $reminder_btn_text = $( '<div>' ).addClass( 'button_text' ).append($TR('epg :: Reminder')).appendTo( $reminder_btn ) ;
		
		var $prevday_btn = $( '<div>' ).attr( 'id', 'prevdaybutton' ).addClass( 'button' ).appendTo( $info ) ;
		var $prevday_btn_icon = $( '<div>' ).addClass( 'prevday_button_icon' ).appendTo( $prevday_btn ) ;
		var $prevday_btn_text = $( '<div>' ).addClass( 'button_text' ).append($TR('epg :: Prev day')).appendTo( $prevday_btn ) ;
		
		var $nextday_btn = $( '<div>' ).addClass( 'button' ).appendTo( $info ) ;
		var $nextday_btn_icon = $( '<div>' ).addClass( 'nextday_button_icon' ).appendTo( $nextday_btn ) ;
		var $nextday_btn_text = $( '<div>' ).addClass( 'button_text' ).append($TR('epg :: Next day')).appendTo( $nextday_btn ) ;
		
		//var $hours_btn_24 = $( '<div>' ).addClass( 'hrs_btn_24' ).appendTo( $info ) ;
		//var $hours_btn_2 = $( '<div>' ).addClass( 'hrs_btn_2' ).appendTo( $info ) ;
		//var $hours_btn_2_2 = $( '<div>' ).addClass( 'hrs_btn_2_2' ).appendTo( $info ) ;
		//var $hours_btn_24_2 = $( '<div>' ).addClass( 'hrs_btn_24_2' ).appendTo( $info ) ;
		

		//var $channelbar = $( '<div>' ).attr( 'id', 'channelbar' ).appendTo( self.$root ) ;
		//var $left  = $( '<div>' ).addClass( 'arrow_left' ).appendTo( $channelbar ) ;
		//var $name  = $( '<div>' ).attr( 'id', 'channelbar_name' ).addClass( 'name' ).text( "Channelbar" ).appendTo( $channelbar ) ;
		//var $right = $( '<div>' ).addClass( 'arrow_right' ).appendTo( $channelbar ) ;



		var Row = function( channel ) {
			//console.log( "----> epg.Row -> channel.name = " + channel.name ) ;
			var $row = $( '<div>' ).addClass( 'row' ) ;
			
			var $channel_lcn = $('<div>').addClass('channel_lcn').text(channel.lcn).appendTo($row);
			var $channel = $( '<div>' ).addClass( 'channel' ).text( channel.name ).appendTo( $row ) ;
			var $events = $( '<tr>' ) ;
			$( '<div>' ).addClass( 'events' ).append( $( '<table>' ).append( $events ) ).appendTo( $row ) ;

			this.isCurrentChannel = false;

			var sanitizeEvents = function( events ) {
				events = events.sort( function( a, b ) { return a.startDateTime - b.startDateTime ; } ) ;

				var i = 0 ;

				var filtered_events = [] ;
				for( var i = 1 ; i < events.length ; i++ ) {
					events[i-1].stopDateTime = Math.min( events[i-1].stopDateTime, events[i].startDateTime ) ;
					if( events[i-1].stopDateTime > events[i-1].startDateTime ) {
						filtered_events.push( events[i-1] ) ;
					}
				}
				if( events[i-1] && events[i-1].stopDateTime > events[i-1].startDateTime ) {
					filtered_events.push( events[i-1] ) ;
				}

				return filtered_events ;
			} ;


			this.$root = $row ;
			this.channel = channel ;
			
			this.updateInfo = function( channel ) {
				$channel_lcn.text( channel.lcn ) ;
				$channel.text( channel.name ) ;
			};

			this.showEvents = function( events ) {
				//console.log( "-> epg.Row.showEvents -> this.isCurrentChannel = " + this.isCurrentChannel ) ;
				var j = 0;
				
				if (this.isCurrentChannel) {
					
					while( j < events.length ) {
						if ( events[j].startDateTime <= Focustime( ) && Focustime( ) <= events[j].stopDateTime ) {
							//console.log( "-> epg.Row.showEvents  -> event[j].name = " + events[j].name ) ;
							currentEvent = events[j];
							//console.log( "-> epg.Row.showEvents  -> j = " + j ) ;
							//console.log( "-> epg.Row.showEvents  -> currentEvent.name = " + currentEvent.name ) ;
							//console.log( "-> epg.Row.showEvents  -> currentEvent.startDateTime = " + currentEvent.startDateTime ) ;
							//console.log( "-> epg.Row.showEvents  -> currentEvent.duration = " + currentEvent.duration ) ;	
							
							//console.log( "-> epg.Row.showEvents  -> focustime_current = " + focustime_current ) ;
							focustime_prev = currentEvent.startDateTime - 1*s;
							
							//console.log( "-> epg.Row.showEvents  -> fcurrentEvent.startDateTime = " + currentEvent.startDateTime ) ;
							//console.log( "-> epg.Row.showEvents  -> Timeline.start() = " + Timeline.start() ) ;							
							
							if (currentEvent.startDateTime < Timeline.start()) {
								focustime_current = Timeline.start() + 1*s;
								//console.log( "-> epg.Row.showEvents 2  -> focustime_current = " + focustime_current ) ;
							} else {			
								focustime_current = currentEvent.startDateTime + 1*s;
								//console.log( "-> epg.Row.showEvents 3  -> focustime_current = " + focustime_current ) ;
							};
							focustime_next = currentEvent.stopDateTime + 1*s;
							//console.log( "-> epg.Row.showEvents  -> focustime_prev = " + focustime_prev ) ;
							//console.log( "-> epg.Row.showEvents  -> focustime_current = " + focustime_current ) ;
							//console.log( "-> epg.Row.showEvents  -> focustime_next = " + focustime_next ) ;
							//console.log( "-> epg.Row.showEvents  -> currentEvent.length = " + currentEvent.length ) ;
							focustime_direction  = "current"; 
			
							break;
							
						}
						j++ ;
					}
				}
				j = 0;
				while( j < events.length ) {
					
						var hasreminder = hasReminder(events[j].channelUrl, events[j].startDateTime);
						//console.log( "-> epg.Row.showEvents  -> hasreminder = " + hasreminder ) ;
						//console.log( "-> epg.Row.showEvents  -> events[" + j + "].channelUrl = " + events[j].channelUrl ) ;
						events[j]["hasreminder"] = hasreminder;
						//console.log( "-> epg.Row.showEvents  -> events[j].hasreminder = " + events[j].hasreminder ) ;
						
						j++ ;
				};


				var makeDummyEvent = function( range ) {
					if(window.Template.EnableNoInformation == true){
						return {
							//name: " ",
							name: TR( "epg :: noinfo" ),
							startDateTime: range.start,
							stopDateTime: range.stop, 
							hasreminder: false,
						} ;
					}
					else{
						return {
							name: " ",
							//name: TR( "epg :: noinfo" ),
							startDateTime: range.start,
							stopDateTime: range.stop, 
							hasreminder: false,
						} ;
					}
					
				} ;

				var clampEvent = function( event ) {
					return {
						name: event.name,
						startDateTime: Math.max( event.startDateTime, Timeline.start( ) ),
						stopDateTime: Math.min( event.stopDateTime,  Timeline.stop( ) ),
						hasreminder: event.hasreminder,
					} ;
				} ;

				var addEvent = function( event ) {
					var event = new Event( event ) ;
					event.$root.appendTo( $events ) ;
				} ;


				events = sanitizeEvents( events ) ;

				var last_stop = Timeline.start( ) ;

				// skip events that don't appear in the timeline
				var i = 0 ;
				while( events[i] && events[i].stop < last_stop ) {
					i++ ;
				}

				// start adding
				$events.empty( ) ;

				while( events[i] && events[i].startDateTime < Timeline.stop( ) ) {
					var event = clampEvent( events[i] ) ;
					var start = event.startDateTime ;
					var stop = event.stopDateTime ;

					if( (start - last_stop) > 2*m ) {
						addEvent( makeDummyEvent( { start: last_stop, stop: start } ) ) ;
					}

					addEvent( event ) ;

					last_stop = stop ;
					i++ ;
				}

				if( last_stop < Timeline.stop( ) ) {
					addEvent( makeDummyEvent( { start: last_stop, stop: Timeline.stop( ) } ) ) ;
				}

			} ;

			this.select = function( flag ) {
				$row.toggleClass( 'selected', flag ) ;
				//console.log( "-> epg.Row.select -> flag = " + flag ) ;
				//console.log( "-> Platform.dvb.createRows -> index = " + index ) ;
				
				this.isCurrentChannel = flag;
				//console.log( "-> epg.Row.select -> this.isCurrentChannel = " + this.isCurrentChannel ) ; 
			} ;

		} ;

		var Event = function( event ) {
			//var real_timestamp = window.Platform.time.getTime( ) ;
			var start = event.startDateTime ;
			var stop  = event.stopDateTime ;
			
			//console.log( "-> EPG - Event =====================================================================================================================================" );
			//console.log( "-> EPG - Event event.name = " + event.name );
			//console.log( "-> epg.Event -> event.hasreminder = " + event.hasreminder ) ;
			//console.log( "-> EPG - Event Focustime = " + Focustime() + " Focustime tranlated = " + window.Format.formatDateTimeLayoutTS( Focustime(), name) );
			//console.log( "-> EPG - Event Timeline.start() = " + Timeline.start() + " Timeline.start() tranlated = " + window.Format.formatDateTimeLayoutTS( Timeline.start(), name) );
			//console.log( "-> EPG - Event window.Platform.time.getTime( )  = " + window.Platform.time.getTime( ) + " window.Platform.time.getTime( ) tranlated = " + window.Format.formatDateTimeLayoutTS( window.Platform.time.getTime( ) , name) );

			//console.log( "-> EPG - Event event.startDateTime = " + event.startDateTime + " event.startDateTime tranlated = " + window.Format.formatDateTimeLayoutTS( event.startDateTime, name) );
			//console.log( "-> EPG - Event event.stopDateTime = " + event.stopDateTime + " event.stopDateTime tranlated = " + window.Format.formatDateTimeLayoutTS( event.stopDateTime, name) );

			//focustime_direction = "current";
			if ( start <= getCurrentFocustime( ) && getCurrentFocustime( ) < stop ) {
				if (event.hasreminder) {
					var $event = $( '<td>' ).attr( 'id', 'event').addClass( 'event' ).addClass( 'current_time' ).addClass( 'reminder' ).text( event.name ) ;
				} else {
					var $event = $( '<td>' ).attr( 'id', 'event').addClass( 'event' ).addClass( 'current_time' ).text( event.name ) ;
				};
				if ( nextUpdateEvents >= stop || nextUpdateEvents === 0 ) {
					nextUpdateEvents = stop;
				};
			} else {
				if (event.hasreminder) {
					var $event = $( '<td>' ).attr( 'id', 'event').addClass( 'event' ).addClass( 'reminder' ).text( event.name ) ;
				} else {
					var $event = $( '<td>' ).attr( 'id', 'event').addClass( 'event' ).text( event.name ) ;
				}
			};

			var duration = stop - start ;
			var width_percent = (duration / Timeline.length( )) * 100 ;

			$event.css( {
				'width': width_percent + '%',
			} ) ;

			//if ($event[0].scrollWidth >  $event.innerWidth()) {
   			 //Text has over-flowed
			//console.log( "-> EPG - Event Text has over-flowed" );
			//} else {
			//console.log( "-> EPG - Event $event[0][0].scrollWidth = " + $event[0].scrollWidth);
			//console.log( "-> EPG - Event $event.innerWidth() = " + $event.innerWidth() );

			//}


			this.$root = $event ;
			
			/*//If Event is overflown -> marquee to scroll the text
				if ( document.getElementById( 'event' ).scrollWidth > document.getElementById( 'event' ).clientWidth ){
					$( '#event' ).addClass( 'marquee' );					
				}else{
					$( '#event' ).removeClass( 'marquee' );
				};*/
				
		}


		var rows_by_id = {} ;
		var rows = [] ;

		var emptyRows = function( ) {
			//console.log( "-------------------------------------> EPG.emptyRows -> start ") ;
			$rows.empty( ) ;
			//console.log("----------> EPG.emptyRows -> $rows: " + $rows.html())
			//console.log( "-------------------------------------> EPG.emptyRows -> end ") ;
		} ;

		var createRows = function( ) {
			//console.log( "--------------------------------------------------------> epg.createRows -> start") ;
			//console.log( "-> epg.createRows -> rows_offset = " + rows_offset ) ;
			var rows_offset_int = rows_offset;
			_.times( ROWS_IN_SCREEN, function( i ) {
				var index = i + rows_offset_int ;
				var channel = channels[index] ;

				 //console.log( "-> epg.createRows -> i = " + i ) ;
				 //console.log( "-> epg.createRows -> ROWS_IN_SCREEN = " + ROWS_IN_SCREEN ) ;
				 //console.log( "-> epg.createRows -> rows_offset_int = " + rows_offset_int ) ;
				 //console.log( "-> epg.createRows -> index = " + index ) ;
				 //console.log( "-> epg.createRows -> channel.mrl = " + channel.mrl ) ;
				 //console.log( "-> epg.createRows -> channel.name = " + channel.name ) ;



				if( _.isUndefined( channel ) ) { 
				 	//console.log( "------------------->  _.isUndefined( channel )" ) ;
					return ;
				}

				$( '<div>' ).addClass( 'row' ).appendTo( $rows ) ;
				//console.log("----------> epg.createRows -> $rows: " + $rows.html())

				// if( _.isNumber( channel ) ) {
				if( channel ) {

					Platform.dvb.getChannelInfo( channel.mrl, function( error, channel ) {
						if( error ) { throw new Error( error.message ) ; }

						channels[index] = channel ;
						
						//console.log( "-> epg.createRows -> getChannelInfo -> index = " + index ) ;
						//console.log( "-> epg.createRows -> getChannelInfo -> channel.mrl = " + channel.mrl ) ;
						//console.log( "-> epg.createRows -> getChannelInfo -> channel.name = " + channel.name ) ;
						createRow( index, i ) ;
					} ) ;
					//console.log( "-> EPG.createRows if channel -> return") ;
					return ;
				}
				else{
					//console.log( "-> EPG.createRows if channel else") ;
					createRow( index, i ) ;
				}
			} ) ;
			//console.log( "-> EPG.createRows -> Ende") ;
		} ;

		var createRow = function( index, i ) {
			//console.log( "-----------------------> EPG.createRow -> start index = " + index + " i = " + i) ;
			var channel = channels[index] ;

			//console.log( "-> Platform.dvb.createRow -> channel.name = " + channel.name ) ;
			//console.log( "-> Platform.dvb.createRow -> channel.mrl = " + channel.mrl ) ;
			//console.log( "-> Platform.dvb.createRow -> channel.id = " + channel.id ) ;
			
			if( !rows_by_id[channel.mrl]  ) {
				rows_by_id[channel.mrl] = new Row( channel ) ;
			}
			
			//console.log("----------> EPG.createRow -> rows_by_id.length = " + rows_by_id.length);
			
			var row = rows_by_id[channel.mrl] ;
			
			
			
			row.updateInfo( channel ) ;

			var row_by_index = rows[index] ;
			if( !row_by_index || row_by_index.channel != channel ) {
				rows[index] = row ;
			}
			//console.log( "------> EPG.createRow -> rows.length = " + rows.length) ;
			//console.log( "------> EPG.createRow -> i = " + i) ;
			//console.log( "------> EPG.createRow -> index = " + index) ;
			//console.log( "------> EPG.createRow -> rows_selection = " + rows_selection) ;
			//console.log( "------> EPG.createRow -> $rows.length = " + $rows.length) ;
			row.select( index == rows_selection ) ;
			$rows.find( '.row' ).eq( i ).replaceWith( row.$root ) ;
			//console.log("----------> EPG.createRow -> print_r($rows) = " + print_r($rows));

			//console.log("----------> EPG.createRow -> $rows: " + $rows.html())
			
			updateRow( row ) ;
			
			/*
			//console.log( "-> EPG.createRow -> Ende index = " + index + " i = " + i ) ;
			if ( $('.row').length !== ROWS_IN_SCREEN && $('.row').length !== channels.length )  {
			    //console.log("----------> EPG.createRow -> missing entries ");
			    updateRows( );
			}
			*/
		} ;

		var updateRows = function( ) {
			//console.log( "-> EPG.updateRows -> start ") ;
			emptyRows( ) ;
			createRows( ) ;
			//console.log( "-> EPG.updateRows -> end ") ;
		} ;

		var updateEvents = function( ) {
			for( var i in rows ) {
				updateRow( rows[i] ) ;
			}
		} ;

		var updateRow = function( row ) {
			var range = {
				start: Timeline.start( ),
				stop: Timeline.start( ) + Timeline.length( ) - 1,
			} ;
			//var isCurrentChannel = false;
			
			//console.log( "-----------------------> epg.updateRow -> row.channel.mrl = " + row.channel.mrl);
			//console.log( "-----------------------> epg.updateRow -> row.channel.name = " + row.channel.name);
			//console.log( "-----------------------> epg.updateRow -> range.start = " + range.start);
			//console.log( "-----------------------> epg.updateRow -> range.stop = " + range.stop);
			Platform.epg.getChannelEvents( row.channel.mrl, range.start, range.stop , 0, 20, function( error, events ) {
				if( error ) { throw new Error( error.message ) ; }
						row.showEvents( events ) ;
			} ) ;
			//console.log("----------> EPG.updateRow -> $rows: " + $rows.html())
		} ;


		var channels = [] ;
		//var channel_list = 0;

		var updateChannels = function( channel_list_used ) {
			//Platform.dvb.getChannelsNumber( function( error, number ) {
			//	if( error ) { throw new Error( error.message ) ; }

			//var channel_list_used = Platform.dvb.getCurrentList();
			
			//console.log( "epg.updateChannels -> channel_list_used = " + channel_list_used); 
			
			Platform.dvb.getChannels( channel_list_used, 0, window.Template.ChannelMaxLimit, function( error, channel_list ) {
				if( error ) { throw new Error( error.message ) ; }

				channels = channel_list ;
				scrollbar.setMax(channels.length);
				//scrollbar.setCurrent(rows_selection);
				//scrollbar.update();

				var currentchannel = liveTV.judgeIfTunedChannel( ) ;
				for(var i = 0; i < channels.length; i++){
					if( currentchannel.mrl == channels[i].mrl){
						rows_selection = i;
						Platform.dvb.setCurrentList( channel_list_used );
						window.Platform.dvb.getChannelsMrl(Platform.dvb.CurrentList, 0,  window.Template.ChannelMaxLimit);
						break;
					}
					else{
						rows_selection = 0;
					}
				}
				selectRow( rows_selection ) ;
				
				//updateRows( ) ;
			} ) ;
			//} ) ;
		} ;


		// channel selection

		var selectRow = function( index ) {
			//console.log( "-> EPG - selectRow -> index = " + index);
			if( index < 0 ) { return ;}
			else if(index >= channels.length){
				var currentchannel = liveTV.judgeIfTunedChannel( ) ;
				for(var i = 0; i < channels.length; i++){
					if( currentchannel.mrl == channels[i].mrl){
						index = i;
					}
					else{
						index = channels.length - 1;
					}
				}
			}
			
			if( index < rows_offset ) {
				rows_offset = index ;
			}
			else if( index >= rows_offset + ROWS_IN_SCREEN ) {
				if(index >= channels.length - 1){
					rows_offset = index - (index % ROWS_IN_SCREEN);
				}else{
					rows_offset = index + 1 - ROWS_IN_SCREEN ;
				}
			}

			rows_selection = index ;
			scrollbar.setCurrent(rows_selection);
			scrollbar.update();
			//console.log( "-> EPG - selectRow -> rows_selection = " + rows_selection);
			updateRows( ) ;
			updateRows( ) ; // second update is needed to resolve a focus issue
		} ;

		var selectionUp = function( ) {
			selectRow( rows_selection - 1 ) ;
			//console.log( "-> EPG - selectionUp -> rows_selection = " + rows_selection);
		} ;

		var selectionDown = function( ) {
			selectRow( rows_selection + 1 ) ;
		} ;
		
		var channelUp = function( ) {
			selectRow( rows_selection - ROWS_IN_SCREEN ) ;
		} ;

		var channelDown = function( ) {
			selectRow( rows_selection + ROWS_IN_SCREEN ) ;
		} ;

		var toggleLISTEPG = function() {
			if (favoriteToggleList) {
				favoriteToggleList = false;	
			} else {
				favoriteToggleList = true;
			};
			$channelbar.select(favoriteToggleList);
		};
		
		var channelLeft = function( ) {
			var current_list = $channelbar.getSelectedList();
			var nextList = 0;
			
			for(var i = 0; i < channelLists.length; i++){
				if(current_list == channelLists[i].id){
					if( i == 0){
						nextList = channelLists[channelLists.length - 1].id;
					}
					else{
						nextList = channelLists[i - 1].id;
					}
					//Platform.dvb.setCurrentList( nextList );
				}
			}
			updateChannels( nextList ) ;
			$channelbar.update( nextList ) ;
		} ;

		var channelRight = function( ) {
			var current_list = $channelbar.getSelectedList();
			var nextList = 0;
			
			for(var i = 0; i < channelLists.length; i++){
				if(current_list == channelLists[i].id){
					if( i == channelLists.length - 1){
						nextList = channelLists[0].id;
					}
					else{
						nextList = channelLists[i + 1].id;
					}
					//Platform.dvb.setCurrentList( nextList );
				}
			}
			updateChannels( nextList ) ;
			$channelbar.update( nextList ) ;
		} ;

		// channel switch
		var switchChannel = function ( ) {
			if (rows_selection < 0 || rows_selection >= channels.length ) { return ; }
			if ( Platform.dvb.getCurrentList() != $channelbar.getSelectedList()) {
				var this_ID = $channelbar.getSelectedList();
				Platform.dvb.setCurrentList( this_ID );			
				window.Platform.dvb.getChannelsMrl(Platform.dvb.CurrentList, 0,  window.Template.ChannelMaxLimit);
			}
			
			var channel_mrl = channels[rows_selection].mrl ;
			liveTV.playChannel( channel_mrl );
			self.updateScreenPosition();
		} ;

		var updateInfoGrid = function( ) {
			
			$currentchannelinfo = liveTV.judgeIfTunedChannel( );
			$currentnextepg = Platform.epg.loadNowNext($currentchannelinfo.mrl) ;
			date_string_start = window.Format.formatDateLayoutTS( $currentnextepg[0].startDateTime, name ) ;
			time_string_start = window.Format.formatTimeLayoutTS( $currentnextepg[0].startDateTime, name ) ;
			time_string_stop = window.Format.formatTimeLayoutTS( $currentnextepg[0].stopDateTime, name ) ;
			//console.log( "-> EPG - updateInfoGrid $currentnextepg[0].name = " + $currentnextepg[0].name );
			$('#programname').text( $currentnextepg[0].name ) ;			
			if (document.getElementById( 'programname' ).scrollWidth > document.getElementById( 'programname' ).clientWidth ){
				$('#programname').html("<marquee scrollamount=10><span style='font-size:32px'>" + $currentnextepg[0].name + "</span></marquee>") ;					
			}

			$('.canalname').text( $currentchannelinfo.name ) ;
			$('.date').text( date_string_start ) ;
			$('.length').text( time_string_start + " " + "-" + " " + time_string_stop ) ;
			//$('.duration').text( window.Format.formatDuration( $currentnextepg[0].duration,2 ).join( ', ' ) ) ;
			nextUpdateCurrentEvents = $currentnextepg[0].stopDateTime;
		} ;
		
		var updateInfoBar = function( ) {
			info_bar.update( ) ;
		} ;

		//var showCurrent = function( ) {
		//	channel_number.showNumber( ) ;
		//} ;


		// timeline movement

		var timelineLeft = function( ) {
			//console.log( "-> EPG - timelineLeft Focustime = " + Focustime() + " Focustime tranlated = " + window.Format.formatDateTimeLayoutTS( Focustime(), name) );
			//console.log( "-> EPG - timelineLeft Timeline.start() = " + Timeline.start() + " Timeline.start() tranlated = " + window.Format.formatDateTimeLayoutTS( Timeline.start(), name) );
			//console.log( "-> EPG - timelineLeft window.Platform.time.getTime( )  = " + window.Platform.time.getTime( ) + " window.Platform.time.getTime( ) tranlated = " + window.Format.formatDateTimeLayoutTS( window.Platform.time.getTime( ) , name) );
			
			if (window.Platform.time.getTime( ) <= Timeline.start() ) {
				
				focustime_auto = false;
				focustime_direction = "prev";
				//console.log( "-> EPG - timelineLeft");
				//Timeline.shift( -0.5*h ) ;
				if (Timeline.focustime()) {
					updateEvents( ) ;
				};
			} else {
				focustime_auto = true;
				focustime_direction = "current";
				if (Timeline.focustime()) {
					updateEvents( ) ;
				};
				//console.log( "-> EPG - timelineLeft -> no left");
			};
			
		} ;

		var timelineRight = function( ) {
			focustime_auto = false;
			focustime_direction = "next";
			//if (Focustime( true ) >  )
			if (Timeline.focustime()) {
				//Timeline.shift( +0.5*h ) ;
				updateEvents( ) ;
			};
		} ;

		var timelineDayLeft = function( ) {
			if ((window.Platform.time.getTime( ) - 1*h) <= (Timeline.start() - 24*h) ) {			
				focustime_auto = false;
				focustime_direction = "prev";
				focustime_prev = focustime_prev - 24*h;
				Timeline.shift( -24*h ) ;
				updateEvents( ) ;
//				if ((window.Platform.time.getTime( ) - 1*h) >= (Timeline.start() - 24*h) ) {
//					$('prevdaybutton').hide();
//				} else {
//					$('prevdaybutton').show();
//				}
			} else {
				focustime_auto = true;
				focustime_direction = "current";
				//console.log( "-> EPG - timelineLeft -> no left");	
			}
		} ;

		var timelineDayRight = function( ) {
			focustime_auto = false;
			focustime_direction = "next";
			focustime_next = focustime_next + 24*h;
			Timeline.shift( +24*h ) ;
			updateEvents( ) ;
		} ;

		// public methods
		
		var reminder_pop_Dlg = new window.popDlg( );
				reminder_pop_Dlg.$root.appendTo( self.$root ) ;
				reminder_pop_Dlg.hide();
		
		var setReminder = function( ) {
		
			//console.log( "-> epg.Row.setReminder  -> currentEvent.channelUrl = " + currentEvent.channelUrl ) ;
			//console.log( "-> epg.Row.setReminder  -> currentEvent.name = " + currentEvent.name ) ;
			//console.log( "-> epg.Row.setReminder  -> currentEvent.startDateTime = " + currentEvent.startDateTime ) ;
			//console.log( "-> epg.Row.setReminder  -> currentEvent.duration = " + currentEvent.duration ) ;	
			
			if (hasReminder(currentEvent.channelUrl,currentEvent.startDateTime )) {
				deleteReminder(currentEvent.channelUrl,currentEvent.startDateTime);
			} else {
			
				var reminderMap = {
					"sourceMediaUrl": currentEvent.channelUrl,
					"startTime": currentEvent.startDateTime,
					"duration": currentEvent.duration,
					"title": currentEvent.name,
					"offset": 30*1000
				};
				var result = Platform.pvr.createReminder(reminderMap );
				
				//console.log("result =" + result);

				if(result > 0){
					var str = "EPG :: Adding timer successful" ;
					reminder_pop_Dlg.show();
					reminder_pop_Dlg.showDlgMsg( TR(str) );
					setTimeout(function( ) { reminder_pop_Dlg.hide(); },3000);
				}
				else if(result <= 0){
					var str = Platform.pvr.getReminderError(result) ;
					reminder_pop_Dlg.show();
					reminder_pop_Dlg.showDlgMsg( TR(str) );
					setTimeout(function( ) { reminder_pop_Dlg.hide(); },3000);
				}
				//console.log( "-> epg.Row.setReminder  -> result = " + result ) ;
			};
		
			getReminder();
			updateEvents();
		
		};

		var getReminder = function( ) {

			reminder = [];
			reminder = Platform.pvr.getSchedules( );
			
			//console.log( "-> epg.Row.setReminder  -> JSON.stringify(reminder) = " + JSON.stringify( reminder ) ) ;

		};

		var hasReminder = function( channel_mrl, start ) {

			var reminder_i = 0;
			while( reminder_i < reminder.length ) {

				if (reminder[reminder_i].mrl === channel_mrl && reminder[reminder_i].time === start ) {
					//console.log( "-> epg.hasReminder  -> reminder[reminder_i].mrl = " + reminder[reminder_i].mrl ) ;
					//console.log( "-> epg.hasReminder  -> channel_mrl = " + channel_mrl ) ;
					//console.log( "-> epg.hasReminder  -> reminder[reminder_i].time = " + reminder[reminder_i].time * 1000 ) ;
					//console.log( "-> epg.hasReminder  -> start = " + start ) ;
					return true;
					break;
				};
				reminder_i++ ;
			};
			return false;
		};

		var deleteReminder = function( channel_mrl, start ) {

			var reminder_i = 0;
			while( reminder_i < reminder.length ) {

				if (reminder[reminder_i].mrl === channel_mrl && reminder[reminder_i].time === start ) {
					//console.log( "-> epg.hasReminder  -> reminder[reminder_i].mrl = " + reminder[reminder_i].mrl ) ;
					//console.log( "-> epg.hasReminder  -> channel_mrl = " + channel_mrl ) ;
					//console.log( "-> epg.hasReminder  -> reminder[reminder_i].time = " + reminder[reminder_i].time * 1000 ) ;
					//console.log( "-> epg.hasReminder  -> start = " + start ) ;
					Platform.pvr.deleteSchedule(reminder[reminder_i].handle);
					//return reminder[reminder_i].handle;
					break;
				};
				reminder_i++ ;
			};
			//return 0;
		};




		var deleteAllReminder = function( ) {

			reminder = [];
			var reminder_i = 0;
			reminder = Platform.pvr.getSchedules( );

			while( reminder_i < reminder.length ) {
				Platform.pvr.deleteSchedule(reminder[reminder_i].handle);
				reminder_i++ ;
			};


		};


		self.onShow = function( ) {
			
			var adjust_top = window.settings.monitor_adjust_top + 27;
			var monitor_width = 1280 -  window.settings.monitor_adjust_left -  window.settings.monitor_adjust_right ;
			var adjust_left = 1280 - Math.abs(window.settings.monitor_adjust_left - window.settings.monitor_adjust_right) - 471 ;//need to get the Resolution Information from Platform.Display

			focustime_auto = true;
			focustime_direction = "current";
			focustime_next = 0;
			focustime_current = 0;
			focustime_prev = 0;
			Timeline.update();
			if (Timeline.focustime()) {
				updateEvents( ) ;
			};
			//deleteAllReminder();
			getReminder();
			Platform.media.setVideoPosition(adjust_left, adjust_top, 471, 265);
			window.AppManager.getWidget( 'Main Menu' ).hide( ) ;
			scrollbar.$root.appendTo( $grid ) ;
			progressbar.setStarttime(Timeline.start( ));
			progressbar.setStoptime(Timeline.stop( ));
			progressbar.setProgress(window.Platform.time.getTime( ));
			progressbar.updateStartStop();
			progressbar.update();
			progressbar.$root.appendTo( self.$root ) ;
			//live_tv.$root.appendTo( self.$root ) ;
			// show child widgets
			top_bar.show( ) ;
			scrollbar.show( ) ;
			progressbar.show( ) ;
			//$channelbar.$root.appendTo( self.$root ) ;
			$channelbar.show( ) ;
			self.start();
			//live_tv.show( ) ;
			
			channelLists = Platform.dvb.getLists( );
			if(window.Template.EnableMaster == false){
					for(var i = 0; i < channelLists.length; i++){
						if(channelLists[i].id >= 0 && channelLists[i].id < 8000 ){
							channelLists[i] = channelLists[i + 1];
						}
					}
					channelLists.splice(channelLists.length - 1,1);
			}
			updateInfoGrid( ) ;
			rows_selection = Platform.dvb.ChannelCurrent.index ;
			
			var channel_list = Platform.dvb.getCurrentList();
			updateChannels( channel_list ) ;
			$channelbar.update( channel_list ) ;
		} ;

		self.updateScreenPosition = function(){
			var adjust_top = window.settings.monitor_adjust_top + 27;
			var monitor_width = 1280 -  window.settings.monitor_adjust_left -  window.settings.monitor_adjust_right ;
			var adjust_left = 1280 - Math.abs(window.settings.monitor_adjust_left - window.settings.monitor_adjust_right) - 471 ;//need to get the Resolution Information from Platform.Display
			Platform.media.setVideoPosition(adjust_left, adjust_top, 471, 265);
			updateInfoGrid( );
			//Add workaround for the issue topBar disappeard
			top_bar.show( ) ;			
		}

		self.onHide = function( ) {			
			Platform.media.setVideoPosition(0, 0, 1280, 720);
			top_bar.hide( ) ;
			scrollbar.hide( ) ;
			self.stop();
			//progressbar.hide( ) ;
			$channelbar.hide ( ) ;
			//live_tv.hide( ) ;
		} ;

		self.onKey = function( ) {
			return true ;
		} ;

		self.onKeyOK = function( ) {
			switchChannel( );
		} ;

		self.onKeyLEFT = function( ) {
			if (favoriteToggleList) {
				channelLeft( );
			} else {
				timelineLeft( ) ;
			};
		} ;

		self.onKeyRIGHT = function( ) {
			if (favoriteToggleList) {
				channelRight( ) ;
			} else {
				timelineRight( ) ;
			};
		} ;

		self.onKeyUP = function( ) {
			//console.log( "-> EPG - onKeyUP");
			selectionUp( ) ;
		} ;

		self.onKeyDOWN = function( ) {
			//console.log( "-> EPG - onKeyDOWN");
			selectionDown( ) ;
		} ;
     
	        self.onKeyNUMBER = function( number ) {
	            liveTV.pushChannelNummber(number);
	        } ;

		self.onKeyFAV = function( ) {
			toggleLISTEPG( ) ;
			//console.log( "-> EPG - onKeyRED favoriteToggleList = " + favoriteToggleList);
		} ;
		self.onKeyREMINDER = function( ) {
			setReminder( ) ;
			console.log( "-> EPG - onKeyREMINDER");
		} ;
		self.onKeyYELLOW = function( ) {
			timelineDayLeft( ) ;
			//console.log( "-> EPG - onKeyYELLOW");
		} ;
		self.onKeyBLUE = function( ) {
			timelineDayRight( ) ;
			//console.log( "-> EPG - onKeyBLUE");
		} ;
		self.onKeyCHANUP = function( ) {
			channelUp( );
			//console.log( "-> EPG - onKeyCHANUP");
		} ;
		self.onKeyCHANDOWN = function( ) {
			channelDown( );
			//console.log( "-> EPG - onKeyCHANDOWN");
		} ;
        
        self.onKeyBACK = function( ) {
            while( window.AppManager.current( ).name !== "Live TV" ) {
                window.AppManager.back( ) ;
            }
		} ;

        self.updateCurrentChannel = function(channel_index) {
			console.log("-> channel_index: " + channel_index);
			selectRow(channel_index);
			updateInfoGrid( );
//			updateInfoBar( );
		};

		var timer = null ;

		self.update = function( ) {
			//console.log( "-> EPG - update");
			//console.log( "-> EPG - update -> rows = " +  rows[rows_selection]);
			if (focustime_auto === true ) {
				focustime_direction = "current";
				if (Timeline.focustime()) {
					updateEvents();
				};
			};

			if (Focustime( ) >= nextUpdateCurrentEvents || Focustime( ) >= nextUpdateEvents ) {
				//console.log( "-> EPG - update - Focustime( ) = " + Focustime( ) + " >= nextUpdateCurrentEvents = " + nextUpdateCurrentEvents);
				nextUpdateEvents = 0;
				nextUpdateCurrentEvents = 0;
				$currentchannelinfo = liveTV.judgeIfTunedChannel( ) ;
				$currentnextepg = Platform.epg.loadNowNext($currentchannelinfo.mrl) ;
				if ($('#programname').text() != $currentnextepg[0].name) {
					updateInfoGrid( );
				};				
				updateEvents( ) ;
			};

			if (Timeline.start( ) < window.Platform.time.getTime( ) && Timeline.stop( ) > window.Platform.time.getTime( ) ) {	
				progressbar.show();
				progressbar.setStarttime(Timeline.start( ));
				progressbar.setStoptime(Timeline.stop( ));
				progressbar.setProgress(window.Platform.time.getTime( ));
				progressbar.updateStartStop();
				progressbar.update();
			} else {
				progressbar.hide();
			} ;
			//end = $currentnextepg[0].name.length ;
			//scrolling_text();
		} ;

	        self.setPreviewText = function ( str ) {
	        	console.log("epg"+str); 
	            $preview.html(window.$TR(str)) ;
          
	        } ;

		self.start = function( ) {
			//console.log( "-> EPG - start");
			favoriteToggleList = false;
			$channelbar.select(favoriteToggleList);
			timer = setInterval( self.update, 500 ) ;
			self.update() ;
		} ;

		self.stop = function( ) {
			clearInterval( timer ) ;
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, EPG ) ;

})( ) ;
