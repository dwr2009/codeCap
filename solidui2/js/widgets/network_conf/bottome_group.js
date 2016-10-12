"use strict";
(function( ) {

	var two_buttons = new window.Class(

        //parent //focusable widget
        window.Widget,

        //constructor
        function(description) {
                Widget.call(this, description.name ) ;

                this.$root.addClass( description.name ) ;
		
		/*	
		var description = 
		{
			name: 'sample_name',
			left_button: 'sample_screen :: left_button',
			right_button: 'sample_screen :: right_button',
			left_callback: some_function,
			right_callback: some_function,
		}
		*/

                var $button_area = $('<div>').addClass('button_label').appendTo(this.$root);
                var $button_left_area = $('<div>').addClass('column_left').appendTo($button_area);
                var $button_right_area = $('<div>').addClass('column_right').appendTo($button_area);
                var $left_button = $('<div>').addClass('panel').append($TR( description.left_button )).appendTo($button_left_area);   
                var $right_button = $('<div>').addClass('panel').append($TR( description.right_button )).appendTo($button_right_area);
		this.$left_button = $left_button;
		this.$right_button = $right_button;
		this.callback_left = description.left_callback;
		this.callback_right = description.right_callback;

		this.pos = 0;
		this.is_focused = 0;

	},



	{
        activate_on_focus: false,

		get_item: function( index )
		{
            if( index == 0 )
				return this.$left_button;

			if( index == 1 )
				return this.$right_button;
                	
			return null;
		},

		get_active_item: function()
		{
				return null;
		},


		is_top: function()
		{ 
			if( this.pos == 0 )
				return true;
			return false;
		},

		is_bottom: function()
		{ 
			if( this.pos == 1 )
				return true;
			return false;
		},

		move_up: function()
		{ 
			if( this.pos == 0 )
				return false;
			--this.pos
			
			if( this.is_focused == 1 )
				this.refresh_focus();
			return true;
		},
		
		move_down: function()
		{
			if( this.pos == 1 )
				return false;
			++this.pos;
			
			if( this.is_focused == 1 )
				this.refresh_focus();
			return true;
		},
		
		leave: function()
		{
			
			if( this.pos == 0 )
				this.$left_button.toggleClass("selected", 0);
			if( this.pos == 1 )
				this.$right_button.toggleClass("selected", 0);
			this.is_focused = 0;

		},

		enter: function()
		{ 
			if( this.pos == 0 )
				this.$left_button.toggleClass("selected", 1);
			if( this.pos == 1 )
				this.$right_button.toggleClass("selected", 1);
			this.is_focused = 1;
		},

		set_top: function()
		{
			this.pos = 0;	
			
			if( this.is_focused == 1 )
				this.refresh_focus();
		},

		set_bottom: function()
		{
			this.pos = 1; 
			
			if( this.is_focused == 1 )
				this.refresh_focus();
		},

		set_pos: function(new_pos)
		{ 
			if( new_pos == 0 || new_pos == 1 )
			{
				this.pos = new_pos;
				if( this.is_focused == 1 )
					this.refresh_focus();
				return true;
			}
			/*
			if( this.is_focused == 1 )
				this.refresh_focus();
			*/
			return false;
		},

		activate: function()
		{
			if( this.pos == 0 )
				this.callback_left();
			if( this.pos == 1 )
				this.callback_right();
				return false
		},

		de_activate: function()
		{ return false },
		
		refresh_focus: function()
		{ 
			if( this.pos == 0 )
			{
				this.$left_button.toggleClass("selected" , 1);
                                this.$right_button.toggleClass("selected", 0);
			}
				
			if( this.pos == 1 )
			{
				this.$left_button.toggleClass("selected" , 0);
                                this.$right_button.toggleClass("selected", 1);
			}
		},
	
	}
);

window.two_buttons = two_buttons;

})( ) ;
