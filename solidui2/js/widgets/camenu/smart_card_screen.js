"use strict";
(function( ){

		var name = "Smart Card";
		

		
	var SmartCards = function(){
			var parent = new window.MainMenuScreen( name );
			var self = Object.create(parent);
			self.$root.attr('id','smartcard');
	
			var $smartcard = $('<div>').appendTo(self.$root);	
			var $smartcard_caption = $('<div>').addClass('caption').append($TR('smartcard_caption :: caption')).appendTo($smartcard);	
			
			var $smartcard_first_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_smartcardsnname = $('<div>').addClass('column_left ').append($TR('smartcard_smartcardsnname :: smartcardsnname')).appendTo($smartcard_first_lable);
			var $smartcard_smartcardsn = $('<div>').addClass('column_right').appendTo($smartcard_first_lable);			

			var $smartcard_third_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_upgradetimename = $('<div>').addClass('column_left ').append($TR('smartcard_upgradetimename :: upgradetimename')).appendTo($smartcard_third_lable);
			var $smartcard_upgradetime = $('<div>').addClass('column_right').appendTo($smartcard_third_lable);	
			
			var $smartcard_forth_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_upgradestatusname = $('<div>').addClass('column_left').append($TR('smartcard_upgradestatusname :: upgradestatusname')).appendTo($smartcard_forth_lable);
			var $smartcard_upgradestatus = $('<div>').addClass('column_right').appendTo($smartcard_forth_lable);	
			
			var $smartcard_fifth_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_ratingname = $('<div>').addClass('column_left').append($TR('smartcard_ratingname :: ratingname')).appendTo($smartcard_fifth_lable);
			var $smartcard_rating = $('<div>').addClass('column_right ').appendTo($smartcard_fifth_lable);	
			
			var $smartcard_sixth_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_worktimename = $('<div>').addClass('column_left').append($TR('smartcard_worktimename :: worktimename')).appendTo($smartcard_sixth_lable);
			var $smartcard_worktime = $('<div>').addClass('column_right').appendTo($smartcard_sixth_lable);	
			
			var $smartcard_seventh_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_pairingstatusname = $('<div>').addClass('column_left').append($TR('smartcard_pairingstatusname :: pairingstatusname')).appendTo($smartcard_seventh_lable);
			var $smartcard_pairingstatus = $('<div>').addClass('column_right').appendTo($smartcard_seventh_lable);	
			
			var $smartcard_eighth_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_operatorname = $('<div>').addClass('column_left').append($TR('smartcard_operatorname :: operatorname')).appendTo($smartcard_eighth_lable);
			var $smartcard_pairingstbidname = $('<div>').addClass('column_left').append($TR('smartcard_pairingstbidname :: pairingstbidname')).appendTo($smartcard_eighth_lable);	
			
			var $smartcard_ninth_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_operator_one = $('<div>').addClass('column_left').appendTo($smartcard_ninth_lable);
			var $smartcard_pairingstbid_one = $('<div>').addClass('column_left').appendTo($smartcard_ninth_lable);	
			
			var $smartcard_tenth_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_operator_two = $('<div>').addClass('column_left ').appendTo($smartcard_tenth_lable);
			var $smartcard_pairingstbid_two = $('<div>').addClass('column_left ').appendTo($smartcard_tenth_lable);
			
			var $smartcard_eleventh_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_operator_three = $('<div>').addClass('column_left').appendTo($smartcard_eleventh_lable);
			var $smartcard_pairingstbid_three = $('<div>').addClass('column_left').appendTo($smartcard_eleventh_lable);	
			
			var $smartcard_twelfth_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_operator_four = $('<div>').addClass('column_left').appendTo($smartcard_twelfth_lable);
			var $smartcard_pairingstbid_four = $('<div>').addClass('column_left').appendTo($smartcard_twelfth_lable);	
			
			var $smartcard_thirteenth_lable = $('<div>').addClass('lable').appendTo($smartcard);
			var $smartcard_operator_five = $('<div>').addClass('column_left').appendTo($smartcard_thirteenth_lable);
			var $smartcard_pairingstbid_five = $('<div>').addClass('column_left').appendTo($smartcard_thirteenth_lable);		
			
			var handleSmcEvent = function(param){
		 			if(param.smcStatus == 0){
		 					self.onKeyBACK();
		 			}
		 	};
		 	
			var updateScreen = function(){
					
					var idx;
					var upgradeTime = new Date();
					
					var stbID = window.Platform.ca.GetSTBID();
					var smcSN = window.Platform.ca.GetCardSN();
					var smcST = window.Platform.ca.GetCardUpgradeStatus();
					var rating = window.Platform.ca.GetRating();
					var wt = window.Platform.ca.GetWorkTime();
					var paired = window.Platform.ca.IsPaired();
					var operator = window.Platform.ca.GetOperatorIds();
					
					upgradeTime.setTime(smcST.upgradeTime*1000); 
								
					$smartcard_smartcardsn.text(smcSN.retVal?"":smcSN.cardSN);
					$smartcard_upgradetime.text(smcST.upgradeTime?(window.Format.formatDate(upgradeTime) + " " + window.Format.formatTime(upgradeTime,'addsecond')):"");
					$smartcard_upgradestatus.text(smcST.upgradeTime?TR(smcST.upgradeStatus?'smartcard :: Successful':'smartcard :: Failed'):"");   
					$smartcard_rating.text(rating.retVal?"":rating.rating);
					$smartcard_worktime.text(wt.retVal?"":window.Format.formatWorkTime(wt));
					
					var stbList = [
							$smartcard_pairingstbid_one,
							$smartcard_pairingstbid_two,
							$smartcard_pairingstbid_three,
							$smartcard_pairingstbid_four,
							$smartcard_pairingstbid_five
					];
					if (paired.retVal == 0 || paired.retVal == 0x07){
							$smartcard_pairingstatus.text(TR('smartcard :: The card is not paired with this STB'));
							for (idx=0; idx<paired.stbidList.length; idx++){
									if (paired.stbidList[idx] == stbID.stbID){
											$smartcard_pairingstatus.text(TR('smartcard :: The card is paired with this STB'));
									}
									stbList[idx].text(paired.stbidList[idx]);
							}
							for (; idx<5; idx++){
									stbList[idx].text("");
							} 
 					}
 					else{
 							$smartcard_pairingstatus.text("");
 							for (idx=0; idx<5; idx++){
 									stbList[idx].text("");
 							}
 					}
 					
 					var operatorList = [
 							$smartcard_operator_one,
 							$smartcard_operator_two,
 							$smartcard_operator_three,
 							$smartcard_operator_four
 					];
 					if (operator.retVal == 0){
 							for (idx=0; idx<operator.operatorList.length && operator.operatorList[idx]!=0; idx++){
 									var opid = {operatorID:operator.operatorList[idx]};
									var opinfo = window.Platform.ca.GetOperatorInfo(opid);
									operatorList[idx].text(opinfo.retVal?operator.operatorList[idx]:(operator.operatorList[idx]+"    "+opinfo.operatorInfo));
							}
							for (; idx<4; idx++){
									operatorList[idx].text("");
							}
 					}
 					else{
 							for (idx=0; idx<4; idx++){
 									operatorList[idx].text("");
 							}
 					}			
 					window.Platform.ca.setNotifySmcStatusCallback(handleSmcEvent);			
			};
			
		self.onKey = function( ) {
			return true ;
		} ;
		
		self.onKeyBACK = 
    self.onKeyLEFT = function() {
    		window.Platform.ca.setNotifySmcStatusCallback(function(st){});
        window.AppManager.back();
    }
        
		self.onShow = function(){
			updateScreen();
		};
				
		return self ;
	} ;
		
			
			window.AppManager.addScene( name,SmartCards ) ;
})();