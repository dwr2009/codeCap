"use strict";
(function( ) {

	var name = "confirm PopDlg" ;

	var confirmPopDlg = function( ) 
	{
		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;
		
    self.$root.attr('id','confirmPopDlg');
    
    var $popMsg = $('<div>').addClass('message').appendTo(self.$root);
    
    var $button_lable = $('<div>').addClass('button_lable').appendTo(self.$root);
    var $confirm_button = $('<div>').addClass('confirm_button').append("OK").appendTo($button_lable);
    
    var flag = 0;
		this.callbackOk = null;  
		


    self.onKey = function() {
        return true;
    };
    
		self.ENTER = function(){
				self.show();
		};
		self.HideDlg = function(){
				self.hide();
		};
    self.onShow = function() {
    		$confirm_button.addClass('selected');		
    };

    self.onKeyOK = function() {
    	if($confirm_button.hasClass('selected')){
    			//self.hide();
	       this.callbackOk && this.callbackOk ( );								
    	}
    };
    
    self.registerCallback = function(callback,string){
    		console.log("regestercallback enter the confirmPopDlg");
    		this.callbackOk = callback;	
    		$popMsg.text(TR(string));
    };
		self.showMsg = function(string){
				$popMsg.text(TR(string));
		};


		return self ;
	} ;

	window.AppManager.addScene( name, confirmPopDlg ) ;

})( ) ;	