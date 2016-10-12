"use strict";

//var MediaEngineScanner = new media_engine_scanner();
//var MediaEngineChannels = new media_engine_channels();
//var MediaEnginePlayback = new media_engine_playback();
//var MediaEngineEPG = new media_engine_epg();
//var Tv2StbCommon = new tv2_stb_common();


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

		channel_data: {
			"dvb://100.100.100": {
				description: {
					service_type: 1,
					mrl: "dvb://100.100.100",
					name: "Channel 000",
					onid: 100,
					tsid: 100,
					sid: 100,				
					full_name: "Channel 000",				
					short_name: "Channel 000",				
					lcn: "1",
					locked: false,			
					unavailable: 0,					
					not_running: 0,					
					recent: 0,					
					scrambled: false,
					adb_service_type: "",
					index: 0,
				},
				epg: [
					{ range: { start: fake_epgdate +  0			, stop: fake_epgdate +  2*h       , duration:  7200  }, name: "Telemarketing", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  2*h       , stop: fake_epgdate +  7*h + 30*m, duration: 19800  }, name: "no programme", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  7*h + 30*m, stop: fake_epgdate +  9*h       , duration:  5400  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  9*h       , stop: fake_epgdate + 10*h + 15*m, duration:  4500  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 10*h + 15*m, stop: fake_epgdate + 11*h + 20*m, duration:  3900  }, name: "film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 11*h + 20*m, stop: fake_epgdate + 12*h       , duration:  2400  }, name: "Morning News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 12*h       , stop: fake_epgdate + 13*h + 40*m, duration:  6000  }, name: "Thriller", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 13*h + 40*m, stop: fake_epgdate + 20*h       , duration: 22800  }, name: "Documentation", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 20*h       , stop: fake_epgdate + 21*h       , duration:  3600  }, name: "Evening News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h       , stop: fake_epgdate + 21*h + 10*m, duration:   600  }, name: "Weather Forecast", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 10*m, stop: fake_epgdate + 21*h + 30*m, duration:  1200  }, name: "Sports News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 30*m, stop: fake_epgdate + 23*h       , duration:  5400  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 23*h       , stop: fake_epgdate + 24*h       , duration:  3600  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
				],
			},
			"dvb://101.101.101": {
				description: {
					service_type: 1,
					mrl: "dvb://101.101.101",
					name: "Televisio",
					onid: 101,
					tsid: 101,
					sid: 101,				
					full_name: "Televisio",				
					short_name: "Televisio",				
					lcn: "5",
					locked: false,			
					unavailable: 0,					
					not_running: 0,					
					recent: 0,					
					scrambled: false,
					adb_service_type: "",
                    index: 1,
				},
				epg: [
					{ range: { start: fake_epgdate +  0			, stop: fake_epgdate +  3*h       , duration: 10800  }, name: "Telemarketing", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  3*h       , stop: fake_epgdate +  5*h + 30*m, duration:  9000  }, name: "no programme", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  5*h + 30*m, stop: fake_epgdate +  9*h       , duration: 12600  }, name: "breakfast show", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  9*h       , stop: fake_epgdate + 10*h + 15*m, duration:  4500  }, name: "Newstime", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 10*h + 15*m, stop: fake_epgdate + 11*h + 20*m, duration:  3900  }, name: "film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 11*h + 20*m, stop: fake_epgdate + 12*h       , duration:  2400  }, name: "Morning News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 12*h       , stop: fake_epgdate + 13*h + 40*m, duration:  6000  }, name: "Thriller", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 13*h + 40*m, stop: fake_epgdate + 20*h       , duration: 22800  }, name: "Documentation", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 20*h       , stop: fake_epgdate + 21*h       , duration:  3600  }, name: "Evening News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h       , stop: fake_epgdate + 21*h + 10*m, duration:   600  }, name: "Weather Forecast", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 10*m, stop: fake_epgdate + 21*h + 30*m, duration:  1200  }, name: "Sports News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 30*m, stop: fake_epgdate + 23*h       , duration:  5400  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 23*h       , stop: fake_epgdate + 24*h       , duration:  3600  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
				],
			},
			"dvb://103.103.103": {
				description: {

					service_type: 1,
					mrl: "dvb://103.103.103",
					name: "Cabelaria",
					onid: 103,
					tsid: 103,
					sid: 103,				
					full_name: "Cabelaria",				
					short_name: "Cabelaria",				
					lcn: "8",
					locked: false,			
					unavailable: 0,					
					not_running: 0,					
					recent: 0,					
					scrambled: false,
					adb_service_type: "",
                    index: 2,
				},
				epg: [
					{ range: { start: fake_epgdate +  0			, stop: fake_epgdate +  2*h       , duration:  7200  }, name: "Telemarketing", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  2*h       , stop: fake_epgdate +  7*h + 30*m, duration: 19800  }, name: "no programme", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  7*h + 30*m, stop: fake_epgdate +  9*h       , duration:  5400  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  9*h       , stop: fake_epgdate + 10*h + 15*m, duration:  4500  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 10*h + 15*m, stop: fake_epgdate + 11*h + 20*m, duration:  3900  }, name: "film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 11*h + 20*m, stop: fake_epgdate + 12*h       , duration:  2400  }, name: "Morning News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 12*h       , stop: fake_epgdate + 13*h + 40*m, duration:  6000  }, name: "Thriller", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 13*h + 40*m, stop: fake_epgdate + 20*h       , duration: 22800  }, name: "Documentation", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 20*h       , stop: fake_epgdate + 21*h       , duration:  3600  }, name: "Evening News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h       , stop: fake_epgdate + 21*h + 10*m, duration:   600  }, name: "Weather Forecast", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 10*m, stop: fake_epgdate + 21*h + 30*m, duration:  1200  }, name: "Sports News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 30*m, stop: fake_epgdate + 23*h       , duration:  5400  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 23*h       , stop: fake_epgdate + 24*h       , duration:  3600  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
				],
			},
			"dvb://104.104.104": {
				description: {
					service_type: 1,
					mrl: "dvb://104.104.104",
					name: "Antena",
					onid: 104,
					tsid: 104,
					sid: 104,				
					full_name: "Antena",				
					short_name: "Antena",				
					lcn: "9",
					locked: false,			
					unavailable: 0,					
					not_running: 0,					
					recent: 0,					
					scrambled: false,
					adb_service_type: "",
			        index: 3,			
				},
				epg: [
					{ range: { start: fake_epgdate +  0			, stop: fake_epgdate +  2*h       , duration:  7200  }, name: "Telemarketing", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  2*h       , stop: fake_epgdate +  7*h + 30*m, duration: 19800  }, name: "no programme", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  7*h + 30*m, stop: fake_epgdate +  9*h       , duration:  5400  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  9*h       , stop: fake_epgdate + 10*h + 15*m, duration:  4500  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 10*h + 15*m, stop: fake_epgdate + 11*h + 20*m, duration:  3900  }, name: "film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 11*h + 20*m, stop: fake_epgdate + 12*h       , duration:  2400  }, name: "Morning News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 12*h       , stop: fake_epgdate + 13*h + 40*m, duration:  6000  }, name: "Thriller", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 13*h + 40*m, stop: fake_epgdate + 20*h       , duration: 22800  }, name: "Documentation", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 20*h       , stop: fake_epgdate + 21*h       , duration:  3600  }, name: "Evening News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h       , stop: fake_epgdate + 21*h + 10*m, duration:   600  }, name: "Weather Forecast", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 10*m, stop: fake_epgdate + 21*h + 30*m, duration:  1200  }, name: "Sports News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 30*m, stop: fake_epgdate + 23*h       , duration:  5400  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 23*h       , stop: fake_epgdate + 24*h       , duration:  3600  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
				],
			},
			"dvb://106.106.106": {
				description: {
					service_type: 1,
					mrl: "dvb://106.106.106",
					name: "TV Sat",
					onid: 106,
					tsid: 106,
					sid: 106,				
					full_name: "TV Sat",				
					short_name: "TV Sat",				
					lcn: "10",
					locked: false,			
					unavailable: 0,					
					not_running: 0,					
					recent: 0,					
					scrambled: false,
					adb_service_type: "",
                    index: 4,
				},
				epg: [
					{ range: { start: fake_epgdate +  0			, stop: fake_epgdate +  2*h       , duration:  7200  }, name: "Telemarketing", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  2*h       , stop: fake_epgdate +  7*h + 30*m, duration: 19800  }, name: "no programme", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  7*h + 30*m, stop: fake_epgdate +  9*h       , duration:  5400  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  9*h       , stop: fake_epgdate + 10*h + 15*m, duration:  4500  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 10*h + 15*m, stop: fake_epgdate + 11*h + 20*m, duration:  3900  }, name: "film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 11*h + 20*m, stop: fake_epgdate + 12*h       , duration:  2400  }, name: "Morning News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 12*h       , stop: fake_epgdate + 13*h + 40*m, duration:  6000  }, name: "Thriller", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 13*h + 40*m, stop: fake_epgdate + 20*h       , duration: 22800  }, name: "Documentation", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 20*h       , stop: fake_epgdate + 21*h       , duration:  3600  }, name: "Evening News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h       , stop: fake_epgdate + 21*h + 10*m, duration:   600  }, name: "Weather Forecast", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 10*m, stop: fake_epgdate + 21*h + 30*m, duration:  1200  }, name: "Sports News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 30*m, stop: fake_epgdate + 23*h       , duration:  5400  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 23*h       , stop: fake_epgdate + 24*h       , duration:  3600  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
				],
			},
			"dvb://200.200.200": {
				description: {
					service_type: 1,
					mrl: "dvb://200.200.200",
					name: "After Eight",
					onid: 200,
					tsid: 200,
					sid: 200,				
					full_name: "After Eight",				
					short_name: "After Eight",				
					lcn: "12",
					locked: false,			
					unavailable: 0,					
					not_running: 0,					
					recent: 0,					
					scrambled: true,
					adb_service_type: "",
                    index: 5,					
				},
				epg: [
					{ range: { start: fake_epgdate +  0			, stop: fake_epgdate +  2*h       , duration:  7200  }, name: "Telemarketing", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  2*h       , stop: fake_epgdate +  7*h + 30*m, duration: 19800  }, name: "no programme", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  7*h + 30*m, stop: fake_epgdate +  9*h       , duration:  5400  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate +  9*h       , stop: fake_epgdate + 10*h + 15*m, duration:  4500  }, name: "cartoons", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 10*h + 15*m, stop: fake_epgdate + 11*h + 20*m, duration:  3900  }, name: "film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 11*h + 20*m, stop: fake_epgdate + 12*h       , duration:  2400  }, name: "Morning News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 12*h       , stop: fake_epgdate + 13*h + 40*m, duration:  6000  }, name: "Thriller", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 13*h + 40*m, stop: fake_epgdate + 20*h       , duration: 22800  }, name: "Documentation", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 20*h       , stop: fake_epgdate + 21*h       , duration:  3600  }, name: "Evening News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h       , stop: fake_epgdate + 21*h + 10*m, duration:   600  }, name: "Weather Forecast", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 10*m, stop: fake_epgdate + 21*h + 30*m, duration:  1200  }, name: "Sports News", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 21*h + 30*m, stop: fake_epgdate + 23*h       , duration:  5400  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
					{ range: { start: fake_epgdate + 23*h       , stop: fake_epgdate + 24*h       , duration:  3600  }, name: "Film", shortdesc: "", extendeddesc: "", hassubtitle: false, hasaudiodesc: false, parentalage: 0, contentgenre: "", isfta: true },
				],
			},
		},

		all_channels: function( ) {
			console.log( "-> Platform.all_channels" );
			console.log( "-> Platform.all_channels: " +  _.pluck( _.values( this.channel_data ), 'description' ) );
			return _.pluck( _.values( this.channel_data ), 'description' ) ;
		},

		current_channel: function( ) {
			var list = fake_data.all_channels( ) ;
			var index = rand( list.length ) ;
			var channel = list[index] ;
			console.log( "-> Platform.current_channel mrl = " + channel.mrl );
			console.log( "-> Platform.current_channel lcn = " + channel.lcn );
			return channel ;
		},

		channels_list: function( start, limit ) {
			console.log( "-> Platform.channels_list" );
			console.log( "-> Platform.channels_list: " + _.pluck( fake_data.all_channels( ), 'mrl' ).slice( start, start + limit ));
			return fake_data.all_channels( ).slice( start, start + limit ) ;
		},

		channels_mrl_list: function( start, limit ) {
			console.log( "-> Platform.channels_list" );
			console.log( "-> Platform.channels_list: " + _.pluck( fake_data.all_channels( ), 'mrl' ).slice( start, start + limit ));
			return _.pluck( fake_data.all_channels( ), 'mrl' ).slice( start, start + limit ) ;
		},

		channels_number: function( ) {
			console.log( "-> Platform.channels_number length = " + fake_data.all_channels( ).length )
			return fake_data.all_channels( ).length ;
		},

		channel_info: function( mrl ) {
			console.log( "-> Platform.channel_info mrl = " + mrl )
			console.log( "-> Platform.channel_info: " + fake_data.channel_data, mrl )
			return fake_data.channel_data[mrl].description ;
		},

		channel_events: function( mrl, range ) {
			console.log( "-> Platform.channel_events - mrl:" + mrl ) ;
			var epg = this.channel_data[mrl].epg ;

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
							startDateTime: event.range.start,
							stopDateTime:  event.range.stop,
						} ;
						events.push( _event ) ;
					}
				}
			}

			return events ;
		},

		channel_events_current_next: function( mrl ) {
			console.log( "-> Platform.channel_events_current_next - mrl:" + mrl ) ;

			var events = [] ;

			var epg = this.channel_data[mrl].epg ;

			for( var i in epg ) {
				console.log( "-> Platform.channel_events_current_next epg - i:" + i ) ;
				var event = epg[i] ;
				var from = event.range.start;
				var to   = event.range.stop;
				var real_timestamp = window.Platform.time.getTime( ) ;
				
				if (from <= real_timestamp && real_timestamp <= to )	{
					var _event = {
                        name: event.name,
						startDateTime: event.range.start,
						stopDateTime:  event.range.stop,
						duration: event.range.duration,
					} ;
					events.push( _event ) ;
					//return _event;
					var j = 0;
					j = parseInt(i) + 1;
	
					if (epg.length >= j ) {
					
						console.log( "-> Platform.channel_events_current_next - next event i+1 = " + j);

						var event_next = epg[j] ;
						var _event_next = {
                            name: event_next.name,
						    startDateTime: event_next.range.start,
							stopDateTime:  event_next.range.stop,
							duration: event_next.range.duration,
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

		connection_changed_callback: [],
		drives_changed_callback: null,
/*
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
*/
		wireless_signal: function( ) {
			return rand( 101 ) ;
		},
		

		// this here simulates the functionality of the network stb api for the pc version
		// it could be that i misunderstood some descriptions of the api
		// but i hope o got everthing right
		onInterfaceChanged_connect: function( callback )
		{
			this.connection_changed_callback.push( callback );
		},

		empty: function( inter ) { },

		onInterfaceChanged_disconnect: function( callback )
		{
			for( var i in this.connection_changed_callback )
			{
				if( callback == this.connection_changed_callback[i] )
					// this is a bit a dirty trik since i don't know how to make lists in java script
					// if you know how to do it just replace this line whit an remove_elemnt(i) or such a thing
					// the trik can be used here since in the pc verion there is no save so the list will not grow into infinity
					this.connection_changed_callback[i] = empty;
				
			}
		},
		
		onInterfaceChanged: function( inter )
		{

			for(var i in this.connection_changed_callback )
			{
				this.connection_changed_callback[i]( inter );
			}
		},
			

		activateInterface: function( interfaceId, activate )
		{
			var old = getActiveInterface();
			
			old.active = false;
			old.state = window.Platform.network.DISCONNECTED;

			
			var inter = get_interface_by_id( interfaceId );
			
			inter.active = true;
			inter.state = window.Platform.network.CONNECTED;
			
			onInterfaceChanged( inter );
			
			return 1;
		},

		get_interface_by_id: function(id)
		{
			for( var i in this.fake_interface() )
				if( this.fake_interface()[i].id == id )
					return this.fake_interface()[i];

			return 0;
		},		

		configureInterface: function( interfaceId, config )
		{
			var inter = get_interface_by_id();
			

			if( config.dhcp != null )
				inter.dhcp = config.dhcp;

			if( config.ip_address != null )
				inter.ip_address = config.ip_address;
			
			if( config.dns1 != null )
				inter.dns1 = config.dns1;

			if( config.dns2 != null )
				inter.dns2 = config.dns2;

			if( config.gateway != null )
				inter.gateway = config.gateway;

			if( config.mask != null )
				inter.mask = config.mask;

			onInterfaceChanged(inter);
				
		},

		getInterfaceInfo: function( interfaceId )
		{
			return this.get_interface_by_id( interfaceId );
		},
		
		// the std returns here only the properis in the template 
		// because of recurse resons here we rentun all because here we dont have them
		// we still filter the results
		getInterfaces: function( interfaceTemplate )
		{
			var all = fake_interface();
			return all;
/*
			var take = [];
			var ret = [];
			
	
//			turns out the media engine dosnt do taht :( 
//			would have been nice
			for( var i in all )
			{
				take[i] = 1;				

				if( interfaceTemplate.type != null )
					if(interfaceTemplate.type != all[i].type )
						take[i] = 0;
			

				if( interfaceTemplate.state != null )
					if( interfaceTemplate.state != all[i].state )
						take[i] = 0;


				if( interfaceTemplate.dhcp != null )
					if( interfaceTemplate.dhcp != all[i].dhcp )
						take[i] = 0;

			}
			var j = 0;
			for( var i in take )
				if(take[i] == 1)
				{
					ret[j] = all[i];
					++j;
				}


			return ret;
*/
		},


		getActiveInterface: function( )
		{
			for( var i in this.fake_interface() )
			{
				if( this.fake_interface()[i].active )
					return this.fake_interface()[i];
			}
			
			return 0; 
		},
	
		getWifiHotspots: function( )
		{
			return _.where(this.fake_interface(), {type: window.Platform.network.WIRELESS});
		},


		fake_interface: function()
		{
		return	[
				{
					id: "1",
					active: false,
					type: window.Platform.network.WIRELESS, 
					state: window.Platform.network.DISCONNECTED,
	//				linkState: "disconnected", "connected",
					dhcp: 1,
					ip_address: "192.168.231.33",
					dns1: "8.8.8.8",
					dns2: "",
					gateway: "192.168.231.55",
					mask: "255.255.255.0",
					ssid: "ip4_office",
					security: 2,
					signal: 77,
				},
				{
					id: "2",
					active: false,
					type:  window.Platform.network.ETHERNET,
					state: window.Platform.network.CONNECTED,
					linkState: window.Platform.network.PLUGGED,
					dhcp: 1,
					ip_address: "192.168.144.21",
					dns1: "8.8.8.8",
					dns2: "8.8.4.4",
					gateway: "192.168.144.1",
					mask: "255.255.255.0",
				},
				{
					id: "3",
					active: false,
					type: window.Platform.network.WIRELESS, 
					state: window.Platform.network.DISCONNECTED,
	//                              linkState: "disconnected",
					dhcp: 0,
					ip_address: "",
					dns1: "",
					dns2: "",
					gateway: "",
					mask: "",
					ssid: "sptest",
					security: 3,
					signal: 92,
				},
				{
					id: "4",
					active: true,
					type: window.Platform.network.WIRELESS,
					state: window.Platform.network.DISCONNECTED,
	//                              linkState: "disconnected", "connected",
					dhcp: 1,
					ip_address: "",
					dns1: "",
					dns2: "",
					gateway: "",
					mask: "",
                                        ssid: "conference_room",
                                        security: 3,
                                        signal: 45,
				},
			];
		},

		// end of network stb simultion


		drives: function( ) {
			return [
				{
					name: "hdd_1",
					path: '/mnt/hdd_1',
					fileSystem: 'FAT32',
					size: 8 * Math.pow(1024, 6),
					free: 5 * Math.pow(1024, 6),
				},
                {
					name: "hdd_2",
					path: '/mnt/hdd_2',
					fileSystem: 'NTFS',
					size: 16 * Math.pow(1024, 6),
					free: 2 * Math.pow(1024, 6),
				},
                {
					name: "hdd_3",
					path: '/mnt/hdd_3',
					fileSystem: 'FAT32',
					size: 32 * Math.pow(1024, 6),
					free: 20 * Math.pow(1024, 6),
				}
			] ;
		},
     
        getUpcommingTimer: function () {
            return this.timers()[0];
        },
     
        timers: function()
		{
		return	[
				{
					name: "Program 1",
					serviceName: "Channel 1",
					time: Date.now( ) / 1000 + 2*60, // just with secs
					length: 60 * m / s,  // just with secs
				},
				{
					name: "Program 2",
					serviceName: "Channel 2",
					time: Date.now( ) / 1000 + 2*60*60, // just with secs
					length: 60 * m / s,  // just with secs
				},
				{
					name: "Program 3",
					serviceName: "Channel 3",
					time: Date.now( ) / 1000 + 4*60*60, // just with secs
					length: 60 * m / s,  // just with secs
				},
				{
					name: "Program 4",
					serviceName: "Channel 4",
					time: Date.now( ) / 1000 + 6*60*60, // just with secs
					length: 60 * m / s,  // just with secs
				},
			];
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


		messages: function( ) {
			return [
				{
					date: new Date( Date.now( ) - 100000 ),
					title: "Message 1",
					is_read: true,
				},
				{
					date: new Date( Date.now( ) - 200000 ),
					title: "Message 9",
					is_read: true,
				},
				{
					date: new Date( Date.now( ) - 300000 ),
					title: "Message 2",
					is_read: true,
				},
				{
					date: new Date( Date.now( ) - 400000 ),
					title: "Message 8",
					is_read: true,
				},
				{
					date: new Date( Date.now( ) - 500000 ),
					title: "Message 6",
					is_read: true,
				},
			] ;
		},
		
		service_info: function( ){
		return[
			{
					service_name:"HongKongTV",
					service_endtime:new Date( Date.now( ) - 100000 ),
			},
			{
					service_name:"ShenZhenTV",
					service_endtime:new Date( Date.now( ) - 110000 ),		
			},
			{
					service_name:"ShenZhenTV",
					service_endtime:new Date( Date.now( ) - 110000 ),			
			},
			{
					service_name:"ShenZhenTV",
					service_endtime:new Date( Date.now( ) - 110000 ),	
			},
			{
					service_name:"ShenZhenTV",
					service_endtime:new Date( Date.now( ) - 110000 ),	
			},
			{
					service_name:"ShenZhenTV",
					service_endtime:new Date( Date.now( ) - 110000 ),	
			},
			{
					service_name:"ShenZhenTV",
					service_endtime:new Date( Date.now( ) - 110000 ),	
			},
			{
					service_name:"ShenZhenTV",
					service_endtime:new Date( Date.now( ) - 110000 ),	
			},

			];
		},
		
		service_info_last_updatetime: function(){
				return Date.now();
		},
		
		statement:function(){
			return[{
			statementdate   : new Date(Date.now()),
			duedate   : new Date(Date.now()-120000),
			accountnum : "231231234435465",
			previousamountdue   : 245.20,
			paymentrecieved: "0CR",
			previousblance: 45.00,
			currentservicecharges: 198.00,
			othercharges  : 100.00,
			chargeadjustment : "0.6CR",
			valueaddedservice : 68.41,
			totalamountdue : 411.00,
			}	
			];
		},
				

		ratingData:[{
				currentrating:11,
			}],
			
		rating:function(){
			return fake_data.ratingData;
		},					

		
		ippv:function(){
				return[{
					caption:"IPPV",
					productId:"11111",
					OperatorId:"111",
					experidtime:new Date(Date.now( )),
					price:[100,200,300,400],
					slotId:"111",				
					}
				];	
		},
		ippt:function(){
			return[{
				caption:"IPPT",
				productId:"11111",
				OperatorId:"111",
				experidtime:new Date(Date.now( )),
				price:[100,200],
				slotId:"1111111",		
				}
			];
		},
		
		password:function(){
			return '123456';
			},
			
		system_info:function(){
			return[{
					SmartCardNumber:"299303203",
					MACAddress:"00-22-5F-7B-EC-0B",
					STBSerialNumber:"AOOC772D",
					IPAddress:"192.168.1.10",
			}
			];	
		},
		emails_data: [
				{
					status:1,
					id:0x01,
					importance:2,
					time:new Date(Date.now( )),
					title:"First Email",
					
				},
				{
					status:1,
					id:0x02,
					importance:1,
					time:new Date(Date.now( )),
					title:"Second Email",
					
				},
				{
					status:1,
					id:0x03,
					importance:2,
					time:new Date(Date.now( )),
					title:"Third Email",
					
				},
				{
					status:1,
					id:0x04,
					importance:3,
					time:new Date(Date.now( )),
					title:"Forth Email",
					
				},
				{
					status:0,
					id:0x05,
					importance:3,
					time:new Date(Date.now( )),
					title:"Fifth Email",
					
				},
				{
					status:0,
					id:0x06,
					importance:4,
					time:new Date(Date.now( )),
					title:"Sixth Email",
					
				},
				{
					status:0,
					id:0x07,
					importance:1,
					time:new Date(Date.now( )),
					title:"Seventh Email",
					
				},
				{
					status:0,
					id:0x08,
					importance:1,
					time:new Date(Date.now( )),
					title:"Eighth Email",
					
				},
				{
					status:0,
					id:0x09,
					importance:1,
					time:new Date(Date.now( )),
					title:"ninth Email",
					
				},{
					status:0,
					id:0xa0,
					importance:1,
					time:new Date(Date.now( )),
					title:"tenth Email",
					
				}],
	} ;


	var Platform = new function( ) {						    
		this.keymap = {
			  8: "BACK", // backspace
			  9: "INFO", // tab
			 13: "OK",   // enter
			 27: "EXIT", // escape
			 36: "MENU", // home

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

			112: "RED",    // F1
			113: "GREEN",  // F2
			114: "YELLOW", // F3
			115: "BLUE",   // F4

			116: "VOLUP",    // F5
			117: "VOLDOWN",  // F6
			118: "CHANUP",   // F7
			119: "CHANDOWN", // F8

			120: "EPG",     // F9
			121: "MUTE",    // F10
			122: "POWER",   // F11
			123: undefined, // F12
		} ;

		this.init = function( done ) {
			done && done( ) ;
		} ;


		this.media = {

			play: function( url, callback ) {
				console.log( "-> Platform.media.play", url ) ;

				if( !callback ) { return ; }
				_.delay( function( ) { callback( null ) ; }, Math.random( ) * 1000 ) ;
			},

			stop: function( callback ) {
				console.log( "-> Platform.media.stop" ) ;

				if( !callback ) { return ; }
				_.delay( function( ) { callback( null ) ; }, Math.random( ) * 1000 ) ;
			},

			getVolume: function( callback ) {
				console.log( "-> Platform.media.getVolume" ) ;

				var return_value = fake_data.volume( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			setVolume: function( volume, callback ) {
				console.log( "-> Platform.media.setVolume" ) ;

				var return_value = fake_data.set_volume( volume ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			getMute: function( callback ) {
				console.log( "-> Platform.media.getMute" ) ;

				var return_value = fake_data.get_mute( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			setMute: function( mute, callback ) {
				console.log( "-> Platform.media.setMute" ) ;

				var return_value = fake_data.set_mute( mute ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			setVideoPosition: function( x, y, w, h, callback )	{
				//console.log( "-> Platform.media.setVideoPosition" ) ;

				var return_value = 0 ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;

			}

		} ;


		this.dvb = {

			Channels: [],			
			ChannelsMrl: [],
			ChannelCurrent: [],
			
			getCurrentChannel: function( callback ) {
				console.log( "-> Platform.dvb.getCurrentChannel" ) ;

		                if( typeof(this.ChannelCurrent.mrl) =="undefined" )
		                {
		                    this.ChannelCurrent = fake_data.current_channel( ) ;
		                }
		                else
		                {
		                    this.ChannelCurrent = fake_data.channel_info( this.ChannelCurrent.mrl ) ;
		                }

		                var return_value = this.ChannelCurrent ;
 
				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			getChannels: function( list_id, start, limit, callback ) {
				console.log( "-> Platform.dvb.getChannels", list_id, start, limit ) ;

				var return_value = fake_data.channels_list( start, limit ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			getChannelsWithTemplate: function( list_id, start, limit, template, callback ){
				console.log( "-> Platform.dvb.getChannelsWithTemplate" ) ;
				
				var return_value = fake_data.channels_list( start, limit ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			getChannelsMrl: function( list_id, start, limit, callback ) {
				console.log( "-> Platform.dvb.getChannelsMrl" ) ;
          
				var return_value = fake_data.channels_mrl_list( start, limit ) ;
				this.ChannelsMrl = return_value;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			getChannelInfo: function( id, callback ) {
				console.log( "-> Platform.dvb.getChannelInfo", id ) ;

				var return_value = fake_data.channel_info( id ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			deleteChannels: function( callback ) {
				console.log( "-> Platform.dvb.deleteChannels") ;

				var return_value = 0;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			createList: function( name, callback ) {
				console.log( "-> Platform.dvb.createList name = " + name) ;

				var return_value = 0;
                
				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			setChannels: function( id, channels, callback ) {
				console.log( "-> Platform.dvb.setChannels id = " + id + " channels = " + channels ) ;

				var return_value = 0;
                
				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			destroyList: function( id, callback ) {
				console.log( "-> Platform.dvb.setChannels id = " + id ) ;

				var return_value = 0;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			}, 
			
			getListInfo: function( id, callback ) {
				console.log( "-> Platform.dvb.setChannels id = " + id ) ;

				var return_value = 0;
                
				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			}, 
			
			generateFAVList: function( List_id, callback ) {
				console.log( "-> Platform.dvb.generateFAVList List_id = " + List_id ) ;
				
				var return_value = 0;
                
				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
				
			setCurrentList: function( id, callback ) {
				console.log( "-> Platform.dvb.setCurrentList id = " + id) ;

				var return_value = 0;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			getCurrentList: function( callback ) {
				console.log( "-> Platform.dvb.getCurrentList" ) ;

				var return_value = 0;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			getLists: function( callback ) {
				console.log( "-> Platform.dvb.getCurrentList" ) ;

				var return_value = [{name: "master", id: 0}];

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
		} ;
     
        this.pvr = {
            getUpcommingReminderData: function( callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = fake_data.getUpcommingTimer() ;
				}
				catch( e ) {
					error = { message: "Error calling fake_data.getUpcommingTimer()" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
		
			getSchedules: function( callback ) {
				var error = null ;
				var return_value = null ;

				try {
					return_value = fake_data.timers() ;
				}
				catch( e ) {
					error = { message: "Error calling MediaEnginePVR.getSchedules( )" } ;
				}
				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
			
		};


		this.okList = {
			getCurrentChannel: function( callback ) {
				console.log( "-> Platform.dvb.getCurrentChannel" ) ;

				var return_value = window.Platform.dvb.ChannelCurrent ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			getChannelsWithTemplate: function( list_id, start, limit, template, callback ){
				console.log( "-> Platform.dvb.getChannelsWithTemplate" ) ;
				
				var return_value = fake_data.channels_list( start, limit ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			createList: function( name, callback ){
				console.log( "-> Platform.dvb.createList" ) ;
				
				var return_value = fake_data.create_list( name ) ;
				
				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			getLists: function( channelTemplate, callback ) {
				console.log( "-> Platform.dvb.getLists" ) ;
				
				var return_value = 0 ;
				
				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			getChannelsMrl: function( list_id, start, limit, callback ) {
				console.log( "-> Platform.okList.getChannelsMrl list_id = " + list_id ) ;

				var return_value = 0;

				if( !callback ) { return error || return_value ; }
				_.defer( function( ) { callback( error, return_value ) ; } ) ;
			},
		};


		this.time = {
			timeChecked:           false,
			syncDVBTimer:          null,
    	    
			syncDVBTime: function( ) {
				console.log ("Start sync DVB time!!!");	        
			},

			getTime: function( callback ) {
				console.log( "-> Platform.time.getTime" ) ;

				var return_value = fake_data.time( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			getTimezone: function( callback ) {
				console.log( "-> Platform.time.getTimezone" ) ;

				var return_value = fake_data.timezone( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			setTimezone: function( timezone, callback ) {
				console.log( "-> Platform.time.setTimezone", timezone ) ;

				if( !callback ) { return ; }
				_.delay( function( ) { callback( null ) ; }, Math.random( ) * 1000 ) ;
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
		    wifiKey : "",
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
				wifi_id: null,
				lan_id: null,
			},

			// gets the settings from store (only needet once a boot and shouhld be done before connections get astablished)
			init: function()
			{
				// TODO
				// get settings from store
				// get interface_ids from store
			},
			
			// this does not get the network configuration only the intern settings ( so be careful)
			get_settings: function()
			{
				return ip_settings;
			},


			/*
			*
			* This function set's the connection Details ( ETHERNET and WIRELESS )
			*
			* returns 0 on succses
			*	  1 if paramether istn't valid
			*/
 			setConnectionDetails_type: function( Details ){

				switch( Details )
				{
					case this.ETHERNET:
						ip_settings.type = Details;
						return 0;
					case this.WIRELESS:
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
				ip_settings.wifiKey = wifiKey;
			},
			
			setConnectionDetails_security: function( security )
			{
				ip_settings.security = security;
			},

			//tryes to apply / find an iteface whit the wanted settings
			/*
			*	
			*	if no iterface_Id is given the function will automaticly select one that fits (if one exists)
			*
			*
			*	return 	id on succses
			*			-1 if settings.type is set wrong
			*			-2 if not interface for the configuration is found
			*
			*/
			apply_settings: function( interface_Id )
			{
				// TODO
				// store settings
				
				if( interface_Id == null )
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

							if (interfaces[i].type == this.ETHERNET && interfaces[i].linkState == this.PLUGGED)
									interface_Id = interfaces[i].id;
						}
									
							if(interface_Id == null )
								return -2;

					}
					else if( ip_settings.type == this.WIRELESS )
					{
						interfaces = this.getInterfaces( {type: "", linkState: "", id: ""} );
						for( var i in interfaces)
							if (interfaces[i].type == this.WIRELESS )
									interface_Id = interfaces[i].id;
									
							if(interface_Id == null )
								return -2;
							
					}
					else
						return -1;

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
				console.log( "settings wifiKey:", ip_settings.wifiKey);
				console.log( "settigns security:", ip_settings.security);

				//this.activateInterface( this.getActiveInterface(), false);
				//ip_settings.interface = interface_Id;

				console.log(" return of config interface: ", this.configureInterface( interface_Id, ip_settings ) );
				this.activateInterface( interface_Id, true );
				return interface_Id;

			},

			normalise_ip: function(ip)
			{
				 return ip.split(".").map(Number).join(".");
			},


			/*
			*	
			*	this function set's the network connection
			*	according to the settings
			*	it also tryes to always chose the right interface it will be faster than
			*	apply_settings call whit no paramethers since it saves the interfaces
			*
			*	return 	return paramethers of apply_settings
			*			-3 if settings.type is set wrong
			*			-4 unexpexted error (you found a bug!!)
			*
			*/
			auto_apply_settings: function()
			{
				var interface_id;
				
				if( settings.type == this.ETHERNET )
				{
					if (this.interface_ids.lan_set == false)
					{
						interface_id = this.apply_settings();
						if ( interface_id >= 0 )
						{
							this.interface_ids.lan_set = true;
							this.interface_ids.lan_id = interface_id;
							// TODO
							// store this.interface_ids
						}
						
						return interface_id;
					}
					else
						return this.apply_settings( this.interface_ids.lan_id);
				}
				else if ( settings.type == this.WIRELESS )
				{
					if (this.interface_ids.wifi_set == false)
					{
						interface_id = this.apply_settings();
						if ( interface_id >= 0 )
						{
							this.interface_ids.wifi_set = true;
							this.interface_ids.wifi_id = interface_id;
							// TODO
							// store this.interface_ids
						}
						
						return interface_id;
					}
					else
						return this.apply_settings( this.interface_ids.wifi_id);
				}
				else
					return -3
				
				return -4;
			},
                                    
			//*********************************************************************************
			//*********************************************************************************
			Selected_hotspot: {},

			scanWifiStart: function()
			{
				// TODO
			},
			
			scanWifiStop: function()
			{
				// TODO
			},
         
			//makes callback function to a slot
			set_onScanStateChanged_callback: function( function_callback ) 
			{  
				var return_value = fake_data.getWifiHotspots( ) ;
             
				if( !function_callback ) { return return_value ; }
				_.delay( function( ) { function_callback( 2, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			//*********************************************************************************
			//*********************************************************************************

			activateInterface: function( interfaceId, activate )
			{
				return fake_data.activateInterface(interfaceId, activate );
			},

			configureInterface: function( interfaceId, config )
			{
				return fake_data.configureInterface( interfaceId, config);
			},

			getInterfaceInfo: function( interfaceId )
			{
				return fake_data.getInterfaceInfo( interfaceId );
			},

			getInterfaces: function( interfaceTemplate )
			{
				return fake_data.getInterfaces( intefaceTemplate );
			},

			getActiveInterface: function( )
			{
				return fake_data.getActiveInterface();
			},

			set_onInterfaceChanged_callback: function( callback )
			{
				fake_data.onInterfaceChanged_connect( callback );
			},		

			getConnectionDetails: function( callback ) {
					console.log( "-> Platform.network.getConnectionType" ) ;

					var return_value = this.getActiveInterface(); //fake_data.connection_details( ) ;

					if( !callback ) { return return_value ; }
					_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

            onConnectionChanged: function( callback ) {
				console.log( "-> Platform.network.onConnectionChanged", callback ) ;

				//this.set_onInterfaceChanged_callback( callback );
                                //fake_data.connection_changed_callback = callback ;
			},

			getWirelessSignal: function( callback ) {
					console.log( "-> Platform.network.getWirelessSignal" ) ;

					var return_value = fake_data.wireless_signal( ) ;

					if( !callback ) { return return_value ; }
					_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
		} ;


		this.storage = {
			keys: function( group ) {
				return ;
			},
		    
			load: function( key, defValue, group ) {
				return defValue;
			},
		    
			store: function( key, value, group ) {
				return ;
			},

			getDrives: function( callback ) {
				//console.log( "-> Platform.storage.getDrives" ) ;

				var return_value = fake_data.drives( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},

			setOnDrivesChangedCallback: function( callback ) {
			//	console.log( "-> Platform.storage.onDrivesChanged", callback ) ;

				fake_data.drives_changed_callback = callback ;
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
				console.log( "-> Platform.epg.getChannelEvents channel_mrl = " + channel_mrl ) ;
                
		                var range = { 
		                    "start": start,
		                    "stop": stop,
		                };

				var return_value = fake_data.channel_events( channel_mrl, range ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			loadNowNext: function( channel_id, callback ) {
				console.log( "-> Platform.epg.loadNowNext for channel = " + channel_id ) ;

				var return_value = fake_data.channel_events_current_next( channel_id ) ;

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
			
			getServiceInfo: function(callback){
		//	console.log( "-> Platform.hkc.getServerInfo" ) ;

				var return_value = fake_data.service_info( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			getLastUpdateTime: function(callback){
		//	console.log( "-> Platform.hkc.getLastUpdateTime" ) ;

				var return_value = fake_data.service_info_last_updatetime( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},
			
			getStatement: function(callback){
		//	console.log( "-> Platform.hkc.getStatement" ) ;

				var return_value = fake_data.statement( ) ;

				if( !callback ) { return return_value ; }
				_.delay( function( ) { callback( null, return_value ) ; }, Math.random( ) * 1000 ) ;
			},			
			getSystemInfo: function( callback ) {
		//			console.log( "-> Platform.hkc.getSystemInfo" ) ;
					
					var return_value = fake_data.system_info( );
					
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

			pincode                                       : "000000",
			rating                                        : 13,
			
			worktime : {
				startHour:0,
				startMin:0,
				startSec:0,
				endHour:0,
				endMin:0,
				endSec:0,	    	
		    	},

			InitCA:function(){
				return true;
			},
			SetDRMHost:function(param){
			        return true;
			},
			FormatBuffer:function(){
				return true;
			},
	   		GetCardStatus:function(){
				var cardStatus ={
					retVal:0,
					cardStatus:1,	
				}
				return cardStatus;
	   		},
 		 	GetTerminalTypeID:function(){
				var termialTypeID ={
					retVal:0,
					terminalTypeID: "0x0001",
				}
				return termialTypeID; 
		 	},
		 	GetSTBID:function(){
				var stbid ={
					retVal:0,
					stbID:0x129383ffce,
				}
				return stbid; 
		 	},
		 	GetSTBSecurity:function(){
				var security = {
					retVal:0,
					secureBoot:1,
					jtagProtection:0,
					marketID:0x29793cef,
				}
				return security;
		 	},
		 	GetSTBVer:function(){
				var stbver = {
					retVal:0,
					hwVer:"V1.0.0",
					swVer:"V1.0.0",
					ldrVer:"V1.1.1",
				}
				return stbver;
		 	},
		 	GetVer:function(){
				var caver = {
					retVal:0,
					caVer:"9wjdsocasidhf",	
				}
				return nstv.GetVer();
		 	},
		 	GetOperatorIds:function(){
	 			var operator = {
	 				retVal:0,
	 				operatorList:[3,4,0,0],
	 			}
	 			return operator;
		 	},
		 	GetOperatorInfo:function(opid){
				var opinfo ={
					retVal:0,
					operatorInfo:["a","b","c","d"],
				}
				return opinfo;
		 	},
			GetCardSN: function() {
				var smcSN = {
					retVal:0,
					cardSN:"124563432",	
				}
				return smcSN;
			},
			GetCardUpgradeStatus: function() {
		    		smcST = {
			    		retVal:0,
			    		upgradeStatus:1,
			    		upgradeTime:65539,	
				}
				return smcST;       
				},
			GetRating: function() {
				var rating = {
					retVal:0,
					rating:this.rating,
				}
      				return rating;
			},
			GetWorkTime: function() {
				var wt = {
					retVal:0,
					startHour:this.worktime.startHour,
					startMin:this.worktime.startMin,
					wstartSec:this.worktime.startSec,
					endHour:this.worktime.endHour,
					endMin:this.worktime.endMin,
					endSec:this.worktime.endSec,	
				}
				return wt;
			},		
			IsPaired: function() {
				var paired = {
					retVal:0,
					stbidList:["122122","232123","432443","345435","2132134"],	
				}
				return paired;
			},
     
			ChangePin:function(pin){
				if(pin.OldPinCode == this.pincode){
					this.pincode = pin.NewPinCode	
				}	 				
			},
			SetRating:function(rt){
				if(rt.pinCode == this.pincode){
					this.rating = rt.rating;
				}
			},
			GetEmailSpaceInfo:function(){
				var ret = {
					emptyNum:70,
				}
				return ret;
			},
			email_data:[{
	 				id:0x01,
	 				newEmail:0,
	 				actionID:"111",
	 				importance:"1",
	 				createTime:520,
	 				emailHead:"i love you"
	 				},
	 				{
	 				id:0x02,
	 				newEmail:0,
	 				actionID:"222",
	 				importance:"1",
	 				createTime:520,
	 				emailHead:"i love you"	 					
	 				},
	 				{
	 				id:0x03,
	 				newEmail:0,
	 				actionID:"111",
	 				importance:"1",
	 				createTime:520,
	 				emailHead:"i love you"	 					
	 				},
	 				{
	 				id:0x04,
	 				newEmail:0,
	 				actionID:"111",
	 				importance:"1",
	 				createTime:520,
	 				emailHead:"i love you"	 					
	 				},
	 				{
	 				id:0x05,
	 				newEmail:0,
	 				actionID:"111",
	 				importance:"1",
	 				createTime:520,
	 				emailHead:"i love you"	 					
	 				}
			],  
			GetEmailHeads:function(req){
				var emailHeads = {
					retVal:0,
					emailHeadList:email_data,	
				}
				return 	emailHeads; 	
			},
			GetEmailContent:function(eid){
				var content = [
				{
					id:0x01,
					content:"This is the first email.If you hava any problem,please call us at once.Thank you!",
				},
				{
					id:0x02,
					content:"This is the second email.If you hava any problem,please call us at once.Thank you!",
				},
				{
					id:0x03,
					content:"This is the third email.If you hava any problem,please call us at once.Thank you!",
				},
				{
					id:0x04,
					content:"This is the forth email.If you hava any problem,please call us at once.Thank you!",
				},
				{
					id:0x05,
					content:"This is the fifth email.If you hava any problem,please call us at once.Thank you!",
				},
				{
					id:0x06,
					content:"This is the sixth email.If you hava any problem,please call us at once.Thank you!",
				},
				{
					id:0x07,
					content:"This is the seventh email.If you hava any problem,please call us at once.Thank you!",
				},
				{
					id:0x08,
					content:"This is the eighth email.If you hava any problem,please call us at once.Thank you!",
				},
				{
					id:0x09,
					content:"This is the ninth email.If you hava any problem,please call us at once.Thank you!",
				},
				{
					id:0xa0,
					content:"This is the tenth email.If you hava any problem,please call us at once.Thank you!",
				},
				]
				for(var i = 0; i < content.length ; i++){
					if(eid == content[i].id){
						var index = i;
					}
				}
				return 	content[index].content;		
			},
			DelEmail:function(eid){
				var emailid = eid.emailID;
				for(var i = 0; i < email_data.length; i++){
					if(emailid == email_data[i].id){
						email_data.pop();
					}
				}
			},
			SetWorkTime:function(wt){
				if(wt.pinCode == this.pincode){
					this.worktime.startHour = wt.startHour;
					this.worktime.startMin  = wt.startMin;
					this.worktime.startSec  = wt.startSec;
					this.worktime.endHour   = wt.endHour;
					this.worktime.endMin    = wt.endMin;
					this.worktime.endSec    = wt.endSec;
				} 			 			
			},
			GetServiceEntitles: function(opid){
			 	console.log( "-> Platform.ca.getEntitlement" ) ;
				var entitles = {
					retVal:0,
					entitleList:[
						{
							productID:1,
							canTape:0,
							expiredDate:520
						},
						{
							productID:2,
							canTape:0,
							expiredDate:520
						},
						{
							productID:3,
							canTape:0,
							expiredDate:520
						},				
					],
				}
				return 	entitles;		
			},
			GetSlotIDs: function(opid){
				var slotIDs = {
					retVal:0,
					slotList: [0, 1, 2],	
				}
				return 	slotIDs; 
			},
	 	
			GetSlotInfo: function(slot){
				var slotInfo = {
					retVal:0,
					creditLimit: 0,	
					balance: 0,
				}
				return 	slotInfo; 
			},		 			 	
			GetACList: function(opid){
				var ACList = {
					retVal:0,
					acList: [0, 1, 2],
				}
				return 	ACList; 
			},
	 			 		
			GetIPPVProgram: function(opid){
				var IPPVProgram = {
					retVal:0,
					ippvList: [{productID: 0, bookedFlag: 0, canTape: 0, price: 0, expiredDate: 0, slotID: 0},
					          ],
				}
				return 	IPPVProgram; 
			},
	 			 		
			GetOperatorChildStatus: function(opid){
				var operatorChildStatus = {
					retVal:0,
					isChild: 0,	
					delayTime: 0,
					lastFeedTime: 0,
					isCanFeed: 0,
					parentCardSN: 0,
				}
				return 	operatorChildStatus; 
			},
		 		
			GetDetitleChkNums: function(opid){
				var detitleChkNums = {
					retVal:0,
					readFlag: 0,	
					detitleList: [0, 1, 2],
				}
				return 	detitleChkNums; 
			},
			ReadParent:function(opid){
				var status = {
					retVal:0,
				}
				return 	status; 	
			},
			WriteChild:function(opid){
				var status = {
					retVal:0,
				}
				return 	status; 
			},	 		 				 					    
			
			setActionRequestExtCallback: function(function_callback) {
				return true; 
			},
			
			setLockServiceCallback: function(function_callback) {
				return true;             
			},
			
			setUNLockServiceCallback: function(function_callback) {
		                return true;       			    
			},
			setShowFingerMessageExtCallback:function( callback ) {
			 	this.show_finger_message_ext_callback = callback;
			 	/*
			 	setInterval(function() {
					var param = {
			 	 	fingerMessage:1923920,
			 	 	};
			 		this.show_finger_message_ext_callback(param);
			 	},3000);
			 	*/
			},			
			 setNotifySmcStatusCallback:function( callback ) {
				this.notify_smc_status_callback = callback;
				/*
				setInterval(function() {
					var param = {
					smcStatus:1,
					};
					this.notify_smc_status_callback(param);
				},5000);
				*/
			 },			
			setEmailNotifyIconCallback:function( callback ) {
				this.email_notify_icon_callback = callback;
				/*
				setInterval(function() {
					var param = {
					showFlag:2,
					};
					this.email_notify_icon_callback(param);
				},10000);
			    */
			 },			
			setShowOSDMessageCallback:function( callback ) {
			 	this.show_OSD_message_callback = callback;
			 	/*
			 	setInterval(function() {
					var param = {
			 	 	style:2,
			 	 	message:"always work and not play make jack a duck boy",
			 	 	};
			 		this.show_OSD_message_callback(param);
			 	},5000);
			 	*/
			},
			setHideOSDMessageCallback:function( callback ) {
			 	this.hide_OSD_message_callback = callback;
			 	/*
			 	setInterval(function() {
					var param = {
			 	 	style:4,
			 	 	};
			 		this.hide_OSD_message_callback(param);
			 	},5000);
			 	*/
			},
			setShowCurtainNotifyCallback:function( callback ) {
			 	this.show_curtain_notify_callback = callback;
			 	/*
			 	setInterval(function() {
					var param = {
			 	 	curtainCode:1,
			 	 	};
			 		this.show_curtain_notify_callback(param);
			 	},10000);
			 	*/
			},			
			setShowProgressStripCallback:function( callback ) {
			 	this.show_progress_strip_callback = callback;
			 	var i = 0;
			 	/*
			 	setInterval(function() {
					var param = {
						mark:2,
						progress:i++,
			 	 	};
			 		this.show_progress_strip_callback(param);
			 	},2000);
			 	*/
			},		
			setRequestFeedingCallback:function( callback ) {
			 	this.request_feeding_callback = callback;
			 	/*
			 	setInterval(function() {
					var param = {
			 	 	readStatus:4,
			 	 	};
			 		this.request_feeding_callback(param);
			 	},5000);
			 	*/
			},
			setOTADetectedCallback:function(callback){
				this.ota_detected_callback = callback;
				/*
				setInterval(function() {
					var param = {
					upgradeType:1,	
					imageType:1,
					dsmccPID:23456,
					frequency:360,
					symbolRate:115200,
					modulation:"64qam",
					downloadType:2,
					swVer:"V1.03",
			 	 	};
			 		this.ota_detected_callback(param);
			 	},5000);	
			 	*/
			},
			SetLoaderParam:function(ldrParam){
				var status = {
					retVal:0,
				}
				return 	status; 
			},			
			StartOTA:function(otaParam){
				var ldrParam = {};
				ldrParam.upgradeType = 1;
				ldrParam.imageType = otaParam.imageType;
				ldrParam.dsmccPID = otaParam.dsmccPID;
				ldrParam.frequency = otaParam.frequency;
				ldrParam.symbolRate = otaParam.symbolRate;
				ldrParam.modulation = otaParam.modulation;
								
			},
			HideOTA:function(otaParam){
				if (otaParam.downloadType == 2) {
					this.StartOTA(otaParam);
				}
			},
			setStartIppvBuyDlgCallback:function(callback){
				this.start_ippv_buydlg_callback = callback;
				/*
				setInterval(function() {
					var param = {
							priceList:[{price:100},{price:200}],
							messageType:2,
							ippvTime:500,
							productID:111,
							operatorID:222,
							slotID:333,
			 	 	};
			 		this.start_ippv_buydlg_callback(param);
			 	},5000);
			 	*/
			},
			setHideIppvDlgCallback:function(function_callback){
				return true;					
				},		 	
		 	StopIPPVBuyDlg:function(buyIppv){
				var status = {
					retVal:0,
				}
				return status;
			},
			setShowBuyMessageCallback:function(callback){
				this.show_buy_message_callback = callback;
				/*
				setTimeout(function() {
					var param = {
					messageType:0x05,
				};
				this.show_buy_message_callback(param);
				},5000);	
				*/	 		
			},
			
			
			PlayLockService: function( ) {
				var status = {
					retVal:0,
				}
				return status;
			},
			
			StopLockService: function( ) {
				if (this.lockServiceEvent == true) {
					//nstv.StopLockService();
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
	    		    
			startDvbcScan: function(from_freq, to_freq, freq_step, bandwidth, symbolrate) {
				return true;
			},
		    
			setType: function(type) {
				return true;
			},
		    
			setDvbcScannerStatusCallback: function(function_callback) {
				return true;
			}
		 };
		 
		 
		 
		this.system = { 
			MediaEngineStandbyMode: {
        			OFF: 0,
        			ACTIVE: 1,
        			PASSIVE: 2
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
		 		//MediaEngineSystem.setStandbyMode( mode ) ;
		 	},
		 	
			setUIFreezed: function( ) {
				this.UIFreezed = true;
			},
            
			setUIUnFreezed: function( ) {
				this.UIFreezed = false;
			},
            
			rebootSTB: function( ) {
				return true;
			},		
			
			factoryResetStart: function( type ) {
				return true;
			},    
			
			getFactoryResetStatus: function( ) {
				return true;
			},
		 };

		 this.browser = {
		 	onDemandId: -1,
			appId: -1,
		 };

		 this.settings = {

				//TODO: store the Data on the box		 	
				VideoModeStatus:{
					"VIDEO_MODE_1080I50":6,
					"VIDEO_MODE_720P50":4,
					"VIDEO_MODE_576P":3,	 			
				},	 		
				RatioStatus:{
					"ASPECT_RATIO_16_9":1,
					"ASPECT_RATIO_4_3":2,
				},
				VideoFormatStatus:{
					"VIDEO_ZOOM_NORMAL":0,
					"VIDEO_ZOOM_ENLARGE": 1,
					"VIDEO_ZOOM_MAKE_WIDER":   2,
					"VIDEO_ZOOM_NON_LINEAR_STRETCH_TO_FULL_SCREEN":4,   
				},
				AudioOutputStatus:{
					"AUDIO_STEREO":0,
					"AUDIO_HDMI_SPDIF":1,
					"AUDIO_SPDIF":2,
				},
				
				setLanguage: function( lang ) {
					window.settings.language = lang;      
				},
				
				getLanguage: function( ) {
					return window.settings.language;      
				},				
                
				setConversionMode:function(mode){
					return true;    		
				},
				
				getConversionMode:function(){
					return 0;    			
				},
				
				setAspectRatio:function(ratio){
					return true;     
				},
				
				getAspectRatio:function(){
					return 0;    
				},	       
				
				setResolution: function( mode ) {
					return true;
				},
				
				getResolution: function( ) {
					return 0;
				},
				
				setAudioOutput:function(mode){
					return true;	        
				},
				
				getAudioOutput:function(){
					return 0;	        
				},

				setAudioTrack:function(track){
					return true;
				},
				
				getAudioTrack:function(){
					return 0;
				},
				
				getAudioTracksInfo:function(){
					return [{
						"index": 0,
						"lang": "eng",
					}, ];
				},
				
				setIsFirstTimeRun: function( value ) {
					window.Platform.storage.store("IsFirstTimeRun", value, "");	
				},			
				
				getIsFirstTimeRun: function() {
					return window.Platform.storage.load("IsFirstTimeRun", true, "");
				},				
				
				firstTimeRun: function ( callback ) {
					var leaveCallback = function( ) {
						window.Platform.settings.setIsFirstTimeRun( false ) ;							
						callback && callback( ) ;
					} ;
					
					if ( this.getIsFirstTimeRun() == true ) {
						window.Platform.settings.setAspectRatio("ASPECT_RATIO_16_9");
						window.Platform.settings.setConversionMode("VIDEO_ZOOM_NORMAL");
						window.Platform.settings.setResolution("VIDEO_MODE_1080I50");
						window.Platform.settings.setAudioOutput("AUDIO_STEREO");

						if ( window.Template.EnableFrqHunting == true ) {
							var frqHunting = window.AppManager.getWidget( 'Auto Scan C') ;				    
							window.AppManager.enter( frqHunting.name ) ;
							frqHunting.setLeaveCallback( leaveCallback ) ;
						} else {
							leaveCallback();
						}						      			
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
