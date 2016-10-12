"use strict";
(function( ) {


        var name = "alert_box" ;


        var alert = function( ) {
        
		var parent = new window.Scene(name);
		//var parent = new window.Scene( name ) ;
                var self = Object.create( parent ) ;
                

        self.$root.attr( 'id', 'alert_box' ) ;
        
        var $alert_box = $( '<div>').addClass( 'alert_box').appendTo( self.$root );
        var $text = $('<div>').addClass('label_text').appendTo( $alert_box );
        var $button = $('<div>').addClass('panel').append( $TR('alert box screen :: Ok') ).appendTo( $alert_box );

        $button.toggleClass("selected", 1);
        
        
        self.text = function( text )
        {
            $text.text( text );
        };
        
        self.onKey = function( ) {
            return true ;
        } ;
        
        self.onKeyOK = function( )
        {
            window.AppManager.back( );
        };
                
        return self;
        

	} ;
	
	
        window.AppManager.addScene( name, alert ) ;


})( ) ;
