"use strict";
(function( ) {

	var name = "ippv Bar" ;

	var IppvBar = function( ) 
	{
		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;
		
    self.$root.attr('id','ippv_bar');

				var switchIPPVPrice = function(item) {
            console.log("Selected :operator", item.name);
            setPriceIndex(item.value);            
          }
        var lableProductid = {
            name: 'IPPV_productidname :: productidname',
            value: ''
        };
        var lableOperatorid = {
            name: 'IPPV_operatoridname :: operatoridname',
            value: ''
        };
       	var lableExperidtime = {
            name: ' ',
            value: ' '
        };
        var lableSlotid = {
            name: 'IPPV_slotidname :: slotidname',
            value: ''
        };
        
        var $IPPV_Control = $('<div>').appendTo(self.$root);
        var $IPPV_Dlg = $('<div>').appendTo($IPPV_Control);
				var $IPPV_caption = $('<div>').addClass('caption').appendTo($IPPV_Dlg);
        var $IPPVScene = $('<div>').addClass('ippvscene').appendTo($IPPV_Dlg);
                
        var $IPPV_first_lable = $('<div>').addClass('IPPV_lable').appendTo($IPPVScene);
        var IPPV_productid = new window.LabelView(lableProductid);
        IPPV_productid.$root.appendTo($IPPV_first_lable);
        IPPV_productid.show();
        
        var $IPPV_second_lable = $('<div>').addClass('IPPV_lable').appendTo($IPPVScene);
        var IPPV_operatorid = new window.LabelView(lableOperatorid);
        IPPV_operatorid.$root.appendTo($IPPV_second_lable);
        IPPV_operatorid.show();        

        var $IPPV_third_lable = $('<div>').addClass('IPPV_lable').appendTo($IPPVScene);
        var IPPV_experidtime = new window.LabelView(lableExperidtime);
        IPPV_experidtime.$root.appendTo($IPPV_third_lable);
        IPPV_experidtime.show();          

        var $IPPV_forth_lable = $('<div>').addClass('IPPV_lable').appendTo($IPPVScene);
        var IPPV_slotid = new window.LabelView(lableSlotid);
        IPPV_slotid.$root.appendTo($IPPV_forth_lable);
        IPPV_slotid.show();
        
				var selectDescription = {
            name: "IPPV_pricename :: pricename",
            items: [],
            action: switchIPPVPrice,
        };
        var $IPPV_fifth_lable = $('<div>').addClass('IPPV_lable').appendTo($IPPVScene);
        var IPPV_price = new window.SelectView(selectDescription, selectDescription.action);
        IPPV_price.$root.appendTo($IPPV_fifth_lable);
        IPPV_price.show();

        var inputDescription = {
            name: 'parentalrating_passwordname :: passwordname',
            size: 6,
            type: 'password',
        };
        var $IPPV_six_lable = $('<div>').addClass('IPPV_lable').appendTo($IPPVScene);
        var input = new window.InputView(inputDescription);
        input.$root.appendTo($IPPV_six_lable);
        input.show();
    
        var $IPPV_message_lable = $('<div>').addClass('messagelable').appendTo($IPPVScene);
        var $IPPV_message = $('<div>').addClass('message').appendTo($IPPV_message_lable);

        var $IPPV_last_lable = $('<div>').appendTo($IPPVScene);
        var $IPPV_left_area = $('<div>').addClass('column_left').appendTo($IPPV_last_lable);
        var $IPPV_right_area = $('<div>').addClass('column_right').appendTo($IPPV_last_lable);
        var $IPPV_confirm_panel = $('<div>').addClass('panel').append($TR('pincode_btn_confirm :: confirm')).appendTo($IPPV_left_area);
        var $IPPV_cancel_panel = $('<div>').addClass('panel').append($TR('IPPV_btn_cancel :: cancel')).appendTo($IPPV_right_area);
     		

    var listData = [
        {
        		name:'price',
            $root: $IPPV_fifth_lable,
            callback: null,
        },
        {
        		name:'input',
        		$root:$IPPV_six_lable,
        		callback:null,
        },
        {
        		name:'btn_buy',
            $root: $IPPV_confirm_panel,
            callback: null,
        },
        {
        		name:'btn_cancel',
            $root: $IPPV_cancel_panel,
            callback:null,            	
        }
    ];
    var focusList = new window.FocusList(listData);
    
		var buySuccessfulDlg = new window.popDlg();
		buySuccessfulDlg.$root.addClass('buySuccessfulDlg').appendTo($IPPV_Control);
		buySuccessfulDlg.hide();
		    
    var ippvInfo;
    var priceindex = 0;
    var messageFlag = 0;
    var setPriceIndex = function(index){
    		priceindex = index;	
    }
    var getPriceIndex = function(){
    		return 	priceindex;
    }
    
		var BuyIPPV = function(buyFlag) {
				var idx = getPriceIndex();
				var buyIppv = {};
				buyIppv.buyProgram = buyFlag;
				buyIppv.ecmPID = ippvInfo.ecmPID;
				buyIppv.price = ippvInfo.priceList[idx].price;
				buyIppv.priceCode = ippvInfo.priceList[idx].priceCode;
				buyIppv.pinCode = input.getText();
				var return_value = Platform.ca.StopIPPVBuyDlg(buyIppv);
				switch (return_value.retVal){
						case 0:
								if (buyFlag == 1) {
										$IPPV_Dlg.hide();
										buySuccessfulDlg.show();	
										buySuccessfulDlg.showDlgMsg(TR('IPPV_IPPV product purchased successfully'));
										setTimeout(function(){
											show_flag = 0;
											buySuccessfulDlg.hide();
											window.AppManager.back();
										},3000);
								}
								else {
										show_flag = 0;
										self.hide();
        						window.AppManager.back();
								}
								break;
						case 3:
								messageFlag = 1;
								$IPPV_message.text(TR('IPPV_Unidentified Card!'));
								break;
						case 4:
								messageFlag = 1;
								$IPPV_message.text(TR('IPPV_Invalid PIN!'));
								break;
						case 10:
								messageFlag = 1;
								$IPPV_message.text(TR('IPPV_No room for new IPPV!'));
								break;
						case 8:
								messageFlag = 1;
								$IPPV_message.text(TR('IPPV_Operator ID error!'));
								break;
						case 9:
								messageFlag = 1;
								$IPPV_message.text(TR('IPPV_Program status invalid!'));
								break;
						default:
								messageFlag = 1;
								$IPPV_message.text(TR('IPPV_Unknown error!'));
								break;
				}
		}
		var updateScreen = function(){
				messageFlag = 0;
				self.show();
				$IPPV_Dlg.show();
				buySuccessfulDlg.hide();
				$IPPV_message.text(" ");
				input.clear();	
		}
		var updateIPPTWarning = function() {
				if (ippvInfo.messageType == 2) {
						$IPPV_message.text(TR('IPPV_ipptwarning :: auto deduction')+ " (" + ippvInfo.priceList[priceindex].price + "/" + ippvInfo.ippvTime + " " + TR('duration :: min') + ")" );
				}
		}

    var select_view = 0;
    self.onKey = function() {
        return true;
    };

    self.onShow = function() {
				focusList.select(0);			
    };
    self.onKeyENTER = function(){
    		self.show();
    };

    self.onKeyUP = function() {
        if (select_view) {
            return;
        }
        if(messageFlag == 1) {
        		if (ippvInfo.messageType == 2) {
        				updateIPPTWarning();
        		}
        		else {
								$IPPV_message.text(" ");
						}
						input.clear();
						messageFlag = 0;		
        }
				focusList.selectPrev();
    };

    self.onKeyDOWN = function() {
        if (select_view) {
            return;
        }
        if(messageFlag == 1) {
						if (ippvInfo.messageType == 2) {
        				updateIPPTWarning();
        		}
        		else {
								$IPPV_message.text(" ");
						}
						input.clear();
						messageFlag = 0;		
        }
				focusList.selectNext();
    };
    self.onKeyNUMBER = function(num) {
    		if (focusList.getSelectedName() == 'input'){
        		input.appendChar(num.toString());
      	}
    };
    self.onKeyRED = function() {
       input.removeChar();
    };
    self.onKeyCHANDOWN = function(){
				var handleChannel = window.AppManager.getWidget("Live TV");
				handleChannel.onKeyCHANDOWN();    
    };
    self.onKeyCHANUP = function(){    	
				var handleChannel = window.AppManager.getWidget("Live TV");
				handleChannel.onKeyCHANUP();      
    };
    self.onKeyOK = function() {
    	  var focusListname = focusList.getSelectedName();
            switch (focusListname){
            case 'price':
                {
                    if (!select_view) {
                        select_view = IPPV_price;
                        select_view.activate(true);
                    } else {
                        select_view.activate(false);
                        select_view = null;
                    }
                    break;
                }
            case 'input':{
            			break;
                	}
            case 'btn_buy':
                {
                    BuyIPPV(1);
                    break;
                }
            case 'btn_cancel':
                {
                		show_flag = 0;
                		BuyIPPV(0);
										self.hide();
                    window.AppManager.back();
                    break;
                }
              }
    };

    self.onKeyLEFT = function() {
            if (!select_view) {
                return;
            }
            console.log("select_view" + select_view);
            select_view.selectPrev();
            updateIPPTWarning();
    }
    self.onKeyEXIT =
    self.onKeyBACK = function(){
					return;	
			}
    self.onKeyRIGHT = function() {
            if (!select_view) {
                return;
            }
            console.log("select_view" + select_view);
            select_view.selectNext();
            updateIPPTWarning();
    }
    var show_flag = 0;
    var display_flag = 1;
    self.StartIppvBuyDlg = function(param){
    		console.log("StartIppvBuyDlg is function");
    		if(show_flag == 0)	{
    				if (display_flag) {
    						window.AppManager.enter( "ippv Bar" ) ;
    				}
						updateScreen();
				}
				ippvInfo = param;							
				var q = new Array(); 			
				for (var i=0; i<param.priceList.length; i++) {		            						
						var item = {
							name:param.priceList[i].price + " (" + ((param.priceList[i].priceCode) ? TR('IPPV_recordingenablename :: recordingenablename') : TR('IPPV_recordingenablename :: recordingdisablename'))+")",
							value:i,
						}
						q.push(item);											
				}
				IPPV_price.updateItems(q);
								
				if (param.messageType == 2) {				
						$IPPV_caption.text(TR('IPPV_caption :: ippt'));
						IPPV_experidtime.setName(TR('IPPV_experidtimename :: interval'));
						IPPV_experidtime.setText(param.ippvTime + " " + TR('duration :: min'));	
						if (show_flag == 0){
								updateIPPTWarning();
						}												
				}
				else {
						var dt = new Date();
						dt.setTime(Date.parse("2000/1/1") + param.ippvTime*24*3600*1000);
						$IPPV_caption.text(TR('IPPV_caption :: ippv'));
						IPPV_experidtime.setName(TR('IPPV_experidtimename :: experidtimename'));
						var date_string = window.Format.formatDate(dt)+ " " + window.Format.formatTime(dt); 
						IPPV_experidtime.setText(date_string);						
				}
				IPPV_productid.setText(param.productID);
				IPPV_operatorid.setText(param.operatorID);
				IPPV_slotid.setText(param.slotID);
				show_flag = 1;											
		}
		self.HideIPPVDlg = function(param){
				show_flag = 0;
				self.hide();
        window.AppManager.back();
		}
		self.ippvControl = function(flag){
				if (flag == 0){
						$IPPV_Control.hide();
						if (show_flag){
								window.AppManager.back();
						}
				}
				else {
						$IPPV_Control.show();
						if (show_flag) {
								window.AppManager.enter( "ippv Bar" ) ;
						}
				}
				display_flag = flag;
		}		
		return self ;
	} ;

	window.AppManager.addScene( name, IppvBar ) ;

})( ) ;				
		


