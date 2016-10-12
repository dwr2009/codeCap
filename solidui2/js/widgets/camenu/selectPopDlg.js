"use strict";
(function( ) {

	var name = "Select PopDlg" ;

	var selectPopDlg = function( ) 
	{
		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;
		
    self.$root.attr('id','selectPopDlg');    
    var $button_lable = $('<div>').appendTo(self.$root);
    
    var $popMsg = $('<div>').addClass('message').appendTo($button_lable);
    var $selectPopDlg_lable = $('<div>').addClass('selectPopDlg_lable').appendTo($button_lable);
    
    var $confirm_button = $('<div>').addClass('confirm_button').append($TR('selectPopDlg :: yes')).appendTo($selectPopDlg_lable);   
    var $cancel_button = $('<div>').addClass('cancel_button').append($TR('selectPopDlg :: no')).appendTo($selectPopDlg_lable);  
		
		this.callbackOk = null;
		this.callbackCancel = null;

    var listData = [
		    {
		    		name:'confirm_button',
		        $root: $confirm_button,
		        callback: null,
		    },
		    {
		    		name:'cancel_button',
		        $root: $cancel_button,
		        callback: null,
		    },
		];

		var focusList = new window.FocusList(listData);
    self.onKey = function() {
        return true;
    };
    self.onENTER = function(){
    		self.show();	
    };
    self.onShow = function() {
				focusList.select(0);			
    };
    self.onKeyLEFT = function(){
    		focusList.selectPrev();			
    };
    self.onKeyRIGHT = function(){
    		focusList.selectNext();	
    };
    self.onKeyOK = function() {
        if( focusList.getSeletedIndex() == 0 ){
        	this.callbackOk && this.callbackOk ( );
        }
        else if( focusList.getSeletedIndex() == 1){
        	this.callbackCancel && this.callbackCancel ( );
        }
    };

		self.registerCallback = function( callbackOk, callbackCancel, string ){
				this.callbackOk = callbackOk;
				this.callbackCancel = callbackCancel;
				$popMsg.text(TR(string));
		}
		self.HideSelectPopDlg = function(){
				self.hide();	
				console.log("hide select popdlg successful");					
		} 
		
		return self ;
	} ;

	window.AppManager.addScene( name, selectPopDlg ) ;

})( ) ;	