var tunerStatus = new window.Class(

	// parent
	window.Widget,

	// constructor
	function() {
		Widget.call( this,"tuner status") ;
		
		this.tipCableDisconnected_pop_Dlg = new window.popDlg( );
		this.tipCableDisconnected_pop_Dlg.$root.appendTo( self.$root ) ;
		this.tipCableDisconnected_pop_Dlg.hide();
		this.tipCableDisconnected_pop_Dlg_showed = false;
		this.lostCount = 0;
		this.lockCount = 0;
		var checkCableDisconnected = function( ) {
			var tuneStatus = Platform.system.getTuneStatus();
			if ( tuneStatus === Platform.system.MediaEngineTuneStatus.TUNE_LOCKED ) {
				this.lockCount++;
				if (this.lockCount >= 100) {
					this.lockCount = 100;
				}
				
				if ( this.tipCableDisconnected_pop_Dlg_showed === true ) {
					this.tipCableDisconnected_pop_Dlg.hide();
					this.tipCableDisconnected_pop_Dlg_showed = false;
				}
			}
			else if ( tuneStatus === Platform.system.MediaEngineTuneStatus.TUNE_NO_LOCK ) {
				this.lostCount++;
				if (this.lostCount >= 100) {
					this.lostCount = 100;
				}				
				if ( this.lostCount == 9 && this.lockCount == 0 ) {
					if ( window.Template.EnableFrqHunting == true ) {
						window.Platform.settings.setFrqHuntingRun( true ) ;
						this.lostCount = 0;
						window.location.reload();
					}
				}				
				
				var liveTV = window.AppManager.getWidget("Live TV");
				if( !liveTV.isActive( ) ) {
					if ( this.tipCableDisconnected_pop_Dlg_showed === true ) {
						this.tipCableDisconnected_pop_Dlg.hide();
						this.tipCableDisconnected_pop_Dlg_showed = false;
					}
					return ;
				}				
				
				if ( this.tipCableDisconnected_pop_Dlg_showed === false ) {
					var str = "popDlg :: no signal" ;
					this.tipCableDisconnected_pop_Dlg.show();
					this.tipCableDisconnected_pop_Dlg.showDlgMsg( str );
					this.tipCableDisconnected_pop_Dlg_showed = true;
				}
				
			}
		}
		setInterval(_.bind(checkCableDisconnected, this), 1000);		
	},

	// prototype
	{
		
	}
);	