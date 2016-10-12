"use strict";
(function( ) {

	var name = "Channel Number" ;


	var ChannelNumber = function( ) {

		var parent = new window.Widget( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'channel_number' ).appendTo( '#interface' ) ;


		var setText = function( text ) {
			self.$root.text( text ) ;
			self.show( ) ;
		} ;

		var digits = [] ;
		var max_digits = 3 ;
		var placeholder = "-" ;

		var callback = null ;

		var autohide = _.debounce( function( ) {
			self.hide( ) ;
			self.flush( ) ;
		}, window.settings.channel_number_autohide_delay ) ;


		self.onShow = function( ) {
			autohide( ) ;
		} ;

		self.pushDigit = function( digit ) {
			digits.push( digit ) ;

			var string = digits.join( '' ) ;
			while( string.length < max_digits ) {
				string = placeholder + string ;
			}
			setText( string ) ;

			if( digits.length == max_digits ) {
				self.flush( ) ;
			}
		} ;

		self.isWriting = function( ) {
			return digits.length > 0 ;
		}

		self.showNumber = function( ) {
			var liveTV = window.AppManager.getWidget("Live TV");
			var $currentchannelinfo = liveTV.judgeIfTunedChannel( );
			
			if (!$.isEmptyObject($currentchannelinfo)) {
				digits = [] ;
				setText( window.Format.padWithZeroes($currentchannelinfo.lcn, 3) ) ;
			}			
		} ;
		self.flush = function( ) {
			if( this.isWriting( ) ) {
				var number = 0;
				if (Platform.settings.getChannelNumberType() == "lcn") {
					number = parseInt( digits.join( '' ) ) ;
				} else {
					number = parseInt( digits.join( '' ) ) - 1 ;
				};
				digits = [] ;
				callback && callback( number ) ;
			}
		} ;

		self.onChange = function( _callback ) {
			callback = _callback ;
		} ;

		return self ;
	} ;

	window.AppManager.addWidget( name, ChannelNumber ) ;

})( ) ;
