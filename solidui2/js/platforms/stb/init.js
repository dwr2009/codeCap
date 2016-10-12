"use strict";

//var MediaEngineScanner = new media_engine_scanner();
//var MediaEngineChannels = new media_engine_channels();
//var MediaEnginePlayback = new media_engine_playback();
//var MediaEngineEPG = new media_engine_epg();
//var Tv2StbCommon = new tv2_stb_common();
//var MediaEngineNetwork = new media_engine_network();


(function( ) {

	if (typeof media_engine_scanner==="undefined") {
	//	console.log( "-> media_engine_scanner not exists" ) ;
	} else {
//		console.log( "-> media_engine_scanner exists" ) ;
	};


	if (typeof media_engine_nstv==="undefined") {
//		console.log( "-> media_engine_nstv not exists" ) ;
	} else {
	//	console.log( "-> media_engine_nstv exists" ) ;
		var nstv = new media_engine_nstv();
	};


//	var nstv = new media_engine_nstv();

	var rand = function( x ) {
			return Math.floor( Math.random( ) * x ) ;
	} ;
	
     	
	var s = 1000 ;
	var m = 60 * s ;
	var h = 60 * m ;
	var fake_epgdatebase = new Date( ) ;
	var fake_epgdate = new Date(fake_epgdatebase.getFullYear(),fake_epgdatebase.getMonth(),fake_epgdatebase.getDate(),0,0,0).getTime();
	 


	var fake_data = {

		all_channels: function( ) {
			return _.pluck( _.values( this.channel_data ), 'description' ) ;
		},

		current_channel: function( ) {
			var list = fake_data.all_channels( ) ;
			var index = rand( list.length ) ;
			var channel = list[index] ;

			return channel.id ;
		},

		channels_list: function( start, limit ) {
			return _.pluck( fake_data.all_channels( ), 'id' ).slice( start, start + limit ) ;
		},

		channels_number: function( ) {
			return fake_data.all_channels( ).length ;
		},

		channel_info: function( id ) {
	//		console.log( fake_data.channel_data, id )
			return fake_data.channel_data[id].description ;
		},

		channel_events: function( id, range ) {
			var epg = this.channel_data[id].epg ;

			var intervals = [] ;

			var offset = Math.floor( range.start / (24*h) ) ;
			var from   = range.start;
			var to     = range.stop;
			
			if( to < from ) {
				intervals.push( [from, 24*h] ) ;
				intervals.push( [   0,   to] ) ;
			}
			else{
				intervals.push( [from, to] ) ;
			}

			var events = [] ;

			for( var j in intervals ) {
				var interval = intervals[j] ;

				for( var i in epg ) {
					var event = epg[i] ;
					var from = event.range.start;
					var to   = event.range.stop;

					if( from <= interval[1] && to >= interval[0] ) {
						var _event = {
							name: event.name,
							range: {
								start: event.range.start,
								stop:  event.range.stop,
							},
						} ;
						events.push( _event ) ;
					}
				}
			}

			return events ;
		},

		channel_events_current_next: function( id ) {
	//		console.log( "-> channel_events_current_next - id:" + id ) ;

			var events = [] ;

			var epg;
			if (this.channel_data[id] == undefined) {
				epg = this.channel_data[0].epg ;
			}
			else {
				epg = this.channel_data[id].epg ;
			}

			for( var i in epg ) {
	//			console.log( "-> channel_events_current_next epg - i:" + i ) ;
				var event = epg[i] ;
				var from = event.range.start;
				var to   = event.range.stop;
				var real_timestamp = window.Platform.time.getTime( ) ;

				if (from <= real_timestamp && real_timestamp <= to )	{
					var _event = {
   					name: event.name,
						range: {
							start: event.range.start,
							stop:  event.range.stop,
							duration: event.range.duration,
						},
					} ;
					events.push( _event ) ;
					//return _event;
					var j = 0;
					j = parseInt(i) + 1;

					if (epg.length >= j ) {

			//			console.log( "-> channel_events_current_next - next event i+1 = " + j);

						var event_next = epg[j] ;
						var _event_next = {
   						name: event_next.name,
							range: {
								start: event_next.range.start,
								stop:  event_next.range.stop,
								duration: event_next.range.duration,
							},
						} ;
						events.push( _event_next ) ;
					};
					return events;
				};
			};
			return events; // todo: return error "no data"
		},

		volume_value: 50,

		volume: function( ) {
			return fake_data.volume_value ;
		},

		set_volume: function( volume ) {
			return fake_data.volume_value = volume ;
		},


		mute: false,

		get_mute: function( ) {
			return fake_data.mute ;
		},

		set_mute: function( mute ) {
			fake_data.mute = mute ;
		},


		time: function( ) { return Date.now( ) ; },
		timezone: function( ) { return -1 }, // enums?

		connection_changed_callback: null,
		drives_changed_callback: null,

		connection_details: function( ) {
			return {
				type: Platform.network.WIRELESS,
				state: Platform.network.CONNECTED,
			} ;

			// return {
			// 	type: Platform.network.ETHERNET,
			// 	cable_state: Platform.network.PLUGGED,
			// 	state: Platform.network.CONNECTED,
			// } ;

			// return {
			// 	type: Platform.network.ETHERNET,
			// 	cable_state: Platform.network.UNPLUGGED,
			// 	state: Platform.network.DISCONNECTED,
			// } ;
		},

		wireless_signal: function( ) {
			return rand( 101 ) ;
		},

		drives: function( ) {
			return [
				{
					//name: "NO LABEL",
					//path: '/mnt/hdd_1',
					//format: 'FAT32',
					//size: 8 * 1024 * 1024 * 1024,
					//free: 5 * 1024 * 1024 * 1024,
					mediaUrl: "/file:///xxx",
					path: "/mnt/hdd1",
					uuid: 0,
					label: "test_stream",
					name: "NO LABEL",
					size: 8 * 1024 * 1024 * 1024,
					free: 5 * 1024 * 1024 * 1024,
					fileSystem: 'FAT32',
					writable: true,
					mounted: true
				},
			] ;
		},

		recordings: function( ) {
			return [
				{
					date: new Date( Date.now( ) - 100000 ),
					channel: "Channel 1",
					name: "Recording 1",
					range: {
						start: 10*h,
						stop: 11*h + 30*m,
					},
				},
				{
					date: new Date( Date.now( ) - 200000 ),
					channel: "Channel 34",
					name: "Recording 9",
					range: {
						start: 10*h + 15*m,
						stop: 12*h + 30*m,
					},
				},
				{
					date: new Date( Date.now( ) - 300000 ),
					channel: "Channel 1",
					name: "Recording 2",
					range: {
						start: 15*h,
						stop: 15*h + 30*m,
					},
				},
				{
					date: new Date( Date.now( ) - 400000 ),
					channel: "Channel 1",
					name: "Recording 8",
					range: {
						start: 20*h + 15*m,
						stop: 21*h + 15*m,
					},
				},
				{
					date: new Date( Date.now( ) - 500000 ),
					channel: "Channel 36",
					name: "Recording 6",
					range: {
						start: 10*h,
						stop: 11*h + 30*m,
					},
				},
			] ;
		},

	} ;
    
	var Platform = new function( ) {
	    var dvbc_scan_callback       = null ;
	    var show_finger_message_ext_callback = null;
        
		this.keymap = {
			461: "BACK", // backspace
			457: "INFO", // tab
			 13: "OK",   // enter
			 27: "EXIT", // escape
			462: "MENU", // home

			 37: "LEFT",
			 38: "UP",
			 39: "RIGHT",
			 40: "DOWN",

			 48: "0",
			 49: "1",
			 50: "2",
			 51: "3",
			 52: "4",
			 53: "5",
			 54: "6",
			 55: "7",
			 56: "8",
			 57: "9",

			403: "RED",    // F1
			404: "GREEN",  // F2
			405: "YELLOW", // F3
			406: "BLUE",   // F4

			447: "VOLUP",    // F5
			448: "VOLDOWN",  // F6
			427: "CHANUP",   // F7
			428: "CHANDOWN", // F8
			171: "FAV",      // FAV

			458: "EPG",     // F9
			449: "MUTE",    // F10
			182: "VOD",
			409: "POWER",   // F11
			123: undefined, // F12
			
			 36: "HOME",

			515: "MAIL",     //mail
			460: "SUBTITLE", //subtitle
			113: "AUDIO",    //audio track
			513: "REMINDER", //reminder
			
			251: "PURPLE",
			407: "ORANGE",
			514: "GRAY",
			512: "KEYBOARD",

			//116: "RETURN", // F5 Return
			
			19: "PAUSE", // pause
			413: "STOP", //stop
			415: "PLAY", //play
			416: "REC", //recording
			412: "FASTREWIND", //fast rewind
			417: "FASTFORWARD", //fast forward
			424: "SKIPBACK", //skip back
			425: "SKIPFORWARD", // skip forward
		} ;

		this.keycodes = {} ;
		for( var code in this.keymap ) {
			var key = this.keymap[code] ;
			this.keycodes[key] = code ;
		}

		this.init = function( done ) {
			done && done( ) ;
		} ;


		this.media = {
			getPlaybackState:function(callback){
				return MediaEnginePlayback.getPlaybackState();
			},
			play: function( url, callback ) {
				//console.log( "-> Platform.media.play", url ) ;

				var error = null ;

				try {
					window.Platform.ca.StopLockService();      
					MediaEnginePlayback.play( url ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePlayback.play( url )" } ;
				}

				if( !callback ) { return error ; }
				_.defer( function( ) { callback( error ) ; } ) ;
			},

			stop: function( callback ) {
				//console.log( "-> Platform.media.stop" ) ;

				var error = null ;

				try {
					MediaEnginePlayback.stop( ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePlayback.stop( )" } ;
				}

				if( !callback ) { return error ; }
				_.defer( function( ) { callback( error ) ; } ) ;
			},

			getVolume: function( callback ) {
				//console.log( "-> Platform.media.getVolume" ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEnginePlayback.getVolume( ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePlayback.getVolume( )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			setVolume: function( volume, callback ) {
				//console.log( "-> Platform.media.setVolume" ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEnginePlayback.setVolume( volume ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePlayback.setVolume( )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			getMute: function( callback ) {
				//console.log( "-> Platform.media.getMute" ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEnginePlayback.getMute( ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePlayback.getMute( )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			setMute: function( mute, callback ) {
			//	console.log( "-> Platform.media.setMute" ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEnginePlayback.setMute( mute ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePlayback.setMute( )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			setVideoPosition: function( x, y, w, h, callback )	{
				//console.log( "-> Platform.media.setVideoPosition" ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEnginePlayback.setVideoInWindow( x, y, w, h ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePlayback.setVideoPosition( )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;

			}

		} ;


		this.dvb = {
			Lists: [],
			Channels: [],
			ChannelsMrl: [],
			ChannelsLcn: [],
			ChannelCurrent: [],

			getCurrentChannel: function( callback ) {
				//console.log( "-> Platform.dvb.getCurrentChannel" ) ;

				var error = null ;
				var return_value = null ;

				try {
				//	console.log( "-> Platform.dvb.getCurrentChannel -> try" ) ;
					return_value = MediaEngineChannels.getCurrentChannel( ) ;
				//	console.log( "-> Platform.dvb.getCurrentChannel -> try -> return_value = " + return_value ) ;
				}
				catch( e ) {
			//		console.log( "-> Platform.dvb.getCurrentChannel -> catch" ) ;
					error = { message: "Error calling MediaEngineChannels.getCurrentChannel( )" } ;
			//		console.log( "-> Platform.dvb.getCurrentChannel -> catch -> error = " + error ) ;
				}
				//return_value = 1;
				//console.log( "-> Platform.dvb.getCurrentChannel -> return_value = " + return_value ) ;
				//console.log( "-> Platform.dvb.getCurrentChannel JSON = " + JSON.stringify(return_value) );


				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			getChannels: function( list_id, start, limit, callback ) {
			//	console.log( "-> Platform.dvb.getChannels", list_id, start, limit ) ;

				var error = null ;
				var return_value = null ;
				var channelTemplate =  { "mrl":"","name":"" } ;

				try {
					return_value = MediaEngineChannels.getChannels( list_id, start, limit, channelTemplate ) ;
					this.Channels = return_value;
				//	console.log( JSON.stringify(this.Channels) );
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getChannels( list_id, start, limit, channelTemplate)" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			getChannelsWithTemplate: function( list_id, start, limit, template, callback ) {
				console.log( "-> Platform.dvb.getChannelsWithTemplate", list_id, start, limit, template ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.getChannels( list_id, start, limit, template ) ;
					this.Channels = return_value;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getChannels( list_id, start, limit, Template)" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			getChannelsMrl: function( list_id, start, limit, callback ) {
			//	console.log( "-> Platform.dvb.getChannelsMrl list_id = " + list_id ) ;
				var error = null ;
				var return_value = null ;
				var channelTemplate =  { "mrl":"","lcn":"" } ;

				try {
					return_value = MediaEngineChannels.getChannels( list_id, start, limit, channelTemplate ) ;

					this.ChannelsMrl = [];
					this.ChannelsLcn = [];
					for(var i = 0; i < return_value.length; i++)
					{
  							this.ChannelsMrl.push(return_value[i].mrl);
							this.ChannelsLcn.push(return_value[i].lcn);
  				//			console.log( "-> Platform.dvb.getChannelsMrl return_value[i].mrl = " + return_value[i].mrl );
					}


					//this.ChannelsMrl = return_value;
					//console.log( "-> Platform.dvb.getChannelsMrl this.ChannelsMrl.length = " + this.ChannelsMrl.length );
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getChannelsMrl( list_id, start, limit, channelTemplate)" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			getChannelInfo: function( id, callback ) {
			//	console.log( "-> Platform.dvb.getChannelInfo", id ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.getChannelInfo( id ) ;
				//	console.log( "-> Platform.dvb.getChannelInfo JSON = " + JSON.stringify(return_value) );
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getChannelInfo( id )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			deleteChannels: function( callback ) {
			//	console.log( "-> Platform.dvb.deleteChannels") ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.deleteChannels( ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.deleteChannels( )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			createList: function( name, callback ) {
			//	console.log( "-> Platform.dvb.createList name = " + name) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.createList( name ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.createList( name )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			setChannels: function( id, channels, callback ) {
			//	console.log( "-> Platform.dvb.setChannels id = " + id + " channels = " + channels ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.setChannels( id, channels ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.setChannels( id, channels )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			destroyList: function( id, callback ) {
			//	console.log( "-> Platform.dvb.setChannels id = " + id ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.destroyList( id ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.destroyList( id )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			getListInfo: function( id, callback ) {
			//	console.log( "-> Platform.dvb.setChannels id = " + id ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.getListInfo( id ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getListInfo( id )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			setChannelLocked:function(mrl,locked,callback){
				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.setChannelLocked(mrl,locked) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.setChannelLocked( mrl )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			}, 
			
			
			generateFAVList: function( List_id, callback ) {
				
				// only Temp .... for first run
				console.log( "-> Platform.dvb.generateFAVList List_id = " + List_id ) ;
				
				var error = null ;
				var return_value = "" ;
				var channelTemplate =  { "mrl":"", "locked":"", "not_running":"", "scrambled":"", "service_type":"", "unavailable":"" } ;
				var masterchannelsinf = null ;
				var mrls = [] ;

				var MasterListInfo =  this.getListInfo(0);
				console.log( "-> Platform.dvb.generateFAVList MasterListInfo = " + JSON.stringify(MasterListInfo) );

				try {
					masterchannelsinf = MediaEngineChannels.getChannels( 0, 0, MasterListInfo.size, channelTemplate ) ;

					for(var i = 0; i < masterchannelsinf.length; i++)
					{
							if (masterchannelsinf[i].locked === false && masterchannelsinf[i].not_running === false && masterchannelsinf[i].unavailable === false && masterchannelsinf[i].service_type === 1 ) {
								mrls.push(masterchannelsinf[i]["mrl"]);
  							};
					};


					this.setChannels( List_id, mrls );
					//this.ChannelsMrl = return_value;
					//console.log( "-> Platform.dvb.getChannelsMrl this.ChannelsMrl.length = " + this.ChannelsMrl.length );
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getChannelsMrl( list_id, start, limit, channelTemplate)" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;

			},


			setCurrentList: function( id, callback ) {
				console.log( "-> Platform.dvb.setCurrentList id = " + id) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.setCurrentList( id ) ;
					this.CurrentList = id ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.setCurrentList( id )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			getCurrentList: function( callback ) {
				console.log( "-> Platform.dvb.getCurrentList" ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.getCurrentList( ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getCurrentList( )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			getLists: function( callback ) {
				var error = null ;
				var return_value = null ;
				var listTemplate = {"id":"","name":"","size":"","predefined":""};

				try {
					return_value = MediaEngineChannels.getLists( listTemplate ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getLists( name )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			updateServices: function( callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.updateServices( ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.updateServices( )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},			
		} ;
		
		this.pvr = {
			
			createReminder: function( reminderMap, callback ) {
				var error = null ;
				var return_value = null ;
				

				try {
					return_value = MediaEnginePVR.createReminder( reminderMap ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePVR.createReminder( reminderMap )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			getSchedules: function( callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEnginePVR.reminderList ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePVR.getSchedules( )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},		
			
			getUpcommingReminderData: function( callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEnginePVR.nextReminder ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePVR.getUpcommingReminderData( )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},		
			
			setReminderCallback: function(function_callback) {
				this.pvr_reminder_callback && MediaEnginePVR.nextReminderChanged.disconnect( this.pvr_reminder_callback ) ;

				this.pvr_reminder_callback = function() {
					function_callback( MediaEnginePVR.nextReminderChanged ) ;
				} ;
				MediaEnginePVR.nextReminderChanged.connect( this.pvr_reminder_callback ) ;
			},
			
			deleteSchedule: function( handle, callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEnginePVR.deleteSchedule( handle ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePVR.deleteSchedule( handle )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

			clearUpcommingReminder: function( callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEnginePVR.clearUpcommingReminder( ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePVR.clearUpcommingReminder( )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},

 			getReminderError: function( error ) {

				switch(error)
				{
					case -1:
						return "EPG :: Adding timer failure \nbecause of invalid service or URL." ;
					case -2:
						return "EPG :: Adding timer failure \nbecause name or url length long." ;
					case -3:
						return "EPG :: Adding timer failure \nbecause start time is in past." ;
					case -4:
						return "EPG :: Adding timer failure \nbecause conflicting with existing timer" ;
					case 0:
					default:
						return "EPG :: Adding timer failed" ;
				};
			},

		};

		this.okList = {
			mainListId: 8008,
			
			getMainListID: function( ) {
				return this.mainListId;
			},
			
			getCurrentChannel: function( callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.getCurrentChannel( ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getCurrentChannel( )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			getChannelsWithTemplate: function( list_id, start, limit, template, callback ) {
				console.log( "-> Platform.okList.getChannelsWithTemplate", list_id, start, limit, template ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.getChannels( list_id, start, limit, template ) ;
					
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getChannels( list_id, start, limit, Template)" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			createList: function( name, callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.createList( name ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.createList( name )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			getLists: function( channelTemplate, callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.getLists( channelTemplate ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getLists( name )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			getChannelsMrl: function( list_id, start, limit, callback ) {
				console.log( "-> Platform.okList.getChannelsMrl list_id = " + list_id ) ;
				var error = null ;
				var return_value = null ;
				var channelTemplate =  { "mrl":"" } ;

				try {
					return_value = MediaEngineChannels.getChannels( list_id, start, limit, channelTemplate ) ;
					
					
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.getChannelsMrl( list_id, start, limit, channelTemplate)" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			setChannels: function( id, channels, callback ) {
			//	console.log( "-> Platform.dvb.setChannels id = " + id + " channels = " + channels ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.setChannels( id, channels ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.setChannels( id, channels )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
			destroyList: function( id, callback ) {
				console.log( "-> Platform.okList.destroyList id = " + id ) ;

				var error = null ;
				var return_value = null ;

				try {
					return_value = MediaEngineChannels.destroyList( id ) ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEngineChannels.destroyList( id )" } ;
				}

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
		};

		this.time = {
			timeChecked:           false,
			syncDVBTimer:          null,

			syncDVBTime: function( ) {
		//	console.log ("Start sync DVB time!!!");

				function intervalFun(thisObj) {
					var time;
					if (thisObj.timeChecked == false) {
						try {
						if (new Date().getFullYear() < 2000) {
							time = MediaEngineTime.getDvbTime();
							if (time > 0) {
							var result = MediaEngineTime.setSysTime(time);
						}
						} else {
							thisObj.timeChecked = true;
							clearInterval(thisObj.syncDVBTimer);
							thisObj.syncDVBTimer = null;
						}
						} catch (e) {
							console.log (e);
						}
					}       
				}
        	    
				this.syncDVBTimer = setInterval(intervalFun(this), 1000);    	        
			},

			getTime: function( callback ) {
			//	console.log( "-> Platform.time.getTime" ) ;
				var return_value = Date.now( );

				if( !callback ) { return return_value ; }
				callback( null, return_value ) ;
			},

			getTimezone: function( callback ) {
			//	console.log( "-> Platform.time.getTimezone" ) ;
				var return_value = MediaEngineTime.getTimeZone();

				if( !callback ) { return return_value ; }
				callback( null, return_value ) ;
			},

			setTimezone: function( timezone, callback ) {
			//	console.log( "-> Platform.time.setTimezone", timezone ) ;
				
				var return_value = MediaEngineTime.setTimeZone(timezone);
                
				if( !callback ) { return return_value ; }
				callback( null, return_value ) ;
			},

		} ;

		var ip_settings =
		{
		    type: "",
		    dhcp: true,
		    // ip_type: "",
		    ip_address: "",
	        dns1: "",
		    dns2: "",
		    gateway: "",
		    mask: "",
	
		    wps : false,
		    ssid : "",
		    key : "",
		    security: "",

		    // ipv6_address: "",
		    // ipv6_DNS1: "",
		    // ipv6_DNS2: "",
		    // ipv6_gateway: "",
		    // ipv6_mask: "",

		};
		
		this.network = {

			// these are the conection Details

			// type
			ETHERNET:    0,
			WIRELESS:    2,
			NONE:		 4,

			// link state
			PLUGGED:     0,
			UNPLUGGED:   1,

			// state
			CONNECTED:	  1,
			CONNECTING:   0,
			DISCONNECTED: 2,
			ERROR:		  3,

			// dhcp
			DHCP:	     1,
			MANUAL:	     0,
			
			// wifi Scan state
			WIFI_SCAN_INIT:			0,
			WIFI_SCAN_IN_PROGRESS:	1,
			WIFI_SCAN_FINISHED:		2,
			WIFI_SCAN_ERROR:		4,

			/*
				settings
			     *  {
			     *    type: 		see_above
			     *    state: 		see_above
			     *    linkState: 	see_above
			     *    dhcp: 		see_above
			     *    ip_address: "xxxx",
			     *    DNS1: "xxxx",
			     *    DNS2: "xxxx",
			     *    gateway: "xxxx",
			     *    mask: "xxxx",
			     *  }
			*/

			interface_ids:
			{
				wifi_set: 0,
				lan_set: 0,
				none_set: 0,
				wifi_id: null,
				lan_id: null,
				none_id: null,
			},

			// gets the settings from store (only needet once a boot and shouhld be done before connections get astablished)
			init: function()
			{
				
				// get settings from store
				this.setConnectionDetails_type( window.Platform.storage.load("type", this.ETHERNET, "network"));
				this.setConnectionDetails_settings( window.Platform.storage.load("dhcp", true, "network"));
				this.setConnectionDetails_addres( window.Platform.storage.load("ip_address", "", "network"));
				this.setConnectionDetails_dns1( window.Platform.storage.load("dns1", "", "network"));
				this.setConnectionDetails_dns2( window.Platform.storage.load("dns2", "", "network"));
				this.setConnectionDetails_gateway( window.Platform.storage.load("gateway", "", "network"));
				this.setConnectionDetails_mask( window.Platform.storage.load("mask", "", "network"));
				this.setConnectionDetails_wps( window.Platform.storage.load("wps", false, "network"));
				this.setConnectionDetails_ssid( window.Platform.storage.load("ssid", "", "network"));
				this.setConnectionDetails_wifiKey( window.Platform.storage.load("wifiKey", "", "network"));
				this.setConnectionDetails_security( window.Platform.storage.load("security", "", "network"));
				

				// get interface_ids from store
				this.interface_ids.wifi_set = window.Platform.storage.load("wifi_set", 0, "network" );
				this.interface_ids.lan_set = window.Platform.storage.load("lan_set", 0, "network" );
				this.interface_ids.none_set = window.Platform.storage.load("none_set", 0, "network");
				this.interface_ids.wifi_id = window.Platform.storage.load("wifi_id", null, "network" );
				this.interface_ids.lan_id = window.Platform.storage.load("lan_id", null, "network" );
				this.interface_ids.none_id = window.Platform.storage.load("none_id", null, "network");
				
				
			},

			// this does not get the network configuration only the intern settings ( so be careful)
			get_settings: function()
			{
				return ip_settings;
			},


			/*
			*
			* This function set's the connection Details ( ETHERNET and WIRELESS and NONE )
			*
			* returns 0 on succses
			*	  1 if paramether istn't valid
			*/
 			setConnectionDetails_type: function( Details ){

				switch( Details )
				{
					case this.ETHERNET:
					case this.WIRELESS:
					case this.NONE:
						ip_settings.type = Details;
						return 0;

					default:
						return 1;
				};
			},


            setConnectionDetails_settings: function( Details )
			{
				switch( Details )
				{
					case this.DHCP:
						ip_settings.dhcp = Details;
						return 0;
					case this.MANUAL:
						ip_settings.dhcp = Details;
						return 0;

					default:
						return 1;
				};
			},

/*			setConnectionDetails_ip_type: function( Details )
			{
					switch( Details )
						{
							case IPV4:
							case IPV6:
							case IPV4_IPV6:
								settings.ip_type = Details;
								break;
							default:
								return 1;
						};
			}
*/
			setConnectionDetails_addres: function( ip_adr )
			{
				ip_settings.ip_address = this.normalise_ip( ip_adr );
				return 0;
			},

			setConnectionDetails_dns1: function( dns_adr )
			{
				ip_settings.dns1 = this.normalise_ip( dns_adr );
				return 0;
			},

			setConnectionDetails_dns2: function( dns_adr )
			{
				ip_settings.dns2 = this.normalise_ip( dns_adr );
				return 0;
			},

			setConnectionDetails_gateway: function( gatway_adr )
			{
				ip_settings.gateway = this.normalise_ip( gatway_adr );
				return 0;
			},

			setConnectionDetails_mask: function( mask_adr )
			{
				ip_settings.mask = this.normalise_ip( mask_adr );
				return 0;
			},
			
			setConnectionDetails_wps: function( bool )
			{
				ip_settings.wps = bool;
			},
			
			setConnectionDetails_ssid: function( ssid )
			{
				ip_settings.ssid = ssid;
			},
			
			setConnectionDetails_wifiKey: function( wifiKey )
			{
				ip_settings.key = wifiKey;
			},
			
			setConnectionDetails_security: function( security )
			{
				ip_settings.security = security;
			},

			//tryes to apply / find an iteface whit the wanted settings
			/*
			*
			*	if no iterface_Id is given the function will automaticly select one that fits (if one exists)
			*	the function will test if the given iterface id exist if the id does not exists
			*	it tryes to select one automaticly
			*
			*	return 	id on succses
			*			-1 ip_settings.type is ETHERNET and ethernet cable isn't plugged in
			*			-2 if unknown settings.type
			*			-3 if no pulged in ethernet interface is found on auto selection mode
			*			-4 if no wifi interface is found on auto selection mode 
			*/
			apply_settings: function( interface_Id )
			{
				
				// store settings
				window.Platform.storage.store("type", ip_settings.type, "network");
				window.Platform.storage.store("dhcp", ip_settings.dhcp, "network");
				window.Platform.storage.store("ip_address", ip_settings.ip_address, "network");
				window.Platform.storage.store("dns1", ip_settings.dns1, "network");
				window.Platform.storage.store("dns2", ip_settings.dns2, "network");
				window.Platform.storage.store("gateway", ip_settings.gateway, "network");
				window.Platform.storage.store("mask", ip_settings.mask, "network");
				window.Platform.storage.store("wps", ip_settings.wps, "network");
				window.Platform.storage.store("ssid", ip_settings.ssid, "network");
				window.Platform.storage.store("wifiKey", ip_settings.key, "network");
				window.Platform.storage.store("security", ip_settings.security, "network");
				
				if( interface_Id == null || !this.does_interface_exist(interface_Id) )
				{

					var interfaces;
					var interfaceId = null;
					if( ip_settings.type == this.ETHERNET )
					{
						//	interfaces = this.getInterfaces({type: "ethernet", linkState: "connected"});
						interfaces = this.getInterfaces( {type: "", linkState: "", id: "", name: ""} );
						for( var i in interfaces)
						{
							console.log( "Interface name: ", interfaces[i].name ) ;
							console.log( "Interface type: ", interfaces[i].type ) ;
							console.log( "Interface linkState: ", interfaces[i].linkState ) ;
							console.log( "Interface id: ", interfaces[i].id ) ;

							if (interfaces[i].type == this.ETHERNET )//&& interfaces[i].linkState == this.PLUGGED)
									interface_Id = interfaces[i].id;
						}

							if(interface_Id == null )
								return -3; // no Ethernet interface that is pluged in

					}
					else if( ip_settings.type == this.WIRELESS )
					{
						interfaces = this.getInterfaces( {type: "", linkState: "", id: ""} );
						for( var i in interfaces)
						{
							if (interfaces[i].type == this.WIRELESS )
							{
									interface_Id = interfaces[i].id;
									break;
							}
						}
							
							if(interface_Id == null )
								return -4; // no wifi interface

					}
					else if( ip_settings.type == this.NONE )
					{
						//	interfaces = this.getInterfaces({type: "ethernet", linkState: "connected"});
						interfaces = this.getInterfaces( {type: "", id: ""} );
						for( var i in interfaces)
						{

							if (interfaces[i].type == this.ETHERNET )
									interface_Id = interfaces[i].id;
						}

							if(interface_Id == null )
								return -3;		// no Ethernet interface
					}
					else
						return -2; // unknown settings type
				}
				/*else
				{
					// test if given Ehternet interface has a plugged in ethernet cable
					if( ip_settings.type == this.ETHERNET )
					{
						var interface_data = this.getInterfaceInfo(interface_Id)
						if ( interface_data.linkState != this.PLUGGED ) {
							return -1; // no ethernet cable is plugged in refuse to apply connection
						}
						
					}
					
				}*/

				
				
				if( ip_settings.type == this.NONE )
				{
					
					// workauround since deactivation
					// of all interfaces are not posible
					var none_ip_set =
						{
							dhcp: false,
							ip_address: "0.0.0.0",
							dns1: "0.0.0.0",
							dns2: "0.0.0.0",
							gateway: "0.0.0.0",
							mask: "0.0.0.0",
						};
					this.configureInterface( interface_Id, none_ip_set );
					this.activateInterface( interface_Id, true );
					return interface_Id;
				}
				

				
				

				console.log( "settings type:", ip_settings.type ) ;
				console.log( "settings dhcp:", ip_settings.dhcp ) ;
				console.log( "settings ip_address:", ip_settings.ip_address ) ;
				console.log( "settings dns1:", ip_settings.dns1 ) ;
				console.log( "settings dns2:", ip_settings.dns2 ) ;
				console.log( "settings mask:", ip_settings.mask ) ;
				console.log( "settings gatway:", ip_settings.gateway );
				console.log( "settings interface id:", interface_Id);
				console.log( "settings wps :", ip_settings.wps);
				console.log( "settings ssid:", ip_settings.ssid);
				console.log( "settings wifiKey:", ip_settings.key);
				console.log( "settigns security:", ip_settings.security);

				//this.activateInterface( this.getActiveInterface(), false);
				//ip_settings.interface = interface_Id;

				var ret = this.configureInterface( interface_Id, ip_settings )
				console.log(" return of config interface: ", ret );
				//this.activateInterface( interface_Id, true );
				return interface_Id;

			},
			
			/*
			 * this function is for the error_codes of apply_settings/auto_apply_settings
			 *
			 * it opens a alert_box with the appropriate message for the return code from apply_settings/auto_apply_settings
			 * if the return code isn't negative than there is no error and no alert_box is opend
			 *
			 * the functon returns the error_code that is given in
			 */
			standard_error_reaction: function( error_code )
			{
				if (error_code < 0)
				{	
					switch( error_code )
					{
						case -1:
							window.AppManager.getScene('alert_box').text(TR('alert msg screen :: Ethernet cable is not plugged in'));
							window.AppManager.enter('alert_box');
							break;
						
						case -2:
							window.AppManager.getScene('alert_box').text(TR('alert msg screen :: settings type is unknown'));
							window.AppManager.enter('alert_box');
							break;
						
						case -3:
							window.AppManager.getScene('alert_box').text(TR('alert msg screen :: no Ethernet interface with pluged in cable'));
							window.AppManager.enter('alert_box');
							break;
						
						case -4:
							window.AppManager.getScene('alert_box').text(TR('alert msg screen :: no wifi interface is available'));
							window.AppManager.enter('alert_box');
							break;
						
						case -5:
							window.AppManager.getScene('alert_box').text(TR('alert msg screen :: unexpected error occurred'));
							window.AppManager.enter('alert_box');
							break;
						
						default:
							window.AppManager.getScene('alert_box').text(TR('alert msg screen :: unexpected error occurred'));
							window.AppManager.enter('alert_box');
							break;
						
					};
				}
				return error_code;
			},

			/*
			 * returns the input ip without leading zeros
			 * z.b. 192.168.004.025 ----> 192.186.4.25
			 */
			normalise_ip: function(ip)
			{
				 return ip.split(".").map(Number).join(".");
			},
			
			/*
			 * tests if interface exists
			 *
			 * return 1 if interface exists
			 * return 0 if the interface dosen't exist
			 *
			 */
			does_interface_exist: function( interface_id )
			{
				// if the network media engin had sutch a function it would be nice
				// so mabey it will be possible replace the code below whit a function call if
				// sutch a function gest implemented in the media engine
				if(this.getInterfaceInfo( interface_id ).type == this.NONE)
					return 0;
				
				return 1;
			},

			
			wifi_callback: function(){},

			
			// this function can only save 1 callback
			register_callback_function_wifi_connect: function( callback )
			{
				this.wifi_callback = callback;
			},
			
			
			lan_callback: function(){},
			
			// this function can only save 1 callback
			register_callback_function_lan_connect: function( callback )
			{
				this.lan_callback = callback;

			},
			

			
			
			/*
			*
			*	this function set's the network connection
			*	according to the settings
			*	it also tryes to always chose the right interface it will be faster than
			*	apply_settings call whit no paramethers since it saves the interfaces
			*
			*	return 	return paramethers of apply_settings
			*			-1 ip_settings.type is ETHERNET and ethernet cable isn't plugged in ( return value from apply_settings )
			*			-2 if unknown settings.type ( return value from apply_settings )
			*			-3 if no pulged in ethernet interface is found on auto selection mode ( return value from apply_settings )
			*			-4 if no wifi interface is found on auto selection mode ( return value from apply_settings )
			*			-5 unexpexted error (you found a bug!!)
			*
			*/
			auto_apply_settings: function()
			{
				var interface_id;

				if( ip_settings.type == this.ETHERNET )
				{
					this.lan_callback();
					
					if (this.interface_ids.lan_set == false)
					{
						interface_id = this.apply_settings();
						if ( interface_id >= 0 )
						{
							this.interface_ids.lan_set = true;
							this.interface_ids.lan_id = interface_id;
							
							// store this.interface_ids
							window.Platform.storage.store("lan_set", this.interface_ids.lan_set, "network" );
							window.Platform.storage.store("lan_id", this.interface_ids.lan_id, "network" );
							
						}

						return interface_id;
					}
					else
					{
						
						// in case the id somehow changed 
						interface_id = this.apply_settings( this.interface_ids.lan_id);
						if ( interface_id >= 0 && interface_id != this.interface_ids.lan_id)
						{
							this.interface_ids.lan_id = interface_id;
							
							// store this.interface_ids
							window.Platform.storage.store("lan_id", this.interface_ids.lan_id, "network" );
							
						}

						return interface_id;
						
					}
				}
				else if ( ip_settings.type == this.WIRELESS )
				{
					this.wifi_callback();
					
					if (this.interface_ids.wifi_set == false)
					{
						interface_id = this.apply_settings();
						if ( interface_id >= 0 )
						{
							this.interface_ids.wifi_set = true;
							this.interface_ids.wifi_id = interface_id;
							
							// store this.interface_ids
							window.Platform.storage.store("wifi_set", this.interface_ids.wifi_set, "network" );
							window.Platform.storage.store("wifi_id", this.interface_ids.wifi_id, "network" );
						}

						return interface_id;
					}
					else
					{
						// in case the id somehow changed 
						interface_id = this.apply_settings( this.interface_ids.wifi_id);
						if ( interface_id >= 0 && interface_id != this.interface_ids.wifi_id)
						{
							this.interface_ids.wifi_id = interface_id;
							
							// store this.interface_ids
							window.Platform.storage.store("wifi_id", this.interface_ids.wifi_id, "network" );
						}

						return interface_id;

					}
				}
				else if (ip_settings.type == this.NONE)
					if (this.interface_ids.none_set == false)
					{
						interface_id = this.apply_settings();
						if ( interface_id >= 0 )
						{
							this.interface_ids.none_set = true;
							this.interface_ids.none_id = interface_id;
							
							// store this.interface_ids
							window.Platform.storage.store("none_set", this.interface_ids.none_set, "network" );
							window.Platform.storage.store("none_id", this.interface_ids.none_id, "network" );
						}

						return interface_id;
					}
					else
					{
						interface_id =  this.apply_settings( this.interface_ids.none_id);
						if ( interface_id >= 0 && interface_id != this.interface_ids.none_id )
						{
							this.interface_ids.none_id = interface_id;
							
							// store this.interface_ids
							window.Platform.storage.store("none_id", this.interface_ids.none_id, "network" );
						}

						return interface_id;
					}
				else
					return -2;

				return -5;
			},
			
            //*********************************************************************************
            //*********************************************************************************
            Selected_hotspot: {},
				
            scanWifiStart: function()
            {
                MediaEngineNetwork.scanWifiStart();
			},
            
            scanWifiStop: function()
            {
                MediaEngineNetwork.scanWifiStop();
            },
            
			//makes callback function to a slot
            set_onScanStateChanged_callback: function( function_callback )
            {
				/*
                this.scan_state_callback && MediaEngineNetwork.onScanStateChanged.disconnect( this.scan_state_callback ) ;
                
                this.scan_state_callback = function (state, hotspots) {
                    function_callback(state, hotspots);
                };
                */
                
                MediaEngineNetwork.onScanStateChanged.connect( function_callback );//this.scan_state_callback );
            },
			
			unset_onScanStateChanged_callback: function( function_callback )
			{
				MediaEngineNetwork.onScanStateChanged.disconnect( function_callback );
			},
         
            set_networkStateChanged_callback: function( function_callback )
            {
                this.network_state_callback && _StbNetworkObj.networkStateChanged.disconnect( this.network_state_callback ) ;
                
                this.network_state_callback = function ( ) {
                    function_callback( );
                };
                
                _StbNetworkObj.networkStateChanged.connect( this.network_state_callback );
            },

            activateWifiInterface: function( config, callback )
            {
                var error = null;
                var return_value = null;

                var return_value = _StbNetworkObj.applyConfig(config);

                if( !callback ) { return return_value ; }
                _.defer( function( ) { callback( null, return_value ) ; } ) ;
            },
            //*********************************************************************************
            //*********************************************************************************

			activateInterface: function( interfaceId, activate )
			{
				return MediaEngineNetwork.activateInterface( interfaceId, activate );                		
			},

			configureInterface: function( interfaceId, config )
			{
				return MediaEngineNetwork.configureInterface( interfaceId , config);
			},

			getInterfaceInfo: function( interfaceId )
			{
				return MediaEngineNetwork.getInterfaceInfo( interfaceId );
			},

			getInterfaces: function( interfaceTemplate )
			{
				return MediaEngineNetwork.getInterfaces( interfaceTemplate );
			},

			getActiveInterface: function( )
			{
				return MediaEngineNetwork.getActiveInterface();
			},

			set_onInterfaceChanged_callback: function( function_callback )
			{
				/*
				this.active_interface_callback && MediaEngineNetwork.onActiveInterfaceChanged.disconnect( this.active_interface_callback ) ;
                
				this.active_interface_callback = function ( param ) {
                    function_callback( param );
                };
                */
                
                MediaEngineNetwork.onActiveInterfaceChanged.connect( function_callback );//this.active_interface_callback );
			},
			
			unset_onInterfaceChanged_callback: function( function_callback_to_unset )
			{
				MediaEngineNetwork.onActiveInterfaceChanged.disconnect( function_callback_to_unset );
			},
			
			set_onInterfaceConfigurationError_callback: function( function_callback )
			{
				MediaEngineNetwork.onInterfaceConfigurationError.connect( function_callback );
			},
			
			set_onNetworkCableChanged_callback: function( function_callback )
			{
				MediaEngineNetwork.onNetworkCableChanged.connect( function_callback );
			},
			
			unset_onNetworkCableChanged_callback: function( function_callback )
			{
				MediaEngineNetwork.onNetworkCableChanged.disconnect( function_callback );
			},
			
			unset_onInterfaceConfigurationError_callback: function( function_callback_to_unset )
			{
				MediaEngineNetwork.onInterfaceConfigurationError.disconnect( function_callback_to_unset );				
			},
			
			getConnectionDetails: function( callback ) {
					console.log( "-> Platform.network.getConnectionType" ) ;

					var return_value = this.getActiveInterface(); //fake_data.connection_details( ) ;

					if( !callback ) { return return_value ; }
					_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

		} ;


		this.storage = {
			keys: function( group ) {
				return MediaEnginePersistence.keys( group );
			},
		    
			load: function( key, defValue, group ) {
				return MediaEnginePersistence.load( key, defValue, group ) ;
			},
		    
			store: function( key, value, group ) {
				MediaEnginePersistence.store( key, value, group ) ;
			},

			getDrives: function( callback ) {
				//console.log( "-> Platform.storage.getDrives" ) ;
				var return_value = null;
				//return_value = fake_data.drives( ) ;
                
				var driverTemplate = { 
					"mediaUrl": "",
					"path": "",
					"uuid": "",
					"label": "",
					"name": "",
					"size": "",
					"free": "",
					"fileSystem": "",
					"writable": "",
					"mounted": ""
				} ;
                
				return_value = MediaEngineDrives.getDrives(driverTemplate);
				console.log("Getting the info of drives: " + JSON.stringify(return_value));

				if( !callback ) { return return_value ; }
				callback( null, return_value ) ;
			},

			setOnDrivesChangedCallback: function( function_callback ) {
                this.drive_changed_callback && MediaEngineDrives.onDrivesChanged.disconnect( this.drive_changed_callback ) ;
                
                this.drive_changed_callback = function (drives) {
                    function_callback(drives);
                };
                
                MediaEngineDrives.onDrivesChanged.connect( this.drive_changed_callback );
            },

			getRecordingsList: function( callback ) {
			//	console.log( "-> Platform.storage.getRecordingsList" ) ;

				var return_value = fake_data.recordings( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

		} ;

		this.epg = {

			getChannelEvents: function( channel_mrl, start, stop, offset, count,  callback ) {
				//console.log( "-> Platform.epg.getChannelEvents" ) ;

				//var return_value = fake_data.channel_events( channel_id, range ) ;
				var return_value = MediaEngineEPG.loadSchedule( channel_mrl, start, stop, offset, count  ) ;
				//var return_value2 = MediaEngineEPG.loadNowNext( channel_mrl ) ;

				//console.log( JSON.stringify(return_value) );
				//console.log( JSON.stringify(return_value2) );

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			loadNowNext: function( channel_id, callback ) {
			//	console.log( "-> Platform.epg.loadNowNext for channel = " + channel_id ) ;

				//var return_value = fake_data.channel_events_current_next( channel_id ) ;
				var return_value = MediaEngineEPG.loadNowNext( channel_id ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
		} ;

		this.hkc = {

			getMessages: function( callback ) {
		//		console.log( "-> Platform.hkc.getMessages" ) ;

				var return_value = fake_data.messages( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},



		} ;

		 this.ca = {
			UIFreezed                                     : false,
			lockServiceEvent                              : false,		    
			action_request_ext_callback                   : null,
			lock_service_callback                         : null,
			unlock_service_callback                       : null,	
			show_finger_message_ext_callback              : null,
			notify_smc_status_callback                    : null,
			email_notify_icon_callback                    : null,
			show_OSD_message_callback                     : null,
			hide_OSD_message_callback                     : null,
			show_curtain_notify_callback                  : null,
			request_feeding_callback                      : null,
			ota_detected_callback                         : null,
			start_ippv_buydlg_callback                    : null,
			hide_ippv_dlg_callback                        : null,
			show_buy_message_callback                     : null,
			show_progress_strip_callback                  : null,


			InitCA:function(){
				nstv.Init();
			},
			CloseCA:function(){
				nstv.Close();
			},
			SetDRMHost:function(param){
			        nstv.SetDRMHost(param);
			},
			FormatBuffer:function(){
				nstv.FormatBuffer();
			},
			GetAccountProfile: function(param){
				return nstv.GetAccountProfile(param);
			},
			GetStatement: function(param){
				return nstv.GetStatement(param);
			},			
			GetCardStatus:function(){
				return nstv.GetCardStatus();
			},
 		 	GetTerminalTypeID:function(){
				return nstv.GetTerminalTypeID();
		 	},
		 	GetSTBID:function(){
				return nstv.GetSTBID();
		 	},
		 	GetSTBSecurity:function(){
				return nstv.GetSTBSecurity();
		 	},
		 	GetSTBVer:function(){
				return nstv.GetSTBVer();
		 	},
		 	GetVer:function(){
				return nstv.GetVer();
		 	},
		 	GetOperatorIds:function(){
				return nstv.GetOperatorIds();
		 	},
		 	GetOperatorInfo:function(opid){
				return nstv.GetOperatorInfo(opid);
		 	},
			GetCardSN: function() {
				return nstv.GetCardSN();
			},
			GetCardUpgradeStatus: function() {
				return nstv.GetCardUpgradeStatus();        
			},
			GetRating: function() {
				return nstv.GetRating();
			},
			GetWorkTime: function() {
				return nstv.GetWorkTime();
			},		
			IsPaired: function() {
				return nstv.IsPaired();
			},	
			VerifyPin:function(pin){
				return nstv.VerifyPin(pin);
			},		 	
			ChangePin:function(pin){
				return nstv.ChangePin(pin);		 				
			},
			SetRating:function(rt){
	 			return nstv.SetRating(rt);
			},
			GetEmailSpaceInfo:function(){
	 			return 	nstv.GetEmailSpaceInfo();
			},
			GetEmailHeads:function(req){	 	
				return nstv.GetEmailHeads(req);
			},
			GetEmailContent:function(eid){
				return nstv.GetEmailContent(eid);
			},
			DelEmail:function(eid){
				return nstv.DelEmail(eid);
			},
			SetWorkTime:function(wt){
			 	return nstv.SetWorkTime(wt);		 			
			},		 		
			GetServiceEntitles: function(opid){
				return nstv.GetServiceEntitles(opid);				
			},
	 			 		
			GetSlotIDs: function(opid){
				return nstv.GetSlotIDs(opid);			
			},
	 	
			GetSlotInfo: function(slot){
			 	return nstv.GetSlotInfo(slot);	
			},		 			 	
			GetACList: function(opid){
				return nstv.GetACList(opid);		
			},
	 			 		
			GetIPPVProgram: function(opid){	
				return nstv.GetIPPVProgram(opid);
			},
	 			 		
			GetOperatorChildStatus: function(opid){
				return nstv.GetOperatorChildStatus(opid);
			},
		 		
			GetDetitleChkNums: function(opid){
				return nstv.GetDetitleChkNums(opid);
			},
			ReadParent:function(opid){
	 			return nstv.ReadFeedDataFromParent(opid);		 		
	 		},
			WriteChild:function(opid){
	 			return nstv.WriteFeedDataToChild(opid);	
			},
			StopIPPVBuyDlg:function(buyIppv){
				return nstv.StopIPPVBuyDlg(buyIppv);	
			},
			SetLoaderParam:function(ldrParam){
				return nstv.SetLoaderParam(ldrParam);
			},
			
			setActionRequestExtCallback: function(function_callback) {
				this.action_request_ext_callback && nstv.onActionRequestExt.disconnect( this.action_request_ext_callback ) ;

				this.action_request_ext_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onActionRequestExt.connect( this.action_request_ext_callback ) ;
			},

			setLockServiceCallback: function(function_callback) {
				this.lock_service_callback && nstv.onLockService.disconnect( this.lock_service_callback ) ;

				this.lock_service_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onLockService.connect( this.lock_service_callback ) ;
			},

			setUNLockServiceCallback: function(function_callback) {
				this.unlock_service_callback && nstv.onUNLockService.disconnect( this.unlock_service_callback ) ;

				this.unlock_service_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onUNLockService.connect( this.unlock_service_callback ) ;
			},

			setShowFingerMessageExtCallback: function(function_callback) {
				this.show_finger_message_ext_callback && nstv.onShowFingerMessageExt.disconnect( this.show_finger_message_ext_callback ) ;

				this.show_finger_message_ext_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onShowFingerMessageExt.connect( this.show_finger_message_ext_callback ) ;   				 
			},	
			setNotifySmcStatusCallback:function( function_callback ) {
				this.notify_smc_status_callback && nstv.onNotifySmcStatus.disconnect( this.notify_smc_status_callback )
      	
				this.notify_smc_status_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onNotifySmcStatus.connect ( this.notify_smc_status_callback );
			},
			setEmailNotifyIconCallback:function( function_callback ) {
				this.email_notify_icon_callback && nstv.onEmailNotifyIcon.disconnect( this.email_notify_icon_callback )
      	
				this.email_notify_icon_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onEmailNotifyIcon.connect ( this.email_notify_icon_callback );				
			},
			setShowOSDMessageCallback:function( function_callback ) {
			      	this.show_OSD_message_callback && nstv.onShowOSDMessage.disconnect( this.show_OSD_message_callback )
      	
				this.show_OSD_message_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onShowOSDMessage.connect ( this.show_OSD_message_callback );					
			},
			setHideOSDMessageCallback:function( function_callback ) {
				this.hide_OSD_message_callback && nstv.onHideOSDMessage.disconnect( this.hide_OSD_message_callback )
      	
				this.hide_OSD_message_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onHideOSDMessage.connect ( this.hide_OSD_message_callback );			  	
			},
			setShowCurtainNotifyCallback:function( function_callback ) {
				this.show_curtain_notify_callback && nstv.onShowCurtainNotify.disconnect( this.show_curtain_notify_callback )
      	
				this.show_curtain_notify_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onShowCurtainNotify.connect ( this.show_curtain_notify_callback );					
			},
			setShowProgressStripCallback:function( function_callback ) {
				this.show_progress_strip_callback && nstv.onShowProgressStrip.disconnect( this.show_progress_strip_callback )
      	
				this.show_progress_strip_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onShowProgressStrip.connect ( this.show_progress_strip_callback );					
			},			
			setRequestFeedingCallback:function( function_callback ) {
				this.request_feeding_callback && nstv.onRequestFeeding.disconnect( this.request_feeding_callback )
      	
				this.request_feeding_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onRequestFeeding.connect ( this.request_feeding_callback );					
			},
			setOTADetectedCallback:function(function_callback){
				this.ota_detected_callback && nstv.onOTADetected.disconnect( this.ota_detected_callback )
      	
				this.ota_detected_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onOTADetected.connect ( this.ota_detected_callback );				
			},
		 	setStartIppvBuyDlgCallback:function(function_callback){
				this.start_ippv_buydlg_callback && nstv.onStartIppvBuyDlg.disconnect( this.start_ippv_buydlg_callback )
      	
				this.start_ippv_buydlg_callback = function( param ) {
					function_callback( param ) ;
				} ;
				nstv.onStartIppvBuyDlg.connect ( this.start_ippv_buydlg_callback );			 		
		 	},
			setHideIppvDlgCallback:function(function_callback){
				this.hide_ippv_dlg_callback && nstv.onHideIPPVDlg.disconnect( this.hide_ippv_dlg_callback )
      	
				this.hide_ippv_dlg_callback = function(param) {
					function_callback(param) ;
				} ;
				nstv.onHideIPPVDlg.connect ( this.hide_ippv_dlg_callback );					
				},		 	
			setShowBuyMessageCallback:function(function_callback){
				this.show_buy_message_callback && nstv.onShowBuyMessage.disconnect( this.show_buy_message_callback )
      	
				this.show_buy_message_callback = function( param ) {
					console.log("call back");
					function_callback( param ) ;
				} ;
				nstv.onShowBuyMessage.connect ( this.show_buy_message_callback );					
			},			
			
			PlayLockService: function( ) {
				window.Platform.media.stop();
				nstv.PlayLockService(); 
				this.lockServiceEvent = true;
			},
			
			StopLockService: function( ) {
				if (this.lockServiceEvent == true) {
					nstv.StopLockService();
					this.lockServiceEvent = false;
				}
			}
		 };

		 this.scanner = {
			dvbc_scan_callback: null,

			ScannerState: {
				"SCANNER_STATE_STOPPED" : 0,
				"SCANNER_STATE_INITIALIZING" : 1,
				"SCANNER_STATE_SCANNING" : 2,
				"SCANNER_STATE_SUCCEEDED" : 3,
				"SCANNER_STATE_FAILED" : 4,
				"SCANNER_STATE_CANCELLED" : 5,
				"SCANNER_STATE_INVALID" : 6,
			},

			getStatus: function( ) {
				return MediaEngineScanner.getStatus();
			},

			startDvbcScan: function(from_freq, to_freq, freq_step, bandwidth, symbolrate) {
				MediaEngineScanner.startDvbcScan(from_freq, to_freq, freq_step, bandwidth, symbolrate);
			},

			setType: function(type) {
				MediaEngineScanner.setType(type);
			},

			setDvbcScannerStatusCallback: function(function_callback) {
				this.dvbc_scan_callback && MediaEngineScanner.statusChanged.disconnect( this.dvbc_scan_callback ) ;

				this.dvbc_scan_callback = function() {
					var status = MediaEngineScanner.getStatus();
					function_callback( status ) ;
				} ;
				MediaEngineScanner.statusChanged.connect( this.dvbc_scan_callback ) ;
			}
		 };



		this.system = { 

			MediaEngineStandbyMode: {
	        		OFF: 0,
	        		ACTIVE: 1,
				PASSIVE: 2,
				ACTIVE_LED_ONLY: 3,
			},

			MediaEngineTuneStatus: {
				TUNE_WAITING: 0,
				TUNE_NO_LOCK: 1,
				TUNE_LOCKED: 2,
				TUNE_STOPPED: 3,
			},
			
			MediaEngineResetType: {
				"MEDIA_ENGINE_RESET_TYPE_NAND": 0,         /**< Erase the NAND flash partitions. */
				"MEDIA_ENGINE_RESET_TYPE_HDD": 1,          /**< Format the HDD. */
				"MEDIA_ENGINE_RESET_TYPE_NAND_AND_HDD": 2  /**< Reset both NAND and HDD. */
			},

			MediaEngineResetStatus: {
				"MEDIA_ENGINE_RESET_STATUS_UNKNOWN": 0,     /**< The status is unknown; a reset has not yet been started. */
				"MEDIA_ENGINE_RESET_STATUS_IN_PROGRESS": 1, /**< A factory reset is currently in progress. SysHalFactoryResetGetProgress will return a valid value. */
				"MEDIA_ENGINE_RESET_STATUS_FINISHED": 2,    /**< The factory reset is complete. The next stage should be to reboot the system. */
				"MEDIA_ENGINE_RESET_STATUS_ERROR": 3        /**< An error occurred during factory reset. */
			},

		 	setStandbyMode: function( mode ) {
		 		console.log( "-> Platform.system.setStandbyMode mode = " + mode ) ;	
		 		MediaEngineSystem.setStandbyMode( mode ) ;
		 	},

			setUIFreezed: function( ) {
				this.UIFreezed = true;
			},

			setUIUnFreezed: function( ) {
				this.UIFreezed = false;
			},

			rebootSTB: function( ) {
				MediaEngineSystem.reboot();
			},		
			
			factoryResetStart: function( type ) {
				MediaEngineSystem.factoryResetStart( type ) ;
			},    
			
			getFactoryResetStatus: function( ) {
				return MediaEngineSystem.getFactoryResetStatus();
			},
			
			getTuneStatus: function( ) {
				return MediaEngineSystem.getTuneStatus();
			}
		 };
		 this.browser = {
		 	onDemandId: -1,
			appId: -1,
		 };

		 this.settings = {

				//TODO: store the Data on the box		 	
				VideoModeStatus:{
					"VIDEO_MODE_1080I50":7,
					"VIDEO_MODE_720P50":5,
					"VIDEO_MODE_576I50":3,	 			
				},	 		
				RatioStatus:{
					"ASPECT_RATIO_16_9":1,
					"ASPECT_RATIO_4_3":2,
					"ASPECT_RATIO_AUTO":0,
				},
				VideoFormatStatus:{
					"VIDEO_ZOOM_NORMAL":0,
					"VIDEO_ZOOM_ENLARGE": 1,
					"VIDEO_ZOOM_MAKE_WIDER":   2,
					"VIDEO_ZOOM_NON_LINEAR_STRETCH_TO_FULL_SCREEN":4,
					"VIDEO_ZOOM_FULL_SCREEN":7,   
				},
				AudioOutputStatus:{
					"AUDIO_STEREO":0,
					"AUDIO_HDMI_SPDIF":1,
					"AUDIO_SPDIF":2,
				},
				
				VideoEnhance:{
					"VIDEO_ENHANCE_BRIGHTNESS":0,
					"VIDEO_ENHANCE_CONTRAST":1,
					"VIDEO_ENHANCE_SATURATION":2,
					"VIDEO_ENHANCE_SHARPNESS":3,
					"VIDEO_HUE":4,
				},
				
				setVideoEnhance: function(type, value) {
					return MediaEngineAVoutput.setVideoEnhance( type, value ) ;
				},

				setReminderPreAlertTime: function(value, callback) {
					var error = null ;
					var return_value = null ;
				
					try {
						return_value = MediaEnginePersistence.store( "ReminderPreAlertTime", value, "Reminder" ) ;
						}
					catch( e ) {
						error = { message: "Error calling MediaEnginePersistence.store( ReminderPreAlertTime, value, Reminder )" } ;
	
					}

					if( !callback ) { return error || return_value ; }
					_.defer( function( ) { callback( error, return_value ) ; } ) ;
				},
				
				getReminderPreAlertTime: function( callback ) {
					var error = null ;
					var return_value = null ;
				
					try {
						return_value = MediaEnginePersistence.load( "ReminderPreAlertTime", 2, "Reminder" ) ;
						}
					catch( e ) {
						error = { message: "Error calling MediaEnginePersistence.store( ReminderPreAlertTime, value, Reminder )" } ;
					}

					if( !callback ) { return error || return_value ; }
					_.defer( function( ) { callback( error, return_value ) ; } ) ;
				},

				setLanguage: function( lang ) {
					MediaEngineSystem.setLanguage(lang);      
				},
				
				getLanguage: function( ) {
					return MediaEngineSystem.getLanguage( ) ;      
				},				
                
				setConversionMode:function(mode){
					var Mode = this.VideoFormatStatus[mode];
					MediaEngineAVoutput.setConversionMode(Mode);    		
				},
				
				getConversionMode:function(){
					return MediaEngineAVoutput.getConversionMode();    			
				},
				
				setAspectRatio:function(ratio){
					var Ratio = this.RatioStatus[ratio];
					MediaEngineAVoutput.setAspectRatio(Ratio);        
				},
				
				getAspectRatio:function(){
					return MediaEngineAVoutput.getAspectRatio();        
				},	       
				
				setResolution: function( mode ) {
					var Mode = this.VideoModeStatus[mode];
					MediaEngineAVoutput.setResolution(Mode);
				},
				
				getResolution: function( ) {
					return MediaEngineAVoutput.getResolution();
				},
				
				setAudioOutput:function(mode){
					var Mode = this.AudioOutputStatus[mode];
					MediaEngineAVoutput.setAudioOutput(Mode);	        
				},
				
				getAudioOutput:function(){
					return MediaEngineAVoutput.getAudioOutput();	        
				},

				setAudioTrack:function(track){
					MediaEnginePlayback.setAudioTrack(track);
				},
				
				getAudioTrack:function(){
					return MediaEnginePlayback.getAudioTrack();
				},
				
				getAudioTracksInfo:function(){
					return MediaEnginePlayback.getAudioTracksInfo();
				},
				
				setIsFirstTimeRun: function( value ) {
					window.Platform.storage.store("IsFirstTimeRun", value, "");	
				},

				getIsFirstTimeRun: function() {
					return window.Platform.storage.load("IsFirstTimeRun", true, "");
				},

				setFrqHuntingRun: function( value ) {
					window.Platform.storage.store("frqHuntingRun", value, "");	
				},

				getFrqHuntingRun: function() {
					return window.Platform.storage.load("frqHuntingRun", true, "");
				},

				enterFrqHunting: function( callback ) {
					var leaveCallback = function( ) {
						window.Platform.settings.setFrqHuntingRun( false ) ;									
						callback && callback( ) ;
					} ;					
					
					if ( window.Template.EnableFrqHunting == true ) {
						var frqHunting = window.AppManager.getWidget( 'Auto Scan C') ;				    
						window.AppManager.enter( frqHunting.name ) ;
						frqHunting.setLeaveCallback( leaveCallback ) ;
					} else {
						leaveCallback();
					}		
				},

				firstTimeRun: function ( callback ) {
					if ( this.getIsFirstTimeRun() == true ) {
						window.Translation.setLanguage( window.Template.DefaultLanguage ) ;
						window.Platform.settings.setAspectRatio("ASPECT_RATIO_AUTO");
						window.Platform.settings.setConversionMode("VIDEO_ZOOM_NORMAL");
						window.Platform.settings.setResolution("VIDEO_MODE_1080I50");
						window.Platform.settings.setAudioOutput("AUDIO_STEREO");
						window.Platform.media.setVolume(100);
						window.Platform.settings.setIsFirstTimeRun( false ) ;		
					}
						
					if ( this.getFrqHuntingRun() == true ) {
						this.enterFrqHunting( callback ) ;
					}
					else {
						callback && callback( ) ;
					}
				},

				getChannelNumberType: function( callback ) {
					var channelnumbertype = window.Template.customersetting.channelnumbertype;
					if (channelnumbertype == "") {
						channelnumbertype = "index";
					};
					
					return channelnumbertype;
				}

		 };
	} ;

	window.Platform = Platform ;

})( ) ;
