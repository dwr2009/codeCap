"use strict";
(function( ){

		var name = "Statement";
		

		
	var Statements = function(){
			var parent = new window.MainMenuScreen( name );
			var self = Object.create(parent);
			self.$root.attr('id','statement');
			
			var $statement_table = $('<div>').addClass('statement_table').appendTo(self.$root);
								
			var $statement_first_lable = $('<div>').addClass('first_lable').appendTo($statement_table);
			var $statement_latesdbill = $('<div>').append($TR('statement_latestbill :: latestbill')).appendTo($statement_first_lable);
			
			var $statement_second_lable = $('<div>').addClass('lable').appendTo($statement_table);
			var $statement_accountnum = $('<div>').addClass('column_left ').append($TR('statement_accountnum :: accountnum')).appendTo($statement_second_lable);
			var $statement_emitdate = $('<div>').addClass('column_right').append($TR('statement_emitdate :: emitdate')).appendTo($statement_second_lable);

			var $statement_third_lable = $('<div>').addClass('lable ').appendTo($statement_table);
			var $statement_accountdetails = $('<div>').addClass('column_three_left').append($TR('statement_accountdetails :: accountdetails')).appendTo($statement_third_lable);
			var $statement_date = $('<div>').addClass('column_three ').append($TR('statement_date :: date')).appendTo($statement_third_lable);
			var $statement_bill = $('<div>').addClass('column_three_right').append($TR('statement_bill :: bill')).appendTo($statement_third_lable);
			
			var $statement_forth_lable = $('<div>').addClass('lable').appendTo($statement_table);
			var $statement_previousamountduename = $('<div>').addClass('column_left').append($TR('statement_previousamountduename :: previousamountduename')).appendTo($statement_forth_lable);
			var $statement_previousamountdue = $('<div>').addClass('column_right').appendTo($statement_forth_lable);
			
			var $statement_fifth_lable = $('<div>').addClass('lable').appendTo($statement_table);
			var $statement_paymentrecievedname = $('<div>').addClass('column_left').append($TR('statement_paymentrecievedname :: paymentrecievedname')).appendTo($statement_fifth_lable);
			var $statement_paymentrecieved = $('<div>').addClass('column_right').appendTo($statement_fifth_lable);

			var $statement_sixth_lable = $('<div>').addClass('lable').appendTo($statement_table);
			var $statement_previousbalancename = $('<div>').addClass('column_left ').append($TR('statement_previousblancename :: previousblancename')).appendTo($statement_sixth_lable);
			var $statement_previousblance = $('<div>').addClass('column_right').appendTo($statement_sixth_lable);

			var $statement_seventh_lable = $('<div>').addClass('lable').appendTo($statement_table);
			
			var $statement_eighth_lable = $('<div>').addClass('lable').appendTo($statement_table);
			var $statement_currentservicechargesname = $('<div>').addClass('column_left').append($TR('statement_currentservicechargesname :: currentservicechargesname')).appendTo($statement_eighth_lable);
			var $statement_currentservicecharges = $('<div>').addClass('column_right ').appendTo($statement_eighth_lable);

			var $statement_ninth_lable = $('<div>').addClass('lable').appendTo($statement_table);
			var $statement_otherchargesname = $('<div>').addClass('column_left').append($TR('statement_otherchargesname :: otherchargesname')).appendTo($statement_ninth_lable);
			var $statement_othercharges = $('<div>').addClass('column_right').appendTo($statement_ninth_lable);

			var $statement_tenth_lable = $('<div>').addClass('lable').appendTo($statement_table);
			var $statement_chargeadjustmentname = $('<div>').addClass('column_left').append($TR('statement_chargeadjustmentname :: chargeadjustmentname')).appendTo($statement_tenth_lable);
			var $statement_chargeadjustment = $('<div>').addClass('column_right').appendTo($statement_tenth_lable);			

			var $statement_eleventh_lable = $('<div>').addClass('lable').appendTo($statement_table);
			var $statement_valueaddedservicename = $('<div>').addClass('column_left').append($TR('statement_valueaddedservicename :: valueaddedservicename')).appendTo($statement_eleventh_lable);
			var $statement_valueaddedservice = $('<div>').addClass('column_right').appendTo($statement_eleventh_lable);
			
			var $statement_twelveth_lable = $('<div>').addClass('lable').appendTo($statement_table);

			var $statement_thirteenth_lable = $('<div>').addClass('lable').appendTo($statement_table);
			var $statement_totalamountduename = $('<div>').addClass('column_three_left').append($TR('statement_totalamountduename :: totalamountduename')).appendTo($statement_thirteenth_lable);
			var $statement_paymentduedate = $('<div>').addClass('column_three ').append($TR('statement_paymentduedatename :: paymentduedatename')).appendTo($statement_thirteenth_lable);
			var $statement_totalamountdue = $('<div>').addClass('column_three_right').appendTo($statement_thirteenth_lable);								
					
			var $statement_reminderinfo2 = $('<div>').addClass('lastlable').append($TR('statement_reminderinfo2 :: reminderinfo2')).appendTo($statement_table);	

			
			var updateScreen = function( ){
				var currentlang = window.Platform.settings.getLanguage();
				var lang = {
					language:currentlang.toUpperCase(),
				}
				var statementList = Platform.ca.GetStatement(lang);
				if(statementList.retVal == 0){
					updateText(statementList);
				}			
			};
			
			var outputMoney = function(numString) {
			    var number = numString.replace(/\,/g, "");
			    var num = number.substr(0,(numString.length-2));
			    if (num == "") return "";
			    if (num < 0) {
			        return '-' + outputDollars(Math.floor(Math.abs(num) - 0) + '') + outputCents(Math.abs(num) - 0);
			    } else {
			        return outputDollars(Math.floor(num - 0) + '') + outputCents(num - 0) +" "+ outputCR(numString);
			    }
			}
			
			var outputCR = function(number){
				var crString = number.substr(-2,2);
				console.log("crString = "+ crString);
				return crString;
			}
			var outputDollars = function(number) {
			    if (number.length <= 3) {
			        return (number == '' ? '0' : number);
			    } else {
			        var mod = number.length % 3;
			        var output = (mod == 0 ? '' : (number.substring(0, mod)));
			        for (var i = 0; i < Math.floor(number.length / 3); i++) {
			            if ((mod == 0) && (i == 0)) {
			                output += number.substring(mod + 3 * i, mod + 3 * i + 3);
			            } else {
			                output += ',' + number.substring(mod + 3 * i, mod + 3 * i + 3);
			            }
			        }
			        return (output);
			    }
			}
			
			var outputCents = function(amount) {
			    amount = Math.round(((amount) - Math.floor(amount)) * 100);
			    return (amount < 10 ? '.0' + amount : '.' + amount);
			}
										
			var updateText = function(statementlist){
				$statement_emitdate.text(TR('statement_emitdate :: emitdate') + window.Format.formatDateString(statementlist.statementDate));
				$statement_paymentduedate.text(TR('statement_paymentduedatename :: paymentduedatename') + window.Format.formatDueDateString(statementlist.paymentDueDate));   
				$statement_accountnum.text(TR('statement_accountnum :: accountnum') +statementlist.accountNo);
			 	$statement_previousamountdue.text(outputMoney(statementlist.previousAmountDue)); 
				$statement_paymentrecieved.text(outputMoney(statementlist.paymentReceived)); 
				$statement_previousblance.text(outputMoney(statementlist.previousBalance)); 
				$statement_currentservicecharges.text(outputMoney(statementlist.currentServiceCharges));
				$statement_othercharges.text(outputMoney(statementlist.otherCharges));  
				$statement_chargeadjustment.text(outputMoney(statementlist.chargeAdjustment));
				$statement_valueaddedservice.text(outputMoney(statementlist.valueAddedServiceCharge)); 
				$statement_totalamountdue.text(outputMoney(statementlist.totalAmountDue));
			}	;
			
			
		self.onKey = function( ) {
			return true ;
		} ;
		
			self.onShow = function(){
				
				updateScreen();
			}
				
		return self ;
	} ;
		
			
			window.AppManager.addScene( name,Statements ) ;
})();