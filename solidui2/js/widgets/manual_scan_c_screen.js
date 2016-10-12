"use strict";
(function( ) {

	var name = "Manual Scan C" ;


	var ManualScanC = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;
		self.$root.attr('id', 'manualscanc');

        var lockMenu = false;

        /*
        var $lbScanner = $('<div>').appendTo(self.$root);
        var lbScanner_description = {
            name: 'Scanner:',
            value: 'DVB-C'
        };        
        var lbScanner_item = new window.LabelView(lbScanner_description);
        lbScanner_item.$root.appendTo($lbScanner);
        lbScanner_item.show();
        */

        var $ipFrequency = $('<div>').appendTo(self.$root);
        var ipFrequency_description = {
            name: 'manual scan screen :: frequency',
            size: 5,
            accurationLength: 2,
            type: 'decimal',
        };        
        var ipFrequency_item = new window.InputView(ipFrequency_description);
        ipFrequency_item.$root.appendTo($ipFrequency);
        ipFrequency_item.show();


        var $svCableModulation = $('<div>').appendTo(self.$root);
        var svCableModulation_description = {
            name: "manual scan screen :: modulation",
            items: [
            {
                name: 'QAM_8',
                value: 2,
            },
            {
                name: 'QAM_16',
                value: 3,
            },
            {
                name: 'QAM_32',
                value: 4,
            },
            {
                name: 'QAM_64',
                value: 5,
            },      
            {
                name: 'QAM_128',
                value: 7,
            }, 
            {
                name: 'QAM_256',
                value: 8,
            },  
            {
                name: 'BPSK',
                value: 0,
            },
            {
                name: 'QPSK',
                value: 1,
            },                                                     
            ],
            action: null,
        };        
        var svCableModulation_item = new window.SelectView(svCableModulation_description, svCableModulation_description.action);
        svCableModulation_item.$root.appendTo($svCableModulation);
        svCableModulation_item.show();

        var $ipSymbolRate = $('<div>').appendTo(self.$root);
        var ipSymbolRate_description = {
            name: 'manual scan screen :: symbolRate',
            size: 4,
            type: 'normal',
        };        
        var ipSymbolRate_item = new window.InputView(ipSymbolRate_description);
        ipSymbolRate_item.$root.appendTo($ipSymbolRate);
        ipSymbolRate_item.show();
        
        var $svSearchingMode = $('<div>').appendTo(self.$root);
        var svSearchingMode_description = {
            name: "manual scan screen :: searchingMode",
            items: [{
                name: 'Frequency',
                value: 0,
            },
            {
                name: 'Network',
                value: 1,
            },                       
            ],
            action: null,
        };        
        var svSearchingMode_item = new window.SelectView(svSearchingMode_description, svSearchingMode_description.action);
        svSearchingMode_item.$root.appendTo($svSearchingMode);
        svSearchingMode_item.show(); 

        var $lbStrength = $('<div>').addClass('lbStrength').appendTo(self.$root);
        var lbStrength_description = {
            name: 'manual scan screen :: strength',
            value: ''
        };        
        var lbStrength_item = new window.LabelView(lbStrength_description);
        lbStrength_item.$root.appendTo($lbStrength);
        lbStrength_item.show();


        var $lbQuality = $('<div>').appendTo(self.$root);
        var lbQuality_description = {
            name: 'manual scan screen :: quality',
            value: ''
        };        
        var lbQuality_item = new window.LabelView(lbQuality_description);
        lbQuality_item.$root.appendTo($lbQuality);
        lbQuality_item.show();
        
        
        var $pbSearchingBar = $('<div>').addClass('pbSearchingBar').appendTo(self.$root);
        var pbSearchingBar_description = {
            name: 'processBar',
        };        
        var pbSearchingBar_item = new window.ProcessBar(pbSearchingBar_description);
        pbSearchingBar_item.$root.appendTo($pbSearchingBar);
        pbSearchingBar_item.show();


        var $lbChannelCount = $('<div>').appendTo(self.$root);
        var lbChannelCount_description = {
            name: 'manual scan screen :: count',
            value: ''
        };        
        var lbChannelCount_item = new window.LabelView(lbChannelCount_description);
        lbChannelCount_item.$root.appendTo($lbChannelCount);
        lbChannelCount_item.show();
        
        var $barAction = $('<div>').addClass('barAction').appendTo(self.$root);
        
        var $btStart = $('<div>').addClass('btStart').appendTo($barAction);
        var btStart_description = {
            name: 'manual scan screen :: start',
            callback: null
        };        
        var btStart_item = new window.ButtonView(btStart_description);
        btStart_item.$root.appendTo($btStart);
        btStart_item.show();

        var $btClean = $('<div>').addClass('btClean').appendTo($barAction);
        var btClean_description = {
            name: 'manual scan screen :: clean',
            callback: null
        };        
        var btClean_item = new window.ButtonView(btClean_description);
        btClean_item.$root.appendTo($btClean);
        btClean_item.show();

	var $btFactoryReset = $('<div>').addClass('btFactoryReset').appendTo($barAction);
	var btFactoryReset_description = {
		name: 'manual scan screen :: factoryreset',
		callback: null
	};        
	var btFactoryReset_item = new window.ButtonView(btFactoryReset_description);
	btFactoryReset_item.$root.appendTo($btFactoryReset);
	btFactoryReset_item.show();

        var listData = [
            {
                name: 'ipFrequency',
                $root: $ipFrequency,
                callback: _.bind(ipFrequency_item.activate, ipFrequency_item),
            },
            {
                name: 'svCableModulation',
                $root: $svCableModulation,
                callback: null,
            },            
            {
                name: 'ipSymbolRate',
                $root: $ipSymbolRate,
                callback: _.bind(ipSymbolRate_item.activate, ipSymbolRate_item),
            },
            {
                name: 'svSearchingMode',
                $root: $svSearchingMode,
                callback: null,
            },          
            {
                name: 'btStart',
                $root: $btStart,
                callback: null,
            },
            {
                name: 'btClean',
                $root: $btClean,
                callback: null,
            }, 
            {
                name: 'btFactoryReset',
                $root: $btFactoryReset,
                callback: null,
            }                                               
        ];
        var focusList = new window.FocusList(listData);
        var select_view = null;
        
        var updateScreen = function() {
            
        };
        
		var checkSISQ = function( ) {
			var status = window.Platform.scanner.getStatus();
			lbStrength_item.setText(status.signalLevel + "%");
			lbQuality_item.setText(status.signalQuality + "%");  			
		}
		var checkSISQtimer = null;
        
        var dvbcScannerStatusCallback = function(status) {
            //pbSearchingBar_item.setValue(status.progress);
            lbChannelCount_item.setText(status.foundChannelsNumber);
            lbStrength_item.setText(status.signalLevel + "%");
            lbQuality_item.setText(status.signalQuality + "%");
            
			console.log ("status.state=" + status.state + 
			             " status.progress=" + status.progress + 
			             " status.foundChannelsNumber=" + status.foundChannelsNumber + 
			             " status.signalLevel=" + status.signalLevel + 
			             " status.signalQuality=" + status.signalQuality);
            
            if ( status.state == Platform.scanner.ScannerState["SCANNER_STATE_SUCCEEDED"] || status.state == Platform.scanner.ScannerState["SCANNER_STATE_FAILED"] ) {
            	if (status.foundChannelsNumber > 0 ) {
            		//Update the services
            		Platform.dvb.updateServices();
           		}
            	clearInterval(checkSISQtimer);
                pbSearchingBar_item.setValue(100);
                btStart_item.activate(false);
                lockMenu = false;
            }
            
            if ( status.progress == 100 ) {
            	clearInterval(checkSISQtimer);
            	pbSearchingBar_item.setValue(100);
                btStart_item.activate(false);
                lockMenu = false;
            }
        };
        
        var startDvbcManualScan = function() {
            var from_freq   = parseInt(ipFrequency_item.getText());
            var to_freq     = from_freq;
            var freq_step   = 1;
            var bandwidth   = svCableModulation_item.getSelected().value;
            var symbolrate  = parseInt(ipSymbolRate_item.getText());
            
			checkSISQtimer = setInterval(checkSISQ, 2000);
            
			console.log ("from_freq=" + from_freq + 
			             " to_freq=" + to_freq +
			             " freq_step=" + freq_step +
			             " bandwidth=" + bandwidth +
			             " symbolrate=" + symbolrate); 
			             
            pbSearchingBar_item.setValue(10);              
            Platform.scanner.setType({
                    scannerType: 'cable',
                });
            Platform.scanner.setDvbcScannerStatusCallback(dvbcScannerStatusCallback);
            Platform.scanner.startDvbcScan(from_freq, to_freq, freq_step, bandwidth, symbolrate);
        };
        
        
        var checkMenuLock = function() {
            return lockMenu;
        };
        
        self.onKey = function() {
             return true;
        };

        self.onShow = function() {
            updateScreen();
			Platform.media.stop( function( error ) {
				if( error ) { throw new Error( error.message ) ; }
			} ) ;
        };

        self.onKeyNUMBER = function(num) {
            if (checkMenuLock() == true) {
                return ;
            }
            if (focusList.getSelectedName() == 'ipFrequency') {
                ipFrequency_item.appendChar(num.toString());
            }
            else if (focusList.getSelectedName() == 'ipSymbolRate') {
                ipSymbolRate_item.appendChar(num.toString());
            }            
        };

        self.onKeyUP = function() {
            if (checkMenuLock() == true) {
                return ;
            }          
            if (select_view) {
                return;
            }       
            
            focusList.selectPrev();
        };

        self.onKeyDOWN = function() {
            if (checkMenuLock() == true) {
                return ;
            }            
            if (select_view) {
                return;
            }
            focusList.selectNext();
        };

        self.onKeyOK = function() {
            if (checkMenuLock() == true) {
                return ;
            }
                        
            if (focusList.getSelectedName() == 'svCableModulation') {
                if (!select_view) {
                    select_view = svCableModulation_item;
                    select_view.activate(true);
                } else {
                    select_view.activate(false);
                    select_view = null;
                }
            }
            else if (focusList.getSelectedName() == 'svSearchingMode') {
                if (!select_view) {
                    select_view = svSearchingMode_item;
                    select_view.activate(true);
                } else {
                    select_view.activate(false);
                    select_view = null;
                }
            }            
            else if (focusList.getSelectedName() == 'btStart') {
                btStart_item.activate(true);
                startDvbcManualScan();
                lockMenu = true;
            }
            else if (focusList.getSelectedName() == 'btClean') {
                Platform.dvb.deleteChannels();
            }            
            else if (focusList.getSelectedName() == 'btFactoryReset') {
				var syncFactoryResetStatus = null;
				var callbackOk = function ( ) {
					window.Platform.system.factoryResetStart(window.Platform.system.MediaEngineResetType["MEDIA_ENGINE_RESET_TYPE_NAND"]);	
					function intervalFun() {
						var status = window.Platform.system.getFactoryResetStatus();
						if ( status == window.Platform.system.MediaEngineResetStatus["MEDIA_ENGINE_RESET_STATUS_FINISHED"] )
						{
							window.Platform.system.rebootSTB();
							clearInterval(syncFactoryResetStatus);
							syncFactoryResetStatus = null;
						}
					}
					if ( syncFactoryResetStatus == null ) {
						syncFactoryResetStatus = setInterval(intervalFun, 1000);     
					}  																																			
				}
				var callbackCancel = function ( ) {
					window.AppManager.back() ;					
				}
				var string = "manual scan screen :: ensure factoryreset";
				var selectPopDlg = window.AppManager.getWidget("Select PopDlg");
				selectPopDlg.registerCallback( callbackOk,callbackCancel,string );
				window.AppManager.enter( "Select PopDlg" ) ;           	
			}
        };

        self.onKeyLeft = function() {
			if (focusList.getSelectedName() == 'btStart' || focusList.getSelectedName() == 'btClean') {
                focusList.selectNext();
            }       
        };

        self.onKeyRight = function() {
			if (focusList.getSelectedName() == 'btFactoryReset' || focusList.getSelectedName() == 'btClean') {
                focusList.selectPrev();
            }       
        };
        

        self.onKeyLEFT = function() {
		if (checkMenuLock() == true) {
			return ;
		} 
            
		if (select_view) {
			select_view.selectPrev();
			return ;
		}
            
		if (focusList.getSelectedName() == 'btStart' || focusList.getSelectedName() == 'btClean') {
			focusList.selectNext();
			return ;
		}
        	
		window.AppManager.back();
		return;
        };

        self.onKeyRIGHT = function() {
		if (checkMenuLock() == true) {
			return ;
		}      
                  
		if (select_view) {
			select_view.selectNext();
			return ;
		}
            
		if (focusList.getSelectedName() == 'btFactoryReset' || focusList.getSelectedName() == 'btClean') {
			focusList.selectPrev();
			return ;
		}               
        };

        self.onKeyRED = function() {
            if (checkMenuLock() == true) {
                return ;
            }            
            if (focusList.getSelectedName() == 'ipFrequency') {
                ipFrequency_item.removeChar();
            }
            else if (focusList.getSelectedName() == 'ipSymbolRate') {
                ipSymbolRate_item.removeChar();
            }
        };        
		return self ;
	} ;

	window.AppManager.addScene( name, ManualScanC ) ;

})( ) ;
