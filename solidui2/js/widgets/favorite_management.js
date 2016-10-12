"use strict";
(function( ) {
	
	var name = "Favorite Management";
	
	var List_Channel = new window.Class(
	//parent
	window.ListViewDelegate,
	
	//constructor
	function( channel ){
		ListViewDelegate.call( this, name ) ;
		
		this.$root.addClass('fav_management_listlable');
		var $Channel_lcn = $( '<div>' ).addClass( 'column_three' ).text( channel.lcn ).appendTo( this.$root ) ;
		var $Channel_name = $( '<div>' ).addClass( 'column_three' ).text( channel.name ).appendTo( this.$root ) ;
		
		var $channel_status = $( '<div>' ).addClass( 'column_three' ).appendTo( this.$root ) ;
		var $channel_favflag = $('<img src = ./templates/hkc/images/heart.png>').addClass('favflag').appendTo($channel_status);
		var $channel_lockflag = $('<img src = ./templates/hkc/images/lock.png height=10px width=8px>').addClass('lockflag').appendTo($channel_status);
		
		this.mrl = channel.mrl;
		this.lcn = channel.lcn;
		this.name = channel.name;
		
		this.favStatus = false;
		
		var loadStr = Platform.storage.load("FavFlag",'{\"mrl\":{\"Fav\":{\"count\":\"0\"}}}');
		var defaultFavStr = JSON.parse(loadStr);
		
		var favflag_count = 0;
		if(defaultFavStr[this.mrl] == undefined){
			$channel_favflag.hide();
			defaultFavStr[this.mrl] = {
				Fav : {
					count:favflag_count,
				}
			}
			var favData = JSON.stringify(defaultFavStr);
			Platform.storage.store("FavFlag",favData);
		}
		else	
		{
			favflag_count = defaultFavStr[this.mrl].Fav.count;
			if(favflag_count > 0){
				$channel_favflag.show();
			}
			else{
				$channel_favflag.hide();
			}
		}
		
		if(channel.locked){
			$channel_lockflag.show();
		}
		else{
			$channel_lockflag.hide();
		}
		
		this.lockStatus = channel.locked;
		this.$channel_lockflag = $channel_lockflag;
		this.$channel_favflag = $channel_favflag;
	},
	//prototype
	{
		getMrl: function(){
			return this.mrl;
		},
		getLcn: function(){
			return this.lcn;
		},
		getName: function(){
			return this.name;
		},
		getFavStatus:function(){
			return this.favStatus;
		},
		setFavStatus:function( status ){
			this.favStatus = status ;
			if(this.favStatus == false){
				this.$channel_favflag.hide();
			}
			else{
				this.$channel_favflag.show();
			}
		},
		getLockStatus:function(){
			return this.lockStatus;
		},
		
	}
	);
	
	var this_channel = [];
	var favList_Id = [];
	var channelList_Id = [];
	
	var favCount = 0;
	
	var FavManagement = function( ) {
			var parent = new window.Scene( name ) ;
			var self = Object.create( parent ) ;
			self.$root.attr( 'id', 'fav_management' );
		
			var $left_div = $('<div>').addClass('left_div').appendTo(self.$root);
			var $fav_name = $( '<div>' ).addClass( 'favName' ).appendTo($left_div);
			
			var getFavListData = function( favList_Id ){
				var start = 0;
				var limit = window.Template.ChannelMaxLimit;
				var template =  { "mrl":"","name":"","lcn":"","locked":""  } ;
				
				var updateChannels = function( error, channels ){
						if( error ) { throw new Error( error.channels ) ; }
						if(channels.length > 0){
							var favList_length = channels.length;
							var index = favList_length - 1 ;
						}
						else{
							var index = 0;
						}
						favList.updateItems( channels,index );
						this_channel = channels;
				};
				Platform.okList.getChannelsWithTemplate( favList_Id, start, limit, template, updateChannels );
			};
			
			var favFlag_add = function( mrl ){
					var loadStr = Platform.storage.load("FavFlag",'{\"mrl\":{\"Fav\":{\"count\":\"0\"}}}');
					var defaultFavStr = JSON.parse(loadStr);
					
					if(defaultFavStr[mrl] == undefined){
						
						defaultFavStr[mrl] = {
							Fav : {
								count: favCount,
							}
						}
					}
					else{
						favCount = defaultFavStr[mrl].Fav.count;
						defaultFavStr[mrl] = {
							Fav : {
								count: favCount += 1,
							}
						}
					}
					var FavData = JSON.stringify(defaultFavStr);
					Platform.storage.store("FavFlag",FavData);
			};
			
			var favFlag_delete = function( mrl ){
					var loadStr = Platform.storage.load("FavFlag",'{\"mrl\":{\"Fav\":{\"count\":\"0\"}}}');
					var defaultFavStr = JSON.parse(loadStr);
					
					favCount = defaultFavStr[mrl].Fav.count;
					defaultFavStr[mrl] = {
						Fav : {
							count: favCount -= 1,
						}
					}
					
					if(favCount < 0){
						favCount = 0;
					}
					
					var FavData = JSON.stringify(defaultFavStr);
					Platform.storage.store("FavFlag",FavData);
			};
			
			var addChannel = function( item ){
					var mrl = item.getMrl();
					var lcn = item.getLcn();
					var name = item.getName();
					var status = item.getLockStatus();
					
					var add_mrl = _.pluck(this_channel,'mrl');
					if(add_mrl.length == 0){
							var json = {
										lcn: lcn,
										name: name,
										mrl:mrl,
										locked:status,
							};
							this_channel.push(json);
					}
					else{
							for(var i = 0; i < add_mrl.length; i++){
								if(mrl == add_mrl[i]){
									return ; 
								}
								else{
									var json = {
										lcn: lcn,
										name: name,
										mrl:mrl,
										locked:status,
									};
								}
							}
							this_channel.push(json);
							this_channel.sort( function( channelPrev,channelNext ){
									return channelPrev.lcn > channelNext.lcn ? 1:-1;			
					  	});
					}
					
					favFlag_add(mrl);
					
					if(favCount > 0){
						item.setFavStatus(true);
					}
					else{
						item.setFavStatus(false);
					}
					
					var added_mrl = _.pluck(this_channel,'mrl');
					var index = added_mrl.indexOf(mrl);
					favList.updateItems( this_channel,index );
					favList.show();
			};
			
			
			var deleteChannel = function( item ){
				var delete_mrl = _.pluck(this_channel,'mrl');
				var mrl = item.getMrl();
				
				var index = delete_mrl.indexOf(mrl);
				this_channel.splice(index,1);
				
				if(index == 0){
					var _index = this_channel.length - 1;
				}
				else{
					var _index = index - 1;
				}
				
				favFlag_delete(mrl);
				
				var master_channels = channelList.getItems();
				for(var i = 0; i < master_channels.length; i++){
					if(mrl == master_channels[i].mrl){
						var master_item = master_channels[i];
						if(favCount > 0){
							master_item.setFavStatus(true);
						}
						else{
							master_item.setFavStatus(false);
						}
					}
				}
				favList.updateItems( this_channel, _index);
				favList.show();
			};
			
			var setChannels = function(){
				var add_mrl = _.pluck(this_channel,'mrl');
				var updateSetChannels = function(error, setChannels){
					if( error ) { throw new Error( error.setChannels ) ; }
					this_channel = [];
				};
				Platform.okList.setChannels( favList_Id, add_mrl, updateSetChannels);
			};
			
			var $fav_list = $( '<div>' ).addClass( 'favList' ).appendTo($left_div);
			var favList = new window.scrollListView( List_Channel ) ;
			favList.$root.appendTo( $fav_list ) ;
			
			
			var $right_div = $('<div>').addClass('right_div').appendTo(self.$root);
			var $channelList_name = $( '<div>' ).addClass( 'channelListName' ).appendTo($right_div);
			
			var getChannelsData = function( list_id ){
				var start = 0;
				var limit = window.Template.ChannelMaxLimit;
				var template =  { "mrl":"","name":"","lcn":"","locked":"" } ;
				
				var updateChannels = function( error, channels ){
					if( error ) { throw new Error( error.channels ) ; }
					Platform.okList.getCurrentChannel(function( error, currentChannels ){
							if( error ) { throw new Error( error.currentChannels ) ; }
							if( channels.length > 0 ){
								var mrl = currentChannels.mrl;
								var Mrls = _.pluck( channels, 'mrl' );
								var index = Mrls.indexOf(mrl);
								if(index == -1){
									index = 0;
								}
							}
							else{
								var index = 0;
							}
							channelList.updateItems( channels , index);
					});
				};
				Platform.okList.getChannelsWithTemplate( list_id, start, limit, template, updateChannels ); 
			};
			
			var $channelList_list = $( '<div>' ).addClass( 'channelList' ).appendTo($right_div);
			var channelList = new window.scrollListView( List_Channel ) ;
			channelList.$root.appendTo( $channelList_list ) ;
			
			var listData = [];
			(function(){
				var json1 = {
					$root:$left_div,
					callback:null,
				};
				var json2 = {
					$root:$right_div,
					callback:null,
				};
				listData.push(json2);
				listData.push(json1);
			})();
			var bigFocusList = new window.FocusList(listData);
			
			self.onKeyUP = function( ) {
				if( bigFocusList.getSeletedIndex() == 0 ){
					channelList.selectPrev();
				}
				else{
					favList.selectPrev();
				}
			};
		
			self.onKeyDOWN = function( ) {
				if( bigFocusList.getSeletedIndex() == 0 ){
					channelList.selectNext();
				}
				else{
					favList.selectNext();
				}
			};
			
			self.onKeyCHANUP = function(){
				if( bigFocusList.getSeletedIndex() == 0 ){
					channelList.getPagePrev();
				}
				else{
					favList.getPagePrev();
				}
			};
		
			self.onKeyCHANDOWN = function(){
				if( bigFocusList.getSeletedIndex() == 0 ){
					channelList.getPageNext();
				}
				else{
					favList.getPageNext();
				}
			};
			
			self.onKeyOK = function(){
				if( bigFocusList.getSeletedIndex() == 0 ){
					var item = channelList.getSelected();
					addChannel(item);
				}
				else{
					var item = favList.getSelected();
					deleteChannel(item);
				}
			};
			
			self.onKey = function() {
        return true;
    	};
    	
    	self.onKeyLEFT = function(){
    		bigFocusList.selectPrev();
    	};
    	
			self.onKeyRIGHT = function(){
    		bigFocusList.selectNext();
    	};
    	
    	self.onKeyBACK = 
    	self.onKeyEXIT = function(){
    		setChannels();
    		window.AppManager.back();
    		
		Platform.dvb.setCurrentList( favList_Id );
		window.Platform.dvb.getChannelsMrl(Platform.dvb.CurrentList, 0,  window.Template.ChannelMaxLimit);
    		var liveTV = window.AppManager.getWidget("Live TV");
    		liveTV.showOkBar();
    	};
    	
    	self.registerCallback = function( favListId, favListName, channelListId, channelListName ){
    		favList_Id = favListId;
    		channelList_Id = channelListId;
    		
    		$fav_name.text( favListName );
    		getFavListData(favList_Id);
    		favList.show();
    		
    		$channelList_name.text( channelListName );
    		getChannelsData(channelList_Id);
    		channelList.show();
    		bigFocusList.select(0);
    	};
    	
    	return self;
			
	};
	
		
	window.AppManager.addScene( name, FavManagement ) ;
	
})( );