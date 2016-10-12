"use strict";
(function( ) {

	var list_manager = new window.Class(

	//parent
	window.Widget,

	//constructor
	function(description) {
		Widget.call(this, description.name ) ;

		this.$root.addClass( description.name ) ;
		/*
		var description =
		{
			name: 'sample_name',
			
			window_function: [
			{ name: 'sample list name', window_function: sample_window_function, description },
			{ name: 'sample list name', window_function: sample_window_function, description },
			{ name: 'sample list name', window_function: sample_window_function, description },
			{ name: 'sample list name', window_function: sample_window_function, description },
			],
		}
		*/
		
		this.lists = [];
		this.hiden = [];
		this.index = [];
		
		for( var i in description.window_function )
		{
			var list = description.window_function[i];
			
			this.lists[list.name] = new list.window_function( list.description );
			this.lists[list.name].$root.appendTo( this.$root );
			this.hiden[list.name] = 0;
			this.index[i] = list.name;
		}

		this.pos = 0;

		this.activate_flag = 0;

		this.find_next_not_hiden_up = function()
		{
				--this.pos;
				if(this.pos < 0 )
					this.pos = this.index.length -1;
				
				if( this.hiden[this.index[this.pos]] == 1 )
					return this.find_next_not_hiden_up();

				return;
		}
		
		this.find_next_not_hiden_down =function()
		{
			++this.pos;
			if(this.pos >= this.index.length)
				this.pos = 0;

			if( this.hiden[this.index[this.pos]] == 1 )
				return this.find_next_not_hiden_down();

			return;

		}

	},

	{
		enter: function( starting_list_name, start_flag, sart_pos )
		{
			for( var i in this.index )
				if( this.index[i] == starting_list_name )
					this.pos = i;

			switch( start_flag )
			{
				case 'start_top':
					this.lists[starting_list_name].set_top();	
					break;
				case 'start_bottom':
					this.lists[starting_list_name].set_bottom();
					break;
				case 'start_at_pos':
					this.lists[starting_list_name].set_pos(start_pos);
					break;
				default:
					this.lists[starting_list_name].set_top();
					break;
			}

			this.lists[starting_list_name].enter();

			return;
		},

		hide_list: function( list_name )
		{
			this.lists[list_name].hide();
			this.hiden[list_name] = 1;
		},

		show_list: function( list_name )
		{
			this.lists[list_name].show();
			this.hiden[list_name] = 0;
		},

		move_up: function()
		{
			if( this.activate_flag == 1 )
			{
				this.activate();
			}

			if( this.lists[this.index[this.pos]].is_top() )
			{
				this.lists[this.index[this.pos]].leave();
				//look for next not hiden element
				this.find_next_not_hiden_up();
				//
				
				this.lists[this.index[this.pos]].set_bottom();
				this.lists[this.index[this.pos]].enter();
				return;
			}
			
			this.lists[this.index[this.pos]].move_up();
			return;
		},

		move_down: function()
		{
	
			if( this.activate_flag == 1 )
			{
				this.activate();
			}

			if( this.lists[this.index[this.pos]].is_bottom() )
			{
	
				this.lists[this.index[this.pos]].leave();

				//look for next not hiden element
				this.find_next_not_hiden_down();

				this.lists[this.index[this.pos]].set_top();
				this.lists[this.index[this.pos]].enter();
				return;
			}

			this.lists[this.index[this.pos]].move_down();
			return;
		},

		activate: function()
		{
			if(this.activate_flag == 0)
			{
				if(this.lists[this.index[this.pos]].activate() == true)
					this.activate_flag = 1;
					
				return;
			}
			
			if(this.activate_flag == 1)
			{
                            if(this.lists[this.index[this.pos]].de_activate() == true)
                                this.activate_flag = 0;
                                return;
			
			}

		},

		get_current_element: function()
		{
			return this.lists[this.index[this.pos]];
		},

		get_element: function(name)
		{
			return this.lists[name];
		},
	
	}
);

window.list_manager = list_manager;

})( ) ;

