"use strict";
(function( ) {

	var reminder_row = new window.Class(

        //parent //focusable widget
        //window.Widget,
        window.ListViewDelegate,

        //constructor
        function(description) {
                //Widget.call(this, description.name ) ;
                ListViewDelegate.call( this, description.name ) ;

                this.$root.addClass( description.name ) ;
		
		/*	
		var description = 
		{
			name: 'sample_name',
			callback_function: callback_when_activated,
			date_time: yyyy/MM/dd-HH:mm:ss,
			channel: 'arte',
			reminder_name: 'natur doku',
			duration: '120 min',
			frequency: 'weekly',
		}
		*/
		
		/*
		console.log( "reminder_row name: ", description.name ) ;
		console.log( "reminder_row date_time: ", description.date_time ) ;
		console.log( "reminder_row channel: ", description.channel ) ;
		console.log( "reminder_row reminder_name: ", description.reminder_name ) ;
		console.log( "reminder_row duration: ", description.duration ) ;
		console.log( "reminder_row frequency: ", description.frequency ) ;
		*/
		
		this.callback = description.callback_function;
		this.name = description.name;
		this.reminder_name = description.reminder_name;
		this.handel = description.handel;
		
		//var $table = $( '<div>').addClass( 'reminder_table' ).appendTo(this.$root);
		
		var $table_row = $( '<div>').addClass( 'reminder_table_row' ).appendTo(this.$root);
		
		var $date = $('<div>').addClass('reminder_date').append(description.date_time).appendTo($table_row);
		var $channel = $('<div>').addClass('reminder_channel').append(description.channel).appendTo($table_row);
		var $reminder_name = $('<div>').addClass('reminder_name').append(description.reminder_name).appendTo($table_row);
		var $duration = $('<div>').addClass('reminder_duration').append(description.duration).appendTo($table_row);
		//var $frequency = $('<div>').addClass('reminder_frequency').append($TR( description.frequency )).appendTo($table_row);


	},



	{
		activate_on_focus: false,
		
		activate: function( bool )
		{
			if ( bool == true ) {
                
				this.callback( this.handel, this.reminder_name );
			}
		},
		
	}
);

window.reminder_row = reminder_row;

})( ) ;
