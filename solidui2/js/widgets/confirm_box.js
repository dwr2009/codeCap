"use strict";
(function( ) {


        var name = "confirm_box" ;


        var confirm = function( ) {
        
		var parent = new window.Scene(name);
		//var parent = new window.Scene( name ) ;
                var self = Object.create( parent ) ;
                

        self.$root.attr( 'id', 'confirm_box' ) ;
        
        //var $confirm_box = $( '<div>').addClass( 'confirm_box').appendTo( self.$root );
        var $headline = $('<div>').addClass('headline_text').appendTo( self.$root );
        var $text = $('<div>').addClass('confirm_box_text').appendTo( self.$root );

        
        
        //flag indicator to see witch button has been pressed
        var OK = false;
        var left_callback = function() {};
        var right_callback = function() {};
        
        
        var right_button_cancel = function()
        {
            // set ok to false ok wasn't pressed;
            OK = false;
            
            // call calback
            right_callback();
            
            // close window and enter Programme Reminder
            window.AppManager.back( );
            
        };
        
        var left_button_ok = function()
        {
        		// set ok to true ok was pressed
            OK = true;
            
            // call calback
            left_callback();
            
            // close window
            window.AppManager.back( );
        };
        
        var desc = {
			name: '$confirm_box_buttons',
			right_button: 'confirm_box_cancel :: right_button',
			left_button: 'confirm_box_ok :: left_button',
			right_callback: right_button_cancel,
			left_callback: left_button_ok,
        };
                
        var buttons = new window.two_buttons(desc);
        buttons.$root.appendTo(self.$root);
        
        buttons.set_top();
        buttons.enter();
        
        //show the buttons
        self.onShow = function( ){
        		buttons.show();	
        };
        
        self.config_confirm_box = function( text, headline, ok_callback, cancel_callback )
        {
            right_callback = cancel_callback;
            left_callback = ok_callback;
            $text.text( text );
            $headline.text( headline );
        };
        
        self.get_flag = function()
        {
            // retunrs the ok flag
            // is true if ok was pressed last
            // and false if cancel was pressed last
            return OK;
        };
        
		self.onKeyLEFT = function() {            
            // move focus up on key up
            buttons.move_up();
        };
        
        self.onKeyRIGHT = function() {
            // move focus down on key down
            buttons.move_down();
        };
        
        self.onKey = function( ) {
            return true ;
        };
        
        self.onKeyOK = function( )
        {
            // when button is activate it calls the
            // callback function of the currently focused item
            buttons.activate();
        };
        
        self.onKeyRED = function() {};
                
        return self;
        

	} ;
	
	
        window.AppManager.addScene( name, confirm ) ;


})( ) ;