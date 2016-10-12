"use strict"; (function() {

var InputView = new window.Class(
	// parent
	window.Widget,
    
	// constructor
	function(description) {
		Widget.call(this, description.name);
    
		this.$root.addClass('input_view');
		this.$panel = $('<div>').addClass('panel').appendTo(this.$root);
		this.$name = $('<div>').addClass('name').append($TR(description.name)).appendTo(this.$panel);
		this.$input = new window.InputItem(description);
		this.$input.$root.addClass('input').appendTo(this.$panel);
	},
    
	// prototype
	{
		activate_on_focus: true,
		
		activate: function(flag) {
			if (flag == this.active) {
				return;
			}
    
			this.active = flag;
			this.$root.toggleClass('active', flag);
    
			if (flag) {
				//
			} else {
				//
			}
		},
    
		getText: function ( ) {
			return this.$input.getText( ) ;
		},
    	
		clear: function ( ) {
			return this.$input.clear( ) ;
		},
    		
		appendChar: function ( char ) {
			return this.$input.appendChar( char ) ;
		},
    		
		removeChar: function ( ) {
			return this.$input.removeChar( ) ;
		},
            
		get_input: function()
		{
			return this.$input;
		},
		
		length: function()
		{
			return this.$input.text_length( ) ;
		},
  }
        
);

	window.InputView = InputView ;

})( ) ;
