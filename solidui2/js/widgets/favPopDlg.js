
(function( ) {

	var name = "Fav PopDlg" ;

	var favPopDlg = function( ) 
	{
		var parent = new window.Scene( name ) ;
		var self = Object.create( parent ) ;
		
    self.$root.attr('id','favPopDlg');
    var $favPopDlg_screen = $('<div>').addClass('favPopDlg_screen').appendTo(self.$root);
    var $favPopDlg = $('<div>').addClass('fav_PopDlg').appendTo(self.$root);
    
    var $title = $('<div>').addClass('title').append($TR('favPopDlg :: title')).appendTo($favPopDlg);
    
    var inputDescription = {
        name: 'worktime_passwordname :: passwordname',
        size: 25,
        type: 'text',
    };
    var $second_row = $('<div>').addClass('second_row').appendTo($favPopDlg);
    var $fav_name = $('<div>').addClass('fav_name').append($TR('favPopDlg :: fav_name')).appendTo($second_row);   
    var $fav_name_text = $('<div>').addClass('fav_name_text').appendTo($second_row); 
    var input = new window.InputItemVK(inputDescription);
		input.$root.appendTo($fav_name_text);
		input.show(); 
    
    var $third_row = $('<div>').addClass('third_row').appendTo($favPopDlg);
    var $confirm_button = $('<div>').addClass('confirm_button').append($TR('favPopDlg :: ok')).appendTo($third_row);   
    var $cancel_button = $('<div>').addClass('cancel_button').append($TR('favPopDlg :: cancel')).appendTo($third_row);  
		
		this.callbackOk = null;
		this.callbackCancel = null;

    var listData = [
    		{
    			name:'input',
    			$root:$fav_name_text,
    			callback:null,
    		},
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
		var otaParam = null;
		var focusList = new window.FocusList(listData);
    self.onKey = function() {
        return true;
    };
    self.onShow = function() {
    		focusList.select(0);
    		input.clear();
    		input.setFocus(true);
    };
    self.onKeyUP = function(){
    	if(focusList.getSeletedIndex() == 2 || focusList.getSeletedIndex() == 1){
    		input.setFocus(true);
    		focusList.select(0);
    	}
    	else if(focusList.getSeletedIndex() == 0 ){
    		input.setFocus(false);
    		focusList.select(1);
    	}
    		//focusList.selectPrev();			
    };
    self.onKeyDOWN = function(){
    	if(focusList.getSeletedIndex() == 0 ){
    		input.setFocus(false);
    		focusList.select(1);
    	}
    	else if(focusList.getSeletedIndex() == 2 || focusList.getSeletedIndex() == 1){
    		input.setFocus(true);
    		focusList.select(0);
    	}
    		//focusList.selectNext();	
    };
    
    self.onKeyLEFT = function(){
    	if( focusList.getSeletedIndex() == 2 ){
        	focusList.selectPrev();
      }
      else if(focusList.getSeletedIndex() == 1){
      	focusList.selectNext();	
      }
    };
    
    self.onKeyRIGHT = function(){
    	if( focusList.getSeletedIndex() == 2 ){
        	focusList.selectPrev();
      }
      else if(focusList.getSeletedIndex() == 1){
      	focusList.selectNext();	
      }
    };
    
    self.onKeyOK = function() {
    		if(focusList.getSeletedIndex() == 0){
    			input.setFocus(true);
    		}
        else if( focusList.getSeletedIndex() == 1 ){
        	input.setFocus(false);
        	var str = input.getText();
        	this.callbackOk && this.callbackOk ( str );
        }
        else if( focusList.getSeletedIndex() == 2){
        	input.setFocus(false);
        	this.callbackCancel && this.callbackCancel ( );
        }
    };
		self.registerCallback = function( callbackOk, callbackCancel){
			this.callbackOk = callbackOk;
			this.callbackCancel = callbackCancel;
		} ;
		self.onKeyNUMBER = function(num) {
			if(focusList.getSeletedIndex() == 0){
				input.appendChar(num.toString());
			}
    };
    self.onKeyRED = function(){
    	input.removeChar();
    };
    
    self.onKeyBACK = function( ){
    	input.setFocus(false);
    	window.AppManager.back() ;
    };
    self.onKeyEXIT = function( ){
			input.setFocus(false);
    	window.App.goToHome( ) ;
    };
		
		return self ;
	} ;

	window.AppManager.addScene( name, favPopDlg ) ;

})( ) ;	