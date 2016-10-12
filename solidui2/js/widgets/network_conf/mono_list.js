"use strict";
(function( ) {

	var text_field = new window.Class(

	//parent
	window.Widget,

	//constructor
	function(description) { 
		Widget.call(this, description.name ) ;

		this.$root.addClass( description.name ) ;

		var items = description.items;
		//this.items = items;
		//this.callback = callback ;
		this.pos = 0;	
		this.old_pos = 0;
		this.active = -1;
		this.is_focused = 0;		
	
		this.text_field = [];
		
		/*
		// sample description vor InputView
		var sample_description_of_item = 
		{
			name: 'sample_page :: sample_name',
			size: 15,
			type: 'ipv4',
		};
		*/

		/*
		// sample description of this element
		var sample_description =
		{
			name: "sample name",
			window_function: window_function,
			items: [
				{ sample_description_of_item1 },
				{ sample_description_of_item2 },
				{ sample_description_of_item3 },
				],
		};
		*/

		for( var i in description.items ){
			var item = description.items[i]

			this.text_field[i] = new description.window_function(item);
			this.text_field[i].$root.appendTo( this.$root ) ;
			this.text_field[i].show();
//			this.active[i] = 0;
		}
		
	},

	

	// prototype
	{
		size: function()
		{
			return this.text_field.length;
		},
		
		
		// starts by 0
		// returns -1 if no item is focused right now
		get_focus_pos: function()
		{
			if(this.is_focused != 1)
				return -1;
			return this.pos;
		},
		
		get_item: function( index )
		{
			return this.text_field[ index ];
		},

		get_active_item: function()
		{
			if( this.active != -1 )
				return this.text_field[this.active];
			
			return null;
		},

		is_top: function()
		{
			if(0 == this.pos)
				return true;
			return false;
		},

		is_bottom: function()
		{
			if(this.text_field.length -1 == this.pos)
				return true;
			return false
		},

		move_down: function()
		{
			
			if( this.active != -1 )
				this.de_activate();
			
			if( this.text_field.length -1 <=  this.pos )
				return false;
			++this.pos;

			if( this.is_focused == 1 )
				this.refresh_focus();
			return true;
		},

		move_up: function()
		{
			
			if( this.active != -1 )
				this.de_activate();
		
			if( 0 >= this.pos )
				return false;
			--this.pos;

			if( this.is_focused == 1 )
				this.refresh_focus();
			return true;
		},

		set_pos: function(position)
		{
			
			if( this.active != -1 )
				this.de_activate();
			
			if( position >= 0 && position <= this.text_field.length -1 )
			{			
				this.pos = position;

				if( this.is_focused == 1 )
					this.refresh_focus();
				return true;
			}
			return false;
		},
	
		set_bottom: function()
		{
			if( this.active != -1 )
				this.de_activate();
			
			this.pos = this.text_field.length -1;

			if( this.is_focused == 1 )
				this.refresh_focus();
		},

		set_top: function()
		{	
			if( this.active != -1 )
				this.de_activate();
		
			this.pos = 0;

			if( this.is_focused == 1 )
				this.refresh_focus();
		},

		refresh_focus: function()
		{
			if( this.is_focused != 1 )
				return false;

			this.text_field[this.old_pos].$root.toggleClass( "selected", 0 );
			this.text_field[this.pos].$root.toggleClass( "selected", 1 );
			this.old_pos = this.pos;
			
			if (this.text_field[this.pos].activate_on_focus == true) {
				this.activate();
			}
		},
		
		activate: function( )
		{
			if (this.text_field.length == 0) {
				return false;
			}
			
			if(this.active != -1 )
				return false;
			this.text_field[this.pos].activate(1);
			this.active = this.pos;
			return true;
		},

		de_activate: function( )
		{
			if (this.text_field.length == 0) {
				return false;
			}
			
			if(this.active == -1 )
				return false;
			this.text_field[this.active].activate(0);
			this.active = -1;
			return true;
		},

		leave: function()
		{
			if (this.text_field.length == 0) {
				return false;
			}
		
			if( this.active != -1 )
				this.de_activate();

			this.text_field[this.old_pos].$root.toggleClass( "selected", 0 );
			this.old_pos = this.pos;
			/*
			for( var i in this.text_field )
			{
				this.text_field[i].$root.toggleClass("selected", 0);
			}
			*/
			this.is_focused = 0;
		},
		
		// set property set_pos, set_top or set_bottom before calling enter
		enter: function()
		{
			if (this.text_field.length == 0) {
				return false;
			}
			
			this.old_pos = this.pos;
			this.text_field[this.pos].$root.toggleClass("selected", 1);
			this.is_focused = 1;
			
			if (this.text_field[this.pos].activate_on_focus == true)
				this.activate();
		},
		
	}	

) ;

window.text_field = text_field;

})( ) ;

