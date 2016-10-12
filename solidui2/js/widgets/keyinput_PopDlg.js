"use strict"; 
(function() {

    var name = "KeyInput PopDlg";

    var  KeyInputPopDlg= function() {

        var parent = new window.Scene(name);
        var self = Object.create(parent);
        self.$root.attr('id', 'keyinput');
				
				var $lcn_display = $('<div>').addClass('lcn_display').appendTo(self.$root);
				var $lcn_display_left = $('<div>').addClass('lcn_display_left').appendTo($lcn_display);
				var $lcn_display_right = $('<div>').addClass('lcn_display_right').appendTo($lcn_display);
 
        var inputDescription = {
            name: 'parentalrating_passwordname :: passwordname',
            size: 6,
            type: 'password',
        };
        var $keyinput_first_lable = $('<div>').addClass('keyinputlable').appendTo(self.$root);
        var input = new window.InputView(inputDescription);
        input.$root.appendTo($keyinput_first_lable);
        input.show();

        var $keyinput_second_lable = $('<div>').addClass('messagelable').appendTo(self.$root);
        var $keyinput_message = $('<div>').addClass('message').appendTo(self.$root);
        
        var $keyinput_forth_lable = $('<div>').addClass('reminderlable').appendTo(self.$root);
        var $red_reminder = $('<div>').addClass('red_reminder').appendTo($keyinput_forth_lable);
        var $red_reminder_text = $('<div>').addClass('red_reminder_text').append($TR('pincode_redwarning :: backspace')).appendTo($keyinput_forth_lable);
        var $exit_reminder = $('<div>').addClass('exit_reminder').appendTo($keyinput_forth_lable);
        var $exit_reminder_text = $('<div>').addClass('exit_reminder_text').append($TR('pincode_redwarning :: back')).appendTo($keyinput_forth_lable);

		var CallbackOK = null;
		var callbackCancel = null;
		var CallbackSaveCurrentChannel = null;
		var flag = 0;
		
        var listData = [{
            name: 'keyinput',
            $root: $keyinput_first_lable,
            callback: _.bind(input.activate, input),
        }];
        var focusList = new window.FocusList(listData);
       
        var updateScreen = function() {
            focusList.select(0);
            input.clear();
            $keyinput_message.text("");
        };
        
				self.onKey = function() {
            return true;
        };
        
        self.onShow = function() {
            updateScreen();
			CallbackSaveCurrentChannel && CallbackSaveCurrentChannel();
        };

        self.onKeyNUMBER = function(num) {
					input.appendChar(num.toString());
					//it will wait the password full display
					setTimeout(function( ) {
						if(input.length() > 5){
							var pin = {
								pinCode: input.getText(),
							};      	
							var  result = window.Platform.ca.VerifyPin(pin);
							if(result.retVal == 0){
								window.AppManager.back();
								CallbackOK && CallbackOK ( );
							}
							else{
								$keyinput_message.text(TR('Pincode :: Invalid PIN'));
								setTimeout(function( ) { $keyinput_message.text(""); },2000);
								input.clear();
							}
						}				
					}, 20);
        };
        
				self.onKeyCHANUP = 
        self.onKeyUP = function() {
        	if(flag == 2 || flag == 3 || flag == 0){
        		return ;
        	}
        	else {
        		while( window.AppManager.current( ).name !== "Live TV" ) {
					    window.AppManager.back( ) ;
					  }
	        	var live = window.AppManager.getWidget("Live TV");
						live.onKeyCHANUP();
        	}
        };
				
				self.onKeyCHANDOWN = 
        self.onKeyDOWN = function() {
        	if(flag == 2 || flag == 3 || flag == 0){
        		return ;
        	}
        	else{
        		while( window.AppManager.current( ).name !== "Live TV" ) {
					    window.AppManager.back( ) ;
					  }
	        	var live = window.AppManager.getWidget("Live TV");
						live.onKeyCHANDOWN();
        	}
        };
        
        self.onKeyRED = function() {
           input.removeChar();
        };
        self.registerCallback = function(callback){
        	CallbackOK = callback.callbackOK;
        	callbackCancel = callback.callbackCancel;
        	CallbackSaveCurrentChannel = callback.saveCurrentChannel;
        	flag = callback.flag;
        	if( flag == 2){
        		$lcn_display_left.text("");
        	}
        	else{
        		$lcn_display_left.text(TR('pincode_lcn :: CurrentChannel'));
        	}
        	$lcn_display_right.text( callback.keyinputLcn );
        	
        };
        
        self.onKeyBACK = function(){
        	if(flag == 2){
        		while( window.AppManager.current( ).name !== "Main Menu" ) {
					    window.AppManager.back( ) ;
					  }
        	}
        	else if(flag == 1 || flag == 3 || flag == 0){
        		callbackCancel && callbackCancel ( );	
						window.AppManager.back( ) ;
        	}
        	else{
        		window.AppManager.back( ) ;
        	}
        };
        
        self.onKeyEXIT = function(){
        	if(flag == 2){
        		while( window.AppManager.current( ).name !== "Live TV" ) {
					    window.AppManager.back( ) ;
					  }
        	}
        	else if(flag == 1 || flag == 3 || flag == 0){
        		callbackCancel && callbackCancel ( );	
        		
        		while( window.AppManager.current( ).name !== "Live TV" ) {
					    window.AppManager.back( ) ;
					  }
        	}
        	else{
        		window.AppManager.back( ) ;
        	}
        };
        self.onKeyLEFT = function(){
        	if(flag == 2){
        		while( window.AppManager.current( ).name !== "Main Menu" ) {
					    window.AppManager.back( ) ;
					  }
        	}
					else{
						return ;
					}
				};
        return self;
    };

    window.AppManager.addScene(name, KeyInputPopDlg);
})();