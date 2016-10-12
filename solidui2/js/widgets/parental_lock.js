"use strict";
(function( ) {
	
	var name = "Parental Lock" ;
	
	var Channel = new window.Class(
	
	//parent
	window.ListViewDelegate,
	
	//constructor
	function( channel ){
			ListViewDelegate.call( this, name ) ;
			
			this.$root.addClass( 'channels_listlable' ) ;
			
			var $channel_name = $( '<div>' ).addClass( 'column_three' ).text( window.Format.padWithZeroes(channel.lcn, 4) ).appendTo( this.$root ) ;
			var $channel_collect = $( '<div>' ).addClass( 'column_three' ).text( channel.name ).appendTo( this.$root ) ;
			var $channel_lockflag = $('<img src = ./templates/hkc/images/lock.png height=10px width=8px>').addClass('lockflag').appendTo(this.$root);
			this.mrl = channel.mrl;
			this.lockStatus = channel.locked;
			if(this.lockStatus){
				$channel_lockflag.show();
			}
			else{
				$channel_lockflag.hide();
			}
			this.$channel_lockflag = $channel_lockflag;
	},
	//prototype
	{
		getMrl: function(){
			return this.mrl;
		},
		getLockStatus:function(){
			return this.lockStatus;
		},
		changeLockStatus:function(status){
			this.lockStatus = status;
			if(status){
				this.$channel_lockflag.show();
			}
			else{
				this.$channel_lockflag.hide();
			}
		},
	}
	)
		
	var ParentalLock = function( ) {
		
		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;
		self.$root.attr( 'id', 'parentlock' );
		
		var $parentlock_screen = $( '<div>' ).appendTo(self.$root);				
		var $parentlock_caption = $( '<div>' ).addClass( 'caption' ).append($TR('parentallock :: caption')).appendTo($parentlock_screen);
		
		var $channels_list = $( '<div>' ).addClass( 'channelslist' ).appendTo($parentlock_screen);	
		var list = new window.scrollListView( Channel ) ;
		list.$root.appendTo( $channels_list ) ;

		var $lock_area = $( '<div>' ).addClass( 'lockarea' ).appendTo($parentlock_screen);
		var $lock_reminder = $('<div>').addClass('lock_reminder').append('OK').appendTo($lock_area);		
		var $lock_name = $('<div>').addClass('lock_name').append($TR('parentallock :: lock/unlock')).appendTo($lock_area);		
		
		var showKeyInputDlg = function(){
			$parentlock_screen.hide();
			var callback = function(){
				updateScreen();	
				$parentlock_screen.show();
			}	
			
			var flag = 2;
			
			var callback = {
				callbackOK: callback,
				callbackCancel: null,
				saveCurrentChannel: null,
				flag: 2,
				keyinputLcn: "",
			};
			
			var keyinputDlg = window.AppManager.getWidget("KeyInput PopDlg");
			keyinputDlg.registerCallback(callback);
			window.AppManager.enter( "KeyInput PopDlg" ) ;
		}
									
		var masterChannels = [];
		var locklist = [];		
										
		var getChannelsData = function( list_id ){
			var start = 0;
			var limit = window.Template.ChannelMaxLimit;
			var template =  { "mrl":"","name":"","lcn":"","locked":"" } ;
			
			var updateChannels = function( error, masterChannels ){
				if( error ) { throw new Error( error.masterChannels ) ; }
				var index = 0;
				list.updateItems(masterChannels,index );
				_.defer( function( ) { list.pageUpAndDown( 0 ) ; } ) ;
				list.show();
			};
			Platform.okList.getChannelsWithTemplate( list_id, start, limit, template,updateChannels );									
		};
									
		var updateScreen = function( ) {			
			getChannelsData( 0 );					
		};
					
		self.onKeyUP = function( ) {
			list.selectPrev();
		};
		
		self.onKeyDOWN = function( ) {
			list.selectNext();
		};
		
		self.onKeyCHANUP = function(){
			list.getPagePrev();
		};
		
		self.onKeyCHANDOWN = function(){
			list.getPageNext();
		};
		
		self.onKey = function( ) {
			return true ;
		} ;
		
		self.onShow = function( ) {
			//Platform.media.stop( function( error ) {
			//	if( error ) { throw new Error( error.message ) ; }
			//} ) ;			
			showKeyInputDlg();
		};
		
		self.onKeyOK = function( ) {			
			var item = list.getSelected();
			var index = list.getSelectedIndex();
			var mrl = item.getMrl();
			var lockstatus =  item.getLockStatus();
			if(lockstatus){
				item.changeLockStatus(false);
				Platform.dvb.setChannelLocked(mrl,false);
				var itemData = list.getItemData( index );
				itemData["locked"] = false;
				list.setItemData( index ,itemData);
			}
			else{
				item.changeLockStatus(true);
				Platform.dvb.setChannelLocked(mrl,true);
				var itemData = list.getItemData(index );
				itemData["locked"] = true;
				list.setItemData( index ,itemData);		
			}			
		} ;
				
		
		self.onKeyLEFT = function(){		
			window.AppManager.back( );
		};	
				
		return self ;
	} ;
	
	window.AppManager.addScene( name, ParentalLock ) ;
	
})( ) ;