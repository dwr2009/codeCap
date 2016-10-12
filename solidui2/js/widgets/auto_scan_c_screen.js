"use strict";
(function( ) {

	var name = "Auto Scan C" ;

	var FrqInfo = new window.Class(

		// parent
		window.ListViewDelegate,

		// constructor
		function( info ) {
			ListViewDelegate.call( this, info.name ) ;

			this.$root.addClass( 'frqInfo' ) ;
			var $frequence     = $( '<div>' ).addClass( 'column_three' ).text( info[0] ).appendTo( this.$root ) ;
            
			var bandwidth = "";
			switch ( info[3] ) {
				case 0:
				bandwidth = "BPSK";    
				break;                 
				case 1:
				bandwidth = "QPSK";    
				break;                 
				case 2:
				bandwidth = "QAM_8";
				break;
				case 3:
				bandwidth = "QAM_16";
				break;
				case 4:
				bandwidth = "QAM_32";    
				break;   
				case 5:
				bandwidth = "QAM_64";    
				break;      
				case 6:
				bandwidth = "QAM_128";    
				break;   
				case 7:
				bandwidth = "QAM_256";    
				break;                                                                            
			}
            
			var $bandwidth     = $( '<div>' ).addClass( 'column_three' ).text( bandwidth ).appendTo( this.$root ) ;
			var $symbolrate    = $( '<div>' ).addClass( 'column_three' ).text( info[4] ).appendTo( this.$root ) ;
		},

		// prototype
		{}
	) ;


	var AutoScanC = function( ) {

		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;
		self.$root.attr('id', 'autoscanc');

		var lockMenu = false;
		var leaveCallback = null;
        
		var $caption    = $( '<div>' ).addClass( 'caption' ).appendTo( self.$root ) ;	
		var $frequence  = $( '<div>' ).addClass( 'column_three' ).append( $TR( 'manual scan screen :: frequency'  ) ).appendTo( $caption ) ;
		var $bandwidth  = $( '<div>' ).addClass( 'column_three' ).append( $TR( 'manual scan screen :: modulation' ) ).appendTo( $caption ) ;
		var $symbolrate = $( '<div>' ).addClass( 'column_three' ).append( $TR( 'manual scan screen :: symbolRate' ) ).appendTo( $caption ) ;
        
		var $frqList = $( '<div>' ).addClass( 'frqList' ).appendTo(self.$root);
		var list = new window.scrollListView( FrqInfo ) ;
		list.$root.appendTo( $frqList ) ;
				

		var $lbStrength = $('<div>').addClass('lbStrength').appendTo(self.$root);
		var lbStrength_description = {
			name: 'manual scan screen :: strength',
			value: ''
		};        
		var lbStrength_item = new window.LabelView(lbStrength_description);
		lbStrength_item.$root.appendTo($lbStrength);
		lbStrength_item.show();


		var $lbQuality = $('<div>').addClass('lbQuality').appendTo(self.$root);
		var lbQuality_description = {
			name: 'manual scan screen :: quality',
			value: ''
		};        
		var lbQuality_item = new window.LabelView(lbQuality_description);
		lbQuality_item.$root.appendTo($lbQuality);
		lbQuality_item.show();
        
		var $lbChannelCount = $('<div>').addClass('lbChannelCount').appendTo(self.$root);
		var lbChannelCount_description = {
			name: 'manual scan screen :: count',
			value: ''
		};        
		var lbChannelCount_item = new window.LabelView(lbChannelCount_description);
		lbChannelCount_item.$root.appendTo($lbChannelCount);
		lbChannelCount_item.show();        
        
		var $pbSearchingBar = $('<div>').addClass('pbSearchingBar').appendTo(self.$root);
			var pbSearchingBar_description = {
			name: 'processBar',
		};        
		var pbSearchingBar_item = new window.ProcessBar(pbSearchingBar_description);
		pbSearchingBar_item.$root.appendTo($pbSearchingBar);
		pbSearchingBar_item.show();
        
		var $btContinue = $('<div>').addClass('btContinue').appendTo(self.$root);
		var btContinue_description = {
			name: 'auto scan screen :: continue',
			callback: null
		};        
		var btContinue_item = new window.ButtonView(btContinue_description);
		btContinue_item.$root.appendTo($btContinue);
		btContinue_item.show();

		var index=0;
		var getFrqInfo = function( ) {
			var progress = 0;
			if ( index != 0 ) {
				progress = parseInt( index / window.Template.frequenceList.length * 100 );
			}
			pbSearchingBar_item.setValue(progress);
			var frq = window.Template.frequenceList[index];
			var retVal = true;
			index++;            
			if ( index == window.Template.frequenceList.length ) {
				retVal = false;
			}
			else {
				retVal = true;
			}
			var info = {
				"retVal": retVal,
				"frq"   : frq,   
			};
			return info;
		}

		var checkSISQ = function( ) {
			var status = window.Platform.scanner.getStatus();
			lbStrength_item.setText(status.signalLevel + "%");
			lbQuality_item.setText(status.signalQuality + "%");  			
		}
		var checkSISQtimer = setInterval(checkSISQ, 2000);

		var dvbcScannerStatusCallback = function(status) {
			lbStrength_item.setText(status.signalLevel + "%");
			lbQuality_item.setText(status.signalQuality + "%");            
			    
			console.log ("status.state=" + status.state + 
			             " status.progress=" + status.progress + 
			             " status.foundChannelsNumber=" + status.foundChannelsNumber + 
			             " status.signalLevel=" + status.signalLevel + 
			             " status.signalQuality=" + status.signalQuality);
			    
			if ( status.state == Platform.scanner.ScannerState["SCANNER_STATE_SUCCEEDED"] && status.signalLevel > 0 ) {
				lbChannelCount_item.setText(status.foundChannelsNumber);
				pbSearchingBar_item.setValue(100);
			    var count = lbChannelCount_item.getText();
			    if ( parseInt(count) > 0 ) {
			    	clearInterval(checkSISQtimer);
			    	
			    	setTimeout(function( ) {
			    		window.AppManager.back( ) ;
				       leaveCallback && leaveCallback();
			    	},1000);                
			    }
			    
				/*
				//Jump to Continue button
				list.getSelected().$root.toggleClass( 'selected', false ) ;
				btContinue_item.$root.toggleClass( 'selected', true ) ;
				lockMenu = false;
				*/
			}
			    
			if ( status.state == Platform.scanner.ScannerState["SCANNER_STATE_FAILED"] || 
			    (status.state == Platform.scanner.ScannerState["SCANNER_STATE_SUCCEEDED"] && status.signalLevel == 0) ) {
				list.selectNext();
				var retValue = startDvbcAutoScan();
				if ( retValue == false ) {
					//Auto scan again
					index = 0;
					list.select(window.Template.frequenceList.length - 1);
					list.selectNext( );
					startDvbcAutoScan();
					/*
					//Jump to Continue button
					pbSearchingBar_item.setValue(100);
					list.getSelected().$root.toggleClass( 'selected', false ) ;
					btContinue_item.$root.toggleClass( 'selected', true ) ;
					lockMenu = false;       
					*/               
				}
			}
			    
			if ( status.progress == 100 ) {
				lockMenu = false;
			}            
        };
        
		var startDvbcAutoScan = function() {
			var ret        = getFrqInfo();
			if ( ret["retVal"] == false ) {
				return false;
			}
			var frqInfo    = ret["frq"];
			var from_freq  = frqInfo[0];
			var to_freq    = frqInfo[1];
			var freq_step  = frqInfo[2];
			var bandwidth  = frqInfo[3];
			var symbolrate = frqInfo[4];
		        
			console.log ("from_freq=" + from_freq + 
			             " to_freq=" + to_freq +
			             " freq_step=" + freq_step +
			             " bandwidth=" + bandwidth +
			             " symbolrate=" + symbolrate);            
		                      
			Platform.scanner.setType({
				scannerType: 'cable',
			});            
			Platform.scanner.setDvbcScannerStatusCallback(dvbcScannerStatusCallback);
			Platform.scanner.startDvbcScan(from_freq, to_freq, freq_step, bandwidth, symbolrate);
			return true;
		};
        
        self.onKey = function() {
		return true;
        };

        self.onShow = function() {
		var _list = window.Template.frequenceList ;
		list.updateItems( _list, 0 );
		list.show();
							
		_.defer( function( ) { list.pageUpAndDown( 0 ) ; } ) ;
		lockMenu = true;
		Platform.media.stop( function( error ) {
			if( error ) { throw new Error( error.message ) ; }
		} ) ;
		startDvbcAutoScan();
        };

        self.onKeyOK = function() {  
            if ( lockMenu == false ) {
                var count = lbChannelCount_item.getText();
                if ( parseInt(count) > 0 ) {
                	clearInterval(checkSISQtimer);
                    window.AppManager.back( ) ;
                    leaveCallback && leaveCallback();                        
                } 
                else {
                    lockMenu = true;
                    index = 0;
                    list.select(window.Template.frequenceList.length - 1);
                    list.selectNext( );
                    startDvbcAutoScan();
                }
            }
        };
        
        self.setLeaveCallback = function( callback ) {
            leaveCallback = callback;
        };

		return self ;
	} ;

	window.AppManager.addScene( name, AutoScanC ) ;

})( ) ;
