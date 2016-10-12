
	var AudioTrackDelegate = new window.Class(

		// parent
		window.ListViewDelegate,

		// constructor
		function( audiotrack ) {
			ListViewDelegate.call( this,name) ;			
			
			var $audiotrack_name = $('<div>').addClass('audiotrack_name').text("Audio"+(audiotrack.index+1)).appendTo(this.$root);
			this.audiotrackIndex = audiotrack.index; 
		},

		// prototype
		{
			getAudiotrackIndex:function(){
				return this.audiotrackIndex;
			},
		}
	) ;

	var AudioTrack = new window.Class(
	
		// parent
		window.Widget,
	
		// constructor
		function() {
			Widget.call( this,"Audio track ") ;
				
			var list_view = new window.ListView(AudioTrackDelegate) ;
			list_view.$root.addClass('audiotrack').appendTo( this.$root ) ;
			this.list_view = list_view;
			
		},
	
		// prototype
		{
			updateAudioTrack:function(audioTracksInfo){
				this.list_view.updateItems(audioTracksInfo);
				this.list_view.show();
			},
			showCurrentAudioTrack:function(index){
				this.list_view.select(index);						
			},
			changeAudioTrack:function(mrl){				
				this.list_view.selectNext();
				var item = this.list_view.getSelected();
				var audioTrackIndex = item.getAudiotrackIndex();
				var loadStr = Platform.storage.load("AudioTrack",'{\"mrl\":{\"audio\":{\"selected\":\"0\"}}}');					
				var defaultAudioTrackStr = JSON.parse(loadStr);					
				defaultAudioTrackStr[mrl] = {
					audio : {
      					 selected:audioTrackIndex, 
					}
				}
				var audioTrackData = JSON.stringify(defaultAudioTrackStr);							
				Platform.storage.store("AudioTrack",audioTrackData);
				window.Platform.settings.setAudioTrack(audioTrackIndex);
				var info_bar       = window.AppManager.getWidget( "Info Bar" ) ;
				info_bar.setAudioTrackText(audioTrackIndex);				
			},	
			
		}
	);	