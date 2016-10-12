"use strict";
(function( ) {
	
	var name = "Ok Bar" ;
	
	//Recording Delegate
	var Channel = new window.Class(
	
	//parent
	window.ListViewDelegate,
	
	//constructor
	function( channel ){
			ListViewDelegate.call( this, name ) ;
			
			this.$root.addClass( 'okbar_listlable' ) ;
			
			var $channel_lcn = $( '<div>' ).addClass( 'column_two' ).text( channel.lcn ).appendTo( this.$root ) ;
			var $channel_name = $( '<div>' ).addClass( 'column_five' ).text( channel.name ).appendTo( this.$root ) ;
			
			var $channel_status = $( '<div>' ).addClass( 'column_thrity' ).appendTo( this.$root ) ;
			var $channel_favflag = $('<img src = ./templates/hkc/images/heart.png>').addClass('favflag').appendTo($channel_status);
			var $channel_lockflag = $('<img src = ./templates/hkc/images/lock.png height=10px width=10px>').addClass('lockflag').appendTo($channel_status);
			
			this.mrl = channel.mrl;
			
			$channel_favflag.hide();
			
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
	},
		//prototype
		{
			getMrl: function(){
				return this.mrl;
			}
		}
	)
	
	var select_index = [];
	var _favCount = 0;
	
	var OkBars = function( ) {
		
			var parent = new window.Scene( name ) ;
			var self = Object.create( parent ) ;
			self.$root.attr( 'id', 'ok_bar' );
		
			var switchToList = function( item ) {	
				
				getChangeToList(item.id);
				
			};
	
			var selectDescription = 
			{
				name: "ok bar screen :: channelList",
				items:[],
				action: switchToList,
			};
			
			var $ok_bar_info = $( '<div>' ).addClass( 'okbarinfo' ).appendTo(self.$root);
			
			var selectList = new window.SelectView(selectDescription, selectDescription.action);
			selectList.$root.appendTo($ok_bar_info);
			selectList.activate(true);
			selectList.show();
			//var select_view = null;
			
			var getListsData  = function( ){
				var channelTemplate = {"id":"","name":""};
				var updateLists = function( error, channelList ){
					if( error ) { throw new Error( error.channelList ) ; }
					if(window.Template.EnableMaster == false){
						for(var i = 0; i < channelList.length; i++){
							if( channelList[i].id == 0 ){
								channelList.splice(i, 1);
								break;
							}
						}					
					}
					selectList.updateItems(channelList);
					Platform.dvb.Lists = channelList;
					
				};
				Platform.okList.getLists( channelTemplate,updateLists);
			};
			
			var getCurrentList = function( ){
				var updateCurrentList = function( error, currentList ){
					if( error ) { throw new Error( error.currentList ) ; }
					var getCurrentList_index = function(){
						for(var i = 0; i < Platform.dvb.Lists.length; i++){
							if( currentList == Platform.dvb.Lists[i].id){
								return i;
							}
						}
					}
					var select_index = getCurrentList_index();
					selectList.changeSelection(select_index);
					hideButton(currentList);
				}
				Platform.dvb.getCurrentList( updateCurrentList );	
			};
			
			var syncLiveTVChannels = function( ) {
				if ( Platform.dvb.getCurrentList() != selectList.getSelected().id ||  _.isUndefined( Platform.dvb.ChannelsLcn[0] ) ) {
					var this_ID = selectList.getSelected().id;
					Platform.dvb.setCurrentList( this_ID );			
					window.Platform.dvb.getChannelsMrl(Platform.dvb.CurrentList, 0,  window.Template.ChannelMaxLimit);					
				}					
			};
			
			var getChannelsData = function( list_id ){
				var start = 0;
				var limit = window.Template.ChannelMaxLimit;
				var template =  { "mrl":"","name":"","lcn":"","locked":"" } ;
				
				var updateChannels = function( error, channels ){
					if( error ) { throw new Error( error.channels ) ; }
							var liveTV = window.AppManager.getWidget("Live TV");
							var $currentchannelinfo = liveTV.judgeIfTunedChannel( );
							var listIndex = 0;
							if( channels.length > 0 ){
								var mrl = $currentchannelinfo.mrl;
								var Mrls = _.pluck( channels, 'mrl' );
								listIndex = Mrls.indexOf(mrl);
								if(listIndex == -1){
									listIndex = 0;
								}
								else {
									syncLiveTVChannels();
									listIndex = Platform.dvb.ChannelsMrl.indexOf( mrl ) ;
									Platform.dvb.ChannelCurrent = {
										mrl: mrl,
										index: listIndex,
									} ;										
								}
							}
							else{
								listIndex = 0;
							}
							list.updateItems( channels , listIndex);
				};
				Platform.okList.getChannelsWithTemplate( list_id, start, limit, template, updateChannels ); 
			};
			
			var getChangeToList = function( list_id ){
				getChannelsData( list_id );
				hideButton(list_id);
			};
			
			var createList = function( name ){
				var updateCreateList = function( error, lists ){
						if( error ) { throw new Error( error.lists ) ; }
						getListsData( );
					
						var favList_index = function(){
							for(var i = 0; i < Platform.dvb.Lists.length; i++){
								if( lists == Platform.dvb.Lists[i].id){
									return i;
								}
							}
						}
						_.defer( function( ) { 
							var favSelect_index = favList_index();
							selectList.changeSelection(favSelect_index) ; 
						} ) ;
						
				};	
				Platform.okList.createList( name, updateCreateList);
			};
			
			var delete_favList = function( favListId ){
				var deleteList = function( error, _list){
					if( error ) { throw new Error( error._list ) ; }
					var AllChannels = list.getAllItems();
					for(var i = 0; i < AllChannels.length; i++){
						var loadStr = Platform.storage.load("FavFlag",'{\"mrl\":{\"Fav\":{\"count\":\"0\"}}}');
						var defaultFavStr = JSON.parse(loadStr);
						
						var _mrl = AllChannels[i].mrl;
						_favCount = defaultFavStr[_mrl].Fav.count;
						defaultFavStr[_mrl] = {
							Fav : {
								count: _favCount - 1,
							}
						}
						
						var FavData = JSON.stringify(defaultFavStr);
						Platform.storage.store("FavFlag",FavData);
					}
					
					getListsData();
					selectList.selectPrev( ) ;
					var changed_listId = selectList.getSelected().id;
					
					Platform.dvb.setCurrentList( changed_listId );
					Platform.dvb.getChannelsMrl(Platform.dvb.CurrentList, 0, window.Template.ChannelMaxLimit);
					getChannelsData(changed_listId);
				};
				Platform.okList.destroyList( favListId , deleteList);
			};
			
			var $ok_bar_list = $( '<div>' ).addClass( 'okbarlist' ).appendTo(self.$root);
			
			var list = new window.scrollListView( Channel ) ;
			list.$root.appendTo( $ok_bar_list ) ;
			
			var updateScreen = function( ) {
					getListsData();
					getCurrentList( );
					list.show();
					selectList.show();
			};
			
			
			var $ok_bar_button = $( '<div>' ).addClass( 'okbarbutton' ).appendTo(self.$root);
			
			var $button_left = $('<div>').addClass('buttonleft').appendTo($ok_bar_button);						
			var $ok_bar_add = $('<div>').addClass('okbaradd').appendTo($button_left);
			var $ok_bar_addText = $('<div>').addClass('okbaraddText').append($TR('ok bar screen :: okbaraddText')).appendTo($button_left);
			
			var $button_middle = $('<div>').addClass('buttonmiddle').appendTo($ok_bar_button);				
			var $ok_bar_delete = $('<div>').addClass('okbardelete').appendTo($button_middle);
			var $ok_bar_deleteText = $('<div>').addClass('okbardeleteText').append($TR('ok bar screen :: okbardeleteText')).appendTo($button_middle);			
			
			var $button_right = $('<div>').addClass('buttonright').appendTo($ok_bar_button);
			var $ok_bar_edit = $('<div>').addClass('okbaredit').appendTo($button_right);
			var $ok_bar_editText = $('<div>').addClass('okbareditText').append($TR('ok bar screen :: okbareditText')).appendTo($button_right);
			
			var hideButton = function( select ) {
				if( select == 0 || select >= 8000){
				   $ok_bar_add.show();
			   	 $ok_bar_addText.show();
			   	 $ok_bar_delete.hide();
			   	 $ok_bar_deleteText.hide();
			   	 $ok_bar_edit.hide();
			   	 $ok_bar_editText.hide();
			  }
			  else{
			  	 $ok_bar_add.show();
			   	 $ok_bar_addText.show();
			   	 $ok_bar_delete.show();
			   	 $ok_bar_deleteText.show();
			   	 $ok_bar_edit.show();
			   	 $ok_bar_editText.show();
			  }
			}
		
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
		};
		
		self.onShow = function( ) {					
			updateScreen();	
		};
		
		self.onKeyOK = function( ) {
			syncLiveTVChannels();
			var item = list.getSelected();
			var mrl = item.getMrl();
			var liveTV = window.AppManager.getWidget("Live TV");
			liveTV.playChannel( mrl );
		};
		
		self.onKeyRIGHT = function(){
			selectList.selectNext( ) ;
		};
		
		self.onKeyLEFT = function(){
			selectList.selectPrev( ) ;
		};
		
		self.onKeyEXIT = function(){
			//syncLiveTVChannels();
			window.AppManager.back( ) ;			
		};
		
		self.onKeyGREEN = function(){
				var callbackOk = function ( str ) {
					var CurrentList = Platform.dvb.getCurrentList();
					if(str == "" || str == undefined){
						return ;
					}
					else{
						createList(str);
					}
					window.AppManager.back( ) ;
				}
				var callbackCancel = function ( ) {
					window.AppManager.back() ;
				}
				var favPopDlg = window.AppManager.getWidget("Fav PopDlg");
				favPopDlg.registerCallback( callbackOk, callbackCancel );
				window.AppManager.enter( "Fav PopDlg" ) ;
		};
		self.onKeyBLUE = function(){
			if( selectList.getSelected().id != 0 && selectList.getSelected().id < 8000){
				window.AppManager.back( ) ;
				var favlist_content = selectList.getSelected();
				//var channellist_content = selectList.getItemByIndex("1");
				
				var fav_management = window.AppManager.getWidget("Favorite Management");
				var favListId = favlist_content.id;
				var favListName = favlist_content.name;
				
				var channelListId;
				if(window.Template.EnableMaster == false){
					channelListId = Platform.okList.getMainListID();
				}
				else {
					channelListId = 0;
				}
				
				var channelListName = TR("ok bar screen :: All Channels");
				fav_management.registerCallback( favListId, favListName, channelListId, channelListName );
				window.AppManager.enter("Favorite Management") ;
			}
		};
		
		self.onKeyRED = function(){
				if( selectList.getSelected().id != 0 && selectList.getSelected().id < 8000 ){
					var favlist_content = selectList.getSelected();
					var favListId = favlist_content.id;
					var selectPopDlg = window.AppManager.getWidget("Select PopDlg");
					var callbackOk = function ( ) {
						//var CurrentList = Platform.dvb.getCurrentList();
						delete_favList( favListId );
						window.AppManager.back() ;
					}
					var callbackCancel = function ( ) {
						window.AppManager.back() ;
					}
					var string = "fav_Are you sure to delete this favorite list";
					selectPopDlg.registerCallback( callbackOk, callbackCancel,string );
					window.AppManager.enter( "Select PopDlg" ) ;
				}
		};
		
		return self ;
	} ;
	
	window.AppManager.addScene( name, OkBars ) ;
	
})( ) ;