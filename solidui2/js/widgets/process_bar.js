"use strict"; (function() {

    var ProcessBar = new window.Class(
    
        // parent
        window.Widget,
        
        // constructor
        function(description) {
            Widget.call(this, description.name);
        
            this.$root.addClass('process_bar');
            this.$panel = $('<div>').addClass('panel').appendTo(this.$root);
            this.$process = $('<div>').addClass('process').appendTo(this.$panel);
        },
        
        // prototype
        {
            setValue: function( precent ) {
                if (precent > 100)
                    precent = 100;
                else if (precent < 0)
                    precent = 0;
				this.$process.css('width', precent + '%');
			},
    	}
    
    );

	window.ProcessBar = ProcessBar ;

})( ) ;
