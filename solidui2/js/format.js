"use strict";
(function( ) {

	var padWithZeroes = function( num, len ) {
		var num = num.toString( ) ;
		while( num.length < len ) {
			num = '0' + num ;
		}
		return num ;
	} ;


	var months = [
		"month :: jan",
		"month :: feb",
		"month :: mar",
		"month :: apr",
		"month :: may",
		"month :: jun",
		"month :: jul",
		"month :: aug",
		"month :: sep",
		"month :: oct",
		"month :: nov",
		"month :: dec",
	] ;

	var longmonths = [
		"month :: january",
		"month :: february",
		"month :: march",
		"month :: april",
		"month :: may",
		"month :: june",
		"month :: july",
		"month :: august",
		"month :: september",
		"month :: october",
		"month :: november",
		"month :: december",
	] ;

	var weekdays = [
		"weekday :: sun",
		"weekday :: mon",
		"weekday :: tue",
		"weekday :: wed",
		"weekday :: thu",
		"weekday :: fri",
		"weekday :: sat",
	] ;

	var longweekdays = [
		"weekday :: sunday",
		"weekday :: monday",
		"weekday :: tuesday",
		"weekday :: wednesday",
		"weekday :: thursday",
		"weekday :: friday",
		"weekday :: saturday",
	] ;

	var formatValue = function (value, format) {
		switch (format) {
			case "d":
				return "" + value;		
				break;

			case "dd":
				if (value < 10) {
					return "0" + value;				
				} else {
					return "" + value;		
				}
				break;

			case "EEEE":
				return TR( longweekdays[ value ] );
				break;
				
			case "EEE":
				return TR( weekdays[ value ] );
				break;

			case "h": 												//hour (0-12)	e.g. 3 
				if (value > 12) {				
					value-=12
				}
				return "" + value;
				break;
			case "hh": 												//hour (0-12)	e.g. 03 
				if (value >= 12) {				
					value-=12
				}
				if (value < 10) {
					return "0" + value;				
				} else {
					return "" + value;		
				}
				break;
			case "H": 												//hour (0-23)	e.g. 3 
				return "" + value;		
				break;

			case "HH": 												//hour (00-23)	e.g. 03 
				if (value < 10) {
					return "0" + value;				
				} else {
					return "" + value;		
				}
				break;
			case "a": 												//am- and pm- text  e.g. AM
				if (value >= 12) {				
					return "" + TR( "format :: pm" )
				} else {
					return "" + TR( "format :: am" )	
				}
				break;

			case "mm":
				if (value < 10) {
					return "0" + value;				
				} else {
					return "" + value;		
				}
				break;

			case "M":
				value = value + 1; 
				return "" + value;		
				break;
			case "MM":
				value = value + 1; 
				if (value < 10) {
					return "0" + value;				
				} else {
					return "" + value;		
				}
				break;
			case "MMM":
				return TR( months[ value ] );
				break;

			case "MMMM":
				return TR( longmonths[ value ] );
				break;

			case "ss":
				if (value < 10) {
					return "0" + value;				
				} else {
					return "" + value;		
				}
				break;

			case "yyyy":
 				if(value<1000) {
   				value+=1900
   			};
 				return "" + value;

			default:
				return "";
				break;
		}
	}; // end formatValue

	var formatLayout = function( date, formatsetting ) {
		
		var date_string = "";
		
		switch (formatsetting) {
			// TIme			
			case "HH:mm":
				date_string += formatValue(date.getHours(),"HH");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				break;
			case "HH:mm:ss":
				date_string += formatValue(date.getHours(),"HH");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += ":";
				date_string += formatValue(date.getSeconds(),"ss");
				break;
			case "h:mm a":
				date_string += formatValue(date.getHours(),"h");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += " ";
				date_string += formatValue(date.getHours(),"a");
				break;
			case "ah:mm":
				date_string += formatValue(date.getHours(),"a");
				date_string += formatValue(date.getHours(),"h");
				date_string += TR("format :: hour");
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += TR("format :: min");
				break;
			case "ah:mm:ss":
				date_string += formatValue(date.getHours(),"a");
				date_string += formatValue(date.getHours(),"h");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += ":";
				date_string += formatValue(date.getSeconds(),"ss");
				//date_string += TR("format :: sec");
				break;
			case "h:mma":
				date_string += formatValue(date.getHours(),"h");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += formatValue(date.getHours(),"a");
				break;
			case "hh:mm a":
				date_string += formatValue(date.getHours(),"hh");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += " ";
				date_string += formatValue(date.getHours(),"a");
				break;
			case "hh:mma":
				date_string += formatValue(date.getHours(),"hh");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += formatValue(date.getHours(),"a");
				break;
			case "h:mm:ssa":
				date_string += formatValue(date.getHours(),"h");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += ":";
				date_string += formatValue(date.getSeconds(),"ss");
				date_string += formatValue(date.getHours(),"a");
				break;

			// Date
			case "dd.MMMM yyyy":
				date_string += formatValue(date.getDate(),"dd");
				date_string += ". ";
				date_string += formatValue(date.getMonth(),"MMMM");
				date_string += " ";
				date_string += formatValue(date.getYear(),"yyyy");
				break;
			case "dd-MMM-yyyy":
				date_string += formatValue(date.getDate(),"dd");
				date_string += "-";
				date_string += formatValue(date.getMonth(),"MMM");
				date_string += "-";
				date_string += formatValue(date.getYear(),"yyyy");
				break;
			case "dd-MMMM-yyyy":
				date_string += formatValue(date.getDate(),"dd");
				date_string += "-";
				date_string += formatValue(date.getMonth(),"MMMM");
				date_string += "-";
				date_string += formatValue(date.getYear(),"yyyy");
				break;
			case "yyyy年M月dd日":
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += TR("format :: year");
				date_string += formatValue(date.getMonth(),"M");
				date_string += TR("format :: month");
				date_string += formatValue(date.getDate(),"dd");
				date_string += TR("format :: day");
				break;

			// DateTime
			case "EEEE - dd.MMMM yyyy  HH:mm:ss":
				date_string += formatValue(date.getDay(),"EEEE");
				date_string += " - ";
				date_string += formatValue(date.getDate(),"dd");
				date_string += ". ";
				date_string += formatValue(date.getMonth(),"MMMM");
				date_string += " ";
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += " ";
				date_string += formatValue(date.getHours(),"HH");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += ":";
				date_string += formatValue(date.getSeconds(),"ss");
				break;
			case "EEEE - dd.MMMM yyyy  h:mm:ssa":
				date_string += formatValue(date.getDay(),"EEEE");
				date_string += " - ";
				date_string += formatValue(date.getDate(),"dd");
				date_string += ". ";
				date_string += formatValue(date.getMonth(),"MMMM");
				date_string += " ";
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += " ";
				date_string += formatValue(date.getHours(),"h");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += ":";
				date_string += formatValue(date.getSeconds(),"ss");
				date_string += formatValue(date.getHours(),"a");
				break;
			case "dd-MMM-yyyy,EEE | HH:mm":
				date_string += formatValue(date.getDate(),"dd");
				date_string += "-";
				date_string += formatValue(date.getMonth(),"MMM");
				date_string += "-";
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += ", ";
				date_string += formatValue(date.getDay(),"EEE");
				date_string += " | ";
				date_string += formatValue(date.getHours(),"HH");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				break;
			case "dd-MMM-yyyy, EEE | h:mma":
				date_string += formatValue(date.getDate(),"dd");
				date_string += "-";
				date_string += formatValue(date.getMonth(),"MMM");
				date_string += "-";
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += ", ";
				date_string += formatValue(date.getDay(),"EEE");
				date_string += " | ";
				date_string += formatValue(date.getHours(),"h");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += formatValue(date.getHours(),"a");
				break;
			case "yyyy年M月dd日 (EEEE)  HH:mm:ss":
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += TR("format :: year");
				date_string += formatValue(date.getMonth(),"M");
				date_string += TR("format :: month");
				date_string += formatValue(date.getDate(),"dd");
				date_string += TR("format :: day") + " (";
				date_string += formatValue(date.getDay(),"EEEE");
				date_string += ") ";
				date_string += formatValue(date.getHours(),"HH");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += ":";
				date_string += formatValue(date.getSeconds(),"ss");
				break;
			case "yyyy年M月dd日 (EEEE)  h:mm:ssa":
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += TR("format :: year");
				date_string += formatValue(date.getMonth(),"M");
				date_string += TR("format :: month");
				date_string += formatValue(date.getDate(),"dd");
				date_string += TR("format :: day") + " (";
				date_string += formatValue(date.getDay(),"EEEE");
				date_string += ") ";
				date_string += formatValue(date.getHours(),"h");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += ":";
				date_string += formatValue(date.getSeconds(),"ss");
				date_string += formatValue(date.getHours(),"a");
				break;
			case "yyyy年M月dd日 (EEEE) | H.mm":
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += TR("format :: year");
				date_string += formatValue(date.getMonth(),"M");
				date_string += TR("format :: month");
				date_string += formatValue(date.getDate(),"dd");
				date_string += TR("format :: day") + " (";
				date_string += formatValue(date.getDay(),"EEEE");
				date_string += ") | ";
				date_string += formatValue(date.getHours(),"H");
				date_string += ".";
				date_string += formatValue(date.getMinutes(),"mm");
				break;
			case "yyyy年M月dd日 (EEEE) | h.mma":
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += TR("format :: year");
				date_string += formatValue(date.getMonth(),"M");
				date_string += TR("format :: month");
				date_string += formatValue(date.getDate(),"dd");
				date_string += TR("format :: day") + " (";
				date_string += formatValue(date.getDay(),"EEEE");
				date_string += ") | ";
				date_string += formatValue(date.getHours(),"h");
				date_string += ".";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += formatValue(date.getHours(),"a");
				break;
			case "yyyy年M月dd日 (EEEE)  ah.mm":
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += TR("format :: year");
				date_string += formatValue(date.getMonth(),"M");
				date_string += TR("format :: month");
				date_string += formatValue(date.getDate(),"dd");
				date_string += TR("format :: day") + " (";
				date_string += formatValue(date.getDay(),"EEEE");
				date_string += ") | ";
				date_string += formatValue(date.getHours(),"a");
				date_string += formatValue(date.getHours(),"h");
				date_string += TR("format :: hour");
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += TR("format :: min");
				break;

			default:
				date_string += formatValue(date.getDate(),"dd");
				date_string += ". ";
				date_string += formatValue(date.getMonth(),"MMMM");
				date_string += " ";
				date_string += formatValue(date.getYear(),"yyyy");
				date_string += " ";
				date_string += formatValue(date.getHours(),"HH");
				date_string += ":";
				date_string += formatValue(date.getMinutes(),"mm");
				date_string += ":";
				date_string += formatValue(date.getSeconds(),"ss");
				break;
		};

		return date_string ;
	} ; // end formatLayout


	var formatDateTimeLayout = function( date, widgetname ) {
		var language = window.Platform.settings.getLanguage();
		if (window.Template.formatsetting[widgetname].DateTime[language])	{
			var formatsetting = window.Template.formatsetting[widgetname].DateTime[language];
	//		console.log( "-> format.js - formatDateTimeLayout - formatsetting = " + formatsetting );
		} else if (window.Template.formatsetting[widgetname].DateTime.default) {	
			var formatsetting = window.Template.formatsetting[widgetname].DateTime.default;
	//		console.log( "-> format.js - formatDateTimeLayout - formatsetting default = " + formatsetting );
		} else {
			var formatsetting = "";
	//		console.log( "-> format.js - formatDateTimeLayout - formatsetting = nicht vorhanden" );
		};		
		
		return formatLayout(date, formatsetting);
	} ;

	var formatDateTimeLayoutTS = function( timestamp, widgetname ) {
	//	console.log( "-> format.js - formatDateTimeLayoutTS" );
		var date = new Date(timestamp);
		return formatDateTimeLayout(date, widgetname);
	};

	var formatDateLayout = function( date, widgetname ) {
		var language = window.Platform.settings.getLanguage();
		if (window.Template.formatsetting[widgetname].Date[language])	{
			var formatsetting = window.Template.formatsetting[widgetname].Date[language];
		} else if (window.Template.formatsetting[widgetname].Date.default) {	
			var formatsetting = window.Template.formatsetting[widgetname].Date.default;
		} else {
			var formatsetting = "";
		};		
		
		return formatLayout(date, formatsetting);
	} ;

	var formatDateLayoutTS = function( timestamp, widgetname ) {
		var date = new Date(timestamp);
		return formatDateLayout(date, widgetname);
	};

	var formatTimeLayout = function( date, widgetname ) {
		var language = window.Platform.settings.getLanguage();
		if (window.Template.formatsetting[widgetname].Time[language])	{
			var formatsetting = window.Template.formatsetting[widgetname].Time[language];
		} else if (window.Template.formatsetting[widgetname].Time.default) {	
			var formatsetting = window.Template.formatsetting[widgetname].Time.default;
		} else {
			var formatsetting = "";
		};		
		
		return formatLayout(date, formatsetting);
	} ;

	var formatTimeLayoutTS = function( timestamp, widgetname ) {
		var date = new Date(timestamp);
		return formatTimeLayout(date, widgetname);
	};

	var formatDateWeekday = function( date ) {
		var day = padWithZeroes( date.getDate( ), 2 ) ;
		var month = date.getMonth( )+1 ;
		var year = date.getFullYear( ) ;
		var weekday = TR( longweekdays[ date.getDay( ) ] ) ;
		var date_string;

		if ( window.settings.language == "ch" ) {
			date_string = year + TR("format :: year") + month + TR("format :: month") + day + TR("format :: day") + "(" + weekday + ")" ;
		} else {
			date_string = day + "/" + month + "/" + year + " " + weekday ;
		}

		return date_string ;
	} ;

	var formatDate = function( date ) {
		var day = padWithZeroes( date.getDate( ), 2 ) ;
		//var month = TR( months[ date.getMonth( ) ] ) ;
		var month = date.getMonth( )+1 ;
		var year = date.getFullYear( ) ;

		var date_string = day + "/" + month + "/" + year ;

		return date_string ;
	} ;

	var formatTime = function( date ,flag) {
		var hours = padWithZeroes( date.getHours( ), 2 ) ;
		var minutes = padWithZeroes( date.getMinutes( ), 2 ) ;
		var seconds = padWithZeroes( date.getSeconds( ), 2 ) ;
		if(flag == 'addsecond'){
				var date_string = hours + ":" + minutes + ":" + seconds ;
		}else{
				var date_string = hours + ":" + minutes
			}
		return date_string ;
	} ;
	var formatWorkTime = function(date) {
		var startHours = padWithZeroes( date.startHour, 2 ) ;
		var startMinutes = padWithZeroes( date.startMin, 2 ) ;
		var startSeconds = padWithZeroes( date.startSec, 2 ) ;
		var endHours = padWithZeroes( date.endHour, 2 ) ;
		var endMinutes = padWithZeroes( date.endMin, 2 ) ;
		var endSeconds = padWithZeroes( date.endSec, 2 ) ;

		var date_string = startHours + ":" + startMinutes + ":" + startSeconds + " - " + endHours + ":" + endMinutes + ":" + endSeconds;

		return date_string ;
	} ;
	
	var formatDateString = function(str){
		var year = str.substr(0,4);
		var month = str.substr(4,2);
		var day = str.substr(6,2);
		if(window.Platform.settings.getLanguage() == "ch"){
			var date_string = year+TR("format :: year")+month+ TR("format :: month")+day+TR("format :: day");
		} else {
			var date_string = day + "/" + month + "/" + year ;
		}
		return date_string;
	};
	var formatDueDateString = function(str){
		if(str.length == 10 && str.substr(5,1) == "-"){
			var year = str.substr(6,4);
			var month = str.substr(3,2);
			var day = str.substr(0,2);
			if(window.Platform.settings.getLanguage() == "ch"){
				var date_string = year+TR("format :: year")+month+ TR("format :: month")+day+TR("format :: day");
			} else {
				var date_string = day + "/" + month + "/" + year ;
			}			
		}else{			
			var date_string = str;	
		}
		return date_string;
	};	

	var durations = [
		{ name: 'duration :: msec', multiplier:    1, },
		{ name: 'duration :: sec',  multiplier: 1000, },
		{ name: 'duration :: min',  multiplier:   60, },
		{ name: 'duration :: hour', multiplier:   60, },
		{ name: 'duration :: day',  multiplier:   24, },
		{ name: 'duration :: mon',  multiplier:   30, },
		{ name: 'duration :: year', multiplier:   12, },
	] ;

	var formatDuration = function( duration, num_buckets ) {
		//console.log( "-> format.js - formatDuration - duration = " + duration );		
		
		var decomposition = [] ;
		var unit_index = 0 ;
		while( duration > 0 && unit_index < durations.length - 1 ) {
			var unit = durations[unit_index+1].multiplier ;
			var mod = duration % unit
			if( mod ) {
				var entry = { unit: durations[unit_index].name , magnitude: mod } ;
				decomposition.unshift( entry ) ;
			}
			var duration = Math.floor( duration / unit ) ;
			unit_index++ ;
		}
		if( duration > 0 ) {
			var entry = { unit: durations[unit_index].name, magnitude: duration } ;
			decomposition.unshift( entry ) ;
		}

		var buckets = [] ;
		var limit = Math.min( decomposition.length, num_buckets ) ;
		for( var i = 0 ; i < limit ; i++ ) {
			var entry = decomposition[i] ;
			buckets.push( entry.magnitude + ' ' + TR( entry.unit ) ) ;
			//console.log( "-> format.js - formatDuration - entry.magnitude = " + entry.magnitude + ' ' + TR( entry.unit ) );	
		}

		return buckets ;
	} ;


	var metric_prefixes = [ "B", "KB", "MB", "GB", "TB", "PB", "EB" ] ;

	var formatByteSize = function( bytes, digits ) {
		digits += 3 ;
		bytes /= 1024 * 1024 *1024;
		var string = bytes.toFixed( 2 ) ;
		var metric_order = 0 ;
		while( string.length > digits && metric_order < metric_prefixes.length ) {
			bytes /= 1024 ;
			string = bytes.toFixed( 2 ) ;
			metric_order++ ;
		}
		if( string.indexOf( '.' ) != -1 ) {
			while( string[ string.length - 1 ] == '0' ) {
				string = string.slice( 0, -1 ) ;
			}
			if( string[ string.length - 1 ] == '.' ) {
				string = string.slice( 0, -1 ) ;
			}
		}
		return string + " " + metric_prefixes[metric_order] ;
	}


	var Format = new function( ) {
		this.padWithZeroes  			= padWithZeroes ;
		this.formatDateTimeLayout 		= formatDateTimeLayout;
		this.formatDateTimeLayoutTS		= formatDateTimeLayoutTS;
		this.formatDateLayout 			= formatDateLayout;
		this.formatDateLayoutTS 		= formatDateLayoutTS;
		this.formatTimeLayout 			= formatTimeLayout;
		this.formatTimeLayoutTS 		= formatTimeLayoutTS;
		this.formatTime     			= formatTime ;
		this.formatDate     			= formatDate ;
		this.formatWorkTime             = formatWorkTime;
		this.formatDuration 			= formatDuration ;
		this.formatByteSize 			= formatByteSize ;
		this.formatDateWeekday 			= formatDateWeekday ;
		this.formatDateString           = formatDateString;
		this.formatDueDateString        = formatDueDateString;
	} ;

	window.Format = Format ;

})( ) ;
