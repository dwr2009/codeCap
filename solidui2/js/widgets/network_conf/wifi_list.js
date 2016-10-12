"use strict";
(function( ) {

        var wifi_list = new window.Class(

        //parent //focusable widget
        window.Widget,

        //constructor
        function(description) {
                Widget.call(this, "wifi_list" ) ;

                this.$root.addClass( "wifi_list" ) ;
		
		
		/*
		this list will contain all wifi hotspots that are receivable 
		it will refres itself after a sertan amount of time ( c.a 5s )
		when in the refresh hotspots disapear than so will they in the list.
		*/

		this.pos = 0;

		

		
